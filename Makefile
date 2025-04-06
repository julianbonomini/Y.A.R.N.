# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# SFML include and lib paths
SFML_INCLUDE = /opt/homebrew/include
SFML_LIB = /opt/homebrew/lib

# Source directory
SRC_DIR = src
# Output directory
BUILD_DIR = build

# Target executable
TARGET = $(BUILD_DIR)/TickPi

# Source files
SRC = $(SRC_DIR)/main.cpp

# Linker flags
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Create build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

# Default target
all: $(TARGET)

# Link and compile the program
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(SFML_INCLUDE) -L$(SFML_LIB) $(SRC) -o $(TARGET) $(LDFLAGS)

# Clean the build files
clean:
	rm -rf $(BUILD_DIR)

# Run
run:
	./$(BUILD_DIR)/TickPi
