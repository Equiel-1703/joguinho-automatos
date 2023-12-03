#include "../include/interactions.h"
#include "../include/graphics.h"
#include "../include/console_interface.h"

#include <stdio.h>
#include <strsafe.h>
#include <stdarg.h>

#define MENU -1
#define SAIR -2

static GameThreadArgs *args = NULL;
static int game_state_id = MENU;

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
void mouthAnim(HBITMAP first_image, HBITMAP last_image, const int rep)
{
    for (int i = 0; i < rep; i++)
    {
        showImageAnim(first_image);

        timer(frame_len);

        showImageAnim(last_image);

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

// Mostra uma pista e processa transições de estado dessa pista
void processPista(
    HBITMAP pista,
    LPWSTR pista_1,
    void (*transicao_1)(),
    int next_state_1,
    LPWSTR pista_2,
    void (*transicao_2)(),
    int next_state_2)
{
    WCHAR pista_1_buf[100];
    WCHAR pista_2_buf[100];
    LPWSTR prompt = L"O que te parece ser suspeito?";

    StringCbPrintfW(pista_1_buf, 100, L"\n1 - %s", pista_1);
    StringCbPrintfW(pista_2_buf, 100, L"\n\n2 - %s", pista_2);

    // Mostra imagem da pista
    showImage(pista);

    drawText(prompt, COLOR_BLACK);
    drawText(pista_1_buf, COLOR_BLACK);
    drawText(pista_2_buf, COLOR_BLACK);

    // Lê resposta do jogador
    wchar_t c = proceedWhenKeyIsPressed(2, L'1', L'2');

    eraseText();
    drawText(prompt, COLOR_BLACK);

    // Lendo qual pista o jogador escolheu
    switch (c)
    {
    // Escolheu pista 1
    case L'1':
        drawText(pista_1_buf, COLOR_BLUE);
        drawText(pista_2_buf, COLOR_BLACK);

        timer(pistas_delay);

        transicao_1();

        game_state_id = next_state_1;
        break;

    // Escolheu pista 2
    case L'2':
        drawText(pista_1_buf, COLOR_BLACK);
        drawText(pista_2_buf, COLOR_BLUE);

        timer(pistas_delay);

        transicao_2();

        game_state_id = next_state_2;
        break;
    }
}

// Prende Jorge G. Tamanco
void q3()
{
    // Carregando sprites da Sherlocka
    HBITMAP img_3_1a = loadBitmapHandle(L"res\\3_1a.bmp");
    HBITMAP img_3_1b = loadBitmapHandle(L"res\\3_1b.bmp");
    HBITMAP img_3_2a = loadBitmapHandle(L"res\\3_2a.bmp");
    HBITMAP img_3_2b = loadBitmapHandle(L"res\\3_2b.bmp");
    HBITMAP img_3_2c = loadBitmapHandle(L"res\\3_2c.bmp");
    HBITMAP img_3_3a = loadBitmapHandle(L"res\\3_3a.bmp");
    HBITMAP img_3_3b = loadBitmapHandle(L"res\\3_3b.bmp");
    HBITMAP img_3_4a = loadBitmapHandle(L"res\\3_4a.bmp");
    HBITMAP img_3_4b = loadBitmapHandle(L"res\\3_4b.bmp");
    HBITMAP img_3_5a = loadBitmapHandle(L"res\\3_5a.bmp");
    HBITMAP img_3_5b = loadBitmapHandle(L"res\\3_5b.bmp");
    HBITMAP img_you_lose = loadBitmapHandle(L"res\\you_lose.bmp");

    // Mostra primeira imagem
    showImage(img_3_1a);

    drawText(
        L"Jorge, você está preso por assalto e invasão de propriedade!\nAchamos sua cuia na cena do crime e sua bota com chimarrão no bailão.",
        COLOR_BLACK);

    mouthAnim(img_3_1b, img_3_1a, 8);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_3_2b);

    drawText(
        L"Bah Sherlocka, tá doida? Se eu tomo chimarrão na minha bota, como que essa cuia ia ser minha?",
        COLOR_BLACK);

    mouthAnim(img_3_2a, img_3_2b, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_3_3a);

    drawText(
        L"E outra, essa bota aí é do dono do bailão, ele finalmente tá aprendendo a ser chucro que nem eu e tá tomando mate bem curtido na butina.",
        COLOR_BLACK);

    mouthAnim(img_3_3b, img_3_3a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_3_2b);

    drawText(
        L"Er, vocês lavam a bota antes né?",
        COLOR_BLACK);

    mouthAnim(img_3_2c, img_3_2b, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_3_4a);

    drawText(
        L"Só uma das botas fica no meu pé, né guria? O outro pé fica sempre limpo.",
        COLOR_BLACK);

    mouthAnim(img_3_4b, img_3_4a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_3_5a);

    drawText(
        L"Ah é, hehe. Desculpa seu Jorge...",
        COLOR_BLACK);

    mouthAnim(img_3_5a, img_3_5b, 5);

    proceedWhenKeyIsPressed(1, ' ');

    eraseText();
    drawText(
        L"Eu já vou indo, tenha um ótimo dia!",
        COLOR_BLACK);

    mouthAnim(img_3_5a, img_3_5b, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_you_lose);

    proceedWhenKeyIsPressed(1, ' ');

    game_state_id = -1; // volta pro menu

    DeleteObject(img_3_1a);
    DeleteObject(img_3_1b);
    DeleteObject(img_3_2a);
    DeleteObject(img_3_2b);
    DeleteObject(img_3_2c);
    DeleteObject(img_3_3a);
    DeleteObject(img_3_3b);
    DeleteObject(img_3_4a);
    DeleteObject(img_3_4b);
    DeleteObject(img_3_5a);
    DeleteObject(img_3_5b);
    DeleteObject(img_you_lose);
}

// Escolheu bota com mate
void transicao_q2_q5()
{
    
}

// Escolheu gaita lança-chamas
void transicao_q2_q6()
{

}

// UFPel
void q2()
{
    // Carregando sprites da Sherlocka
    HBITMAP img_4_1a = loadBitmapHandle(L"res\\4_1a.bmp");
    HBITMAP img_4_1b = loadBitmapHandle(L"res\\4_1b.bmp");
    HBITMAP img_4_2a = loadBitmapHandle(L"res\\4_2a.bmp");
    HBITMAP img_4_2b = loadBitmapHandle(L"res\\4_2b.bmp");

    // Mostra primeira imagem
    showImage(img_4_1a);

    drawText(
        L"Walter, encontramos mais duas coisas que podem ser pistas por aqui.",
        COLOR_BLACK);
    
    mouthAnim(img_4_1b, img_4_1a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_4_2a);

    drawText(
        L"E são bem esquisitas.",
        COLOR_BLACK);

    mouthAnim(img_4_2b, img_4_2a, 3);

    proceedWhenKeyIsPressed(1, ' ');

    // Carrega sprite da pista
    HBITMAP img_pista_2 = loadBitmapHandle(L"res\\pista_2.bmp");

    // Mostra pistas 2
    processPista(
        img_pista_2,
        L"Bota com mate bagual",
        transicao_q2_q5,
        5,
        L"Gaita lança-chamas",
        transicao_q2_q6,
        6);

    // Deletando imagens
    DeleteObject(img_pista_2);
    DeleteObject(img_4_1a);
    DeleteObject(img_4_1b);
    DeleteObject(img_4_2a);
    DeleteObject(img_4_2b);
}

// Escolheu butina com mate
void transicao_q1_q3()
{
    HBITMAP img_2_p1_cm = loadBitmapHandle(L"res\\2_p1_cm.bmp");
    HBITMAP img_2_p3_cm = loadBitmapHandle(L"res\\2_p3_cm.bmp");
    HBITMAP img_2_p3_om = loadBitmapHandle(L"res\\2_p3_om.bmp");

    showImage(img_2_p1_cm);

    drawText(
        L"Walter: Essa butina aí tá estranha...",
        COLOR_BLACK);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_2_p3_cm);

    drawText(
        L"Verdade, quem toma chimarrão assim? Hummm...",
        COLOR_BLACK);

    mouthAnim(img_2_p3_om, img_2_p3_cm, 5);

    proceedWhenKeyIsPressed(1, ' ');

    HBITMAP img_2_p4_cm = loadBitmapHandle(L"res\\2_p4_cm.bmp");
    HBITMAP img_2_p4_om = loadBitmapHandle(L"res\\2_p4_om.bmp");

    showImage(img_2_p4_cm);

    drawText(
        L"Pera, já sei! É claro!! O Jorge Tamanco!",
        COLOR_BLACK);

    mouthAnim(img_2_p4_om, img_2_p4_cm, 5);

    proceedWhenKeyIsPressed(1, ' ');
    eraseText();

    drawText(
        L"Walter: Vi ele na cozinha! Vamos lá pegá-lo!",
        COLOR_BLACK);

    proceedWhenKeyIsPressed(1, ' ');

    HBITMAP img_na_cozinha = loadBitmapHandle(L"res\\na_cozinha.bmp");

    showImage(img_na_cozinha);

    proceedWhenKeyIsPressed(1, ' ');

    DeleteObject(img_2_p1_cm);
    DeleteObject(img_2_p3_cm);
    DeleteObject(img_2_p3_om);
    DeleteObject(img_2_p4_cm);
    DeleteObject(img_2_p4_om);
    DeleteObject(img_na_cozinha);
}

