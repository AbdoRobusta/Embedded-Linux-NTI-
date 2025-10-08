# Servo and Potentiometer Control with PlatformIO and Raspberry Pi

## Servo & Potentiometer this for examples

## 🧠 Overview
This guide explains how to:
- Modify and upload Arduino code using PlatformIO. (on Host "Labtop")
- View potentiometer and servo readings on PlatformIO’s Serial Monitor.
- Communicate between Raspberry Pi and Arduino using Python.
- Install new libraries.
- Edit Arduino code **directly from Raspberry Pi**.

---
## ⚙️ 1. Modify and Upload Arduino Code (on your laptop)
1. Open your project folder in **PlatformIO (VSCode)**.
2. Modify your `src/main.cpp` file.
3. Connect the Arduino board (e.g., UNO) via USB.
4. From VSCode Terminal, run:
   ```bash
   pio run --target upload
   ```
5. Open the Serial Monitor to view readings:
   ```bash
   pio device monitor
   ```
---
## 🧩 2. Install New Libraries in PlatformIO
If your project requires libraries (like Servo or LiquidCrystal), install them using one of these methods:

**Method 1 (Terminal):**
```bash
pio lib install "Servo"
```
**Method 2 (Library Manager):**
- Open PlatformIO sidebar → Libraries.
- Search for the library (e.g., “Servo”) → Click **Install**.

---

## 💾 3. Sending Data from Raspberry Pi to Arduino
If Arduino sends readings like:
```
Potentiometer: 512 -> Servo angle: 90
```
You can read them on Raspberry Pi using this Python script:

```python
import serial, time

ser = serial.Serial("/dev/ttyACM0", 115200, timeout=2)
time.sleep(2)

while True:
    line = ser.readline().decode().strip()
    if line:
        print("From Arduino:", line)
```
```c++
#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int main() {
    const char *port = "/dev/ttyACM0"; 
    int serial_port = open(port, O_RDWR);

    if (serial_port < 0) {
        std::cerr << "Error opening serial port " << port << std::endl;
        return 1;
    }

    termios tty{};
    tcgetattr(serial_port, &tty);
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tcsetattr(serial_port, TCSANOW, &tty);

    char buf[256];
    while (true) {
        int n = read(serial_port, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            std::string line(buf);

            std::cout << "Received: " << line;

            std::ofstream csv("/home/weston/data.csv", std::ios::trunc);
            csv << line;
            csv.close();
        }
    }

    close(serial_port);
    return 0;
}
```

## 🧰 4. Editing and Uploading Arduino Code from Raspberry Pi
You can modify and upload Arduino code **directly on Raspberry Pi** using PlatformIO CLI.

### Install PlatformIO Core on Raspberry Pi
```bash
sudo apt update
sudo apt install python3-pip -y
pip3 install platformio
```
> you can not to use sudo on console terminal put you can use :

    1-opkg
    2-dnf

### Create or open your project
```bash
mkdir ~/arduino_servo_project && cd ~/arduino_servo_project
pio project init --board uno
```

### Add your source code
Edit `src/main.cpp`:
```bash
nano src/main.cpp
or 
vi src/main.cpp
```

### Upload to Arduino
```bash
pio run --target upload
```

### Monitor serial output
```bash
pio device monitor
```
---
## 📦 5. Transfer Files Between Raspberry Pi and Laptop
From your **laptop**, run:
```bash
scp pi@<raspberry_ip>:/home/pi/Servo_Potentiometer_with_PlatformIO_Guide.md .
```
Replace `<raspberry_ip>` with your Pi’s IP (e.g., `192.168.1.10`).

---

✅ **Now you can edit, upload, and test Arduino code either from your laptop or directly from Raspberry Pi!**
