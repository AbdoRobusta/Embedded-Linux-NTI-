#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include <vector>
#include <fstream>

// ---------- Utility ----------
bool write_file(const std::string &path, const std::string &value);
std::string read_file(const std::string &path);

// ---------- Base Class ----------
class GPIOPin {
protected:
    int gpio;
    std::string basePath;

public:
    GPIOPin(int num);
    bool setDirection(const std::string &dir);
    bool setValue(int val);
    int getValue() const;
    bool setPullUp(int enable);
    bool setPullDown(int enable);
    int getNumber() const;
};

// ---------- LED Class ----------
class LED : public GPIOPin {
    bool state;

public:
    LED(int num);
    void setup();
    void toggle();
    void setState(bool s);
    bool getState() const;
};

// ---------- Button Class ----------
class Button : public GPIOPin {
    int lastState;

public:
    Button(int num);
    void setup();
    bool isPressed();
    int getRawValue();
};

// ---------- CSV Logger ----------
class CSVLogger {
    std::string filename;

public:
    CSVLogger(std::string path);
    void log(const std::vector<int> &vals);
};

// ---------- Main App ----------
class GPIOModuleApp {
    std::vector<Button> buttons;
    std::vector<LED> leds;
    std::vector<int> buttonToLed;
    CSVLogger logger;
    const int pollMs = 200;

public:
    GPIOModuleApp(std::vector<int> btnPins, std::vector<int> ledPins, std::vector<int> mapping, std::string csvPath);
    void setup();
    void run();
};

#endif // BUTTON_HPP
