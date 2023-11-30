#include "..\include\console_interface.h"

#define buf_size 1024

static HANDLE stdout_handle = NULL;
static wchar_t buffer[buf_size];

wchar_t *getBuffer()
{
    return buffer;
}

int getBufferSize()
{
    return buf_size;
}

void initializeConsole()
{
    AllocConsole();
    SetConsoleOutputCP(CP_UTF8);
    stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void printConsole(wchar_t *str)
{
    DWORD len;
    WriteConsoleW(stdout_handle, str, lstrlenW(str), &len, NULL);
}