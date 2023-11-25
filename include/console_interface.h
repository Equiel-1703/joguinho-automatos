#ifndef _CONSOLE_API
#define _CONSOLE_API

#include <Windows.h>

void printConsole(wchar_t *str);
wchar_t *getBuffer();
int getBufferSize();
void initializeConsole();

#endif