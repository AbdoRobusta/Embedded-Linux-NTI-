#include "button.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <unistd.h>
using namespace std;

volatile sig_atomic_t keep_running = 1;
void sigint_handler(int) { keep_running = 0; }

// ---------- Utility ----------
bool write_file(const string &path, const string &value) {
    ofstream ofs(path);
    if (!ofs.is_open()) return false;
    ofs << value;
    return true;
}

string read_file(const string &path) {
    ifstream ifs(path);
    if (!ifs.is_open()) return "";
    string s; getline(ifs, s);
    return s;
}

// ---------- GPIOPin ----------
GPIOPin::GPIOPin(int num) : gpio(num), basePath("/sys/class/gpiomod/gpio" + to_string(num)) {}
bool GPIOPin::setDirection(const string &dir) { return write_file(basePath + "/direction", dir); }
bool GPIOPin::setValue(int val) { return write_file(basePath + "/value", to_string(val)); }
int GPIOPin::getValue() const {
    string s = read_file(basePath + "/value");
    if (s.empty()) return -1;
    return (s[0] == '0') ? 0 : 1;
}
bool GPIOPin::setPullUp(int enable) { return write_file(basePath + "/pull_up", to_string(enable)); }
bool GPIOPin::setPullDown(int enable) { return write_file(basePath + "/pull_down", to_string(enable)); }
int GPIOPin::getNumber() const { return gpio; }

// ---------- LED ----------
LED::LED(int num) : GPIOPin(num), state(false) {}
void LED::setup() { setDirection("out"); setValue(0); }
void LED::toggle() { state = !state; setValue(state); }
void LED::setState(bool s) { state = s; setValue(s); }
bool LED::getState() const { return state; }

// ---------- Button ----------
Button::Button(int num) : GPIOPin(num), lastState(1) {}
void Button::setup() { setDirection("in"); setPullDown(0); setPullUp(1); }
bool Button::isPressed() {
    int current = getValue();
    bool pressed = (lastState == 1 && current == 0); // active low
    lastState = current;
    return pressed;
}
int Button::getRawValue() { return getValue(); }

// ---------- CSVLogger ----------
CSVLogger::CSVLogger(string path) : filename(path) {}
void CSVLogger::log(const vector<int> &vals) {
    ofstream csv(filename, ios::trunc);
    if (!csv.is_open()) return;

    // Header
    for (size_t i = 0; i < vals.size(); ++i) {
        csv << "button" << (i + 1);
        if (i + 1 < vals.size()) csv << ",";
    }
    csv << "\n";

    // Values
    for (size_t i = 0; i < vals.size(); ++i) {
        csv << vals[i];
        if (i + 1 < vals.size()) csv << ",";
    }
    csv << "\n";
    csv.flush();
}

// ---------- GPIOModuleApp ----------
GPIOModuleApp::GPIOModuleApp(vector<int> btnPins, vector<int> ledPins, vector<int> mapping, string csvPath)
    : logger(csvPath) {
    for (int b : btnPins) buttons.emplace_back(b);
    for (int l : ledPins) leds.emplace_back(l);
    buttonToLed = mapping;
}

void GPIOModuleApp::setup() {
    for (auto &b : buttons) b.setup();
    for (auto &l : leds) l.setup();
}

void GPIOModuleApp::run() {
    cout << "App running with custom gpiomod driver (/sys/class/gpiomod/...)\n";
    cout << "Press Ctrl+C to exit...\n";

    while (keep_running) {
        vector<int> values;

        for (size_t i = 0; i < buttons.size(); ++i) {
            int val = buttons[i].getRawValue();
            values.push_back(val);

            for (size_t m = 0; m < buttonToLed.size(); ++m) {
                if ((int)i == buttonToLed[m] && buttons[i].isPressed()) {
                    leds[m].toggle();
                }
            }
        }

        // Print to terminal
        cout << "\rButtons: ";
        for (int v : values) cout << v << " ";
        cout << flush;

        // Log to CSV
        logger.log(values);

        this_thread::sleep_for(chrono::milliseconds(pollMs));
    }
}
        