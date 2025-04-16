# Compiler and Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Directories
SRC_DIR = src
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(SRCS:.cpp=.o)
APP_NAME = YARN

# Platform-specific settings
ifeq ($(shell uname), Darwin)
  # macOS-specific settings
  BUILD_DIR = build/macos
  MACOS_APP_DIR = $(APP_NAME).app
  MACOS_CONTENTS_DIR = $(MACOS_APP_DIR)/Contents
  MACOS_MACOS_DIR = $(MACOS_CONTENTS_DIR)/MacOS
  MACOS_RESOURCES_DIR = $(MACOS_CONTENTS_DIR)/Resources
  LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
  LDFLAGS += -L/opt/homebrew/lib -lssl -lcrypto
  CXXFLAGS += -I/opt/homebrew/include
else
  # Linux-specific settings
  BUILD_DIR = build/linux
  CXXFLAGS += -I/usr/local/include
  LDFLAGS += -L/usr/local/lib
  # Add SFML static libraries explicitly
  LDFLAGS += -L/usr/local/lib
  LDFLAGS += -lsfml-graphics-s -lsfml-window-s -lsfml-system-s

  # Core dependencies for SFML static build
  LDFLAGS += -lfreetype -lopenal -lvorbis -lvorbisfile -lFLAC
  LDFLAGS += -lX11 -lXrandr -lXcursor -lXrender -lXfixes -lXinerama -lXext -lXi
  LDFLAGS += -ludev -lGL -lpthread -ldl
  LDFLAGS += -lssl -lcrypto

  # Static linking for standard C++ runtime
  LDFLAGS += -static-libgcc -static-libstdc++
endif

TARGET = $(BUILD_DIR)/YARN


all: $(TARGET)

$(TARGET): $(SRCS)
	@echo "Compiling ..."
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Compiling DONE!"

macos: $(TARGET)
	@echo "Creating macOS specific directories ..."
	mkdir -p $(MACOS_MACOS_DIR)
	mkdir -p $(MACOS_RESOURCES_DIR)/Assets
	mkdir -p $(MACOS_CONTENTS_DIR)
	@echo "Cooping executable ..."
	cp $(BUILD_DIR)/$(APP_NAME) $(MACOS_MACOS_DIR)/$(APP_NAME)
	@echo "Cooping assets ..."
	cp -r assets $(MACOS_RESOURCES_DIR)/Assets
	@echo "Cooping Info.plist ..."
	cp Info.plist $(MACOS_CONTENTS_DIR)/Info.plist

# Linux build (just compiling the executable)
linux: $(TARGET)
	@echo "Building for Linux completed."

clean:
	rm -rf $(BUILD_DIR) $(MACOS_APP_DIR) # Clean up the app bundle if it exists

run:
	./$(TARGET)
