# Jogo em C++ com SDL2

Um jogo simples desenvolvido em C++ usando SDL2.

## Como Compilar

Certifique-se de ter o SDL2 instalado.

### Instalação no Linux (Ubuntu/Debian)

```sh
sudo apt update && sudo apt install libsdl2-dev
sudo apt update && sudo apt install libsdl2-image-dev
sudo apt update && sudo apt install libsdl2-ttf-dev
```

### Compilando o jogo

```sh
g++ -std=c++23  main.cpp -o jogo -lSDL2 -I../my-lib/include
```

### Executando o jogo

```sh
./jogo
```