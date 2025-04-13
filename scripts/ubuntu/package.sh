#!/bin/bash

# Ensure the script stops if any command fails
set -e

# Define some paths
APP_DIR="AppDir"
BUILD_DIR="build/linux"
ASSETS_DIR="./assets"
TARGET_BINARY="YARN"
DESKTOP_FILE="YARN.desktop"
ICON_FILE="icon.jpg"

# Function to prepare the AppImage structure
prepare_appimage_structure() {
  echo "Preparing AppImage structure..."
  echo "PWD is set to: $PWD"
  echo "APP_DIR is set to: $APP_DIR"
  echo "BUILD_DIR is set to: $BUILD_DIR"
  echo "ASSETS_DIR is set to: $ASSETS_DIR"
  echo "TARGET_BINARY is set to: $TARGET_BINARY"
  echo "DESKTOP_FILE is set to: $DESKTOP_FILE"
  mkdir -p "$APP_DIR/usr/bin"
  mkdir -p "$APP_DIR/usr/lib"
  mkdir -p "$APP_DIR/usr/share/$TARGET_BINARY/assets/"

  # Copy the target binary to the AppDir
  cp "$BUILD_DIR/$TARGET_BINARY" "$APP_DIR/usr/bin/"

  # Copy libraries listed by ldd
  echo "Copying shared libraries..."
  ldd "$BUILD_DIR/$TARGET_BINARY" | grep "=>" | awk '{print $3}' | while read lib; do
    echo "Copying $lib..."
    # Check if the library exists and copy it
    if [ -f "$lib" ]; then
      cp "$lib" "$APP_DIR/usr/lib/"
    else
      echo "Warning: Library $lib not found!"
    fi
  done

  # Bundle assets:
  echo "ASSETS_DIR is: $ASSETS_DIR"
  echo "Copying assets from $ASSETS_DIR/. to $APP_DIR/usr/share/$TARGET_BINARY/assets/"
  # Ensure that files inside assets exist
  if [ -d "$ASSETS_DIR" ]; then
    echo "Assets directory exists, copying files..."
    cp -r "$ASSETS_DIR/." "$APP_DIR/usr/share/$TARGET_BINARY/assets/"
  else
    echo "Warning: Assets directory $ASSETS_DIR not found!"
  fi

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
