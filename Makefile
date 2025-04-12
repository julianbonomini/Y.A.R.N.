# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# !!!!THIS IS A SPECIFIC MACOS MAKE FILE!!!!
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

# MACOS APP BUNDLE
MACOS_APP_NAME = YARN
MACOS_APP_DIR = $(MACOS_APP_NAME).app
MACOS_CONTENTS_DIR = $(MACOS_APP_DIR)/Contents
MACOS_MACOS_DIR = $(MACOS_CONTENTS_DIR)/MacOS
MACOS_RESOURCES_DIR = $(MACOS_CONTENTS_DIR)/Resources

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/$(MACOS_APP_NAME) # Target should match the executable name inside the bundle
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

macos: $(TARGET) # 'macos' target depends on the main build target
	mkdir -p $(MACOS_MACOS_DIR)
	mkdir -p $(MACOS_RESOURCES_DIR)/Assets
	cp $(BUILD_DIR)/$(MACOS_APP_NAME) $(MACOS_MACOS_DIR)/$(MACOS_APP_NAME)
	cp -r assets $(MACOS_RESOURCES_DIR)/Assets
	mkdir -p $(MACOS_CONTENTS_DIR)
	cp Info.plist $(MACOS_CONTENTS_DIR)/Info.plist

clean:
	rm -rf $(BUILD_DIR) $(MACOS_APP_DIR) # Clean up the app bundle as well

# Run
run:
	./$(TARGET)