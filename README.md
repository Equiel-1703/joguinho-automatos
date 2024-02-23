# Sherlocka & Walter
## A ideia do jogo
Esse jogo foi elaborado por Henrique Rodrigues e a Prof. Dra. Luciana Foss para a disciplina de Linguagens Formais, dos cursos de Ciência e Engenharia da Computação na UFPel. A nossa ideia era que os alunos tivessem a experiência de modelar um problema com autômato e implementá-lo em código.

O uso de um jogo para chamar a atenção dos alunos e inspirá-los funcionou. Alguns até relataram que fizeram o trabalho porque gostaram do jogo. Isso foi muito gratificante.

## O autômato
Este é o autômato finito e determinístico que descreve o comportamento do jogo:

![Automato_Jogo](https://github.com/Equiel-1703/joguinho-automatos/assets/96885946/a4f81e2f-6303-42f7-80cc-e726c9d70b44)

Uma coisa muito comum que os alunos fizeram foi não colocar o estado de derrota como um estado final no autômato. Apesar de parecer contraintuitivo, o estado de derrota é um estado final porque mesmo o jogador perdendo ele ainda realizou uma sequência de escolhas válidas dentro do jogo. Por isso, o autômato deve reconhecer e aceitar essa palavra (que representa uma sequência de escolhas).

## O código
O jogo todo foi feito na linguagem C, usando a API do Windows para a interface gráfica. O código foi organizado da seguinte maneira:
* main.c -> contém a função principal do programa
* graphics.c -> contém funções usadas para as rotinas de tela do programa: desenhar imagem na tela, desenhar texto, apagar etc.
* interactions.c -> aqui estão todas as funções de interação do jogo, diálogos e a implementação do autômato na função processTheGame.
* console_interface.c -> esse arquivo contém algumas funções que foram usadas para debug no terminal durante a implementação. Elas não foram usadas no jogo final.

Esse código é um pouco complexo, no sentido que usa muitos ponteiros, multithreading e alocação dinâmica de memória. Mas lendo a documentação da API do Windows tudo começa a fazer sentido. Espero que isso inspire mais pessoas a fazer projetos doidos como esse.

A maior desvantagem de fazer esse jogo em C certamente foi não ter os recursos da programação orientada a objetos. Isso teria facilitado muita coisa e deixado o código mais limpo. No mais, foi uma experiência incrível que me ensinou muita coisa nova.

## Compilando
Se você quiser compilar o jogo manualmente, você precisará do compilador Microsoft Visual C++ (MSVC). Ele vem junto com o Microsoft Build Tools, que você pode instalar com [Visual Studio Installer](https://visualstudio.microsoft.com/downloads/):

![Print_VSInstaller](https://github.com/Equiel-1703/joguinho-automatos/assets/96885946/648c8515-a3b8-4e8e-8577-0ee944ebb67f)

Com essas ferramentas instaladas, você deverá abrir o Developer Command Prompt (não é o CMD normal do Windows, esse é o que vem com o Build Tools) e executar o .bat na pasta do jogo com a configuração que você desejar. Eu fiz dois: build-debug.bat e build-release.bat.

A diferença entre eles é que o debug gera um executável mais pesado (e menos otimizado) porque possui informações extras no binário para debugar e corrigir erros. O release gera um executável mais otimizado e leve, esse é para gerar o produto final mesmo.
