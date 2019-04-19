#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>


int add(int a, int b) {
	__asm mov eax, 1;
	return;
}



DWORD get_hook_me_base(DWORD pid) {
	auto ss = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (ss == INVALID_HANDLE_VALUE) {
		throw "error";
	}
	MODULEENTRY32 me;
	me.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(ss, &me))throw "err2";
	do {
		if (!strcmp(me.szModule, "hook_me.exe")) {
			return (DWORD)me.modBaseAddr;
		}
	} while (Module32Next(ss, &me));
	return 0;

}


DWORD get_hook_me_id() {
	auto ss = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (ss == INVALID_HANDLE_VALUE) {
		throw "error";
	}
	PROCESSENTRY32 me;
	me.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(ss, &me))throw "err2";
	do {
		
		if (!strcmp(me.szExeFile, "hook_me.exe"))
			return me.th32ProcessID;
	} while (Process32Next(ss, &me));
	return 0;

}

int main() {

	add(2,3);
	auto pId = get_hook_me_id();
	auto hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
	auto base = get_hook_me_base(pId);
	DWORD pAdd;
	SIZE_T BytesRead;
	BYTE buff[6] = { 0xb8,1,0,0,0,0x90 },b[6];
	DWORD OldProtect,NewProtect;

	
	VirtualProtectEx(hProc, (LPVOID)(base + 0x1083), 6, PAGE_EXECUTE_READWRITE, &OldProtect);
	WriteProcessMemory(hProc, (LPVOID)(base + 0x1083), &buff, 6, &BytesRead);
	VirtualProtectEx(hProc, (LPVOID)(base + 0x1083), 6, OldProtect, &NewProtect);
	CloseHandle(hProc);
	std::cout << BytesRead<<"\n";

	/*
	float ang;

	for (;;) {
		ReadProcessMemory(hProc, (LPCVOID)(base + offset), &ang, sizeof(ang), &BytesRead);
		std::cout << ang;
	}*/


	std::cin.get();

	return 0;
}