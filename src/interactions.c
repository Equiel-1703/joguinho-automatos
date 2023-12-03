#include "../include/interactions.h"
#include "../include/graphics.h"
#include "../include/console_interface.h"

#include <stdio.h>
#include <strsafe.h>
#include <stdarg.h>
#include <time.h>

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
    LPWSTR pista_2,
    void (*transicao_2)())
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
        break;

    // Escolheu pista 2
    case L'2':
        drawText(pista_1_buf, COLOR_BLACK);
        drawText(pista_2_buf, COLOR_BLUE);

        timer(pistas_delay);

        transicao_2();
        break;
    }
}

wchar_t simNaoPrompt(LPWSTR prompt)
{
    drawText(prompt, COLOR_BLACK);

    LPWSTR sim = L"\nS - Sim";
    LPWSTR nao = L"\n\nN - Não";

    drawText(sim, COLOR_BLACK);
    drawText(nao, COLOR_BLACK);

    // Lê resposta do jogador
    wchar_t c = proceedWhenKeyIsPressed(2, L'S', L'N');

    switch (c)
    {
    case L'S':
        drawText(sim, COLOR_BLUE);
        drawText(nao, COLOR_BLACK);

        timer(pistas_delay);
        break;

    case L'N':
        drawText(sim, COLOR_BLACK);
        drawText(nao, COLOR_BLUE);

        timer(pistas_delay);
        break;
    }

    return c;
}

// Prende o Gaiteiro da Meia-Noite
void q4()
{
    // Carrega bitmaps 7
    HBITMAP img_7_1 = loadBitmapHandle(L"res\\7_1.bmp");
    HBITMAP img_7_2 = loadBitmapHandle(L"res\\7_2.bmp");
    HBITMAP img_7_3 = loadBitmapHandle(L"res\\7_3.bmp");
    HBITMAP img_7_4 = loadBitmapHandle(L"res\\7_4.bmp");
    // 7 5a
    HBITMAP img_7_5a = loadBitmapHandle(L"res\\7_5a.bmp");
    HBITMAP img_7_5b = loadBitmapHandle(L"res\\7_5b.bmp");
    HBITMAP img_7_5c = loadBitmapHandle(L"res\\7_5c.bmp");
    // 7 6a
    HBITMAP img_7_6a = loadBitmapHandle(L"res\\7_6a.bmp");
    HBITMAP img_7_6b = loadBitmapHandle(L"res\\7_6b.bmp");
    // 7 7a
    HBITMAP img_7_7a = loadBitmapHandle(L"res\\7_7a.bmp");
    HBITMAP img_7_7b = loadBitmapHandle(L"res\\7_7b.bmp");
    // 7 8
    HBITMAP img_7_8 = loadBitmapHandle(L"res\\7_8.bmp");

    // Carrega imagem de you win
    HBITMAP img_you_win = loadBitmapHandle(L"res\\you_win.bmp");

    // Mostra primeira imagem
    showImage(img_7_1);

    drawText(
        L"Gaiteiro, aqui é a polícia! Você está preso!\nSabemos que foi você que assaltou aquela casa.",
        COLOR_BLACK);
    
    mouthAnim(img_7_2, img_7_1, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_7_3);

    drawText(
        L"Olá de novo, Sherlocka! Eu estava mesmo esperando por você, o mate até esfriou.",
        COLOR_BLACK);
    
    proceedWhenKeyIsPressed(1, ' ');

    eraseText();

    drawText(
        L"Finalmente poderemos acertar as contas do nosso último encontro.",
        COLOR_BLACK);
    
    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_7_4);

    drawText(
        L"Me prenda se puder! hahahaha",
        COLOR_BLACK);

    proceedWhenKeyIsPressed(1, ' ');

    // Sequencia de luta
    srand(time(NULL));

    RECT rect;
    GetClientRect(GetDesktopWindow(), &rect);

    const int limit_x = rect.right - WND_W;
    const int limit_y = rect.bottom - WND_H;

    moveGameWindow(rand() % limit_x, rand() % limit_y);
    showImage(img_7_5a);

    timer(2 * pistas_delay);

    moveGameWindow(rand() % limit_x, rand() % limit_y);
    showImage(img_7_5b);

    timer(2 * pistas_delay);

    moveGameWindow(rand() % limit_x, rand() % limit_y);
    showImage(img_7_5c);

    timer(2 * pistas_delay);

    moveGameWindow(rand() % limit_x, rand() % limit_y);
    showImage(img_7_6a);

    drawText(
        L"Teje preso seu sem-vergonho!\nVai tocar gaita é no presídio!",
        COLOR_BLACK);

    mouthAnim(img_7_6b, img_7_6a, 10);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_7_7a);

    drawText(
        L"A justiça triunfou mais uma vez, Walter!\nObrigada pela ajuda, adoro ter você como parceiro!",
        COLOR_BLACK);
    
    mouthAnim(img_7_7b, img_7_7a, 10);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_7_8); // Imagem fofa do final

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_you_win);

    proceedWhenKeyIsPressed(1, ' ');

    game_state_id = -1; // volta pro menu

    // Deleta bitmaps
    DeleteObject(img_7_1);
    DeleteObject(img_7_2);
    DeleteObject(img_7_3);
    DeleteObject(img_7_4);
    DeleteObject(img_7_5a);
    DeleteObject(img_7_5b);
    DeleteObject(img_7_5c);
    DeleteObject(img_7_6a);
    DeleteObject(img_7_6b);
    DeleteObject(img_7_7a);
    DeleteObject(img_7_7b);
    DeleteObject(img_7_8);
    DeleteObject(img_you_win);

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

