//1.É a biblioteca principal do SDL (Simple DirectMedia Layer), que é utilizada para lidar com a
//parte gráfica do programa.
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

//2.Definir as dimensões da janela
const int SCREEN_WIDTH = 450;     //largura
const int SCREEN_HEIGHT = 450;    //altura
const int SQUARE_SIZE = 150;      //tamanho do quadrado

//3.Função para carregar uma textura de um arquivo de imagem
//Ela recebe um ponteiro para o renderizador renderer e o caminho path para o arquivo de imagem
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path){
    // O loadBMP, É Usado para carregar a imagem
    SDL_Surface* surface = SDL_LoadBMP(path);
    //Verifica se houve erro
    if (surface == NULL)
    {
        printf("Failed to load image %s! SDL Error: %s\n", path, SDL_GetError());
        return NULL;
    }
    //cria uma textura a partir da superfície carregada usando SDL_CreateTextureFromSurface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
        printf("Failed to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
    }
    //Libera a superfície
    SDL_FreeSurface(surface);
    return texture;
}

int main(int argc, char* args[]){
    // A janela que vou renderizar
    SDL_Window* window = NULL;

    // O rendenizador
    SDL_Renderer* renderer = NULL;

    //inicia o SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    //5.Cria a janela
    window = SDL_CreateWindow("Jogo da Velha", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    //6.Cria o rendenizador
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    //7.Carrega as texturas
    SDL_Texture* textureQuadrados = loadTexture(renderer, "C:\\Users\\07rya\\Desktop\\meu_jogo\\Quadrados.bmp");
    SDL_Texture* TextureO = loadTexture(renderer, "C:\\Users\\07rya\\Desktop\\meu_jogo\\oo.bmp");
    SDL_Texture* TextureX = loadTexture(renderer, "C:\\Users\\07rya\\Desktop\\meu_jogo\\xx.bmp");
    if (textureQuadrados == NULL || TextureO == NULL || TextureX == NULL){
        //mostra erro ao carregar as texturas
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Rendenizador "textureQuadrados"
    SDL_Rect quadradosRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, textureQuadrados, NULL, &quadradosRect);

    //8.Define as coordenadas dos quadrados
    SDL_Rect quadradoCoords[3][3];
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            //Os valores das coordenadas são calculados com base no tamanho dos quadrados (SQUARE_SIZE)
            quadradoCoords[row][col].x = col * SQUARE_SIZE;
            quadradoCoords[row][col].y = row * SQUARE_SIZE;
            quadradoCoords[row][col].w = SQUARE_SIZE;
            quadradoCoords[row][col].h = SQUARE_SIZE;
        }
    }
    SDL_Texture* jogadorq;
    SDL_Texture* jogadorr;
    SDL_Texture* jogadors;
    SDL_Surface* o;
    SDL_Surface* x;
    SDL_Surface* e;
    int jogador1 = 0, jogador2 = 0;
    int jogadas = 0;
    int win = 0, row, col;
    char matriz_auxiliar[3][3] = { { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' } };
    int gamerun = 1;
    //9.Loop prinicipal do jogo
    while (gamerun==1) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            //se o evento for de "saída"
            if (event.type == SDL_QUIT) {
                //sai do loop e (fecha a janela através dos SDL_Destroys)
                gamerun=0;
                break;
            }
            //se não for e ele clicar com um dos botões do mause
            else if (event.type == SDL_MOUSEBUTTONUP) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                //se foi dentro das dimensões do tabuleiro
                if (mouseX >= 0 && mouseX <= 450 && mouseY >= 0 && mouseY <= 450) {
                    //linha = botão direito/tamanho do quadrado
                    int row = mouseY / SQUARE_SIZE;
                    //coluna = botão esquerdo/tamanho do quadrado
                    int col = mouseX / SQUARE_SIZE;

                    //conta o número de jogadas
                    jogadas++;

                    //se for par
                    if (jogadas % 2 == 0) {
                        //conta as jogadas da bolinha
                        jogador1++;

                        //as coordenadas
                        SDL_Rect destRect = quadradoCoords[row][col];

                        //coloca a textura lá
                        SDL_RenderCopy(renderer, TextureX, NULL, &destRect);

                        //uma matriz auxiliar para ajudar no calculo de quem venceu
                        matriz_auxiliar[row][col] = 'O';
                    }
                    else {
                    //conta as jogadas do X
                        jogador2++;

                        //as coordenadas
                        SDL_Rect destRect = quadradoCoords[row][col];

                         //coloca a textura lá
                        SDL_RenderCopy(renderer, TextureO, NULL, &destRect);

                        //uma matriz auxiliar para ajudar no calculo de quem venceu
                        matriz_auxiliar[row][col] = 'X';
                    }
                }
            }
        }
        SDL_RenderPresent(renderer);
        //10. se o jogador venceu
        if ((matriz_auxiliar[0][0] == 'O' && matriz_auxiliar[0][1] == 'O' && matriz_auxiliar[0][2] == 'O') ||
            (matriz_auxiliar[1][0] == 'O' && matriz_auxiliar[1][1] == 'O' && matriz_auxiliar[1][2] == 'O') ||
            (matriz_auxiliar[2][0] == 'O' && matriz_auxiliar[2][1] == 'O' && matriz_auxiliar[2][2] == 'O') ||
            (matriz_auxiliar[0][0] == 'O' && matriz_auxiliar[1][0] == 'O' && matriz_auxiliar[2][0] == 'O') ||
            (matriz_auxiliar[0][1] == 'O' && matriz_auxiliar[1][1] == 'O' && matriz_auxiliar[2][1] == 'O') ||
            (matriz_auxiliar[0][2] == 'O' && matriz_auxiliar[1][2] == 'O' && matriz_auxiliar[2][2] == 'O') ||
            (matriz_auxiliar[0][0] == 'O' && matriz_auxiliar[1][1] == 'O' && matriz_auxiliar[2][2] == 'O') ||
            (matriz_auxiliar[0][2] == 'O' && matriz_auxiliar[1][1] == 'O' && matriz_auxiliar[2][0] == 'O')) {
            win = 1;
            gamerun = 0;
        }
        //se o jogador 2 venceu
        if ((matriz_auxiliar[0][0] == 'X' && matriz_auxiliar[0][1] == 'X' && matriz_auxiliar[0][2] == 'X') ||
            (matriz_auxiliar[1][0] == 'X' && matriz_auxiliar[1][1] == 'X' && matriz_auxiliar[1][2] == 'X') ||
            (matriz_auxiliar[2][0] == 'X' && matriz_auxiliar[2][1] == 'X' && matriz_auxiliar[2][2] == 'X') ||
            (matriz_auxiliar[0][0] == 'X' && matriz_auxiliar[1][0] == 'X' && matriz_auxiliar[2][0] == 'X') ||
            (matriz_auxiliar[0][1] == 'X' && matriz_auxiliar[1][1] == 'X' && matriz_auxiliar[2][1] == 'X') ||
            (matriz_auxiliar[0][2] == 'X' && matriz_auxiliar[1][2] == 'X' && matriz_auxiliar[2][2] == 'X') ||
            (matriz_auxiliar[0][0] == 'X' && matriz_auxiliar[1][1] == 'X' && matriz_auxiliar[2][2] == 'X') ||
            (matriz_auxiliar[0][2] == 'X' && matriz_auxiliar[1][1] == 'X' && matriz_auxiliar[2][0] == 'X')) {
            win = 2;
            gamerun = 0;
        }
        //se deu empate
        else if (jogadas == 9) {
            win = 3;
            gamerun = 0;
        }
    }
    //destroi a janela
    SDL_DestroyTexture(textureQuadrados);
    SDL_DestroyTexture(TextureO);
    SDL_DestroyTexture(TextureX);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    //11.Criar a janela de vitória
    SDL_Window* janelaVitoria = SDL_CreateWindow("VITÓRIA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 250, 250, SDL_WINDOW_SHOWN);
    SDL_Renderer* rendererVitoria = SDL_CreateRenderer(janelaVitoria, -1, SDL_RENDERER_SOFTWARE);

    SDL_Surface* imgO = SDL_LoadBMP("C:\\Users\\07rya\\Desktop\\meu_jogo\\jogador 'o'.bmp");
    SDL_Surface* imgX = SDL_LoadBMP("C:\\Users\\07rya\\Desktop\\meu_jogo\\jogador 'x'.bmp");
    SDL_Surface* imgEmpate = SDL_LoadBMP("C:\\Users\\07rya\\Desktop\\meu_jogo\\EMPATE.bmp");

    SDL_Texture* texturaO = SDL_CreateTextureFromSurface(rendererVitoria, imgO);
    SDL_Texture* texturaX = SDL_CreateTextureFromSurface(rendererVitoria, imgX);
    SDL_Texture* texturaEmpate = SDL_CreateTextureFromSurface(rendererVitoria, imgEmpate);

    //11.1.Carregar imagens de acordo com o resultado do jogo
    //se 'O' venceu
    if (win == 1) {
        //coloca a textura lá "O JOGADOR O VENCEU"
        SDL_RenderCopy(rendererVitoria, texturaX, NULL, NULL);
    }
    //se 'X' venceu
    else if (win == 2) {
        //coloca a textura lá "O JOGADOR X VENCEU"
        SDL_RenderCopy(rendererVitoria, texturaO, NULL, NULL);
    }
    else if (win == 3) {
        SDL_RenderCopy(rendererVitoria, texturaEmpate, NULL, NULL);
    }

    SDL_RenderPresent(rendererVitoria);
    int vitoriaAberta = 1;
    SDL_Event eventar;
    while (vitoriaAberta) {
        while (SDL_PollEvent(&eventar)) {
            if (eventar.type == SDL_QUIT) {
                vitoriaAberta = 0;  // Fechar a janela de vitória
                break;
            }
        SDL_Delay(10);
        }
    }

    //12.Destroi a janela vitória
    SDL_DestroyTexture(texturaO);
    SDL_DestroyTexture(texturaX);
    SDL_DestroyTexture(texturaEmpate);
    SDL_FreeSurface(imgO);
    SDL_FreeSurface(imgX);
    SDL_FreeSurface(imgEmpate);
    SDL_DestroyRenderer(rendererVitoria);
    SDL_DestroyWindow(janelaVitoria);
    SDL_Quit();
    return 0;
}
