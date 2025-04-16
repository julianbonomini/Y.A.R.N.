# Y.A.R.N.

(yet another retro node) project. A BIOS inspired desk gadget to run c++ apps, either informative or interactive.

## Hardware

- Raspberry Pi 4
- E-Ink Paper display 800x460

## Local Setup

If you decide you want to checkout the code, build it and run it, the please do the following:

### MacOS
#### Install dependencies, you'll need brew
```bash
./scripts/macos/install-dependencies.sh
```
#### Build macOS
```bash
make macos
```
#### Config file
```bash
touch .config
echo "OPENWEATHER_API_KEY=[YOUR_KEY]" >> .config
echo "LOG_LEVEL=[INFO|DEBUG]" >> .config
```

### Run
#### Run the executable with make
```bash
make run
```
Better for development, but quite bad if you just want to run the app

#### Run from .app bundle
This is the better option, since you can move the .app bundle to your Applications folder and use it as any other app.
But before moving it you have to copy the config into the .app bundle:
```bash
cp .config "YARN.app/Contents/Resources"
```
Now you can double-click the .app bundle anywhere and it should work


### Linux

#### Install dependencies
```bash
./scripts/linux/install-dependencies.sh
```
#### Build macOS
```bash
make linux
```
#### Config file
```bash
touch .config
echo "OPENWEATHER_API_KEY=[YOUR_KEY]" >> .config
echo "LOG_LEVEL=[INFO|DEBUG]" >> .config
```

### Run
#### Run the executable with make
```bash
make run
```

#### Run with package
// TODO

## Release

Tag github with either:
- v[0-9]+.[0-9]+.[0-9]+
- v[0-9]+.[0-9]+.[0-9]+-beta
- v[0-9]+.[0-9]+.[0-9]+-alpha

# Y.A.R.N. macOS Installation Guide

Thank you for downloading YARN! Below are the steps to install and run YARN on macOS.

#### 1. Download YARN
You can download it from GitHub Releases, always get the latest if possible.

#### 2. Extract the Application
This will create the YARN.app file which you can move to your Applications folder or
keep in any folder on your Mac for easy access.

#### 3. Remove the Quarantine Flag (if necessary).
If you try to open the app after unzipping and it says that is damaged, this is most likely due
to a lack of signature of the app.

(macOS places a quarantine flag on unsigned applications downloaded from the internet). 

I refuse to pay an Apple developer account to sign it. And you can get 
around it by removing the restriction yourself by running:

```bash
xattr -cr YARN.app
```
The `xattr -cr` command removes extended attributes (like the quarantine flag) recursively (-r) from the YARN.app file.

Now you should be able to open the app normally!