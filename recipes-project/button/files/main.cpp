#include "button.hpp"
#include <csignal>
#include <iostream>
using namespace std;

extern volatile sig_atomic_t keep_running;
extern void sigint_handler(int);

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
