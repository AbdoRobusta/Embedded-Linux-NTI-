// gpio_buttons_leds_oop.cpp
// Compile: g++ -std=c++17 gpio_buttons_leds_oop.cpp -o gpio_buttons_leds
// Run: sudo ./gpio_buttons_leds

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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

// ---------- Base Class ----------
class GPIOPin {
protected:
    int gpio;
    string basePath;

public:
    GPIOPin(int num) : gpio(num), basePath("/sys/class/gpiomod/gpio" + to_string(num)) {}

    bool setDirection(const string &dir) {
        return write_file(basePath + "/direction", dir);
    }

    bool setValue(int val) {
        return write_file(basePath + "/value", to_string(val));
    }

    int getValue() const {
        string s = read_file(basePath + "/value");
        if (s.empty()) return -1;
        return (s[0] == '0') ? 0 : 1;
    }

    bool setPullUp(int enable) {
        return write_file(basePath + "/pull_up", to_string(enable));
    }

    bool setPullDown(int enable) {
        return write_file(basePath + "/pull_down", to_string(enable));
    }

    int getNumber() const { return gpio; }
};

// ---------- LED Class ----------
class LED : public GPIOPin {
    bool state;

public:
    LED(int num) : GPIOPin(num), state(false) {}

    void setup() {
        setDirection("out");
        setValue(0);
    }

    void toggle() {
        state = !state;
        setValue(state);
    }

    void setState(bool s) {
        state = s;
        setValue(s);
    }

    bool getState() const { return state; }
};

// ---------- Button Class ----------
class Button : public GPIOPin {
    int lastState;

public:
    Button(int num) : GPIOPin(num), lastState(1) {}

    void setup() {
        setDirection("in");
        setPullDown(0);
        setPullUp(1);
    }

    bool isPressed() {
        int current = getValue();
        bool pressed = (lastState == 1 && current == 0); // active low
        lastState = current;
        return pressed;
    }

    int getRawValue() { return getValue(); }
};

// ---------- CSV Logger ----------
class CSVLogger {
    string filename;

public:
    CSVLogger(string path) : filename(path) {}

    void log(const vector<int> &vals) {
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
};

// ---------- Main App ----------
class GPIOModuleApp {
    vector<Button> buttons;
    vector<LED> leds;
    vector<int> buttonToLed;
    CSVLogger logger;
    const int pollMs = 200;

public:
    GPIOModuleApp(vector<int> btnPins, vector<int> ledPins, vector<int> mapping, string csvPath)
        : logger(csvPath) {
        for (int b : btnPins) buttons.emplace_back(b);
        for (int l : ledPins) leds.emplace_back(l);
        buttonToLed = mapping;
    }

    void setup() {
        for (auto &b : buttons) b.setup();
        for (auto &l : leds) l.setup();
    }

    void run() {
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
};

// ---------- Main ----------
int main() {
    signal(SIGINT, sigint_handler);

    vector<int> buttonPins = {5, 6, 13, 19, 26};
    vector<int> ledPins = {17, 27};
    vector<int> mapping = {0, 1};
    string csv = "/home/weston/buttons_log.csv";

    GPIOModuleApp app(buttonPins, ledPins, mapping, csv);
    app.setup();
    app.run();

    cout << "\nProgram terminated.\n";
    return 0;
}

