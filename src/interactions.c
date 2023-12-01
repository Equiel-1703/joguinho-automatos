#include "../include/interactions.h"
#include "../include/graphics.h"
#include "../include/console_interface.h"

#include <stdio.h>
#include <strsafe.h>
#include <stdarg.h>

static GameThreadArgs *args = NULL;
static int game_state_id = 0;

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
            if (*args->terminateThread)
                ExitThread(0);

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

wchar_t proceedWhenKeyIsPressed(int n, ...)
{
    // Declaring pointer to the
    // argument list
    va_list list_start, ptr;

    // Initializing argument to the
    // list pointer
    va_start(list_start, n);

    wchar_t c, varg;
    BOOL flag = TRUE;

    // Limpa tecla pressionada
    *args->pressed_key = '\0';

    while (flag)
    {
        c = *args->pressed_key;

        // Copia lista para iterar sobre ela a cada loop
        va_copy(ptr, list_start);

        // Traverses args
        for (int i = 0; i < n; ++i)
        {
            if (c == va_arg(ptr, wchar_t))
            {
                flag = FALSE;
                break;
            }
        }

        // Libera cópia
        va_end(ptr);

        if (*args->terminateThread)
            ExitThread(0);
    }

    // Reset key (will not be used anymore)
    *args->pressed_key = '\0';

    // Liberando lista
    va_end(list_start);

    return c;
}

static const int frame_len = 150;
static const int pistas_delay = 500;

// Atualiza só a região superior da caixa de texto para o texto não sumir!
void mouthAnim(HBITMAP img_1, HBITMAP img_2, const int rep)
{
    for (int i = 0; i < rep; i++)
    {
        showImageAnim(img_1);

        timer(frame_len);

        showImageAnim(img_2);

        timer(frame_len);
    }
}

void eyeAnim(HBITMAP open_eye, HBITMAP closed_eye, const int rep)
{
    for (int i = 0; i < rep; i++)
    {
        showImageAnim(open_eye);

        timer(8 * frame_len);

        showImageAnim(closed_eye);

        timer(1 * frame_len);
    }
    showImageAnim(open_eye);
}

void transicao_q2()
{
    HBITMAP img_1_p3_ce = loadBitmapHandle(L"res\\1_p3_ce.bmp"); // Essa é ela de zoinho fechado
    HBITMAP img_1_p4_cm = loadBitmapHandle(L"res\\1_p4_cm.bmp");
    HBITMAP img_1_p4_om = loadBitmapHandle(L"res\\1_p4_om.bmp");
    HBITMAP img_1_p4_smile = loadBitmapHandle(L"res\\1_p4_smile.bmp");

    eraseText();

    drawText(
        L"Walter: Esse caderno é bem estranho, hein. O que tem nele?",
        COLOR_BLACK);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_1_p4_cm);
    drawText(
        L"Tem anotações de aula de música e mais pra frente alguns cálculos. Deve ser de alguém que fazia música ou um curso de exatas.",
        COLOR_BLACK);

    mouthAnim(img_1_p4_om, img_1_p4_cm, 5);

    proceedWhenKeyIsPressed(1, ' ');

    eraseText();
    drawText(
        L"Walter: Tem um campus da UFPel aqui perto. Por que não vamos lá dar uma olhada?",
        COLOR_BLACK);

    eyeAnim(img_1_p4_cm, img_1_p3_ce, 2);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_1_p4_smile);
    drawText(
        L"Boa ideia!",
        COLOR_BLACK);

    mouthAnim(img_1_p4_om, img_1_p4_smile, 2);

    proceedWhenKeyIsPressed(1, ' ');

    DeleteObject(img_1_p3_ce);
    DeleteObject(img_1_p4_cm);
    DeleteObject(img_1_p4_om);
    DeleteObject(img_1_p4_smile);
}