// Escolheu gaita lança-chamas (FALTA IMPLEMENTAR)
void transicao_q1_q4()
{
}

// CTG/bailao
void q1()
{
    // Carregando sprites da Sherlocka
    // HBITMAP img_2_p1_ce = loadBitmapHandle(L"res\\2_p1_ce.bmp"); // ela zoinho fechado
    HBITMAP img_2_p1_cm = loadBitmapHandle(L"res\\2_p1_cm.bmp");
    HBITMAP img_2_p1_om = loadBitmapHandle(L"res\\2_p1_om.bmp");
    // HBITMAP img_2_p1_smile = loadBitmapHandle(L"res\\2_p1_smile.bmp"); // ela sorrindinho
    HBITMAP img_2_p2_cm = loadBitmapHandle(L"res\\2_p2_cm.bmp");
    HBITMAP img_2_p2_om = loadBitmapHandle(L"res\\2_p2_om.bmp");

    // Mostra primeira imagem
    showImage(img_2_p1_cm);

    drawText(
        L"Walter, encontramos mais duas coisas que podem ser pistas.",
        COLOR_BLACK);

    mouthAnim(img_2_p1_om, img_2_p1_cm, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_2_p2_cm);

    drawText(
        L"São bem estranhas, aliás.",
        COLOR_BLACK);

    mouthAnim(img_2_p2_om, img_2_p2_cm, 3);

    proceedWhenKeyIsPressed(1, ' ');

    // Carrega sprite da pista
    HBITMAP img_pista_2 = loadBitmapHandle(L"res\\pista_2.bmp");

    // Mostra pistas 2
    processPista(
        img_pista_2,
        L"Bota com mate bagual",
        transicao_q1_q3,
        3,
        L"Gaita lança-chamas",
        transicao_q1_q4,
        4);
    

    // Deletando imagens
    DeleteObject(img_pista_2);
    // DeleteObject(img_2_p1_ce);
    DeleteObject(img_2_p1_cm);
    DeleteObject(img_2_p1_om);
    DeleteObject(img_2_p2_cm);
    DeleteObject(img_2_p2_om);
    // DeleteObject(img_2_p1_smile);
}

