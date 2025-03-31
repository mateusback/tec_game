TARGET = jogo

CXX = g++
CXXFLAGS = -std=c++23 -Wall -Iinclude -I../my-lib/include
LDFLAGS = -lSDL2 -lSDL2_image

SRC = $(wildcard src/**/*.cpp) main.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJ)