// Prende Marilton S. de Aguiar
void q5()
{
    // Carrega bitmaps 5
    HBITMAP img_5_1a = loadBitmapHandle(L"res\\5_1a.bmp");
    HBITMAP img_5_1b = loadBitmapHandle(L"res\\5_1b.bmp");
    HBITMAP img_5_2a = loadBitmapHandle(L"res\\5_2a.bmp");
    HBITMAP img_5_2b = loadBitmapHandle(L"res\\5_2b.bmp");
    HBITMAP img_5_2c = loadBitmapHandle(L"res\\5_2c.bmp");
    HBITMAP img_5_3a = loadBitmapHandle(L"res\\5_3a.bmp");
    HBITMAP img_5_3b = loadBitmapHandle(L"res\\5_3b.bmp");
    HBITMAP img_5_4a = loadBitmapHandle(L"res\\5_4a.bmp");
    HBITMAP img_5_4b = loadBitmapHandle(L"res\\5_4b.bmp");

    // Mostra primeira imagem
    showImage(img_5_1a);

    drawText(
        L"Professor, você está preso por assalto e invasão de propriedade!\nAchamos seu caderno e sua bota de chimarrão.",
        COLOR_BLACK);

    mouthAnim(img_5_1b, img_5_1a, 8);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_5_2b);

    drawText(
        L"Como assim? Esse caderno não é meu, essa série é muito ruim.\nE eu não tomo chimarrão numa bota, isso é muito estranho.",
        COLOR_BLACK);

    mouthAnim(img_5_2a, img_5_2b, 8);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_5_2c);

    drawText(
        L"Fica bem melhor no chapéu.",
        COLOR_BLACK);

    mouthAnim(img_5_2a, img_5_2c, 3);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_5_3a);

    drawText(
        L"E você não devia estar em aula não, Sherlocka?\nMe disseram que você já tem 12 faltas.",
        COLOR_BLACK);

    mouthAnim(img_5_3b, img_5_3a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_5_4a);

    drawText(
        L"Ahh, o pessoal fala pelos cotovelos, professor, hehe.\nSabe como é, né? hehe.",
        COLOR_BLACK);

    mouthAnim(img_5_4b, img_5_4a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    eraseText();

    drawText(
        L"Acho que estão me chamando, professor. Melhor eu já ir. Desculpa o incômodo.",
        COLOR_BLACK);

    mouthAnim(img_5_4a, img_5_4b, 5);

    proceedWhenKeyIsPressed(1, ' ');

    HBITMAP img_you_lose = loadBitmapHandle(L"res\\you_lose.bmp");

    showImage(img_you_lose);

    proceedWhenKeyIsPressed(1, ' ');

    game_state_id = -1; // volta pro menu

    // Deleta bitmaps
    DeleteObject(img_you_lose);
    DeleteObject(img_5_1a);
    DeleteObject(img_5_1b);
    DeleteObject(img_5_2a);
    DeleteObject(img_5_2b);
    DeleteObject(img_5_2c);
    DeleteObject(img_5_3a);
    DeleteObject(img_5_3b);
    DeleteObject(img_5_4a);
    DeleteObject(img_5_4b);
}

