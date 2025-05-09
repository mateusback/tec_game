TARGET = jogo.exe

CXX = g++
CXXFLAGS = -std=c++23 -Wall -Iinclude -I../my-lib/include
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
LINKFLAGS = -mwindows

SRC = $(wildcard src/**/*.cpp) main.cpp

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LINKFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
