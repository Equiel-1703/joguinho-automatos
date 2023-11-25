#ifndef _INTERACTIONS_C
#define _INTERACTIONS_C

#include <Windows.h>
#include <time.h>

// Posso colocar aqui um campo char para armazenar
// oq o player digitou e fazer input/output
typedef struct _process_game_args
{
    HWND window;
    int *state_id;
} process_game_args;

void timer(int tempoMS);
DWORD WINAPI processTheGame(LPVOID lpParam);

#endif