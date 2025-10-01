CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS =  -lglfw -lGL 

SRC_FILES = main.cpp src/shader.cpp  src/model.cpp src/ventana.cpp gl.c
OBJ_FILES = $(SRC_FILES:.cpp=.o)
TARGET = cubo

# Reglas
all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET)