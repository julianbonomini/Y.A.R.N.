#!/bin/bash

set -e  # Exit immediately if any command fails

# Install dependencies
sudo apt-get update

# Dependencies for SFML
sudo apt update && sudo apt install \
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
     libfreetype6-dev \
     libopenal-dev \
     libogg-dev

# For Y.A.R.N.
sudo apt-get install -y \
    nlohmann-json3-dev

# Clone and build SFML 3.0
git clone --depth 1 --branch 3.0.x https://github.com/SFML/SFML.git
cd SFML
mkdir build && cd build
cmake .. -DBUILD_SHARED_LIBS=ON
make -j$(nproc)
sudo make install