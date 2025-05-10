#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ALTURA 64
#define LARGURA 128

#define NUMERO_DE_BOLAS 5

struct Bola {
    int posY;
    int posX;
    int ativa;
};
typedef struct Bola Bola;

char display[ALTURA][LARGURA];

void sleep_ms(int milliseconds)
{
    // Convert milliseconds to microseconds
    usleep(milliseconds * 1000);
}

void inicializaDisplay(){
    for(int i = 0; i < ALTURA; i++){
        for(int j = 0; j < LARGURA; j++){
            display[i][j] = ' ';
        }
    }
}

void imprimeDisplay(){
    for(int i = 0; i < LARGURA+2; i++)
        printf("%c", 205);
    printf("\n");

    for(int i = 0; i < ALTURA; i++){
        printf("%c", 186);
        for(int j = 0; j < LARGURA; j++){
            printf("%c", display[i][j]);
        }
        printf("%c", 186);
        printf("\n");
    }

    for(int i = 0; i < LARGURA+2; i++)
        printf("%c", 205);
    printf("\n");
}

void inserirPino(int y, int x){
    display[y][x] = 254;
}

void inserirBola(int y, int x){
    display[y-1][x-1] = 201; // superior esquerdo
    display[y-1][x] = 205; // superior centro
    display[y-1][x+1] = 187; // superior direito

    display[y][x-1] = 186; // centro esquerdo
    display[y][x] = 169; // centro centro
    display[y][x+1] = 186; // centro direito
    
    display[y+1][x-1] = 200; // inferior esquerdo
    display[y+1][x] = 205; // inferior centro
    display[y+1][x+1] = 188; // inferior direito
}

void limparBola(int y_bola, int x_bola){
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            display[y_bola + i][x_bola + j] = ' ';
        }
    }
}

// Retorna 1 se teve colisão, retorna 0 se não teve colisão
int moverBola(int* y_bola, int* x_bola, int dy, int dx){
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(display[*y_bola + dy + i][*x_bola + dx + j] == -2){
                return 1;
            }
        }
    }
    limparBola(*y_bola, *x_bola);
    *y_bola += dy;
    *x_bola += dx;
    inserirBola(*y_bola, *x_bola);
    return 0;
}

int gerarAleatorio(){ // 0 esquerda - 1 direita
    return rand() % 2;
}

int contarPonto(int pos){
    if(pos < LARGURA/4 - 3*LARGURA/16 + 4)
        return 0;
    if(pos < LARGURA/4 - 1*LARGURA/16 + 4)
        return 1;
    if(pos < LARGURA/4 + 1*LARGURA/16 + 4)
        return 2;
    if(pos < LARGURA/4 + 3*LARGURA/16 + 4)
        return 3;
    return 4;
}

void inicializarPinos(){
    inserirPino(ALTURA/2 + 0*ALTURA/8 - 1, LARGURA/4 + 0*LARGURA/16 + 4);
    inserirPino(ALTURA/2 + 1*ALTURA/8 - 1, LARGURA/4 - 1*LARGURA/16 + 4);
    inserirPino(ALTURA/2 + 1*ALTURA/8 - 1, LARGURA/4 + 1*LARGURA/16 + 4);
    inserirPino(ALTURA/2 + 2*ALTURA/8 - 1, LARGURA/4 - 2*LARGURA/16 + 4);
    inserirPino(ALTURA/2 + 2*ALTURA/8 - 1, LARGURA/4 + 0*LARGURA/16 + 4);
    inserirPino(ALTURA/2 + 2*ALTURA/8 - 1, LARGURA/4 + 2*LARGURA/16 + 4);
    inserirPino(ALTURA/2 + 3*ALTURA/8 - 1, LARGURA/4 - 3*LARGURA/16 + 4);
    inserirPino(ALTURA/2 + 3*ALTURA/8 - 1, LARGURA/4 - 1*LARGURA/16 + 4);
    inserirPino(ALTURA/2 + 3*ALTURA/8 - 1, LARGURA/4 + 1*LARGURA/16 + 4);
    inserirPino(ALTURA/2 + 3*ALTURA/8 - 1, LARGURA/4 + 3*LARGURA/16 + 4);
}

int main(void){
    inicializaDisplay();

    Bola* bolas[NUMERO_DE_BOLAS];
    for(int i = 0; i < NUMERO_DE_BOLAS; i++){
        Bola* b = (Bola*) malloc(sizeof(Bola));
        b->posX = LARGURA/4 + 4;
        b->posY = 3 + 6*i;
        b->ativa = 1;
        bolas[i] = b;
    }

    srand(time(NULL));   // Initialization, should only be called once.

    inicializarPinos();
    
    for(int i = 0; i < NUMERO_DE_BOLAS; i++)
        inserirBola(bolas[i]->posY, bolas[i]->posX);
    
    imprimeDisplay();
    int num_de_bolas = NUMERO_DE_BOLAS;
    int pontos[] = {0, 0, 0, 0, 0};

    char flag = 0;

    while(1){
        if(flag == 1) break;
        for(int i = 0; i < NUMERO_DE_BOLAS; i++){
            if(bolas[i]->ativa == 1){
                if(bolas[i]->posY >= ALTURA-2){
                    num_de_bolas--;
                    bolas[i]->ativa = 0;
                    pontos[contarPonto(bolas[i]->posX)]++;
                    if(num_de_bolas == 0){
                        flag = 1;
                    }
                }
                if(moverBola(&bolas[i]->posY, &bolas[i]->posX, 2, 0)){
                    if(gerarAleatorio())
                        moverBola(&bolas[i]->posY, &bolas[i]->posX, 0, (int)LARGURA/16);
                    else
                        moverBola(&bolas[i]->posY, &bolas[i]->posX, 0, (int)-LARGURA/16);
                }
            }
        }
        sleep_ms(1);
        system("cls");
        imprimeDisplay();
    }
    
    for(int i = 0; i < NUMERO_DE_BOLAS; i++)
        free(bolas[i]);

    printf("Pontuacao:\n");
    for(int i = 0; i < 5; i++)
        printf("Canal %d: %d bolas\n", i+1, pontos[i]);
    
    return 0;
}