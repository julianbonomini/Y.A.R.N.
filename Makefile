CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/TickPi

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

# Run
run:
	./$(BUILD_DIR)/TickPi