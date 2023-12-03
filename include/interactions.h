#ifndef _INTERACTIONS_C
#define _INTERACTIONS_C

#include <Windows.h>
#include <time.h>

// A estrutura foi declarada como volátil para garantir boa comunicação com a thread
// Posso colocar aqui um campo char para armazenar
// oq o player digitou e fazer input/output
typedef struct _GameThreadArgs
{
    volatile BOOL *terminateThread;
    volatile wchar_t *pressed_key;
    volatile HWND hwnd;
} GameThreadArgs;

DWORD WINAPI processTheGame(LPVOID lpParam);

#endif