# Include config.mk for SDL paths
include config.mk

# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Iinclude $(SDL_CFLAGS)
INCLUDES = -Iinclude
CXXFLAGS += $(INCLUDES)

# Source and object files
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, bin/%.o, $(SRC))
TARGET := bin/path_visualizer

# Default target
all: bin $(TARGET)

# Linking object files to create the executable
$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(SDL_LDLIBS) $(LIBS)

# Compiling each .cpp file into .o
bin/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Run the program
run: $(TARGET)
	./$(TARGET)

# Ensure bin/ exists
bin:
	mkdir -p bin

# Clean object files and executable
clean:
	rm -f $(OBJ) $(TARGET)