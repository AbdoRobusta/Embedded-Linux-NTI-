#include <iostream>
#include <fstream>
#include <string>

class Led {
private:
    int gpio;

    void writeFile(const std::string &path, const std::string &value) {
        std::ofstream fs(path);
        if (!fs.is_open()) {
            std::cerr << "Error: cannot open " << path << std::endl;
            return;
        }
        fs << value;
    }

public:
    Led(int pin) : gpio(pin) {
        writeFile("/sys/class/gpio/export", std::to_string(gpio));
        writeFile("/sys/class/gpio/gpio" + std::to_string(gpio) + "/direction", "out");
    }

    void on() {
        writeFile("/sys/class/gpio/gpio" + std::to_string(gpio) + "/value", "1");
        std::cout << "LED ON ✅" << std::endl;
    }

    void off() {
        writeFile("/sys/class/gpio/gpio" + std::to_string(gpio) + "/value", "0");
        std::cout << "LED OFF ❌" << std::endl;
    }

    ~Led() {
        writeFile("/sys/class/gpio/unexport", std::to_string(gpio));
    }
};

int main() {
    Led led(17); // GPIO17 on RPi3 (physical pin 11)

    char input;
    std::cout << "Enter 1=ON, 0=OFF, q=quit" << std::endl;
    while (true) {
        std::cin >> input;
        if (input == '1') led.on();
        else if (input == '0') led.off();
        else if (input == 'q') break;
    }

    return 0;
}

