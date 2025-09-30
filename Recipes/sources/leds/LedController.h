#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <QObject>

class LedController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool ledOn READ ledOn WRITE setLedOn NOTIFY ledOnChanged)

public:
    explicit LedController(QObject *parent = nullptr);

    bool ledOn() const { return m_ledOn; }
    void setLedOn(bool on);

public slots:
    void toggleLed();

signals:
    void ledOnChanged(bool);

private:
    bool m_ledOn = false;
};

#endif // LEDCONTROLLER_H
