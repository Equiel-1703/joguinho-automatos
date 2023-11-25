#include "../include/interactions.h"
#include "../include/graphics.h"
#include <stdio.h>

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

DWORD WINAPI processTheGame(LPVOID lpParam)
{
    process_game_args *args = (process_game_args *)lpParam;

    while (*args->state_id != 1)
    {
        switch (*args->state_id)
        {
        case 0:
        {
            HBITMAP img_2 = loadBitmapHandle(L".\\res\\2.bmp");
            HBITMAP img_3 = loadBitmapHandle(L".\\res\\3.bmp");

            for (int i = 0; i < 3; i++)
            {
                showImage(args->window, img_2);

                timer(500);

                showImage(args->window, img_3);

                timer(500);
            }
        }
        break;

        default:
            break;
        }
    }

    return 0;
}