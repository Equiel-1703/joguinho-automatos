#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include "../include/graphics.h"
#include "../include/interactions.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static HBITMAP hBitmap;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Nome da classe
    const wchar_t *MAIN_CLASS_NAME = L"Janela Principal";

    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = MAIN_CLASS_NAME;
    wc.hCursor = LoadCursor(0, IDC_ARROW);

    // Register the window class.
    RegisterClass(&wc);

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                     // Optional window styles.
        MAIN_CLASS_NAME,       // Window class
        L"Sherlocka & Walter", // Window title
        WS_OVERLAPPEDWINDOW,   // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};

    // Cria state_id do jogo
    int *state_id = malloc(sizeof(int));
    *state_id = 0;

    // Inicializa argumentos para a thread
    process_game_args args;
    args.state_id = state_id;
    args.window = hwnd;

    // Cria e manda a função que processa o jogo para ser executada em outra thread
    HANDLE thread_processo_do_jogo = CreateThread(
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

        ++(*state_id);
    }

    CloseHandle(thread_processo_do_jogo);
    free(state_id);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc;

        // Returns a handle to a device context with the dimensions of the area to be painted
        hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(WHITE_BRUSH));

        // Creates memory device context for the BeginPaint handle
        HDC hdcMem = CreateCompatibleDC(hdc);
        // Selects bitmap to be used in the memory device context (and save the old bitmap)
        HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmap);

        // Stores information about the bitmap in the provided BITMAP structure
        BITMAP bitmap;
        GetObject(hBitmap, sizeof(bitmap), &bitmap);

        // Transfers bitmap block of data from hdcMem to hdc
        BitBlt(hdc, 200, 20, bitmap.bmWidth, bitmap.bmHeight,
               hdcMem, 0, 0, SRCCOPY);

        // Put original bitmap back in the memory device context (windows api rule)
        // See in: https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-selectobject
        SelectObject(hdcMem, oldBitmap);
        // Delete memory device context
        DeleteDC(hdcMem);

        EndPaint(hwnd, &ps);

        return 0;
    }

    case WM_DESTROY:

        DeleteObject(hBitmap);
        PostQuitMessage(0);

        return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}