#include "LedController.h"
#include <fstream>
#include <unistd.h>
#include <iostream>

#define GPIO_PATH "/sys/class/gpio/"

//////////////////////
// GPIO_LED Methods //
//////////////////////

GPIO_LED::GPIO_LED(const std::string& pin) : m_pin(pin), m_state(false) {
    // Export pin
    writeFile(GPIO_PATH "export", m_pin);
    usleep(100000);

    // Set direction to out
    writeFile(GPIO_PATH "gpio" + m_pin + "/direction", "out");
}

GPIO_LED::~GPIO_LED() {
    writeFile(GPIO_PATH "unexport", m_pin);
}

void GPIO_LED::writeFile(const std::string& path, const std::string& value) {
    std::ofstream file(path);
    if (file.is_open()) {
        file << value;
        file.close();
    } else {
        std::cerr << "Failed to open " << path << "\n";
    }
}

void GPIO_LED::turnOn() {
    writeFile(GPIO_PATH "gpio" + m_pin + "/value", "1");
    m_state = true;
    std::cout << "LED ON\n";
}

void GPIO_LED::turnOff() {
    writeFile(GPIO_PATH "gpio" + m_pin + "/value", "0");
    m_state = false;
    std::cout << "LED OFF\n";
}

void GPIO_LED::toggle() {
    m_state ? turnOff() : turnOn();
}

//////////////////////////
// LedController Bridge //
//////////////////////////

LedController::LedController(QObject *parent) : QObject(parent) {
    m_led = std::make_unique<GPIO_LED>("17");  // GPIO 17 للـ LED
    m_ledOn = false;
}

void LedController::toggleLed() {
    if (m_led) {
        m_led->toggle();
        m_ledOn = m_led->state();
        emit ledOnChanged(m_ledOn);
    }
}

