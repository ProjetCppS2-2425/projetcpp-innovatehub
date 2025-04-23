#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    bool envoyerTexte(const QString &texte);
    QString getLastError() const;

private:
    QSerialPort *arduino;
    QString portName = "COM3"; // Mets ici le bon port de ton Arduino
    QString lastError;
};

#endif // ARDUINO_H