// Prende Anderson Ferrugem
void q6()
{
    // Carrega bitmaps 6
    HBITMAP img_6_1a = loadBitmapHandle(L"res\\6_1a.bmp");
    HBITMAP img_6_1b = loadBitmapHandle(L"res\\6_1b.bmp");
    HBITMAP img_6_2a = loadBitmapHandle(L"res\\6_2a.bmp");
    HBITMAP img_6_2b = loadBitmapHandle(L"res\\6_2b.bmp");
    HBITMAP img_6_3a = loadBitmapHandle(L"res\\6_3a.bmp");
    HBITMAP img_6_3b = loadBitmapHandle(L"res\\6_3b.bmp");
    HBITMAP img_6_4a = loadBitmapHandle(L"res\\6_4a.bmp");
    HBITMAP img_6_4b = loadBitmapHandle(L"res\\6_4b.bmp");
    HBITMAP img_6_4c = loadBitmapHandle(L"res\\6_4c.bmp");
    // carrega imagem de você perdeu
    HBITMAP img_you_lose = loadBitmapHandle(L"res\\you_lose.bmp");

    // Mostra primeira imagem
    showImage(img_6_1a);

    drawText(
        L"Professor, você está preso por assalto e invasão de propriedade!\nAchamos seu caderno e sua gaita lança-chamas!",
        COLOR_BLACK);

    mouthAnim(img_6_1b, img_6_1a, 8);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_6_2a);

    drawText(
        L"Como é que é? Gente esse caderno não é meu, essa letra é horrível. E como assim gaita?",
        COLOR_BLACK);

    mouthAnim(img_6_2b, img_6_2a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_6_3b);

    drawText(
        L"Você tem um projeto de gaitas não tem?",
        COLOR_BLACK);

    mouthAnim(img_6_3a, img_6_3b, 3);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_6_2a);

    drawText(
        L"Gaita? Ahh, você tá falando do GAIA!\nÉ um projeto de inteligência artificial que eu tenho, mas não tem nada a ver com gaitas.",
        COLOR_BLACK);

    mouthAnim(img_6_2b, img_6_2a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    eraseText();

    drawText(
        L"Eu não entendo nada de gaita, apesar que eu adoraria ter uma gaita que solta fogo.",
        COLOR_BLACK);

    mouthAnim(img_6_2b, img_6_2a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    eraseText();

    drawText(
        L"O máximo que eu consegui fazer foi colocar um lança-chamas na minha cafeteira. Veja só.",
        COLOR_BLACK);

    mouthAnim(img_6_2b, img_6_2a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    timer(3 * frame_len);

    showImage(img_6_4a);

    timer(3 * frame_len);

    mouthAnim(img_6_4b, img_6_4c, 10);

    showImage(img_you_lose);

    proceedWhenKeyIsPressed(1, ' ');

    game_state_id = -1; // volta pro menu

    // Deleta bitmaps
    DeleteObject(img_you_lose);
    DeleteObject(img_6_1a);
    DeleteObject(img_6_1b);
    DeleteObject(img_6_2a);
    DeleteObject(img_6_2b);
    DeleteObject(img_6_3a);
    DeleteObject(img_6_3b);
    DeleteObject(img_6_4a);
    DeleteObject(img_6_4b);
    DeleteObject(img_6_4c);
}

// Escolheu bota com mate
void transicao_q2_q5()
{
    HBITMAP img_4_1a = loadBitmapHandle(L"res\\4_1a.bmp");
    HBITMAP img_4_1b = loadBitmapHandle(L"res\\4_1b.bmp");
    HBITMAP img_4_2a = loadBitmapHandle(L"res\\4_3a.bmp");
    HBITMAP img_4_2b = loadBitmapHandle(L"res\\4_3b.bmp");
    HBITMAP img_4_5a = loadBitmapHandle(L"res\\4_5a.bmp");
    HBITMAP img_4_5b = loadBitmapHandle(L"res\\4_5b.bmp");

    showImage(img_4_1a);

    drawText(
        L"Walter: Humm, essa bota é muito estranha.",
        COLOR_BLACK);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_4_2a);

    drawText(
        L"Pensei a mesma coisa. Tem que gostar muito de chimarrão pra tomar assim, hein.",
        COLOR_BLACK);

    mouthAnim(img_4_2b, img_4_2a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_4_5a);

    drawText(
        L"Pera, já sei! O Marilton!!\nSempre vejo ele tomando chimarrão e ele é professor de computação, uma área de exatas!",
        COLOR_BLACK);

    mouthAnim(img_4_5b, img_4_5a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    eraseText();

    drawText(
        L"Walter: Vamos no CDTec!",
        COLOR_BLACK);

    proceedWhenKeyIsPressed(1, ' ');

    // Oferece trocar de pista
    showImage(img_4_1a);

    drawText(
        L"Espera! Antes de ir, será que não devíamos dar uma investigada naquela cuia? Tô com um pressentimento.",
        COLOR_BLACK);

    mouthAnim(img_4_1b, img_4_1a, 10);

    proceedWhenKeyIsPressed(1, ' ');

    eraseText();

    wchar_t c = simNaoPrompt(L"O que você acha?");

    switch (c)
    {
    case L'S':
        eraseText();

        drawText(
            L"Walter: Acho que seria uma boa, vamos lá.",
            COLOR_BLACK);

        proceedWhenKeyIsPressed(1, ' ');

        HBITMAP img_no_bailao = loadBitmapHandle(L"res\\no_bailao.bmp");

        showImage(img_no_bailao);

        proceedWhenKeyIsPressed(1, ' ');

        DeleteObject(img_no_bailao);

        // Vai pro q1 (CTG/bailao)
        game_state_id = 1;
        break;

    case L'N':
        eraseText();

        drawText(
            L"Walter: Não, não. Vamos pro CDTec!",
            COLOR_BLACK);

        proceedWhenKeyIsPressed(1, ' ');

        HBITMAP img_no_cdtec = loadBitmapHandle(L"res\\no_cdtec.bmp");

        showImage(img_no_cdtec);

        proceedWhenKeyIsPressed(1, ' ');

        DeleteObject(img_no_cdtec);

        // Vai pro q5 (CDTec)
        game_state_id = 5;
        break;
    }

    // Deleta bitmaps
    DeleteObject(img_4_1a);
    DeleteObject(img_4_2a);
    DeleteObject(img_4_2b);
    DeleteObject(img_4_5a);
    DeleteObject(img_4_5b);
}

// Escolheu gaita lança-chamas
void transicao_q2_q6()
{
    HBITMAP img_4_1a = loadBitmapHandle(L"res\\4_1a.bmp");
    HBITMAP img_4_1b = loadBitmapHandle(L"res\\4_1b.bmp");
    HBITMAP img_4_2a = loadBitmapHandle(L"res\\4_2a.bmp");
    HBITMAP img_4_2b = loadBitmapHandle(L"res\\4_2b.bmp");
    HBITMAP img_4_3a = loadBitmapHandle(L"res\\4_3a.bmp");
    HBITMAP img_4_3b = loadBitmapHandle(L"res\\4_3b.bmp");
    HBITMAP img_4_5a = loadBitmapHandle(L"res\\4_5a.bmp");
    HBITMAP img_4_5b = loadBitmapHandle(L"res\\4_5b.bmp");

    showImage(img_4_1a);

    drawText(
        L"Walter: Nossa, que gaita maluca é essa?",
        COLOR_BLACK);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_4_2a);

    drawText(
        L"Pois é, esse instrumento é no mínimo exótico.",
        COLOR_BLACK);

    mouthAnim(img_4_2b, img_4_2a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_4_3a);

    drawText(
        L"Quem será que teria uma gaita assim?\nHummm ",
        COLOR_BLACK);

    mouthAnim(img_4_3b, img_4_3a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_4_5a);

    drawText(
        L"Já sei!! O Ferrugem!!\nEle dá aula de computação e eu sei que ele tem um projeto sobre gaitas!",
        COLOR_BLACK);

    mouthAnim(img_4_5b, img_4_5a, 5);

    proceedWhenKeyIsPressed(1, ' ');

    eraseText();

    drawText(
        L"Walter: Boa, guria!\nVamos até a sala dele. Eu sei onde é.",
        COLOR_BLACK);

    proceedWhenKeyIsPressed(1, ' ');

    // Oferece trocar de pista
    showImage(img_4_1a);

    drawText(
        L"Ótimo! Mas antes de ir, será que não devíamos dar uma investigada naquela cuia?",
        COLOR_BLACK);

    mouthAnim(img_4_1b, img_4_1a, 10);

    proceedWhenKeyIsPressed(1, ' ');

    eraseText();

    wchar_t c = simNaoPrompt(L"O que você acha?");

    switch (c)
    {
    case L'S':
        eraseText();

        drawText(
            L"Walter: Acho que você tem razão, vamos lá.",
            COLOR_BLACK);

        proceedWhenKeyIsPressed(1, ' ');

        HBITMAP img_no_bailao = loadBitmapHandle(L"res\\no_bailao.bmp");

        showImage(img_no_bailao);

        proceedWhenKeyIsPressed(1, ' ');

        DeleteObject(img_no_bailao);

        // Vai pro q1 (CTG/bailao)
        game_state_id = 1;
        break;

    case L'N':
        eraseText();

        drawText(
            L"Walter: Não, não. Vamos direto pra sala do Ferrugem.",
            COLOR_BLACK);

        proceedWhenKeyIsPressed(1, ' ');

        HBITMAP img_no_gaia = loadBitmapHandle(L"res\\no_gaia.bmp");

        showImage(img_no_gaia);

        proceedWhenKeyIsPressed(1, ' ');

        DeleteObject(img_no_gaia);

        // Vai pro q6 (Sala do Ferrugem)
        game_state_id = 6;
        break;
    }

    // Deleta bitmaps
    DeleteObject(img_4_1a);
    DeleteObject(img_4_2a);
    DeleteObject(img_4_2b);
    DeleteObject(img_4_3a);
    DeleteObject(img_4_3b);
    DeleteObject(img_4_5a);
    DeleteObject(img_4_5b);
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
        L"Gaita lança-chamas",
        transicao_q2_q6);

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

    // vai pra q3 (cozinha)
    game_state_id = 3;
}

