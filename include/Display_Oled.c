#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "./include/ssd1306.h"
#include "./include/ssd1306_font.h"
#include "./include/ssd1306_i2c.h"
#include "hardware/i2c.h"
#include <math.h>

#define I2C_SDA 14
#define I2C_SCL 15

uint8_t ssd[ssd1306_buffer_length];

// Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};

void oled_init(){
    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    calculate_render_area_buffer_length(&frame_area);
}

// Coloca um texto na posição dada
void oled_text(int x, int y, char* text){
    ssd1306_draw_string(ssd, x, y, text);
}

void oled_char(int x, int y, char c){
    ssd1306_draw_char(ssd, x, y, c);
}

void oled_pixel(int x, int y, char c){
    if(c != ' ') ssd1306_set_pixel(ssd, x, y, true);
}

// Atualiza o display
void oled_show(){
    render_on_display(ssd, &frame_area);
}

// Zera o display
void oled_clear(){
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
}

// Função responsável por gerar o gráfico
void oled_graph(uint8_t *pontos, int quantidade, int inicio){ // Recebe um apontador para os pontos, a quantidade deles e em que indice começar
    for(int i = 1; i < quantidade; i++){
        int y0 = pontos[inicio + i-1];
        int y1 = pontos[inicio + i];
        if(y0 > 63) y0 = 63;
        if(y1 > 63) y1 = 63;
        ssd1306_draw_line(ssd, i-1, 63-y0, i, 63-y1, true); // Desenha o gráfico por linhas entre os pontos, começando pelo segundo ponto puxando uma linha a partir do primeiro
    }
}

// Desenhar o bitmap
void oled_draw(){
    const uint8_t bitmap_128x64[] = { 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x40, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 
    0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x3F, 0x00, 0x00, 
    0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 
    0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 
    0x00, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0xFE, 0xFF, 
    0xFF, 0xFF, 0x07, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0x07, 0x00, 
    0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x80, 0xFF, 0xFF, 
    0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 
    0x00, 0xC0, 0xFF, 0xFB, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xC0, 0x3F, 0x80, 
    0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xC0, 0x07, 0x00, 0xF8, 0xFF, 0x7F, 0x00, 
    0x00, 0xC0, 0x03, 0x00, 0x80, 0xFF, 0xFF, 0x00, 0x00, 0xC0, 0x00, 0x00, 
    0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xF8, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x01, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xE0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFF, 0x01, 
    0x00, 0x00, 0x00, 0x00, 0x04, 0xC0, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x80, 
    0x03, 0x80, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x02, 0x80, 0xFF, 0x01, 
    0x00, 0x00, 0x00, 0xC0, 0x03, 0x00, 0xFF, 0x00, 0x80, 0xFF, 0x07, 0x00, 
    0x06, 0x00, 0xFF, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x7F, 0x00, 
    0x80, 0xFF, 0x07, 0x80, 0x1F, 0x00, 0xFF, 0x00, 0x80, 0x31, 0x06, 0x80, 
    0x04, 0x00, 0x7F, 0x00, 0x80, 0x31, 0x06, 0x80, 0x07, 0x00, 0x7E, 0x00, 
    0x80, 0x39, 0x06, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x80, 0x7F, 0x07, 0x00, 
    0x03, 0x00, 0x1F, 0x00, 0x80, 0xFF, 0x07, 0x80, 0x07, 0x00, 0x1F, 0x00, 
    0x00, 0xEF, 0x03, 0x80, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0x07, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 
    0x80, 0xFC, 0x07, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x0C, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x07, 0x80, 
    0x07, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x07, 0x80, 0x05, 0x00, 0x00, 0x00, 
    0x00, 0x0E, 0x0E, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0E, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x07, 0x40, 0x00, 0x00, 0x00, 0x00, 
    0x80, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 
    0x07, 0x00, 0x00, 0x00, 0x80, 0x01, 0x06, 0xC0, 0x02, 0x00, 0x00, 0x00, 
    0x80, 0x01, 0x06, 0xC0, 0x03, 0x00, 0x00, 0x00, 0x80, 0x01, 0x06, 0x00, 
    0x07, 0x00, 0x00, 0x00, 0x80, 0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x80, 0x03, 0x07, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0x40, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1F, 0x00, 0x00, 0x00, 
    0x00, 0xF0, 0x01, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x03, 0x80, 
    0x04, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 
    0x00, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x0E, 0x80, 
    0x06, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x0E, 0x80, 0x04, 0x00, 0x00, 0x00, 
    0x00, 0x1C, 0x07, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x03, 0x80, 0x07, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xFC, 0x07, 0x80, 0x06, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x80, 
    0x04, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 
    0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
    0x04, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x80, 0x04, 0x00, 0x00, 0x00, 
    0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x80, 
    0x07, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x80, 0x04, 0x00, 0x00, 0x00, 
    0x00, 0x0E, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 
    0x03, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0xFC, 0x07, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x07, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 
    0x00, 0x0C, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x80, 
    0x01, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 
    0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x07, 0x80, 
    0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x04, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x80, 0x05, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x03, 0x00, 0x07, 0x00, 0x00, 0x00, 
    0x00, 0xFC, 0x07, 0x80, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x6C, 0x06, 0x80, 
    0x04, 0x00, 0x00, 0x00, 0x00, 0x66, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x6E, 0x0C, 0x80, 0x06, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x0E, 0x80, 
    0x04, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x06, 0x80, 0x07, 0x00, 0x00, 0x00, 
    0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 
    0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, };

    ssd1306_t ssd_bm;
    ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
    ssd1306_config(&ssd_bm);

    ssd1306_draw_bitmap(&ssd_bm, bitmap_128x64);
}
