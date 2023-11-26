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

    // Pega DC da janela
    HDC hdc = GetDC(window);
    // Cria Device Context de memória da tela do programa (é importante usar isso aqui pra evitar flickering)
    HDC hdcMem = CreateCompatibleDC(hdc);
    // Seleciona o bitmap pra usar na tela
    HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmapToDisplay);

    // Copia região do bitmap a ser reescrita de hdcMem para a window
    // Como hdc foi criado por BeginPaint, ele contém só a região que precisa ser alterada
    BitBlt(hdc, 0, 0, WND_W, WND_H, hdcMem, 0, 0, SRCCOPY);

    // Coloca o bitmap original de volta em hdcMem (exigência da API do Windows)
    SelectObject(hdcMem, oldBitmap);
    // Deleta hdcMem e solta hdc, não precisamos mais
    DeleteDC(hdcMem);
    ReleaseDC(window, hdc);
}

void drawText(HWND window, LPCSTR message)
{
    HDC wdc = GetDCEx(window, NULL, DCX_EXCLUDEUPDATE);

    RECT rect;
    GetClientRect(window, &rect);

    SetTextColor(wdc, 0x00000000);
    SetBkMode(wdc, TRANSPARENT);

    rect.left = 50; // x
    rect.top = 555; // y

    HFONT font = CreateFont(18, 0, 0, 0, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hFontOld = (HFONT)SelectObject(wdc, font);

    DrawTextA(wdc, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);

    SelectObject(wdc, hFontOld);
    DeleteObject(font);
    DeleteDC(wdc);
}