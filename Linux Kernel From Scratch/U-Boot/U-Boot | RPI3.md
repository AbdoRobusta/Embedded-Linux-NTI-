# 🖥️ U-Boot, Linux Kernel, and BusyBox Setup for QEMU

---

## 🔹 U-Boot for QEMU

### 1. Download U-Boot
```bash
git clone https://github.com/u-boot/u-boot.git
http://github.com/raspberrypi/firmware.git
cd u-boot
git checkout 2023.01
```

### 2. Export Environment
```bash
export ARCH=arm
export CROSS_COMPILE=~/x-tools/aarch64-rpi3-linux-gnu/bin/aarch64-rpi3-linux-gnu
```

### 3. Configure U-Boot
```bash
make bcmrpi3_defconfig
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
### 6. SD card setup for Raspberry Pi
```bash
dmesg | tail
```
- unmount the 2 partitions if already existing
```bash
sudo umount /dev/sdb1

sudo umount /dev/sdb2
```
- let us now format our partition 
```bash
sudo fdisk /dev/sdb
```
1- type d to delete partition 1
2- type d again to delete partition 2
3- type w to confirm and write updates
4- type n to add new partition 

### 7. Format the 2 Partitions 
```bash
sudo mkfs.vfat -F 32 -n boot /dev/sdb1
sudo mkfs.ext4 -L root /dev/sdb2
```
### 8. mount 2 partitions
```bash
sudo mount /dev/sdb1 /mnt/boot/
sudo mount /dev/sdb2 /mnt/root/
```
##  🔹 Prepare the SDcard
```bash
sudo cp boot.bin /mnt/boot/
```
- copy the Raspberrypi boot firmware from the firmware/boot into the boot partition 
```bash 
sudo cp boot/{bootcode.bin,start.elf} /mnt/boot
```
> create a config.txt file and add this lines for RPI bootloader to load the Uboot:

    a. enable_uart=1
    b. arm_64bit=1
    c. kernel=u-boot.bin

```bash 
sudo mv config.txt /mnt/boot