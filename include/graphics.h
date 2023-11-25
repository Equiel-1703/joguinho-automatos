#ifndef _GRAPHICS_C
#define _GRAPHICS_C

#include <Windows.h>

HBITMAP loadBitmapHandle(LPCWSTR path);
void showImage(HWND window, HBITMAP hBitmap);

#endif