void transicao_q1()
{
    HBITMAP img_1_p3_ce = loadBitmapHandle(L"res\\1_p3_ce.bmp"); // Essa é ela de zoinho fechado
    HBITMAP img_1_p3_oe = loadBitmapHandle(L"res\\1_p3_oe.bmp");
    HBITMAP img_1_p4_cm = loadBitmapHandle(L"res\\1_p4_cm.bmp");
    HBITMAP img_1_p4_om = loadBitmapHandle(L"res\\1_p4_om.bmp");

    eraseText();

    drawText(
        L"Walter: Essa cuia parece muito suspeita.",
        COLOR_BLACK);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_1_p3_oe);
    drawText(
        L"Humm, tem razão. Esse bandido deve gostar muito de um mate... ",
        COLOR_BLACK);

    eyeAnim(img_1_p3_oe, img_1_p3_ce, 1);

    proceedWhenKeyIsPressed(1, ' ');

    eraseText();
    drawText(
        L"Tem um bailão gaúcho aqui perto. Vamos ver se encontramos alguma coisa lá.",
        COLOR_BLACK);

    mouthAnim(img_1_p4_om, img_1_p4_cm, 5);

    proceedWhenKeyIsPressed(1, ' ');

    DeleteObject(img_1_p3_ce);
    DeleteObject(img_1_p3_oe);
    DeleteObject(img_1_p4_cm);
    DeleteObject(img_1_p4_om);
}

void q0()
{
    // Carregando sprites da Sherlocka
    HBITMAP img_1_p1_om = loadBitmapHandle(L"res\\1_p1_om.bmp");
    HBITMAP img_1_p1_cm = loadBitmapHandle(L"res\\1_p1_cm.bmp");
    HBITMAP img_1_p2_om = loadBitmapHandle(L"res\\1_p2_om.bmp");
    HBITMAP img_1_p2_cm = loadBitmapHandle(L"res\\1_p2_cm.bmp");

    // Mostra primeira imagem
    showImage(img_1_p1_om);

    drawText(
        L"Walter, você está atrasado como sempre. No mínimo encheu a cara de chá.",
        COLOR_BLACK);

    mouthAnim(img_1_p1_om, img_1_p1_cm, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_1_p2_om);

    drawText(
        L"Houve um assalto nessa residência. Eu achei duas coisas que parecem ser pistas. Me diga o que acha.",
        COLOR_BLACK);

    mouthAnim(img_1_p2_om, img_1_p2_cm, 5);

    proceedWhenKeyIsPressed(1, ' ');

    // Carrega sprite da pista
    HBITMAP img_pista_1 = loadBitmapHandle(L"res\\pista_1.bmp");

    // Mostra pistas 1
    showImage(img_pista_1);

    drawText(L"O que te parece ser suspeito?\n1 - Caderno\n2 - Cuia", COLOR_BLACK);

    // Lê resposta do jogador
    wchar_t c = proceedWhenKeyIsPressed(2, L'1', L'2');

    eraseText();

    drawText(L"O que te parece ser suspeito?", COLOR_BLACK);

    // Lendo qual pista o jogador escolheu
    switch (c)
    {
    // Escolheu caderno
    case L'1':
        drawText(L"\n1 - Caderno", COLOR_BLUE);
        drawText(L"\n\n2 - Cuia", COLOR_BLACK);

        timer(pistas_delay);

        transicao_q2();

        game_state_id = 2;
        break;

    // Escolheu cuia
    case L'2':
        drawText(L"\n1 - Caderno", COLOR_BLACK);
        drawText(L"\n\n2 - Cuia", COLOR_BLUE);

        timer(pistas_delay);

        transicao_q1();

        game_state_id = 1;
        break;
    }

    // Deletando imagens
    DeleteObject(img_pista_1);
    DeleteObject(img_1_p1_om);
    DeleteObject(img_1_p1_cm);
    DeleteObject(img_1_p2_om);
    DeleteObject(img_1_p2_cm);
}

DWORD WINAPI processTheGame(LPVOID lpParam)
{
    args = (GameThreadArgs *)lpParam;
    game_state_id = 0;

    while (1)
    {
        switch (game_state_id)
        {
        // q0
        case 0:
            q0();
            break;

        // q1
        case 1:
        {
            HBITMAP img_1_p2_cm = loadBitmapHandle(L"res\\1_p2_cm.bmp");

            showImage(img_1_p2_cm);

            DeleteObject(img_1_p2_cm);

            ++game_state_id;
        }
        break;
        }

        if (*args->terminateThread)
            break;
    }
    return 0;
}