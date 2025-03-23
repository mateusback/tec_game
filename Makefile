TARGET = jogo

CXX = g++
CXXFLAGS = -std=c++23 -Wall -I./src -I../my-lib/include
LDFLAGS = -lSDL2

SRC = $(wildcard src/**/impl/*.cpp) main.cpp 
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJ)
