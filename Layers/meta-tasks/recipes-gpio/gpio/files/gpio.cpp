#include "gpio.hpp"
#include <fstream>
#include <unistd.h>

void GPIO::writeFile(const std::string& path, const std::string& value) {
    std::ofstream file(path);
    if (file.is_open()) {
        file << value;
        file.close();
    } else {
        std::cerr << "Failed to open " << path << "\n";
    }
}

std::string GPIO::readFile(const std::string& path) {
    std::ifstream file(path);
    std::string value;
    if (file.is_open()) {
        file >> value;
        file.close();
    }
    return value;
}

GPIO::GPIO(const std::string& gpio_pin) : pin(gpio_pin) {
    writeFile(GPIO_PATH "export", pin);
    usleep(100000); // wait a bit after export
}

GPIO::~GPIO() {
    writeFile(GPIO_PATH "unexport", pin);
}

LED::LED(const std::string& pin) : GPIO(pin), state(false) {
    writeFile(GPIO_PATH "gpio" + pin + "/direction", "out");
}

void LED::turnOn() {
    writeFile(GPIO_PATH "gpio" + pin + "/value", "1");
    state = true;
    std::cout << "LED ON\n";
}

void LED::turnOff() {
    writeFile(GPIO_PATH "gpio" + pin + "/value", "0");
    state = false;
    std::cout << "LED OFF\n";
}

void LED::toggle() {
    state ? turnOff() : turnOn();
}

Button::Button(const std::string& pin) : GPIO(pin) {
    writeFile(GPIO_PATH "gpio" + pin + "/direction", "in");
}

bool Button::isPressed() {
    std::string value = readFile(GPIO_PATH "gpio" + pin + "/value");
    return (value == "0"); // active low
}
