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
brew install nlohmann_json
```

## Build
```bash
make
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

Tag github with eihter:
- v[0-9]+.[0-9]+.[0-9]+
- v[0-9]+.[0-9]+.[0-9]+-beta
- v[0-9]+.[0-9]+.[0-9]+-alpha