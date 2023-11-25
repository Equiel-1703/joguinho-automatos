#include "../include/graphics.h"

HBITMAP loadBitmapHandle(LPCWSTR path)
{
    HBITMAP bitmap = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // Verifica se deu certo
    if (bitmap == NULL)
    {
        WCHAR buffer[100] = L"Failed to load image: ";

        MessageBoxW(NULL, wcscat(buffer, path), L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }

    return bitmap;
}

void showImage(HWND window, HBITMAP hBitmapToDisplay, HBITMAP *hBitmapOnScreen)
{
    // Updates handle of the bitmap on screen
    *hBitmapOnScreen = hBitmapToDisplay;

    // Returns a handle to the Device Context of the screen
    HDC hdc = GetDCEx(window, NULL, DCX_EXCLUDEUPDATE);
    // Create pattern brush with the bitmap to fill the screen
    HBRUSH brush = CreatePatternBrush(hBitmapToDisplay);

    RECT rect;
    GetWindowRect(window, &rect);
    FillRect(hdc, &rect, brush);

    DeleteObject(brush);
    ReleaseDC(window, hdc);
}