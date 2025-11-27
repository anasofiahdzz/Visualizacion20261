CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 
LDFLAGS = -lglfw -lGL -lGLEW 

SRC_FILES = main.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)
TARGET = raycast_volume

# Reglas
all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET)
