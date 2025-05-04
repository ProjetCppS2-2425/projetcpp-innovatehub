#include "arduino.h"
#include <QDebug>
#include <QDateTime>
#include <QSqlQuery>
#include <QTimer>
#include <QSqlError>

Arduino::Arduino(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);

    // Connect signals to slots
    connect(serial, &QSerialPort::readyRead, this, &Arduino::readSerial);
    connect(serial, &QSerialPort::errorOccurred, this, &Arduino::handleError);
}

Arduino::~Arduino()
{
    disconnectFromArduino();
}

bool Arduino::connectToArduino(const QString &portName)
{
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port" << portName << "Error:" << serial->errorString();
        emit connectionStatusChanged(false);
        return false;
    }

    qDebug() << "Connected to Arduino on port" << portName;
    emit connectionStatusChanged(true);
    return true;
}

void Arduino::disconnectFromArduino()
{
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "Disconnected from Arduino";
        emit connectionStatusChanged(false);
    }
}

bool Arduino::isConnected() const
{
    return serial->isOpen();
}

QStringList Arduino::getAvailablePorts() const
{
    QStringList ports;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ports << info.portName();
    }
    return ports;
}

QSerialPort* Arduino::getSerialPort() const
{
    return serial;
}

void Arduino::readSerial()
{
    buffer.append(QString::fromUtf8(serial->readAll()));

    // Process complete messages (ending with newline)
    while (buffer.contains('\n')) {
        int newlineIndex = buffer.indexOf('\n');
        QString message = buffer.left(newlineIndex).trimmed();
        buffer.remove(0, newlineIndex + 1);

        qDebug() << "Message received from Arduino:" << message;

        processMessage(message);
    }
}

void Arduino::handleReadyRead()
{
    readSerial();
}

void Arduino::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qDebug() << "Critical error:" << serial->errorString();
        disconnectFromArduino();
    }
}

void Arduino::processMessage(const QString &message)
{
    qDebug() << "Processing message from Arduino:" << message;

    // Expected format: ID: 123 (with space after colon)
    if (message.startsWith("ID: ")) {
        QString idEmploye = message.mid(4); // Skip "ID: " (4 characters)
        qDebug() << "Extracted employee ID:" << idEmploye;

        emit cardDetected(idEmploye);

        if (employeeExists(idEmploye)) {
            // Employee exists, grant access
            qDebug() << "ACCESS GRANTED: Employee ID" << idEmploye << "exists in database";
            storeAccessLog(idEmploye, true);
            emit accessGranted(idEmploye);

            // Send command to Arduino to activate motor
            qDebug() << "Sending MOTOR_ON command to Arduino";
            sendCommand("MOTOR_ON");

            // Turn off motor after 5 seconds
            QTimer::singleShot(5000, this, [this]() {
                if (isConnected()) {
                    qDebug() << "Sending MOTOR_OFF command to Arduino after timeout";
                    sendCommand("MOTOR_OFF");
                }
            });
        } else {
            // Employee doesn't exist, deny access
            qDebug() << "ACCESS DENIED: Employee ID" << idEmploye << "not found in database";
            storeAccessLog(idEmploye, false);
            emit accessDenied(idEmploye);
            sendCommand("MOTOR_OFF");
        }
    } else if (message == "ouverte") {
        emit doorStatusChanged("Ouverte");
    } else if (message == "Fermée") {
        emit doorStatusChanged("Fermée");
    } else {
        qDebug() << "Unrecognized message format, expected 'ID: [employeeID]'";
    }
}

void Arduino::manualCardInput(const QString &idEmploye)
{
    qDebug() << "Manual card input received:" << idEmploye;

    // Process the ID as if it came from the Arduino
    processMessage("ID: " + idEmploye);
}

bool Arduino::employeeExists(const QString &idEmploye)
{
    QSqlDatabase db = Connection::getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database connection error: Database not open";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT 1 FROM SYRINE.EMPLOYE WHERE IDEMPLOYE = :idEmploye");
    query.bindValue(":idEmploye", idEmploye);

    if (query.exec()) {
        if (query.next()) {
            qDebug() << "Database query successful: Employee" << idEmploye << "found";
            return true;
        } else {
            qDebug() << "Database query successful: Employee" << idEmploye << "NOT found";
            return false;
        }
    } else {
        qDebug() << "Database query error:" << query.lastError().text();
        return false;
    }
}

void Arduino::storeAccessLog(const QString &idEmploye, bool granted)
{
    QSqlDatabase db = Connection::getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database connection error: Cannot log access attempt";
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO SYRINE.ACCESS_LOG (ID_LOG, ID_EMPLOYE, ACCESS_TIME, STATUS) "
                  "VALUES (ACCESS_LOG_SEQ.NEXTVAL, :idEmploye, SYSTIMESTAMP, :status)");

    query.bindValue(":idEmploye", idEmploye);
    query.bindValue(":status", granted ? "GRANTED" : "DENIED");

    if (query.exec()) {
        qDebug() << "Access log saved successfully:"
                 << "ID:" << idEmploye
                 << "Status:" << (granted ? "GRANTED" : "DENIED")
                 << "Time:" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    } else {
        qDebug() << "Failed to store access log:" << query.lastError().text();
    }
}

void Arduino::sendCommand(const QString &command)
{
    if (!isConnected()) {
        qDebug() << "Cannot send command - not connected to Arduino";
        return;
    }

    QString fullCommand = command + "\n";
    qint64 bytesWritten = serial->write(fullCommand.toUtf8());

    if (bytesWritten == -1) {
        qDebug() << "Failed to write command:" << serial->errorString();
    } else if (bytesWritten != fullCommand.length()) {
        qDebug() << "Failed to write complete command";
    } else {
        qDebug() << "Command sent to Arduino:" << command;
    }
}
