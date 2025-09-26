## 🔹 Build the Linux Kernel | Qemu

### 1. Download Kernel Source
```bash
git clone --depth=1 https://github.com/raspberrypi/linux.git  --branch rpi-6.6.y
tar xf linux.tar.xz
cd linux
```

### 2. Install Dependencies
```bash
sudo apt install build-essential flex libncurses5-dev bc libelf-dev bison libssl-dev u-boot-tools
```

### 3. Configure and Build
```bash
export ARCH=arm64
export CROSS_COMPILE=~/x-tools/aarch64-rpi3-linux-gnu/bin/aarch64-rpi3-linux-gnu
make bcmrpi3_defconfig
make menuconfig
make -j$(nproc)
```
> the output Image is in :

    /home/abdo/linux/arch/arm64/boot/Image

## 🔹 SD Card Setup for QEMU

### 1. Create SD Card Image
```bash
dd if=/dev/zero of=sd.img bs=1M count=1024
```

### 2. Partition the Image
```bash
cfdisk sd.img
```
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

## 🔹 Mount SD and add zImage , vexpress-v2p-ca9.dtb and Init process
```bash
sudo cp /home/abdo/linux/arch/arm64/boot/Image /mnt/boot
sudo cp /home/abdo/linux/arch/arm64/boot/dts/broadcom/bcm2710-rpi-3-b.dtb /mnt/boot
```
> next  this step after you cpoy the image , file.dtb :

```bash
sudo umount /mnt/boot
sudo umount /mnt/root
```

## 🔹 In U-Boot terminal
```bash
set loadKernel "load mmc 0:1 $kernel_addr_r zImage" 
set loadDtb "load mmc 0:1 $fdt_addr_r vexpress-v2p-ca9.dtb"
set bootmyImage "bootz $kernel_addr_r - $fdt_addr_r"
setenv bootargs "console=ttyAMA0,115200 root=/dev/mmcblk0p2 rootwait rw"

saveenv

set script "run loadKernel ; run loadDtb ; run bootmyImage" 
set bootcmd "run script" 

saveenv

run bootcmd
```

