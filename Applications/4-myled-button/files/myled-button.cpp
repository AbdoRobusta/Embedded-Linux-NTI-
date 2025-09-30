#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

#define LED_PIN "17"
#define BTN_PIN "27"
#define GPIO_PATH "/sys/class/gpio/"

class GPIO {
protected:
    std::string pin;

    void writeFile(const std::string& path, const std::string& value) {
        std::ofstream file(path);
        if (file.is_open()) {
            file << value;
            file.close();
        } else {
            std::cerr << "Failed to open " << path << "\n";
        }
    }

    std::string readFile(const std::string& path) {
        std::ifstream file(path);
        std::string value;
        if (file.is_open()) {
            file >> value;
            file.close();
        }
        return value;
    }

public:
    GPIO(const std::string& gpio_pin) : pin(gpio_pin) {
        writeFile(GPIO_PATH "export", pin);
        usleep(100000);
    }

    ~GPIO() {
        writeFile(GPIO_PATH "unexport", pin);
    }
};

class LED : public GPIO {
    bool state;
public:
    LED(const std::string& pin) : GPIO(pin), state(false) {
        writeFile(GPIO_PATH "gpio" + pin + "/direction", "out");
    }

    void turnOn() {
        writeFile(GPIO_PATH "gpio" + pin + "/value", "1");
        state = true;
        std::cout << "LED ON\n";
    }

    void turnOff() {
        writeFile(GPIO_PATH "gpio" + pin + "/value", "0");
        state = false;
        std::cout << "LED OFF\n";
    }

    void toggle() {
        state ? turnOff() : turnOn();
    }
};

class Button : public GPIO {
public:
    Button(const std::string& pin) : GPIO(pin) {
        writeFile(GPIO_PATH "gpio" + pin + "/direction", "in");
    }

    bool isPressed() {
        std::string value = readFile(GPIO_PATH "gpio" + pin + "/value");
        return (value == "0");
    }
};

int main() {
    LED led(LED_PIN);
    Button button(BTN_PIN);

    std::cout << "Press button on GPIO" << BTN_PIN 
              << " to toggle LED on GPIO" << LED_PIN << "...\n";

    bool lastState = false;

    while (true) {
        bool current = button.isPressed();
        if (current && !lastState) { 
            led.toggle();
        }
        lastState = current;
        usleep(100000); 
    }

    return 0;
}

