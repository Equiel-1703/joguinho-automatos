#ifndef _GRAPHICS_C
#define _GRAPHICS_C

#include <Windows.h>

#define WND_H 720
#define WND_W 1280

// Text area
#define TEXT_AREA_LEFT 50    // X superior-esquerdo
#define TEXT_AREA_TOP 554    // Y superior-esquerdo
#define TEXT_AREA_RIGHT 1229 // X inferior-direito
#define TEXT_AREA_BOTTOM 678 // Y inferior-direito

// Text properties stuff
#define FONT_SIZE 32
#define LINE_SPACE FONT_SIZE

#define COLOR_BLACK 0x00000000
#define COLOR_BLUE 0x00FF0000
#define COLOR_RED 0xFF000000

void initializeGraphics(HWND window, HBITMAP *hBtmpScrn);
void finalizeGraphics();

void moveGameWindow(int x, int y);

HBITMAP loadBitmapHandle(LPCWSTR path);
void showImage(HBITMAP hBitmapToDisplay);
void showImageAnim(HBITMAP hBitmapToDisplay);

void drawText(LPCWSTR message, COLORREF color);
void textPaintRoutine(HDC hdcPaint);
void eraseText();

#endif