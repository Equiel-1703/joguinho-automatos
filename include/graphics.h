#ifndef _GRAPHICS_C
#define _GRAPHICS_C

#include <Windows.h>

#define WND_H 720
#define WND_W 1280

HBITMAP loadBitmapHandle(LPCWSTR path);
void showImage(HBITMAP hBitmapToDisplay);
void drawText(LPCWSTR message);
void initializeGraphics(HWND window, HBITMAP *hBtmpScrn);
void finalizeGraphics();

#endif