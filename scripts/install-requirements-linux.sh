#!/bin/bash

set -e  # Exit immediately if any command fails

# Install dependencies
sudo apt-get update
sudo apt-get install -y \
    cmake g++ libx11-dev libjpeg-dev libpng-dev libtiff-dev libopenal-dev \
    libflac-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev \
    libfreetype6-dev libvorbis-dev nlohmann-json3-dev

# Clone and build SFML 3.0
git clone --depth 1 --branch 3.0.x https://github.com/SFML/SFML.git
cd SFML
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install