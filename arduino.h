#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "connection.h"

class Arduino : public QObject
{
    Q_OBJECT
signals:
    void doorStatusChanged(const QString &status);
    void cardDetected(const QString &cardID);
    void accessGranted(const QString &cardID);
    void accessDenied(const QString &cardID);
    void connectionStatusChanged(bool connected);

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    // Serial port methods
    bool connectToArduino(const QString &portName);
    void disconnectFromArduino();
    bool isConnected() const;
    QStringList getAvailablePorts() const;
    
    // Getter pour accéder au port série
    QSerialPort* getSerialPort() const;
    
    // Méthode pour la lecture série
    void readSerial();
    
    // Méthode pour tester avec une carte manuelle
    void manualCardInput(const QString &cardID);
    
    // Method to send commands to Arduino
    void sendCommand(const QString &command);
    
    // Method to check if employee ID exists in database
    bool employeeExists(const QString &cardID);

private slots:
    void handleError(QSerialPort::SerialPortError error);
    void handleReadyRead();

private:
    QSerialPort *serial;
    QString buffer; // Pour stocker temporairement les données
    
    // Méthodes pour validation et traitement
    void processMessage(const QString &message);
    void storeAccessLog(const QString &cardID, bool granted);
};

#endif // ARDUINO_H
