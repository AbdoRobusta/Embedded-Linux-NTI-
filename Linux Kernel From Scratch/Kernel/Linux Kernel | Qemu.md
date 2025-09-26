## 🔹 Build the Linux Kernel | Qemu

### 1. Download Kernel Source
```bash
git clone https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.tar.xz
tar xf linux-6.1.tar.xz
cd linux-6.1
```

### 2. Install Dependencies
```bash
sudo apt install build-essential flex libncurses5-dev bc libelf-dev bison libssl-dev u-boot-tools
```

### 3. Configure and Build
```bash
export ARCH=arm
export CROSS_COMPILE=~/x-tools/arm-training-linux-gnueabihf/bin/arm-linux-
make vexpress_ca9x4_defconfig
make menuconfig
make -j$(nproc)
```
> the output Image is in :

    /home/abdo/linux-6.1/arch/arm/boot/zImage

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
sudo mount /dev/loop<x>p1 /mnt/boot
sudo mount /dev/loop<x>p2 /mnt/root

sudo cp /home/abdo/linux-6.1/arch/arm/boot/zImage /mnt/boot
sudo cp /home/abdo/linux-6.1/arch/arm/boot/dts/vexpress-v2p-ca9.dtb /mnt/boot
```
> next  this step after you cpoy the zimage , file.dtb and the init process :

```bash
sudo umount /mnt/boot
sudo umount /mnt/root
```
## 🔹 Build Qemu 
```bash
sudo qemu-system-arm -M vexpress-a9 -m 128M -nographic -kernel u-boot -sd sd.img
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

