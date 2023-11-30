#include "..\include\graphics.h"

static HWND hWnd = NULL;
static HDC windowDC = NULL;
static HDC windowMemDC = NULL;

static HFONT hNewFont = NULL;

static HBITMAP *hBitmapOnScreen = NULL;
static HBITMAP hBmpBuffer = NULL;

static int windowMemDC_Saved = 0;

static BOOL hasTextOnScreen = FALSE;

static RECT textAreaRect;

void initializeGraphics(HWND window, HBITMAP *hBtmpScrn)
{
    hBitmapOnScreen = hBtmpScrn;

    hWnd = window;
    // Get window DC
    windowDC = GetDC(hWnd);
    // Create memory DC to the window
    windowMemDC = CreateCompatibleDC(windowDC);
    // Save current state of the memDC, we will restore it later to delete properly
    windowMemDC_Saved = SaveDC(windowMemDC);

    // Create compatible bitmap of the current screen to windowMemDC use.
    // The bitmap returned is blank and has the same with and height of the screen
    RECT rect;
    GetClientRect(hWnd, &rect);
    hBmpBuffer = CreateCompatibleBitmap(windowDC, rect.right, rect.bottom);

    // Select bitmap to windowMemDC draw into
    SelectObject(windowMemDC, hBmpBuffer);

    // Creating font
    HFONT hFont = GetStockObject(DEFAULT_GUI_FONT);
    LOGFONT logfont;
    GetObject(hFont, sizeof(LOGFONT), &logfont);

    // Now change the logfont.lfHeight member to set the font size
    logfont.lfHeight = FONT_SIZE;

    hNewFont = CreateFontIndirect(&logfont);

    // Initializes rectangle for the text area
    textAreaRect.bottom = TEXT_AREA_BOTTOM;
    textAreaRect.left = TEXT_AREA_LEFT;
    textAreaRect.right = TEXT_AREA_RIGHT;
    textAreaRect.top = TEXT_AREA_TOP;
}

void finalizeGraphics()
{
    ReleaseDC(hWnd, windowDC);
    RestoreDC(windowMemDC, windowMemDC_Saved);
    DeleteDC(windowMemDC);

    DeleteObject(hBmpBuffer);
    DeleteObject(hNewFont);
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

    // Cria Device Context de memória da tela do programa
    // Não posso usar o windowMemDC pois ele salva o texto junto, esse é só pra imagem de fundo
    // É importante usar memory DC's para bitmaps pois evita flickering
    HDC hdcMem = CreateCompatibleDC(windowDC);
    // Seleciona o handle do bitmap no hdcMem
    HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmapToDisplay);

    // Copia região do bitmap a ser reescrita de hdcMem para windowDC
    // Como hdc foi criado por BeginPaint, ele contém só a região que precisa ser alterada
    BitBlt(windowDC, 0, 0, WND_W, WND_H, hdcMem, 0, 0, SRCCOPY);

    // Coloca o bitmap original de volta em hdcMem (exigência da API do Windows)
    SelectObject(hdcMem, oldBitmap);
    // Deleta hdcMem
    DeleteDC(hdcMem);
}

// Mostra imagem só até a text area
void showImageAnim(HBITMAP hBitmapToDisplay)
{
    // Updates handle of the bitmap on screen
    *hBitmapOnScreen = hBitmapToDisplay;

    // Cria Device Context de memória da tela do programa
    // Não posso usar o windowMemDC pois ele salva o texto junto, esse é só pra imagem de fundo
    // É importante usar memory DC's para bitmaps pois evita flickering
    HDC hdcMem = CreateCompatibleDC(windowDC);
    // Seleciona o handle do bitmap no hdcMem
    HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmapToDisplay);

    // Copia região do bitmap a ser reescrita de hdcMem para windowDC
    // Como hdc foi criado por BeginPaint, ele contém só a região que precisa ser alterada
    BitBlt(windowDC, 0, 0, WND_W, TEXT_AREA_TOP - 5, hdcMem, 0, 0, SRCCOPY);

    // Coloca o bitmap original de volta em hdcMem (exigência da API do Windows)
    SelectObject(hdcMem, oldBitmap);
    // Deleta hdcMem
    DeleteDC(hdcMem);
}

void drawText(LPCWSTR message, COLORREF color)
{
    hasTextOnScreen = TRUE;

    // Setup text
    SetTextColor(windowMemDC, color);
    SetBkMode(windowMemDC, TRANSPARENT);
    HFONT hOldFont = (HFONT)SelectObject(windowMemDC, hNewFont);

    // Copies screen data to windowMemDC bitmap buffer
    BitBlt(windowMemDC, 0, 0, WND_W, WND_H, windowDC, 0, 0, SRCCOPY);

    // Draw text in windowMemDC bitmap buffer
    DrawTextW(windowMemDC, message, -1, &textAreaRect, DT_WORDBREAK);

    // Copy windowMemDC data back to windowDC
    BitBlt(windowDC, 0, 0, WND_W, WND_H, windowMemDC, 0, 0, SRCCOPY);

    // Put old font back into windowMemDC
    SelectObject(windowMemDC, hOldFont);
}

void eraseText()
{
    // Cria memory dc da tela
    HDC hdcMem = CreateCompatibleDC(windowDC);
    // Seleciona no memory dc o bitmap que está na tela agora
    HGDIOBJ oldBitmap = SelectObject(hdcMem, *hBitmapOnScreen);

    RECT rect;
    GetClientRect(hWnd, &rect);

    // Coordenadas da área do cliente
    rect.left = TEXT_AREA_LEFT; // x
    rect.top = TEXT_AREA_TOP;   // y

    // Copia a nova tela no hdcMem para a janela (windowDC)
    BitBlt(windowDC,
           TEXT_AREA_LEFT, TEXT_AREA_TOP,
           rect.right - rect.left, rect.bottom - rect.top,
           hdcMem,
           TEXT_AREA_LEFT, TEXT_AREA_TOP, SRCCOPY);

    // Coloca o bitmap original de volta em hdcMem (exigência da API do Windows)
    SelectObject(hdcMem, oldBitmap);
    // Deleta hdcMem
    DeleteDC(hdcMem);

    hasTextOnScreen = FALSE;
}

void textPaintRoutine(HDC hdcPaint)
{
    // Verifica se há texto na tela para repintar
    if (hasTextOnScreen)
    {
        // Copies the bitmap of the last screen into windowDC provided by BeginPaint
        BitBlt(hdcPaint, 0, 0, WND_W, WND_H, windowMemDC, 0, 0, SRCCOPY);
    }
}