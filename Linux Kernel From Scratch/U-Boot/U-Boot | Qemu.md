# 🖥️ U-Boot, Linux Kernel, and BusyBox Setup for QEMU

---

## 🔹 U-Boot for QEMU

### 1. Download U-Boot
```bash
git clone https://github.com/u-boot/u-boot.git
cd u-boot
git checkout v2023.01
```

### 2. Export Environment
```bash
export ARCH=arm
export CROSS_COMPILE=~/x-tools/arm-training-linux-gnueabihf/bin/arm-linux-
```

### 3. Configure U-Boot
```bash
make vexpress_ca9x4_defconfig
```

> If you face errors:
```bash
make distclean
```

### 4. Run Menuconfig
```bash
make menuconfig
```
- Environment submenu:  
  1. Unset: Environment is in flash filesystem  
  2. Set: Environment is in a FAT filesystem  
  3. Name of block device → `mmc`  
  4. Device and Partition → `0:1`  
  ✅ Enables `fatload` commands  

- Command Line Interface submenu:  
  1. Boot commands → enable `bootd`  
  2. Environment commands → enable `editenv`

### 5. Build
```bash
make -j$(nproc)
```

### 6. Test in QEMU
```bash
sudo qemu-system-arm -M vexpress-a9 -m 128M -nographic -kernel u-boot
```

---

## 🔹 SD Card Setup for QEMU

### 1. Create SD Card Image
```bash
dd if=/dev/zero of=sd.img bs=1M count=1024
```

### 2. Partition the Image
```bash
cfdisk sd.img
```
- three Partitions are primary
- Partition 1: 64M, FAT16, bootable  
- Partition 2: 8M, root filesystem  
- Partition 3: rest of card, data filesystem  
- Press **Write** when done  

### 3. Setup Loop Device
```bash
sudo losetup -f --show --partscan sd.img
```

### 4. Format Boot Partition
```bash
sudo mkfs.vfat -F 16 -n boot /dev/loop<x>p1
```

### 5. Detach Loop Device
```bash
sudo losetup -d /dev/loop<x>
```

### 6. Run U-Boot with SD Card
```bash
sudo qemu-system-arm -M vexpress-a9 -m 128M -nographic -kernel u-boot -sd sd.img
```
---
