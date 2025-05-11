#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/rand.h"
#include "./include/Display_Oled.h"

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

void inicializaDisplay(){
    for(int i = 0; i < ALTURA; i++){
        for(int j = 0; j < LARGURA; j++){
            display[i][j] = ' ';
        }
    }
}

void imprimeDisplay(){
    // for(int i = 0; i < LARGURA+2; i++)
    //     printf("%c", 205);
    // printf("\n");
    // char display_str[] = "";
    for(int i = 0; i < ALTURA; i++){
        // printf("%c", 186);
        for(int j = 0; j < LARGURA; j++){
            // sprintf(cont_str, "%c", contagem);
            oled_pixel(j, i, display[i][j]);
            // printf("%c", display[i][j]);
        }
        // printf("%c", 186);
        // printf("\n");
    }

    // for(int i = 0; i < LARGURA+2; i++)
    //     printf("%c", 205);
    // printf("\n");
}

void inserirPino(int y, int x){
    display[y][x] = 254;
}

void inserirBola(int y, int x){
    display[y-1][x-1] = 'I'; // superior esquerdo
    display[y-1][x] = 'I'; // superior centro
    display[y-1][x+1] = 'I'; // superior direito

    display[y][x-1] = 'I'; // centro esquerdo
    display[y][x] = 'I'; // centro centro
    display[y][x+1] = 'I'; // centro direito
    
    display[y+1][x-1] = 'I'; // inferior esquerdo
    display[y+1][x] = 'I'; // inferior centro
    display[y+1][x+1] = 'I'; // inferior direito
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
            if(display[*y_bola + dy + i][*x_bola + dx + j] == 254){
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
    uint8_t randValue = (uint8_t)get_rand_32();
    return (int)(randValue & 1);
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
    inserirPino(ALTURA/2 + 0*ALTURA/8 - 1, LARGURA/4 + 0*LARGURA/16 + 4); //    .

    inserirPino(ALTURA/2 + 1*ALTURA/8 - 1, LARGURA/4 - 1*LARGURA/16 + 4); //   .
    inserirPino(ALTURA/2 + 1*ALTURA/8 - 1, LARGURA/4 + 1*LARGURA/16 + 4); //     .
    
    inserirPino(ALTURA/2 + 2*ALTURA/8 - 1, LARGURA/4 - 2*LARGURA/16 + 4); //  .
    inserirPino(ALTURA/2 + 2*ALTURA/8 - 1, LARGURA/4 + 0*LARGURA/16 + 4); //    .
    inserirPino(ALTURA/2 + 2*ALTURA/8 - 1, LARGURA/4 + 2*LARGURA/16 + 4); //      .
    
    inserirPino(ALTURA/2 + 3*ALTURA/8 - 1, LARGURA/4 - 3*LARGURA/16 + 4); // .
    inserirPino(ALTURA/2 + 3*ALTURA/8 - 1, LARGURA/4 - 1*LARGURA/16 + 4); //   .
    inserirPino(ALTURA/2 + 3*ALTURA/8 - 1, LARGURA/4 + 1*LARGURA/16 + 4); //      .
    inserirPino(ALTURA/2 + 3*ALTURA/8 - 1, LARGURA/4 + 3*LARGURA/16 + 4); //        .
}

void adicionarBarra(int nivel, int pontos){
    for(int i = 63 - pontos*10; i < 63; i++){
        for(int j = 127 - 10*(4 - nivel) - 10; j < 127 - 10*(4 - nivel) - 1; j++){
            display[i][j] = '1';
        }
    }
}

int main(void){
    stdio_init_all();
    
    oled_init();
    oled_clear();

    // sleep_ms(10000);
    // printf("Comecou\n");

    inicializaDisplay();
    
    inicializarPinos();
    imprimeDisplay();
    oled_show();
    

    Bola* bolas[NUMERO_DE_BOLAS];
    for(int i = 0; i < NUMERO_DE_BOLAS; i++){
        Bola* b = (Bola*) malloc(sizeof(Bola));
        b->posX = LARGURA/4 + 4;
        b->posY = 3 + 6*i;
        b->ativa = 1;
        bolas[i] = b;
    }

    
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
                    
                    int pos = contarPonto(bolas[i]->posX);
                    pontos[pos]++;
                    adicionarBarra(pos, pontos[pos]);

                    limparBola(bolas[i]->posY, bolas[i]->posX);
                    

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
        sleep_ms(200);
        oled_clear();
        imprimeDisplay();
        oled_show();
    }
    
    for(int i = 0; i < NUMERO_DE_BOLAS; i++)
        free(bolas[i]);

    for(int i = 0; i < 5; i++)
        oled_char(75+10*i, 0, pontos[i]+'0');
    oled_show();
    return 0;
}