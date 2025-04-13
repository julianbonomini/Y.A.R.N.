#!/bin/bash

# Ensure the script stops if any command fails
set -e

# Define some paths
APP_DIR="AppDir"
BUILD_DIR="build/linux"
ASSETS_DIR="assets"
TARGET_BINARY="YARN"
DESKTOP_FILE="YourApp.desktop"
#ICON_FILE="YourApp.png"

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

  # This should not be needed if everything is statically linked
#  cp "$BUILD_DIR/$TARGET_BINARY" "$APP_DIR/usr/bin/"

  # Do I need this?
  # Copy libraries (note: you may need to copy from specific paths, especially for static linking)
#  cp -r /usr/local/lib/* "$APP_DIR/usr/lib/"


  # Bundle assets:
  echo "Copying assets:"
  cp -r "$ASSETS_DIR/*" "$APP_DIR/usr/share/$TARGET_BINARY/assets/"

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
  echo""
  echo""
  echo "----------------------------------------"
  echo "----------------------------------------"
  echo "checking ldd for my binary"
  ldd "$BUILD_DIR/$TARGET_BINARY"
  echo "----------------------------------------"
  echo "----------------------------------------"
  echo""
  echo""


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
