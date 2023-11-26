#ifndef _GRAPHICS_C
#define _GRAPHICS_C

#include <Windows.h>

#define WND_H 720
#define WND_W 1280

// Text area
#define TEXT_AREA_X 50
#define TEXT_AREA_Y 555

// Text properties stuff
#define FONT_SIZE 32
#define LINE_SPACE FONT_SIZE

#define COLOR_BLACK 0x00000000
#define COLOR_BLUE 0x00FF0000

void initializeGraphics(HWND window, HBITMAP *hBtmpScrn);
void finalizeGraphics();

HBITMAP loadBitmapHandle(LPCWSTR path);
void showImage(HBITMAP hBitmapToDisplay);

void drawText(LPCWSTR message, COLORREF color, int x, int y);
void textPaintRoutine(HDC hdcPaint);
void eraseText();

#endif