
# 🧩 Systemd and Services Guide

## ⚙️ What is Systemd?

**Systemd** is a system and service manager for Linux.  
It is responsible for starting, stopping, and managing background processes (services) during and after system boot.

It replaces the old SysV init system and is now used in most modern Linux distributions, including **Yocto**, **Ubuntu**, and **Raspberry Pi OS**.

---

## 🧱 What is a Service?

A **service** in Linux is a background process managed by systemd.  
It can start automatically during boot or be manually controlled by the user.

**Examples:**
- `NetworkManager.service` → manages network connections  
- `sshd.service` → runs the SSH server  
- `servo.service` → your custom service to control hardware or software logic

---

## 🧠 What Does Systemd Do?

| Function | Description |
|-----------|-------------|
| **Boot Management** | Starts system services in the correct order during boot. |
| **Service Supervision** | Keeps services running, restarts them if they crash. |
| **Dependency Handling** | Defines startup order and dependencies between services. |
| **Logging Integration** | Works with `journald` for central log storage. |
| **Parallel Startup** | Launches multiple services simultaneously to speed up boot. |
| **Socket & Timer Units** | Supports on-demand or scheduled service activation. |

---

## 🧩 Structure of a `.service` File

Systemd service files are stored in `/lib/systemd/system/` or `/etc/systemd/system/`.

Example service file:
```ini
[Unit]
Description=Servo Control Service
After=network.target

[Service]
ExecStart=/usr/bin/servo
Restart=always

[Install]
WantedBy=multi-user.target
```

### Explanation

| Section | Purpose |
|----------|----------|
| **[Unit]** | General information and dependencies (`After`, `Before`, `Requires`). |
| **[Service]** | Defines how the process runs (`ExecStart`, `Restart`, etc.). |
| **[Install]** | Specifies when and how it starts automatically (`WantedBy`). |

---

## 🚀 Systemd Command Reference

| Command | Description |
|----------|-------------|
| `systemctl start myservice.service` | Start the service manually. |
| `systemctl stop myservice.service` | Stop the service. |
| `systemctl restart myservice.service` | Restart the service. |
| `systemctl enable myservice.service` | Enable auto-start at boot. |
| `systemctl disable myservice.service` | Disable auto-start. |
| `systemctl status myservice.service` | View service status and logs. |
| `journalctl -u myservice.service` | View logs related to the service. |
| `systemctl daemon-reload` | Reload configuration after editing `.service` files. |

---

## 🧰 Benefits of Using Systemd Services

| Benefit | Description |
|----------|-------------|
| **Automation** | Automatically starts your app at boot. |
| **Reliability** | Restarts services automatically after crashes. |
| **Logging** | Provides integrated logging with `journalctl`. |
| **Dependency Control** | Ensures correct startup order between services. |
| **Resource Management** | Limits CPU, memory, and I/O per service. |
| **Portability** | Works seamlessly across Linux distributions and Yocto. |

---

## 🕒 When to Use a Systemd Service

Use a service when:
- You need your program to **start at boot**.
- You want it to **run continuously** in the background.
- You want **automatic restarts** on failure.
- Your app **depends on another service**, like networking.
- You want **centralized logs** for debugging or monitoring.
- You're deploying on **Yocto** or **embedded systems** (like Raspberry Pi).

---

## 💡 Real-Life Examples

| Use Case | Service Name | Description |
|-----------|---------------|-------------|
| GPIO Driver | `gpiomod.service` | Runs kernel GPIO driver. |
| Temperature Sensor | `temp.service` | Continuously reads and reports temperature. |
| Button Monitor | `button.service` | Detects button presses and triggers events. |
| Servo Controller | `servo.service` | Controls servo motor via PWM at boot. |

---

## ✅ Summary

- **Systemd** is the modern system manager in Linux.  
- **Services** are background programs managed by systemd.  
- You define them with `.service` files containing `[Unit]`, `[Service]`, and `[Install]` sections.  
- Control them using `systemctl`.  
- Essential for automation and reliability in **Yocto**, **embedded**, and **server** projects.

---

> 🧠 **Tip:** Always reload systemd (`systemctl daemon-reload`) after changing or adding a service file.
