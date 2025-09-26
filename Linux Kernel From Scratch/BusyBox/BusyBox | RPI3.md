## 🔹 BusyBox Compilation

### 1. Download BusyBox
```bash
git clone --b 1_36_stable http://git.busybox.net/busybox  --depth=1
cd busybox/
git checkout 1_36_stable
```

### 2. Setup Build Environment
```bash
export ARCH=arm64
export CROSS_COMPILE=~/x-tools/aarch64-rpi3-linux-gnu/bin/aarch64-rpi3-linux-gnu
```

### 3. Configure BusyBox
```bash
make menuconfig
```
- Enable: `[ * ] Build static binary`  
- Optionally add: `printenv` from core utils  

> If error:
```bash
make bcmrpi3_defconfig          #for RPI3
```
### Build the Busybox
```bash 
make -j(nproc)
```

### Install to _install directory
```bash
make install
```

### Create a root filesystem structure 
```bash 
mkdir -p ../04-busybox-rootfs
rsync -av _install/ ../04-busybox-rootfs/
```

> the next Step is copy the members inside 04-busybox-rootfs:
```bash
sudo cp -r ${$04-busybox-rootfs}/* /mnt/root/
sudo cp -r ~/x-tools/aarch64-rpi3-linux-gnu/aarch64-rpi3-linux-gnu/sysroot/* /mnt/root/
```

---