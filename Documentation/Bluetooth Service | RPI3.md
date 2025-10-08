
# 🔵 Raspberry Pi Bluetooth Autostart & Renaming Guide (Using systemd)

## 🧩 Goal

- Ensure Bluetooth starts automatically on every boot  
- Keep the Bluetooth service always running (auto-restart)  
- Change the Raspberry Pi Bluetooth device name permanently  

---

## ⚙️ Step 1 – Check if Bluetooth Service Exists

Most Raspberry Pi or Yocto-based systems already include a Bluetooth service.

Check its status:
```bash
systemctl status bluetooth.service
```

If it’s inactive or missing, start it manually:
```bash
sudo systemctl start bluetooth.service
```

Verify it’s running:
```bash
systemctl status bluetooth.service
```

---

## 🚀 Step 2 – Enable Auto-Start on Boot

To make sure the Bluetooth service always runs after reboot:
```bash
sudo systemctl enable bluetooth.service
```

Now the service will start automatically on every boot.

---

## 🧰 Step 3 – (Optional) Create a Custom Bluetooth Service

If you want to run custom startup commands (for example, turning on the interface or setting the name),  
you can create your own `systemd` unit.

**Create file:** `/etc/systemd/system/mybluetooth.service`
```ini
[Unit]
Description=Custom Bluetooth Service
After=bluetooth.service
Requires=bluetooth.service

[Service]
ExecStart=/usr/bin/hciconfig hci0 up
ExecStartPost=/usr/bin/hciconfig hci0 name "RaspberryPi-BT"
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

### Explanation

| Directive | Description |
|------------|-------------|
| **After=bluetooth.service** | Starts only after the main Bluetooth service. |
| **ExecStart** | Activates the Bluetooth interface. |
| **ExecStartPost** | Sets a custom name after startup. |
| **Restart=on-failure** | Automatically restarts on crash. |
| **WantedBy=multi-user.target** | Ensures startup during the normal boot process. |

---

## 🧩 Step 4 – Reload and Enable the Custom Service

After creating the service file:
```bash
sudo systemctl daemon-reload
sudo systemctl enable mybluetooth.service
sudo systemctl start mybluetooth.service
```

---

## 🧠 Step 5 – Change Bluetooth Name

### 🔹 Option 1: Temporary Change
This method works until the next reboot.
```bash
sudo hciconfig hci0 name "AbdoPi"
```

---

### 🔹 Option 2: Permanent Change (Recommended)

Edit the main Bluetooth configuration file:
```bash
sudo nano /etc/bluetooth/main.conf
```

Find the line:
```
#Name = BlueZ
```

Uncomment and modify it to:
```
Name = AbdoPi
```

Save and exit, then restart the Bluetooth service:
```bash
sudo systemctl restart bluetooth.service
```

---

## ✅ Step 6 – Verify the Device Name

Check the current Bluetooth name:
```bash
hciconfig hci0 name
```

You should now see your new name (e.g., `AbdoPi`).  
You can also verify it by scanning for Bluetooth devices from another phone or computer.

---

## 💡 Tip: Combine Both Approaches

- Use the permanent name in `/etc/bluetooth/main.conf`.  
- Use your custom systemd service (`mybluetooth.service`) only to **ensure Bluetooth starts automatically** and stays active.

---

## 🧾 Summary

| Task | Command / File |
|------|----------------|
| Check service | `systemctl status bluetooth.service` |
| Enable at boot | `sudo systemctl enable bluetooth.service` |
| Custom service file | `/etc/systemd/system/mybluetooth.service` |
| Reload systemd | `sudo systemctl daemon-reload` |
| Change name | `sudo hciconfig hci0 name "NewName"` or edit `/etc/bluetooth/main.conf` |
| Verify | `hciconfig hci0 name` |

---

> 🧠 **Pro Tip:** If you’re using Yocto, you can include your `.service` file in your recipe and set `SYSTEMD_AUTO_ENABLE = "enable"` to have it installed and auto-started automatically in your custom image.
