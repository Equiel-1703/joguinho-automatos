#include "../include/interactions.h"
#include "../include/graphics.h"
#include "../include/console_interface.h"

#include <stdio.h>
#include <strsafe.h>

// Essa função cria um delay de "tempo" milissegundos
void timer(int tempoMS)
{
    // A variável "start" salva o número de clocks até o momento de execução
    clock_t start = clock();
    // A variável "diff" salvará a diferença de clocks
    clock_t diff;
    // "msec" salvará os milissegundos que passaram
    int msec;

    /*
    Aqui foi usado o método do-while para garantir o bom funcionamento
    até em casos em que o tempo de delay é bem pequeno.
    */
    do
    {
        // Esse for é só para ocupar o processador com alguma coisa
        for (int i = 0; i < 99999; i++)
        {
            continue;
        }

        // Calcula a diferença do número de clocks atuais com os iniciais
        diff = clock() - start;
        // Converte essa diferença para milissegundos
        msec = (diff * 1000) / CLOCKS_PER_SEC;

        /*
        Se a diferença em milissegundos for menor que o tempo solicitado para delay,
        o código no "do" continuará a ser executado repetidamente. Quando a diferença
        for igual ou superior que o tempo solicitado, o laço quebra e a função acaba
        */
    } while (msec < tempoMS);
}

void proceedWhenKeyIsPressed(wchar_t key, GameThreadArgs *args)
{
    while (key != *args->pressed_key)
    {
        if (*args->terminateThread)
            ExitThread(0);
    }
}

DWORD WINAPI processTheGame(LPVOID lpParam)
{
    GameThreadArgs *args = (GameThreadArgs *)lpParam;
    int game_state_id = 0;

    while (game_state_id != 2)
    {
        switch (game_state_id)
        {
        case 0:
        {
            HBITMAP img_2 = loadBitmapHandle(L".\\res\\2.bmp");
            HBITMAP img_3 = loadBitmapHandle(L".\\res\\3.bmp");

            for (int i = 0; i < 3; i++)
            {
                showImage(args->window, img_2, args->hScreenBitmap);

                timer(500);

                showImage(args->window, img_3, args->hScreenBitmap);

                timer(500);
            }

            proceedWhenKeyIsPressed(' ', args);
            ++game_state_id;

            StringCbPrintfW(getBuffer(), getBufferSize(), L"Pasou aqui: %d\n", game_state_id);
            printConsole(getBuffer());

            DeleteObject(img_2);
            DeleteObject(img_3);
        }
        break;

        case 1:
        {
            printConsole(L"Entrou no case 1");
            HBITMAP img_1 = loadBitmapHandle(L".\\res\\1.bmp");

            showImage(args->window, img_1, args->hScreenBitmap);
            ++game_state_id;
        }
        break;
        } // End switch

        if (*args->terminateThread)
            break;
    }

    return 0;
}