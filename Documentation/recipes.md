# Yocto Layer Explanation: meta-myproject

This document explains the structure of the custom Yocto layer `meta-myproject` and the purpose of its recipes.

---

## Layer Tree

```
meta-myproject/
├── conf
│   └── layer.conf
├── recipes-gpio
│   └── gpio
│       ├── files
│       │   ├── gpio.cpp
│       │   └── gpio.hpp
│       └── gpio_1.0.bb
└── recipes-main
    └── main
        ├── files
        │   └── main.cpp
        └── main_1.0.bb
```

### 1. `conf/layer.conf`
- Defines the configuration of the layer.
- Tells BitBake where to find recipes in this layer.

---

## GPIO Library Recipe

**Path:** `recipes-gpio/gpio/gpio_1.0.bb`

### Purpose
- Builds and installs a simple static library `libgpio.a` and its header `gpio.hpp`.
- Provides reusable GPIO functions for other applications.

### Content
```bitbake
DESCRIPTION = "GPIO Library"
LICENSE = "CLOSED"
SRC_URI = "file://gpio.cpp \\
           file://gpio.hpp"

S = "${WORKDIR}"

do_compile() {
    ${CXX} ${CXXFLAGS} -c gpio.cpp -o gpio.o
    ${AR} rcs libgpio.a gpio.o
}

do_install() {
    install -d ${D}${includedir}
    install -d ${D}${libdir}

    install -m 0644 gpio.hpp ${D}${includedir}/
    install -m 0644 libgpio.a ${D}${libdir}/
}
```

### Installed Files
- **`/usr/include/gpio.hpp`** (header file)
- **`/usr/lib/libgpio.a`** (static library)

---

## Main Application Recipe

**Path:** `recipes-main/main/main_1.0.bb`

### Purpose
- Builds the executable `gpio_app` that links against the `gpio` library.

### Content
```bitbake
DESCRIPTION = "Main app using GPIO"
LICENSE = "CLOSED"
SRC_URI = "file://main.cpp"

DEPENDS += "gpio"

S = "${WORKDIR}"

do_compile() {
    ${CXX} ${CXXFLAGS} ${LDFLAGS} \\
        -I${STAGING_INCDIR} \\
        -L${STAGING_LIBDIR} \\
        main.cpp -lgpio \\
        -o gpio_app
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 gpio_app ${D}${bindir}/
}
```

### Installed Files
- **`/usr/bin/gpio_app`** → main program executable

---

## Build Process Overview

1. **gpio recipe**
   - Compiles `gpio.cpp` → `gpio.o`
   - Archives it into `libgpio.a`
   - Installs library + header into sysroot and target image.

2. **main recipe**
   - Compiles `main.cpp` with `-I` and `-L` pointing to sysroot paths.
   - Links against `libgpio.a`.
   - Installs final binary `gpio_app` into `/usr/bin`.

3. **Image Build**
   - When added to an image recipe (`IMAGE_INSTALL += "main gpio"`), both packages get installed on the target (Raspberry Pi).

---

## Summary

- `gpio` provides a reusable static library.
- `main` builds an app that depends on and links against `gpio`.
- Together, they form a modular Yocto setup for GPIO programming on the Raspberry Pi.

# Yocto Recipes Documentation

This document explains the structure and function of two Yocto recipes:  
1. **gpio_1.0.bb**  
2. **main_1.0.bb**  

---

## 📌 gpio_1.0.bb

```bitbake
DESCRIPTION = "GPIO Library"
LICENSE = "CLOSED"
SRC_URI = "file://gpio.cpp \\
           file://gpio.hpp"

S = "${WORKDIR}"

do_compile() {
    ${CXX} ${CXXFLAGS} -c gpio.cpp -o gpio.o
    ${AR} rcs libgpio.a gpio.o
}

do_install() {
    install -d ${D}${includedir}
    install -d ${D}${libdir}

    install -m 0644 gpio.hpp ${D}${includedir}/
    install -m 0644 libgpio.a ${D}${libdir}/
}
```

### 🔎 Explanation

- **DESCRIPTION** → Describes this recipe (“GPIO Library”).  
- **LICENSE** → `CLOSED`, meaning no open-source license.  
- **SRC_URI** → Provides source files (`gpio.cpp`, `gpio.hpp`) from the `files/` directory.  
- **S = "${WORKDIR}"** → The build directory where sources are unpacked.  

#### do_compile
- Compiles `gpio.cpp` into `gpio.o`.  
- Archives it into a static library `libgpio.a`.  

#### do_install
- Installs `gpio.hpp` into `/usr/include/`.  
- Installs `libgpio.a` into `/usr/lib/`.  

👉 **Result:** A reusable static library and header file for other recipes.

---

## 📌 main_1.0.bb

```bitbake
DESCRIPTION = "Main app using GPIO"
LICENSE = "CLOSED"
SRC_URI = "file://main.cpp"

DEPENDS += "gpio"

S = "${WORKDIR}"

do_compile() {
    ${CXX} ${CXXFLAGS} ${LDFLAGS} \\
        -I${STAGING_INCDIR} \\
        -L${STAGING_LIBDIR} \\
        main.cpp -lgpio \\
        -o gpio_app
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 gpio_app ${D}${bindir}/
}
```

### 🔎 Explanation

- **DESCRIPTION** → Recipe for the main application using the GPIO library.  
- **LICENSE** → Closed license.  
- **SRC_URI** → Provides `main.cpp`.  
- **DEPENDS += "gpio"** → Ensures the `gpio` library is built first.  

#### do_compile
- Compiles `main.cpp`.  
- Includes headers from `${STAGING_INCDIR}` (e.g., `/usr/include`).  
- Links against libraries from `${STAGING_LIBDIR}` (e.g., `/usr/lib`).  
- Links with `-lgpio` (the static library built in `gpio_1.0.bb`).  
- Produces an executable `gpio_app`.  

#### do_install
- Installs the compiled executable into `/usr/bin/gpio_app`.  

👉 **Result:** The `gpio_app` binary depends on the `gpio` library.

---

## 📊 Build Flow

```text
+-------------------+          +-------------------+
|   gpio_1.0.bb     |          |   main_1.0.bb     |
|-------------------|          |-------------------|
| gpio.cpp/gpio.hpp |          | main.cpp          |
| ↓ build           |          | DEPENDS on gpio   |
| libgpio.a + .hpp  | -------> | compile & link    |
| installed in sysroot         | with -lgpio       |
+-------------------+          +-------------------+
                                     ↓
                             /usr/bin/gpio_app
```

---

## ✅ Final Image Contents

- `/usr/include/gpio.hpp`  
- `/usr/lib/libgpio.a`  
- `/usr/bin/gpio_app`  

This setup allows you to build a reusable GPIO library and an application that depends on it inside Yocto.

