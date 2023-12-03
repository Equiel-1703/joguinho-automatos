#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <strsafe.h>
#include <dwmapi.h>

#include "..\include\graphics.h"
#include "..\include\interactions.h"
#include "..\include\console_interface.h"

static HBITMAP hScreenBitmap;
static HANDLE thread_processo_do_jogo;
static BOOL terminateThread = FALSE;
static wchar_t pressed_key = '\0';

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Inicializa console para debug
    initializeConsole();

    // Nome da classe
    const wchar_t *MAIN_CLASS_NAME = L"Janela Principal";

    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = MAIN_CLASS_NAME;
    wc.hCursor = LoadCursor(0, IDC_ARROW);

    // Register the window class.
    RegisterClass(&wc);

    SetProcessDPIAware();

    // Configura dimensões da janela
    RECT wnd_rect;
    wnd_rect.bottom = WND_H + 50;
    wnd_rect.top = 50;
    wnd_rect.right = WND_W + 50;
    wnd_rect.left = 50;

    DWORD window_styles = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;

    // Adjust window size to fit the images
    AdjustWindowRect(&wnd_rect, window_styles, FALSE);

    StringCbPrintfW(getBuffer(), getBufferSize(), L"%d %d %d %d\n", wnd_rect.bottom, wnd_rect.top, wnd_rect.right, wnd_rect.left);
    printConsole(getBuffer());
    StringCbPrintfW(getBuffer(), getBufferSize(), L"%d %d\n", wnd_rect.bottom - wnd_rect.top, wnd_rect.right - wnd_rect.left);
    printConsole(getBuffer());

    // Create the window.
    HWND hwnd = CreateWindowExW(
        0,                     // Optional window styles.
        MAIN_CLASS_NAME,       // Window class
        L"Sherlocka & Walter", // Window title
        window_styles,         // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, wnd_rect.right - wnd_rect.left, wnd_rect.bottom - wnd_rect.top,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (hwnd == NULL)
        return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};

    // Inicializa argumentos para a thread
    GameThreadArgs args;
    args.terminateThread = &terminateThread;
    args.pressed_key = &pressed_key;
    args.hwnd = hwnd;

    // Cria e manda a função que processa o jogo para ser executada em outra thread
    thread_processo_do_jogo = CreateThread(
        NULL,           // default security attributes
        0,              // use default stack size
        processTheGame, // thread function name
        &args,          // argument to thread function
        0,              // use default creation flags
        NULL);          // returns the thread identifier

    // Run the message loop.
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Procedure que responde as mensagens da janela
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:

        hScreenBitmap = NULL;
        initializeGraphics(hwnd, &hScreenBitmap);

        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc, hdcMem;
        RECT rect;

        hdc = BeginPaint(hwnd, &ps);

        if (hScreenBitmap)
        {
            // Cria Device Context de memória da tela do programa (é importante usar isso aqui pra evitar flickering)
            hdcMem = CreateCompatibleDC(hdc);
            // Seleciona o bitmap pra usar na tela
            HGDIOBJ oldBitmap = SelectObject(hdcMem, hScreenBitmap);

            // Copia região do bitmap a ser reescrita de hdcMem para hdc
            // Como hdc foi criado por BeginPaint, ele contém só a região que precisa ser alterada
            BitBlt(hdc, 0, 0, WND_W, WND_H, hdcMem, 0, 0, SRCCOPY);

            textPaintRoutine(hdc);

            // Coloca o bitmap original de volta em hdcMem (exigência da API do Windows)
            SelectObject(hdcMem, oldBitmap);
            // Deleta hdcMem, não precisamos mais
            DeleteDC(hdcMem);
        }

        EndPaint(hwnd, &ps);
    }
    break;

    case WM_CHAR:

        pressed_key = (wchar_t)wParam;

        break;

    case WM_DESTROY:

        terminateThread = TRUE;
        WaitForSingleObject(thread_processo_do_jogo, INFINITE);
        finalizeGraphics();
        CloseHandle(thread_processo_do_jogo);
        DeleteObject(hScreenBitmap);
        PostQuitMessage(0);

        return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}