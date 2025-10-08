# **Custom GPIO Driver with Full Pull-Up/Down Support (BCM2837)**

## **Overview**

This kernel module provides a fully modular GPIO driver for the Raspberry Pi (BCM2837 SoC).
It directly accesses the physical GPIO registers via memory mapping and exposes user-friendly control files in `/sys/class/gpiomod/`.

Each GPIO pin is represented as a device directory (e.g., `/sys/class/gpiomod/gpio17`) containing files that allow reading and writing the pin’s **direction**, **value**, and **pull-up/down** state.

---

## **Core Concepts**

### **1. GPIO Devices**

* During initialization, the driver dynamically creates 28 GPIO devices (`gpio0` → `gpio27`).
* Each device corresponds to a physical GPIO pin on the BCM2837.
* Every device directory in `/sys/class/gpiomod/` contains:

  ```
  direction
  value
  pull_up
  pull_down
  ```

These files represent the main control interface between user space and hardware.

---

## **2. Direction Attribute**

**Files:** `/sys/class/gpiomod/gpioX/direction`

Controls whether the GPIO pin is configured as **input** or **output**.

### **Functions**

* **`direction_show()`** → Reads the pin configuration from hardware.
* **`direction_store()`** → Writes configuration (`in` or `out`) to hardware.

### **How it Works**

* The driver calculates the corresponding **GPFSEL register** and bit shift.
* It clears or sets bits in the register to configure the pin as input/output.

### **Example**

```bash
echo out > /sys/class/gpiomod/gpio17/direction
echo in > /sys/class/gpiomod/gpio17/direction
```

---

## **3. Value Attribute**

**Files:** `/sys/class/gpiomod/gpioX/value`

Controls the logic level of the GPIO pin.

### **Functions**

* **`value_show()`** → Reads the current logical state (`0` or `1`).
* **`value_store()`** → Sets the output value (`0` or `1`).

### **How it Works**

* For output pins:

  * Writing `1` sets the bit in the **GPSET0 register**.
  * Writing `0` clears the bit in the **GPCLR0 register**.
* For input pins:

  * The value is read from the **GPLEV0 register**.

### **Example**

```bash
# Set GPIO17 HIGH
echo 1 > /sys/class/gpiomod/gpio17/value

# Set GPIO17 LOW
echo 0 > /sys/class/gpiomod/gpio17/value

# Read input value
cat /sys/class/gpiomod/gpio17/value
```

---

## **4. Pull-Up and Pull-Down Attributes**

**Files:**

* `/sys/class/gpiomod/gpioX/pull_up`
* `/sys/class/gpiomod/gpioX/pull_down`

These control the internal resistors of the GPIO pin.

### **Enum Definition**

```c
enum pull_mode {
    PULL_DISABLE = 0,
    PULL_DOWN    = 1,
    PULL_UP      = 2
};
```

### **Functions**

* **`pull_up_show()` / `pull_down_show()`** → Display the current pull state.
* **`pull_up_store()` / `pull_down_store()`** → Enable/disable pull resistors.

### **How it Works**

* The driver writes to the **GPPUD** and **GPPUDCLK0** registers:

  1. Write the pull mode (disable, down, or up) to GPPUD.
  2. Wait 5 microseconds.
  3. Clock the target GPIO pin in GPPUDCLK0.
  4. Wait another 5 microseconds.
  5. Clear both registers.

### **Example**

```bash
# Enable pull-up resistor on GPIO5
echo 1 > /sys/class/gpiomod/gpio5/pull_up

# Disable pull-up
echo 0 > /sys/class/gpiomod/gpio5/pull_up

# Enable pull-down
echo 1 > /sys/class/gpiomod/gpio5/pull_down
```

---

## **5. Initialization Function (`mygpio_init`)**

### **Purpose**

Executed when the module is loaded into the kernel.

### **Main Steps**

1. **Memory Mapping:**
   Maps physical GPIO registers (`0x3F200000`) into virtual memory.

2. **Class Creation:**
   Creates `/sys/class/gpiomod/` as a sysfs class.

3. **Device Creation:**
   Generates devices for all 28 GPIO pins with associated sysfs attributes:

   * direction
   * value
   * pull_up
   * pull_down

4. **Logging:**
   Prints a message to the kernel log confirming driver load success.

---

## **6. Exit Function (`mygpio_exit`)**

### **Purpose**

Executed when the module is removed (via `rmmod`).

### **Main Steps**

* Destroys all device entries.
* Cleans up sysfs class.
* Unmaps the memory region used by the GPIO registers.
* Logs a message confirming safe unload.

---

## **7. Supporting Structures**

### **Device Attributes**

Each file in `/sys/class/gpiomod/gpioX/` is represented by a `DEVICE_ATTR` structure:

```c
static DEVICE_ATTR_RW(direction);
static DEVICE_ATTR_RW(value);
static DEVICE_ATTR_RW(pull_up);
static DEVICE_ATTR_RW(pull_down);
```

These structures bind each file to its show/store handler functions.

---

## **8. Enums and State Tracking**

### **Enum: `pull_mode`**

Defines all pull configuration states for easy code readability.

### **Global Array**

```c
static enum pull_mode current_pull[GPIO_COUNT] = {
    [0 ... (GPIO_COUNT - 1)] = PULL_DISABLE
};
```

Tracks the last configured pull state for each GPIO pin.

---

## **9. How to Build and Use**

### **Build**

```bash
make
sudo insmod gpiomod.ko
or 
insmod gpiomod.ko
or 
opkg gpiomod.ko
```

### **Check sysfs**

```bash
ls /sys/class/gpiomod/gpio17
```

### **Unload**

```bash
sudo rmmod gpiomod
```

---

## **10. Summary Table**

| Attribute     | Purpose                         | File Example                          |
| ------------- | ------------------------------- | ------------------------------------- |
| **direction** | Set pin mode (in/out)           | `/sys/class/gpiomod/gpio17/direction` |
| **value**     | Read/write logic state          | `/sys/class/gpiomod/gpio17/value`     |
| **pull_up**   | Enable/disable pull-up resistor | `/sys/class/gpiomod/gpio17/pull_up`   |
| **pull_down** | Enable/disable pull-down        | `/sys/class/gpiomod/gpio17/pull_down` |

---

### **Author**

**Abdo Essam AboZaid**
Custom Modular GPIO Driver for Raspberry Pi BCM2837
Licensed under **GPL**
