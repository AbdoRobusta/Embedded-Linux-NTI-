# 🧠 Raspberry Pi Bluetooth Connections Cheat Sheet (Final + Corrected)

## 1️⃣ Raspberry Pi ↔ Arduino (HC-05 / HC-06)

### 🔌 Hardware Connections
- **HC-05 / HC-06 → Arduino:**
  - `VCC → 5V`
  - `GND → GND`
  - `TX (module)` → `RX (Arduino)`
  - `RX (module)` → `TX (Arduino)` *(⚠️ use a voltage divider: Arduino TX = 5V, module RX = 3.3V)*

---

### 💻 Arduino Code Example
```cpp
void setup() {
  Serial.begin(9600); // HC-05 default baud rate
}

void loop() {
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  delay(1000);
}
```

---

### 🐧 Raspberry Pi Side (Linux)
```bash
# 1. Enable Bluetooth service
sudo systemctl enable bluetooth
sudo systemctl start bluetooth

# 2. Open Bluetooth control
bluetoothctl

# 3. Inside bluetoothctl
power on
agent on
default-agent
scan on        # Find your HC-05 MAC address
pair XX:XX:XX:XX:XX:XX
trust XX:XX:XX:XX:XX:XX
connect XX:XX:XX:XX:XX:XX
exit
```

```bash
# 4. Bind to a serial port
sudo rfcomm bind 0 XX:XX:XX:XX:XX:XX

# 5. Check if /dev/rfcomm0 exists
ls /dev/rfcomm*

# 6. Read data
cat /dev/rfcomm0
```

✅ *Tip:* If you want it to auto-connect at boot, create a small **systemd service** that runs the rfcomm bind command automatically.

---

## 2️⃣ Raspberry Pi ↔ ESP32 (Bluetooth Serial)

### ⚙️ ESP32 Code (Arduino IDE)
```cpp
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Device"); // Device name visible on Pi
}

void loop() {
  int sensorValue = analogRead(34);
  SerialBT.println(sensorValue);
  delay(1000);
}
```

---

### 🐧 Raspberry Pi Side
```bash
# 1. Start bluetoothctl
bluetoothctl

# 2. Inside bluetoothctl
power on
agent on
default-agent
scan on
pair XX:XX:XX:XX:XX:XX
trust XX:XX:XX:XX:XX:XX
connect XX:XX:XX:XX:XX:XX
exit
```

```bash
# 3. Bind rfcomm
sudo rfcomm bind 0 XX:XX:XX:XX:XX:XX

# 4. Read incoming data
cat /dev/rfcomm0
```

---

## ⚡️ Optional Enhancements

### 🧩 Auto-connect at Boot
Create a service file `/etc/systemd/system/bt-autoconnect.service`:
```ini
[Unit]
Description=Auto Connect Bluetooth Device
After=bluetooth.service

[Service]
ExecStart=/usr/bin/rfcomm bind 0 XX:XX:XX:XX:XX:XX
Restart=on-failure

[Install]
WantedBy=multi-user.target
```
Then run:
```bash
sudo systemctl enable bt-autoconnect.service
sudo systemctl start bt-autoconnect.service
```

---

### 🏷️ Change Bluetooth Device Name on Raspberry Pi
Edit `/etc/bluetooth/main.conf`:
```ini
[General]
Name = MyRaspberryPi
```
Then restart the service:
```bash
sudo systemctl restart bluetooth
```

---

### 🧠 Troubleshooting
```bash
# Check Bluetooth service status
sudo systemctl status bluetooth

# View logs
journalctl -u bluetooth -f

# Unbind rfcomm if needed
sudo rfcomm release 0
```

---

✅ **Now your cheat sheet is complete and production-ready**  
It works for **Arduino + HC-05/06**, **ESP32**, and supports **auto-connect on boot** and **custom Bluetooth names**.
