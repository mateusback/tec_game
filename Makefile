TARGET = jogo.exe

CXX = g++
CXXFLAGS = -std=c++23 -Wall -Iinclude -I../my-lib/include

# SDL2main vem ANTES de SDL2
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

SRC = $(wildcard src/**/*.cpp) main.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -mwindows

clean:
	rm -f $(TARGET) $(OBJ)
