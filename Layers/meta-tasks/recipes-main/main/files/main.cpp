#include "gpio.hpp"
#include <unistd.h>

#define LED_PIN "17"
#define BTN_PIN "27"

int main() {
    LED led(LED_PIN);
    Button button(BTN_PIN);

    std::cout << "Press button on GPIO" << BTN_PIN 
              << " to toggle LED on GPIO" << LED_PIN << "...\n";

    bool lastState = false;

    while (true) {
        bool current = button.isPressed();
        if (current && !lastState) { // detect falling edge
            led.toggle();
        }
        lastState = current;
        usleep(100000); // 100ms debounce
    }

    return 0;
}
