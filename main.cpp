#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <algorithm>

#define NOP __asm nop




MODULEENTRY32 GetModule(DWORD pID, const char* moduleName)
{
	DWORD ModuleBaseAddress = NULL;
	MODULEENTRY32 me;
	HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	if (ss != INVALID_HANDLE_VALUE)
	{
		me.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(ss, &me))
		{
			do
			{
				if (!strcmp(me.szModule, moduleName))
				{
					ModuleBaseAddress = (DWORD)me.modBaseAddr;
					break;
				}
			} while (Module32Next(ss, &me));
		}
		CloseHandle(ss);
	}
	return me;
}

__declspec(naked) int hook() {
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	std::cout << "\n\nWORKS!!!";
	__asm mov eax,1
	__asm mov esp,ebp
	__asm pop ebp
	__asm ret
}


int add(int a, int b) {
	NOP
	NOP
	NOP
	NOP
	NOP
	return a+b;
}

template<typename T=BYTE>
unsigned long get_offset(std::vector<T> buff, std::vector<T> pattern) {
	return std::search(buff.begin(), buff.end(), pattern.begin(), pattern.end()) - buff.begin();
}

int main() {
	add(1, 2);

	auto hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
	DWORD dwBytesRead;
	auto mod = GetModule(GetCurrentProcessId(), "Project11.exe");
	std::vector<BYTE> buff(mod.modBaseSize);
	ReadProcessMemory(hProc, mod.modBaseAddr, &buff[0], buff.size(), &dwBytesRead);


	auto offset =get_offset<BYTE>(buff, { 0x90, 0x90, 0x90, 0x90, 0x90 });
	auto hook_offset = get_offset<BYTE>(buff, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
	BYTE jmp[5];
	jmp[0] = 0xE9;
	union {
		long of;
		BYTE bytes[4];
	} offset_to_addr;
	offset_to_addr.of = hook_offset-offset;
	for (int i = 0; i < 4; ++i) {
		jmp[i + 1] = offset_to_addr.bytes[i];
	}
	DWORD oldprotect;
	WriteProcessMemory(hProc, mod.modBaseAddr + offset, jmp, 5, &dwBytesRead);
	std::cout <<add(2, 3);
	std::string dllpath = "dll.dll";
	auto p = VirtualAlloc(NULL, dllpath.size(), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(hProc, p, dllpath.c_str(), dllpath.size(), &dwBytesRead);
	LPVOID loadlibrary = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	HANDLE remoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)loadlibrary, p, 0, NULL);
	std::cout << "\n\n" << (DWORD)p;
	std::cin.get();

	return 0;
}