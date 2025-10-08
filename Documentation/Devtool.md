# 🧰 Yocto `devtool` Complete Guide

## 🎯 Purpose of `devtool`
`devtool` is a Yocto Project utility that helps you **develop, modify, and test your code or recipes quickly** without having to rebuild the entire image or flash your Raspberry Pi every time.

In short:  
> It simplifies the **development workflow** when working on recipes or source code before integrating final changes into your layer.

---

## ⚡ Why use `devtool` instead of `bitbake`?

| Comparison | `bitbake` | `devtool` |
|-------------|------------|------------|
| Main Purpose | Full image build | Fast development and testing of a single recipe |
| Speed | Slower (rebuilds dependencies) | Faster (builds only what changed) |
| Flexibility | Static | Dynamic – allows quick edits, builds, and testing |
| Best for | Production builds | Active development |

---

## 🪜 Basic `devtool` Workflow

### 1. Initialize the environment
Before using `devtool`, set up your Yocto build environment:
```bash
source oe-init-build-env
```

### 2. Add a new project
If you already have local source code (e.g., `button.cpp`) and want to create a recipe for it:
```bash
devtool add button /path/to/source/code
```
This automatically:
- Creates a temporary recipe.
- Sets up a build environment.
- Places the project inside `${BUILDDIR}/workspace`.

### 3. Build the code directly
Instead of using `bitbake`, build your recipe fast:
```bash
devtool build button
```

### 4. Deploy the result to the Target (e.g., Raspberry Pi)
```bash
devtool deploy-target button root@<raspberry_ip>
```
This will:
- Copy executables or service files to the target.
- Deploy without flashing or rebooting the device.

> ✅ Ideal for fast testing after small code edits.

### 5. Rebuild and redeploy after editing
```bash
devtool build button
devtool deploy-target button root@<raspberry_ip>
```

### 6. Finalize your recipe
```bash
devtool finish button ../meta-yourlayer/
```

---

## 🧩 Useful `devtool` Commands

| Command | Description |
|----------|--------------|
| `devtool status` | Show the status of all projects in the workspace |
| `devtool modify <recipe>` | Copy an existing recipe to modify its source |
| `devtool update-recipe <recipe>` | Update the recipe with your latest changes |
| `devtool undeploy-target <recipe> root@<ip>` | Remove deployed files from the target |
| `devtool reset <recipe>` | Reset the workspace project to its original state |
| `devtool remove <recipe>` | Remove the project from the workspace |

---

## 🧠 Benefits of `devtool`
- 🚀 **Accelerates the development cycle** without full rebuilds.
- 🧱 **Direct testing** on the target device.
- 🧩 **Temporary workspace** for safe experimentation.
- 🧑‍💻 **Smooth integration** into your Yocto layer.
- 🧼 **Keeps your builds clean** and modular.

---

## 📋 Quick Example

```bash
source oe-init-build-env
devtool add servo /home/abdo/projects/servo
devtool build servo
devtool deploy-target servo root@192.168.1.10
# After editing
devtool build servo
devtool deploy-target servo root@192.168.1.10
# Finalize
devtool finish servo ../meta-abozaid/
```

---

## ⚙️ Example: Using `devtool` with a Systemd Service

### 1. Project Structure
```
servo/
├── servo.cpp
├── CMakeLists.txt
└── servo.service
```

### 2. Example `servo.service`
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

### 3. Create a Temporary Recipe
```bash
source oe-init-build-env
devtool add servo /path/to/servo
```

### 4. Build the Service
```bash
devtool build servo
```

### 5. Deploy to the Raspberry Pi
```bash
devtool deploy-target servo root@192.168.1.10
ssh root@192.168.1.10
systemctl daemon-reload
systemctl enable servo.service
systemctl start servo.service
```

### 6. Rapid Updates
```bash
devtool build servo
devtool deploy-target servo root@192.168.1.10
systemctl restart servo.service
```

### 7. Save the Final Recipe
```bash
devtool finish servo ../meta-abozaid/
```

### 8. Clean Up
```bash
devtool reset servo
```

---

## ⚙️ Integrating `devtool` with Systemd and CMake Automatically
After running `devtool add`, edit the generated recipe and include:
```bash
inherit cmake systemd

SRC_URI += "file://servo.service"

SYSTEMD_SERVICE:${PN} = "servo.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"
```

Now run:
```bash
devtool build servo
devtool deploy-target servo root@192.168.1.10
```

The service will install and start automatically ✅

---

## 🧠 Summary
`devtool` = the **fast development tool** in Yocto.

It lets you:
1. Add and build projects quickly  
2. Deploy and test on the target instantly  
3. Finalize and export recipes to your layer  

> Save time, iterate faster, and use `bitbake` for your final production image builds.
