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

void oled_init();
void oled_text(int x, int y, char* text);
void oled_show();
void oled_clear();
void oled_graph(uint8_t *pontos, int quantidade, int inicio);
void oled_draw();
void oled_char(int x, int y, char c);
void oled_pixel(int x, int y, char c);