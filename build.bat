@echo off
setlocal EnableDelayedExpansion

echo =======================================
echo =             BUILDANDO               =
echo =======================================
echo.

:: A ideia é rodar esse bat no meu pc mesmo, depois trocar esses sets
set SDL2=F:\SDL2\SDL2
set SDL2_IMAGE=F:\SDL2\SDL2_image
set SDL2_TTF=F:\SDL2\SDL2_ttf
set JSON=F:\SDL2\include\include

set SRC=
for /R src %%f in (*.cpp) do (
    set "SRC=!SRC! %%f"
)
set SRC=!SRC! main.cpp

set INCLUDE=-Iinclude -I..\my-lib\include -I%JSON% ^
-I%SDL2%\include -I%SDL2_IMAGE%\include -I%SDL2_TTF%\include

set LIBS=-L%SDL2%\lib -L%SDL2_IMAGE%\lib -L%SDL2_TTF%\lib ^
-lSDL2 -lSDL2_image -lSDL2_ttf

echo Compilando com g++...
g++ -std=c++23 -Wall -DSDL_MAIN_HANDLED %SRC% %INCLUDE% %LIBS% -mconsole -o jogo.exe > build.log 2>&1

if %errorlevel% neq 0 (
    echo.
    echo =============================
    echo =     ERRO NA COMPILACAO    =
    echo =============================
    type build.log
    pause
    exit /b
) else (
    echo.
    echo =============================
    echo =  COMPILADO COM SUCESSO!   =
    echo =============================
)

pause
