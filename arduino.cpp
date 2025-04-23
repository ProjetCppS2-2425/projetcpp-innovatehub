#include "arduino.h"
#include <QSerialPortInfo>
#include <QDebug>

Arduino::Arduino(QObject *parent) : QObject(parent)
{
    arduino = new QSerialPort(this);
}

Arduino::~Arduino()
{
    if (arduino->isOpen())
        arduino->close();
}

bool Arduino::envoyerTexte(const QString &texte)
{
    arduino->setPortName(portName);
    arduino->setBaudRate(QSerialPort::Baud9600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);

    if (!arduino->open(QIODevice::WriteOnly)) {
        lastError = "Impossible d’ouvrir le port série : " + arduino->errorString();
        return false;
    }

    QByteArray data = texte.toUtf8();
    arduino->write(data);
    arduino->flush();
    arduino->close();

    return true;
}

QString Arduino::getLastError() const
{
    return lastError;
}
