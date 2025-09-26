#ifndef GPIO_HPP
#define GPIO_HPP

#include <iostream>
#include <string>

#define GPIO_PATH "/sys/class/gpio/"

class GPIO {
protected:
    std::string pin;

    void writeFile(const std::string& path, const std::string& value);
    std::string readFile(const std::string& path);

public:
    GPIO(const std::string& gpio_pin);
    virtual ~GPIO();
};

class LED : public GPIO {
    bool state;
public:
    LED(const std::string& pin);
    void turnOn();
    void turnOff();
    void toggle();
};

class Button : public GPIO {
public:
    Button(const std::string& pin);
    bool isPressed();
};

#endif // GPIO_HPP
