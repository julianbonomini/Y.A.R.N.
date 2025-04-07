# Raspberry Pi Kiosk App - C++ Development Setup

This repository outlines the process for developing a kiosk application in **C++** for a **Raspberry Pi 4**, utilizing an **e-paper display** (8 inches), **GPIO pins**, and potential future peripherals.

## Project Overview

The goal of this project is to build a simple desktop gadget using the **Raspberry Pi 4** that will display information (e.g., weather, stock ticker) on an **e-paper display** in **kiosk mode**. The application will run fullscreen and only display the custom app, limiting desktop interaction.

---

## ⚙️ **Development Setup**

### 1. **Prepare Raspberry Pi**

- Install **Raspberry Pi OS** (or Raspberry Pi OS Lite) on your SD card.
- Connect the Raspberry Pi to a monitor (via HDMI), keyboard, and mouse for initial setup.
- Set up Wi-Fi (if using wireless connection) or connect to Ethernet.

Update everything on your Pi
```bash
sudo apt update && sudo apt upgrade
```

### 2. **Install Development Tools**
Install the necessary C++ development tools:

```bash
sudo apt install build-essential cmake g++ libssl-dev
```

For GUI development (if required), install `SDL2` or `Qt`:
SDL2 (lightweight, fast graphics):
```bash
sudo apt install libsdl2-dev
```
Qt (robust GUI options):
```bash
sudo apt install qt5-qmake qtbase5-dev
```

### 3. **Set Up SSH for Remote Access (Optional)**
Enable SSH on the Raspberry Pi to remotely access it from your Mac:

```bash
sudo systemctl enable ssh
sudo systemctl start ssh
```
Find the Raspberry Pi’s IP address:
```bash
hostname -I
```
SSH into the Pi from your Mac:
```bash
ssh pi@<Pi-IP-address>
```

### 🧑‍💻 **Development Process**

1. Start coding
2. Compile Your Code on the Pi
   To compile your C++ code, run:
```bash
g++ -o my-app app_space.cpp `sdl2-config --cflags --libs`
```
This will generate the executable my-app.

3. Test Your App on the Pi
   Run your compiled application on the Raspberry Pi:
```bash
./my-app
```


### **Final Steps**

1. Run Your App in Kiosk Mode
   To automatically launch your app in kiosk mode:

Create a startup script (~/start.sh):

```bash
#!/bin/bash
/home/pi/my-app
```
Set up a systemd service to run your app at startup:
`sudo nano /etc/systemd/system/kiosk.service`

Add the following content:
```bash
[Unit]
Description=Kiosk App

[Service]
ExecStart=/home/pi/start.sh
Restart=always
User=pi

[Install]
WantedBy=multi-user.target
```
Enable and start the service:
```bash
sudo systemctl enable kiosk.service
sudo systemctl start kiosk.service
```
2. Testing and Final Touches
   Test that everything works smoothly on the Pi:

Ensure the app starts in fullscreen mode.

Test interactions with GPIO pins and peripherals like the e-paper display.

Configure the Pi to boot directly into your app (disable the desktop, if needed).