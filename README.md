
# Projetos de Sistemas Embarcados - EmbarcaTech 2025

Autor: **Eduardo Pereira Tejada**

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, 9 maio de 2025

---

# 🎰 Simulador de Pachinko com Display OLED – Raspberry Pi Pico

Este projeto implementa uma versão simplificada de uma máquina **Pachinko**, utilizando um **display OLED**, com controle por meio da **Raspberry Pi Pico**. Esferas ("bolas") descem em um tabuleiro com pinos fixos, desviando aleatoriamente até alcançarem uma das cinco regiões de pontuação na base do display.

---

## 📷 Demonstração

> *(Insira aqui uma imagem ou vídeo do jogo rodando, caso disponível)*

---

## ⚙️ Funcionalidades

- Geração de 5 bolas que descem pela tela e colidem com pinos.
- Desvio aleatório das bolas ao colidir com um pino.
- Contagem de pontos com base na posição final de cada bola.
- Atualização em tempo real do display OLED via I²C.
- Representação gráfica das pontuações com barras verticais.

---

## 🔧 Tecnologias Utilizadas

- **Linguagem**: C  
- **Plataforma**: [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/)  
- **Display**: OLED 128x64 (via protocolo I²C)  
- **Bibliotecas**:
  - `pico/stdlib.h`
  - `hardware/i2c.h`
  - `pico/rand.h`
  - Arquivo personalizado: `Display_Oled.h` (para controle do display OLED)

---

## 📁 Estrutura do Projeto

projeto-pachinko/
├── include/
│ └── Display_Oled.h # Interface de controle para o display OLED
├── main.c # Código principal do simulador
├── CMakeLists.txt # Script de build para o SDK do Pico
└── README.md # Documentação do projeto

---

## 📜 Licença
MIT License - MIT GPL-3.0.

