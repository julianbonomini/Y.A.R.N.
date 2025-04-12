#!/bin/bash

# Ensure the script stops if any command fails
set -e

# Define some paths
APP_DIR="AppDir"
BUILD_DIR="build/linux"
TARGET_BINARY="YARN"
DESKTOP_FILE="YourApp.desktop"
#ICON_FILE="YourApp.png"

install_sfml() {
  original_dir=$PWD
  SFML_DIR="$HOME/SFML"  # Path to the SFML directory where it will be cloned

  # Check if SFML is already installed
  if [ -d "$SFML_DIR" ] && [ -f "$SFML_DIR/CMakeLists.txt" ]; then
    echo "SFML is already installed in $SFML_DIR. Skipping installation."
  else
    echo "SFML not found. Cloning and building SFML..."

    # Clone SFML 3.0 into the target directory
    git clone --depth 1 --branch 3.0.x https://github.com/SFML/SFML.git "$SFML_DIR"

    # Navigate to the SFML directory
    cd "$SFML_DIR"

    # Create build directory and navigate into it
    mkdir -p build && cd build

    # Configure CMake to build with static libraries
    cmake .. -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release

    # Compile with all available cores
    make -j$(nproc)

    # Install SFML
    sudo make install

    # Go back to the original directory
  fi
  cd "$original_dir"
}

install_appimagetool() {
  original_dir=$PWD
  APP_IMG_TOOL_DIR="$HOME/AppImageTool"
  if [ -d "$APP_IMG_TOOL_DIR" ] && [ -f "$APP_IMG_TOOL_DIR/CMakeLists.txt" ]; then
    echo "AppImageTool is already installed in $APP_IMG_TOOL_DIR. Skipping installation."
  else
    echo "AppImageTool not found. Cloning and building AppImageTool..."

    git clone https://github.com/AppImage/appimagetool.git "$APP_IMG_TOOL_DIR"
    cd "$APP_IMG_TOOL_DIR"
    cmake -DCMAKE_SYSTEM_PROCESSOR=arm64 .
    make appimagetool
    sudo mv appimagetool /usr/local/bin/
    file /usr/local/bin/appimagetool
  fi
  cd "$original_dir"
}

# Function to install dependencies (useful for both local and CI environments)
install_dependencies() {
  echo "Installing dependencies..."
  sudo apt-get update
  sudo apt-get install -y \
    build-essential \
    cmake \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libdrm-dev \
    libgbm-dev \
    libogg-dev \
    libfreetype6-dev \
    libopenal-dev \
    libsndfile1-dev \
    libfuse-dev \
    pkg-config \
    libgpgme-dev \
    libgcrypt-dev \
    libglib2.0-dev \
    libgio-2.0-dev \
    libcurl4-openssl-dev \
    nlohmann-json3-dev

  install_sfml
  install_appimagetool
}

# Function to build the application using Makefile
build_application() {
  echo "Building the application using Makefile..."
#  make clean
  make linux
}

# Function to prepare the AppImage structure
prepare_appimage_structure() {
  echo "Preparing AppImage structure..."
  echo "PWD is set to: $PWD"
  echo "APP_DIR is set to: $APP_DIR"
  echo "BUILD_DIR is set to: $BUILD_DIR"
  echo "TARGET_BINARY is set to: $TARGET_BINARY"
  echo "DESKTOP_FILE is set to: $DESKTOP_FILE"
  mkdir -p "$APP_DIR/usr/bin"
  mkdir -p "$APP_DIR/usr/lib"

  echo ""
  echo ""
  echo "LS AppDir"
  ls -l $APP_DIR
  echo "LS AppDir/usr"
  ls -l $APP_DIR/usr
  echo "LS AppDir/usr/bin"
  ls -l $APP_DIR/usr/bin
  echo ""
  cp "$BUILD_DIR/$TARGET_BINARY" "$APP_DIR/usr/bin/"
  echo "target copied"

  # Copy libraries (note: you may need to copy from specific paths, especially for static linking)
  cp -r /usr/local/lib/* "$APP_DIR/usr/lib/"

  # Copy the desktop entry and icon (if available)
  if [ -f "$DESKTOP_FILE" ]; then
      cp "$DESKTOP_FILE" "$APP_DIR/"
  else
      echo "Warning: Desktop entry ($DESKTOP_FILE) not found."
  fi
  if [ -f "$ICON_FILE" ]; then
      cp "$ICON_FILE" "$APP_DIR/"
  else
      echo "Warning: Icon file ($ICON_FILE) not found."
  fi

  chmod +x "$APP_DIR/usr/bin/$TARGET_BINARY"
}

# Function to build the AppImage
build_appimage() {
  echo "Building AppImage..."
  appimagetool "$APP_DIR"
}

# Main logic
main() {
  echo "----------------------------------------"
  echo "---------INSTALL DEPENDENCIES-----------"
  echo "----------------------------------------"
  install_dependencies

  # Build the application using the Makefile
  echo "----------------------------------------"
  echo "--------------BUILD APP-----------------"
  echo "----------------------------------------"
  build_application

  echo "----------------------------------------"
  echo "------------PREPARE IMAGE---------------"
  echo "----------------------------------------"
  # Prepare the AppImage structure
  prepare_appimage_structure

  # Build the AppImage
  echo "----------------------------------------"
  echo "-------------BUILD IMAGE----------------"
  echo "----------------------------------------"
  build_appimage

  echo "AppImage build completed successfully!"
}

# Trigger the main function
main
