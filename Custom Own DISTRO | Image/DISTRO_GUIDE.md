# 🧩 Abozaid Linux - Full Custom Yocto Build Guide (A to Z)

This guide explains how to build a complete **custom Yocto distribution** for **Raspberry Pi 3**,  
based on **Poky** and **core-image-weston**, but rebranded as **Abozaid Linux**.

---

## 1️⃣ Prerequisites
Make sure you have:
- Ubuntu 22.04 or later
- Git, Python3, GCC, and build tools
- At least 100GB free disk space

Install dependencies:
```bash
sudo apt update
sudo apt install gawk wget git-core diffstat unzip texinfo gcc build-essential chrpath socat libsdl1.2-dev xterm
```

---

## 2️⃣ Download Poky & Layers
```bash
mkdir ~/yocto && cd ~/yocto
git clone git://git.yoctoproject.org/poky -b kirkstone
git clone git://git.openembedded.org/meta-openembedded -b kirkstone
git clone https://github.com/agherzan/meta-raspberrypi.git -b kirkstone
```

---

## 3️⃣ Initialize Build Environment
```bash
cd poky
source oe-init-build-env ../abozaid_build
```

Now you have:
```
abozaid_build/
├── conf/
│   ├── local.conf
│   └── bblayers.conf
```

---

## 4️⃣ Add Shared Directories (Downloads & Sstate)
To save rebuild time, add this in `conf/local.conf`:
```bash
DL_DIR ?= "/home/abdo/yocto/downloads"
SSTATE_DIR ?= "/home/abdo/yocto/sstate-cache"
```

---

## 5️⃣ Add Your Machine
Set the target board:
```bash
MACHINE ?= "raspberrypi3-64"
```

---

## 6️⃣ Add Layers to bblayers.conf
Edit `conf/bblayers.conf`:
```bash
BBLAYERS += " \
  /home/abdo/yocto/poky/meta \
  /home/abdo/yocto/poky/meta-poky \
  /home/abdo/yocto/meta-openembedded/meta-oe \
  /home/abdo/yocto/meta-raspberrypi \
  /home/abdo/yocto/meta-abozaid \
"
```

---

## 7️⃣ Create Your Custom Layer
```bash
cd ~/yocto
bitbake-layers create-layer ../meta-abozaid
bitbake-layers add-layer ../meta-abozaid
```

---

## 8️⃣ Create Custom Distro Configuration
Inside your layer:
```bash
mkdir -p meta-abozaid/conf/distro
nano meta-abozaid/conf/distro/abozaid.conf
```
Paste the config from this repo.

Then set your distro in `local.conf`:
```bash
DISTRO ?= "abozaid"
```

---

## 9️⃣ Create Your Custom Image
```bash
mkdir -p meta-abozaid/recipes-core/images
nano meta-abozaid/recipes-core/images/core-image-abozaid.bb
```

Paste the Weston-based image content.

---

## 🔟 Build Your Image
```bash
bitbake core-image-abozaid
```

---

## 11️⃣ Deploy to SD Card
After build completes:
```bash
cd tmp/deploy/images/raspberrypi3-64
sudo dd if=core-image-abozaid-raspberrypi3-64.wic.gz of=/dev/sdX bs=4M status=progress
sync
```

---

## 12️⃣ Boot & Enjoy
Insert SD card into Raspberry Pi 3, power it up,  
and you should see **Abozaid Linux with Weston Desktop**.

---

## ✅ Folder Structure
```
meta-abozaid/
├── conf/
│   ├── layer.conf
│   └── distro/
│       └── abozaid.conf
└── recipes-core/
    └── images/
        └── core-image-abozaid.bb
```

---

## 🧑‍💻 Author
**Abdo Essam Abozaid**  
Custom Yocto Distribution for Raspberry Pi 3  
Version 1.0 - "Falcon"
