#ifndef _GRAPHICS_C
#define _GRAPHICS_C

#include <Windows.h>

#define WND_H 720
#define WND_W 1280

HBITMAP loadBitmapHandle(LPCWSTR path);
void showImage(HWND window, HBITMAP hBitmapToDisplay, HBITMAP *hBitmapOnScreen);
void drawText(HWND window, LPCSTR message);

#endif