// Escolheu gaita lança-chamas
void transicao_q1_q4()
{
    // Carrega bitmaps
    HBITMAP img_2_p1_cm = loadBitmapHandle(L"res\\2_p1_cm.bmp");
    HBITMAP img_2_p1_ce = loadBitmapHandle(L"res\\2_p1_ce.bmp");
    HBITMAP img_2_p1_om = loadBitmapHandle(L"res\\2_p1_om.bmp");
    HBITMAP img_2_p2_cm = loadBitmapHandle(L"res\\2_p2_cm.bmp");
    HBITMAP img_2_p2_om = loadBitmapHandle(L"res\\2_p2_om.bmp");
    HBITMAP img_2_p3_cm = loadBitmapHandle(L"res\\2_p3_cm.bmp");
    HBITMAP img_2_p3_om = loadBitmapHandle(L"res\\2_p3_om.bmp");
    HBITMAP img_2_p4_cm = loadBitmapHandle(L"res\\2_p4_cm.bmp");
    HBITMAP img_2_p4_om = loadBitmapHandle(L"res\\2_p4_om.bmp");
    HBITMAP img_2_p1_smile = loadBitmapHandle(L"res\\2_p1_smile.bmp");

    // Mostra primeira imagem
    showImage(img_2_p1_cm);

    drawText(
        L"Walter: Essa gaita aí é muito suspeita. Esse negócio solta fogo!",
        COLOR_BLACK);

    eyeAnim(img_2_p1_cm, img_2_p1_ce, 2);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_2_p2_cm);

    drawText(
        L"Tem razão, isso certamente foi usado para invadir a residência! Eu lembro de ter visto marcas de queimado pela casa.",
        COLOR_BLACK);

    mouthAnim(img_2_p2_om, img_2_p2_cm, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_2_p3_cm);

    drawText(
        L"Mas pra quem nossas pistas apontam?\nUma cuia, e uma gaita que solta fogo. Hummm...",
        COLOR_BLACK);

    mouthAnim(img_2_p3_om, img_2_p3_cm, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_2_p4_cm);

    drawText(
        L"Já sei! Só podia ser!!",
        COLOR_BLACK);
    
    drawText(
        L"\nO Gaiteiro da Meia-Noite!!",
        COLOR_RED);

    mouthAnim(img_2_p4_om, img_2_p4_cm, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_2_p1_smile);

    drawText(
        L"Walter: Ontem na delegacia fizeram uma denúncia dizendo que ouviram sons de gaita tocando Amigo Punk e um incêndio!",
        COLOR_BLACK);
    
    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_2_p2_cm);

    drawText(
        L"Finalmente ele aprendeu uma música nova. Não aguentava mais Marília Mendonça.",
        COLOR_BLACK);

    mouthAnim(img_2_p2_om, img_2_p2_cm, 5);

    proceedWhenKeyIsPressed(1, ' ');

    showImage(img_2_p1_smile);

    drawText(
        L"Pega o endereço e vamos lá!",
        COLOR_BLACK);
    
    mouthAnim(img_2_p1_om, img_2_p1_smile, 5);

    proceedWhenKeyIsPressed(1, ' ');

    // Carrega image de "no covil do gaiteiro"
    HBITMAP img_no_covil = loadBitmapHandle(L"res\\no_covil.bmp");

    showImage(img_no_covil);

    proceedWhenKeyIsPressed(1, ' ');

    DeleteObject(img_no_covil);

    // Deleta os bitmaps
    DeleteObject(img_2_p1_smile);
    DeleteObject(img_2_p1_cm);
    DeleteObject(img_2_p1_ce);
    DeleteObject(img_2_p1_om);
    DeleteObject(img_2_p2_cm);
    DeleteObject(img_2_p2_om);
    DeleteObject(img_2_p3_cm);
    DeleteObject(img_2_p3_om);
    DeleteObject(img_2_p4_cm);
    DeleteObject(img_2_p4_om);

    // Vai pro q4 (covil do gaiteiro)
    game_state_id = 4;
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
        L"Gaita lança-chamas",
        transicao_q1_q4);

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

    // Vai pro q2 (UFPel)
    game_state_id = 2;
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

    // Vai pro q1 (CTG/bailao)
    game_state_id = 1;
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
        L"Cuia de mate",
        transicao_q0_q1);

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

        // q4 (fim correto do jogo)
        case 4:
            q4();
            break;

        // q5
        case 5:
            q5();
            break;

        // q6
        case 6:
            q6();
            break;
        }

        if (*args->terminateThread)
            break;
    }
    return 0;
}