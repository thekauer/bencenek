#include <iostream>
#include <Windows.h>




typedef  struct {
	int x; int y;
} val_t,*pval,*lpval;





int ScreenX = GetDeviceCaps(GetDC(0), HORZRES);
int ScreenY = GetDeviceCaps(GetDC(0), VERTRES);
BYTE* ScreenData = new BYTE[3 * ScreenX*ScreenY];
void ScreenCap()
{
	HDC hScreen = GetDC(GetDesktopWindow());
	ScreenX = GetDeviceCaps(hScreen, HORZRES);
	ScreenY = GetDeviceCaps(hScreen, VERTRES);

	HDC hdcMem = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, ScreenX, ScreenY);
	HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);
	BitBlt(hdcMem, 0, 0, ScreenX, ScreenY, hScreen, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hOld);

	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = ScreenX;
	bmi.biHeight = -ScreenY;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;

	if (ScreenData)
		free(ScreenData);
	ScreenData = (BYTE*)malloc(4 * ScreenX * ScreenY);

	GetDIBits(hdcMem, hBitmap, 0, ScreenY, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
}

inline int PosB(int x, int y)
{
	return ScreenData[4 * ((y*ScreenX) + x)];
}

inline int PosG(int x, int y)
{
	return ScreenData[4 * ((y*ScreenX) + x) + 1];
}

inline int PosR(int x, int y)
{
	return ScreenData[4 * ((y*ScreenX) + x) + 2];
}

bool ButtonPress(int Key)
{
	bool button_pressed = false;

	while (GetAsyncKeyState(Key))
		button_pressed = true;

	return button_pressed;
}



int main() {
	FreeConsole();
	while(!GetAsyncKeyState(VK_F10)==TRUE) {
		ScreenCap();
		bool b = false;
		for (int i = 0; i < ScreenX; ++i) {
			for (int j = 0; j < ScreenY; j++) {
				if (PosR(i, j) > 235 && PosG(i, j) >150 && PosG(i, j) < 170 && PosB(i, j) > 70 && PosB(i, j) < 130) {
					SetCursorPos(i, j);
					b = true;
					break;
				}
			}
			if (b) break;
		}
	}

	std::cin.get();
	return 0;
}
