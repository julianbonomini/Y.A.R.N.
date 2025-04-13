#!/bin/bash

# Ensure the script stops if any command fails
set -e

install_sfml() {
  echo "----------------------------------------"
  echo "--------------  SFML  ------------------"
  echo "----------------------------------------"
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
    echo "/usr/local/lib" | sudo tee /etc/ld.so.conf.d/sfml.conf
    sudo ldconfig

    echo "Check if installed properly:"
    ls -l /usr/local/lib | grep sfml

  fi
  # Go back to the original directory
  cd "$original_dir"
}

install_appimagetool() {
  echo "----------------------------------------"
  echo "----------  AppImageTool  --------------"
  echo "----------------------------------------"
  wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage -O appimagetool

  # Make it executable
  chmod +x appimagetool

  # Optional: Move to /usr/local/bin to use as `appimagetool`
  sudo mv appimagetool /usr/local/bin/appimagetool

  echo "Check if installed properly:"
  ls -l /usr/local/bin | grep appimagetool
}

# Function to install dependencies (useful for both local and CI environments)
aptget() {
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
    libglib2.0-dev \
    libcurl4-openssl-dev \
    nlohmann-json3-dev
}

# Function to install dependencies (useful for both local and CI environments)
main() {
  aptget
  install_sfml
  install_appimagetool
}

main
