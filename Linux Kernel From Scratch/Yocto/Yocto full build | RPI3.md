# Yocto Build Workflow — Fetch + Full Build (with Shared Caches & Raspberry Pi 3 Setup)
**Example target:** `core-image-weston` for `raspberrypi3-64`

---

## 1️⃣ Repository Setup (Cloning Poky + Layers)

Before building, clone the essential Yocto layers:

```bash
# Create workspace
mkdir -p ~/yocto && cd ~/yocto

# Clone Poky (Yocto Project Core)
git clone git://git.yoctoproject.org/poky -b scarthgap

# Clone meta-openembedded (extra recipes and dependencies)
git clone git://git.openembedded.org/meta-openembedded -b scarthgap

# Clone Raspberry Pi BSP layer
git clone git://git.yoctoproject.org/meta-raspberrypi -b scarthgap

# Clone meta-yocto-bsp (for reference board support)
git clone git://git.yoctoproject.org/meta-yocto-bsp -b scarthgap
```

Then add these layers to your environment later with `bitbake-layers add-layer`.

---

## 2️⃣ Configure Build Environment

```bash
cd ~/yocto/poky
source oe-init-build-env build
bitbake-layers add-layer ../meta-openembedded/meta-oe
bitbake-layers add-layer ../meta-openembedded/meta-networking
bitbake-layers add-layer ../meta-openembedded/meta-python
bitbake-layers add-layer ../meta-raspberrypi
bitbake-layers add-layer ../meta-yocto-bsp
bitbake-layers show-layers
```

Update your configuration file `conf/local.conf`:

```conf
MACHINE ?= "raspberrypi3-64"
DISTRO_FEATURES:append = " systemd wifi bluetooth"
EXTRA_IMAGE_FEATURES += "ssh-server-dropbear"

# Shared cache directories (recommended for CI or multiple developers)
DL_DIR ?= "/home/shared/yocto/downloads"
SSTATE_DIR ?= "/home/shared/yocto/sstate-cache"

# Parallel build optimizations
BB_NUMBER_THREADS ?= "8"
PARALLEL_MAKE ?= "-j 8"
```

> 💡 Shared `DL_DIR` and `SSTATE_DIR` allow multiple projects to reuse downloads and build caches, greatly reducing build time and bandwidth.

---

## 3️⃣ Running the Fetch Stage Only

To download all required sources for your image:

```bash
bitbake core-image-weston --runonly=fetch
```

### What happens:
- BitBake runs only `do_fetch` for all recipes needed by `core-image-weston`.
- Sources (tarballs, git repos) are stored in `/home/shared/yocto/downloads/`.
- No build or compile occurs.

### Typical output example:
```
NOTE: Fetching git://anongit.freedesktop.org/wayland/wayland;branch=master
NOTE: Downloaded to /home/shared/yocto/downloads/wayland-master.tar.gz
NOTE: Fetching weston from git://anongit.freedesktop.org/weston/weston
```
If something fails:
```
ERROR: Fetcher failure for URL: 'https://example.com/missing.tar.gz'
ERROR: Function failed: do_fetch
```
Check logs:
```bash
less tmp/work/*/weston/*/temp/log.do_fetch.*
```

---

## 4️⃣ Running the Full Build

Once everything is fetched, perform the full build:

```bash
bitbake core-image-weston
```

### Expected results:
- Builds all required packages and dependencies.
- Assembles a complete image for Raspberry Pi 3.
- Final image files appear in:
```
tmp/deploy/images/raspberrypi3-64/core-image-weston-<version>.rpi-sdimg
tmp/deploy/images/raspberrypi3-64/core-image-weston-<version>.wic
```

### Example output snippet:
```
Parsing recipes: 100% |##########################################| Time: 00:00:34
Building environment: 100% |####################################| Time: 00:00:12
NOTE: Running task 1200 of 4200 (ID: 123, weston_9.0.bb, do_compile)
...
NOTE: Tasks Summary: Attempted 4200 tasks of which 4200 succeeded
```

---

## 5️⃣ Yocto Build Stages (Quick Summary)

| Stage | Task | Description |
|-------|------|-------------|
| **1** | `do_fetch` | Downloads all sources |
| **2** | `do_unpack` | Extracts archives |
| **3** | `do_patch` | Applies patches |
| **4** | `do_configure` | Configures build system |
| **5** | `do_compile` | Compiles sources |
| **6** | `do_install` | Installs into pseudo root |
| **7** | `do_package` | Creates `.ipk/.deb/.rpm` packages |
| **8** | `do_image` | Builds final filesystem/image |

---

## 6️⃣ Troubleshooting & Maintenance

### 🧩 Common Errors

**Fetcher failure (URL not found)**  
Check your `SRC_URI` and internet connection. Retry or update recipe URL.

**Checksum mismatch**  
If source updated, replace hash manually with:  
```bash
bitbake -c fetch <recipe>
sha256sum <downloaded-file>
```

**Compile failure (missing dependency)**  
Add required dependency:
```bitbake
DEPENDS += "libxkbcommon"
```

### 🧰 Useful BitBake commands
```bash
bitbake -c fetch <recipe>        # Fetch only one recipe
bitbake -c cleanall <recipe>     # Clean recipe (downloads + cache)
bitbake -c devshell <recipe>     # Open interactive shell for debugging
bitbake-layers show-recipes      # List available recipes
```

---

## 7️⃣ Using Shared Caches Across Multiple Builds

To share `downloads/` and `sstate-cache` across builds or CI servers:

```bash
mkdir -p /home/shared/yocto/{downloads,sstate-cache}
sudo chmod -R 777 /home/shared/yocto
```
In each project’s `conf/local.conf`, reference the shared directories:
```conf
DL_DIR ?= "/home/shared/yocto/downloads"
SSTATE_DIR ?= "/home/shared/yocto/sstate-cache"
```

Now multiple developers or build nodes reuse the same sources and precompiled outputs.

---

## 8️⃣ Next Steps

- Flash the image to an SD card:
```bash
sudo dd if=tmp/deploy/images/raspberrypi3-64/core-image-weston-*.rpi-sdimg         of=/dev/sdX bs=4M status=progress conv=fsync
```
- Boot the Raspberry Pi and enjoy Weston GUI running from your custom Yocto build!

---

### 🧾 Reference Links

| Layer | Repository | Branch |
|--------|-------------|---------|
| **Poky (Yocto Core)** | [git://git.yoctoproject.org/poky](https://git.yoctoproject.org/poky) | `scarthgap` |
| **meta-openembedded** | [git://git.openembedded.org/meta-openembedded](https://git.openembedded.org/meta-openembedded) | `scarthgap` |
| **meta-raspberrypi** | [git://git.yoctoproject.org/meta-raspberrypi](https://git.yoctoproject.org/meta-raspberrypi) | `scarthgap` |
| **meta-yocto-bsp** | [git://git.yoctoproject.org/meta-yocto-bsp](https://git.yoctoproject.org/meta-yocto-bsp) | `scarthgap` |

---

✅ **Summary:**  
You now have a complete end-to-end Yocto setup for Raspberry Pi 3 using shared caches, layer sources, and `bitbake` fetch + build commands.
