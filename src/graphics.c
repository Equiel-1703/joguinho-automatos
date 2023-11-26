#include "../include/graphics.h"

static HDC window_text_backup = NULL;
static HDC windowDC = NULL;
static HWND hWnd = NULL;
static HFONT hNewFont = NULL;
static HBITMAP *hBitmapOnScreen = NULL;
static int wtbSavedDC = 0;

void initializeGraphics(HWND window, HBITMAP *hBtmpScrn)
{
    hBitmapOnScreen = hBtmpScrn;

    hWnd = window;
    windowDC = GetDC(hWnd);
    window_text_backup = CreateCompatibleDC(windowDC);
    wtbSavedDC = SaveDC(window_text_backup);

    // Creating font
    HFONT hFont = GetStockObject(DEFAULT_GUI_FONT);
    LOGFONT logfont;
    GetObject(hFont, sizeof(LOGFONT), &logfont);

    // Now change the logfont.lfHeight member
    logfont.lfHeight = 32;

    HFONT hNewFont = CreateFontIndirect(&logfont);
}

void finalizeGraphics()
{
    DeleteObject(hNewFont);
    ReleaseDC(hWnd, windowDC);
    RestoreDC(window_text_backup, wtbSavedDC);
    DeleteDC(window_text_backup);
}

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

void showImage(HBITMAP hBitmapToDisplay)
{
    // Updates handle of the bitmap on screen
    *hBitmapOnScreen = hBitmapToDisplay;

    // Pega DC da janela
    HDC hdc = GetDC(hWnd);
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
    ReleaseDC(hWnd, hdc);
}

void drawText(LPCWSTR message)
{
    RECT rect;
    GetClientRect(hWnd, &rect);

    // Create bitmap of the current screen
    HBITMAP hCurrentScreen = CreateCompatibleBitmap(windowDC, rect.right, rect.bottom);

    SetTextColor(window_text_backup, 0x00000000);
    SetBkMode(window_text_backup, TRANSPARENT);

    HFONT hOldFont = (HFONT)SelectObject(window_text_backup, hNewFont);

    // Coordinates do draw
    rect.left = 50; // x
    rect.top = 555; // y

    // Select current bitmap on screen to display
    SelectObject(window_text_backup, hCurrentScreen);
    // Copies screen to memory DC
    BitBlt(window_text_backup, 0, 0, WND_W, WND_H, windowDC, 0, 0, SRCCOPY);

    // Put text in the memory DC
    DrawTextW(window_text_backup, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);

    // Copies memory DC to the window DC back
    BitBlt(windowDC, 0, 0, WND_W, WND_H, window_text_backup, 0, 0, SRCCOPY);

    // Always select the old font back into the DC
    SelectObject(windowDC, hOldFont);
    DeleteObject(hCurrentScreen);
}