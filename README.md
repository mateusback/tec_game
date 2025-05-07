# 🛠️ Compilando o Jogo feito em C++ no Windows com MSYS2

Esse documento via explicar a como configurar o ambiente de desenvolvimento do jogo no Windows utilizando MSYS2.

---

## 1. Instalar o MSYS 2

O MSYS2 com MinGW-w64 é um dos compiladores C/C++ mais populares para Windows.

- Acesse: [https://www.msys2.org/](https://www.msys2.org/)
- Baixe o instalador e realize a instalação
---

## 2. Adicionar o MinGW ao `PATH` do Sistema

- Pesquise por **"Variáveis de ambiente"**
- Clique em **“Editar variáveis de ambiente do sistema”**
- Edite a variável `Path` e adicione:
```
C:\msys64\mingw64\bin
```
- Salve e **reinicie o terminal** (PowerShell ou CMD)
- Verifique a instalação com:
```bash
g++ --version
```
Se tudo estiver certo, verá algo como: `g++ (GCC) 14.2.0`

---

## 3. Instalar as dependências + Make

**Após a instalação, abra o terminal _MSYS2 MSYS_ (roxo) e execute:**

```bash
pacman -Syu
```

> Reabra o terminal após este comando!

```bash
pacman -Su
pacman -S mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_gfx mingw-w64-ucrt-x86_64-SDL2_image mingw-w64-ucrt-x86_64-SDL2_mixer mingw-w64-ucrt-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer
pacman -S make
pacman -S mingw-w64-ucrt-x86_64-toolchain
pacman -S mingw-w64-ucrt-x86_64-nlohmann-json
```
> Este último comando instala `gcc`, `g++`, `make`, `gdb`, entre outros.
---

## 4. Definir as libs para o Visual Studio Code Reconhecer

Ao abrir o VsCode, você pode se deparar com problemas nos includes, isso pode ser porque ele não encontrou as libs, então siga os seguintes passos:

Presione CTRL + Shift + P e abra C/C++: Edit Configurations (UI)
1. Defina a C++ standard para c++23
2. Adicione o diretório das libs no Include path, por exemplo
```
${workspaceFolder}/**
${workspaceFolder}/include/**
${workspaceFolder}/../my-lib/include
C:/msys64/mingw64/include
C:/msys64/mingw64/include/c++
C:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/14.2.0/include
C:/msys64/mingw64/include/SDL2
```

3. No Compiler path:
```
C:/msys64/ucrt64/bin/g++.exe
```

---
## 5. Compilar o Projeto com Make

Abra o terminal **MSYS2 MinGW UCRT64** (ícone marrom) e navegue até o diretório do seu projeto:

```bash
cd /c/projects/games/tec_game
```
Certifique-se de que a my-lib está instalada no mesmo diretório que a pasta do projeto.
Caso não tenha, clone por aqui: [https://github.com/ehmcruz/my-lib/](https://github.com/ehmcruz/my-lib/) 

Agora tente compilar o projeto com:
```bash
make
```
Se tudo estiver certo, o jogo estará pronto para rodar:
```bash
./jogo
```
---

# 🐛 TroubleShooting

## 1. Bibliotecas não encontradas:

Baixe e extraia as versões (Estamos utilizando todas as versões 2) compatíveis com MinGW:

- [SDL2](https://github.com/libsdl-org/SDL/releases/download/release-2.32.4/SDL2-devel-2.32.4-mingw.zip)
- [SDL_image](https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.8/SDL2_image-devel-2.8.8-mingw.zip)
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.24.0/SDL2_ttf-devel-2.24.0-mingw.zip)
- [nlohmann/json](https://github.com/nlohmann/json/releases/download/v3.12.0/include.zip)


- Extraia os diretórios `include/` e `lib/` das bibliotecas baixadas.
- Copie os conteúdos para:

```
C:/msys64/mingw64/include
C:/msys64/mingw64/lib
```

📂 Exemplo final de estrutura:

- `C:/msys64/mingw64/include/SDL2/SDL.h`  
- `C:/msys64/mingw64/lib/libSDL2.a` (e outros arquivos `.a`)

## 2. Compilação do Main com SDL

Antes de usar `SDL2`, certifique-se de:

- Adicionar no topo do seu `main.cpp`:
```cpp
#define SDL_MAIN_HANDLED
```

- Usar a seguinte assinatura da função principal:
```cpp
int main(int argc, char* argv[]) { }
```

- Compilar com `-mwindows` para aplicações com interface gráfica.
- Adicionar `-lSDL2main` antes de `-lSDL2` no `Makefile`.

---