// Escolheu caderno
void transicao_q0_q2()
{
    HBITMAP img_1_p3_ce = loadBitmapHandle(L"res\\1_p3_ce.bmp"); // Essa é ela de zoinho fechado
    HBITMAP img_1_p4_cm = loadBitmapHandle(L"res\\1_p4_cm.bmp");
    HBITMAP img_1_p4_om = loadBitmapHandle(L"res\\1_p4_om.bmp");
    HBITMAP img_1_p4_smile = loadBitmapHandle(L"res\\1_p4_smile.bmp");
    HBITMAP img_na_ufpel = loadBitmapHandle(L"res\\na_ufpel.bmp");

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

    showImage(img_na_ufpel);

    proceedWhenKeyIsPressed(1, ' ');

    DeleteObject(img_1_p3_ce);
    DeleteObject(img_1_p4_cm);
    DeleteObject(img_1_p4_om);
    DeleteObject(img_1_p4_smile);
    DeleteObject(img_na_ufpel);
}

// Escolheu cuia
void transicao_q0_q1()
{
    HBITMAP img_1_p3_ce = loadBitmapHandle(L"res\\1_p3_ce.bmp"); // Essa é ela de zoinho fechado
    HBITMAP img_1_p3_oe = loadBitmapHandle(L"res\\1_p3_oe.bmp");
    HBITMAP img_1_p4_cm = loadBitmapHandle(L"res\\1_p4_cm.bmp");
    HBITMAP img_1_p4_om = loadBitmapHandle(L"res\\1_p4_om.bmp");
    HBITMAP img_no_bailao = loadBitmapHandle(L"res\\no_bailao.bmp");

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

    showImage(img_no_bailao);

    proceedWhenKeyIsPressed(1, ' ');

    DeleteObject(img_1_p3_ce);
    DeleteObject(img_1_p3_oe);
    DeleteObject(img_1_p4_cm);
    DeleteObject(img_1_p4_om);
    DeleteObject(img_no_bailao);
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
    processPista(
        img_pista_1,
        L"Caderno da Wandinha",
        transicao_q0_q2,
        2,
        L"Cuia de mate",
        transicao_q0_q1,
        1);

    // Deletando imagens
    DeleteObject(img_pista_1);
    DeleteObject(img_1_p1_om);
    DeleteObject(img_1_p1_cm);
    DeleteObject(img_1_p2_om);
    DeleteObject(img_1_p2_cm);
}

void menu()
{
    // Carregando sprite do menu
    HBITMAP img_menu = loadBitmapHandle(L"res\\menu.bmp");

    // Mostra menu
    showImage(img_menu);

    wchar_t c = proceedWhenKeyIsPressed(2, L'1', L'2');

    // Lendo qual opção o jogador escolheu
    switch (c)
    {
    // Escolheu jogar
    case L'1':
        game_state_id = 0;
        break;

    // Escolheu sair
    case L'2':
        PostMessage(args->hwnd, WM_DESTROY, 0, 0);
        game_state_id = SAIR;
        break;
    }

    // Deletando imagem
    DeleteObject(img_menu);
}

DWORD WINAPI processTheGame(LPVOID lpParam)
{
    args = (GameThreadArgs *)lpParam;

    while (1)
    {
        switch (game_state_id)
        {
        // Menu
        case MENU:
            menu();
            break;

        // q0
        case 0:
            q0();
            break;

        // q1
        case 1:
            q1();
            break;

        // q2
        case 2:
            q2();
            break;

        // q3
        case 3:
            q3();
            break;
        }

        if (*args->terminateThread)
            break;
    }
    return 0;
}