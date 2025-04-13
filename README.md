# Y.A.R.N.

(yet another retro node) project. A BIOS inspired desk gadget to run c++ apps, either informative or interactive.

## Hardware

- Raspberry Pi 4
- E-Ink Paper display 800x460

## Setup

### MacOS
- Install SFML with Homebrew:
```bash
brew install sfml
brew install nlohmann-json
```

### Linux based
```bash
./scripts/ubuntu/install_dependencies.sh
```
Should install all needed packages to clone and install SFML 3.0.

## Build macOS
```bash
make macos
```

## Run the executable:
```bash
make run
```

## Make clean
```bash
make clean
```

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