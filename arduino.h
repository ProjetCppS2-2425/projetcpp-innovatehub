#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTextBrowser>
#include "connection.h"

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    bool connectToArduino(const QString &portName);
    void disconnectFromArduino();
    bool isConnected() const;
    void setOutputWidget(QTextBrowser *widget);

signals:
    void accessGranted(const QString &idEmploye, const QString &poste);
    void accessDenied(const QString &idEmploye);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *serialPort;
    QString buffer;
    QTextBrowser *outputWidget;
    void processMessage(const QString &message);
    void storeAccessLog(const QString &idEmploye, bool granted, const QString &poste = QString());
    void updateTransactionDisplay(const QString &idEmploye, bool granted, const QString &poste = QString());
    bool isEmployeeAuthorized(const QString &idEmploye, QString &poste);
};

#endif // ARDUINO_H 