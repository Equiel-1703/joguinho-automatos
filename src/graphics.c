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

void showImage(HWND window, HBITMAP hBitmap)
{
    // Returns a handle to the Device Context of the screen
    HDC hdc = GetDC(window);

    // Creates memory device context for the screen handle
    HDC hdcMem = CreateCompatibleDC(hdc);
    // Selects bitmap to be used in the memory device context (and save the old bitmap)
    HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmap);

    // Stores information about the bitmap in the provided BITMAP structure
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(bitmap), &bitmap);

    // Transfers bitmap block of data from hdcMem to hdc
    BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
           hdcMem, 0, 0, SRCCOPY);

    // Put original bitmap back in the memory device context (Windows api rule)
    // See in: https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-selectobject
    SelectObject(hdcMem, oldBitmap);
    // Delete memory device context
    ReleaseDC(window, hdcMem);
}