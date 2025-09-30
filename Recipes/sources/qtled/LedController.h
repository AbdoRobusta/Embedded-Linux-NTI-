#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <QObject>
#include <memory>

// GPIO LED class
class GPIO_LED {
public:
    GPIO_LED(const std::string& pin);
    ~GPIO_LED();
    void turnOn();
    void turnOff();
    void toggle();
    bool state() const { return m_state; }

private:
    std::string m_pin;
    bool m_state;
    void writeFile(const std::string& path, const std::string& value);
};

class LedController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool ledOn READ ledOn NOTIFY ledOnChanged)

public:
    explicit LedController(QObject *parent = nullptr);

    bool ledOn() const { return m_ledOn; }

public slots:
    void toggleLed();

signals:
    void ledOnChanged(bool);

private:
    bool m_ledOn = false;
    std::unique_ptr<GPIO_LED> m_led;
};

#endif // LEDCONTROLLER_H

