#include "LedController.h"

LedController::LedController(QObject *parent) : QObject(parent) {}

void LedController::setLedOn(bool on) {
    if (m_ledOn != on) {
        m_ledOn = on;
        emit ledOnChanged(m_ledOn);
    }
}

void LedController::toggleLed() {
    setLedOn(!m_ledOn);
}
