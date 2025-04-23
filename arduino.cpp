#include "arduino.h"
#include <QDebug>
#include <QDateTime>
#include <QSqlQuery>

Arduino::Arduino(QObject *parent) : QObject(parent), outputWidget(nullptr)
{
    serialPort = new QSerialPort(this);
    connect(serialPort, &QSerialPort::readyRead, this, &Arduino::handleReadyRead);
    connect(serialPort, &QSerialPort::errorOccurred, this, &Arduino::handleError);
}

Arduino::~Arduino()
{
    disconnectFromArduino();
}

bool Arduino::connectToArduino(const QString &portName)
{
    serialPort->setPortName(portName);
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port" << portName << "Error:" << serialPort->errorString();
        return false;
    }

    qDebug() << "Connected to Arduino on port" << portName;
    return true;
}

void Arduino::disconnectFromArduino()
{
    if (serialPort->isOpen()) {
        serialPort->close();
        qDebug() << "Disconnected from Arduino";
    }
}

bool Arduino::isConnected() const
{
    return serialPort->isOpen();
}

void Arduino::setOutputWidget(QTextBrowser *widget)
{
    outputWidget = widget;
}

void Arduino::handleReadyRead()
{
    buffer.append(serialPort->readAll());
    
    // Process complete messages (ending with newline)
    while (buffer.contains('\n')) {
        int newlineIndex = buffer.indexOf('\n');
        QString message = buffer.left(newlineIndex).trimmed();
        buffer.remove(0, newlineIndex + 1);
        
        processMessage(message);
    }
}

void Arduino::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qDebug() << "Critical error:" << serialPort->errorString();
        disconnectFromArduino();
    }
}

bool Arduino::isEmployeeAuthorized(const QString &idEmploye, QString &poste)
{
    QSqlDatabase db = Connection::getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT POSTE FROM SYRINE.EMPLOYE WHERE IDEMPLOYE = :idEmploye");
    query.bindValue(":idEmploye", idEmploye);

    if (query.exec() && query.next()) {
        poste = query.value(0).toString();
        return true;
    }

    return false;
}

void Arduino::processMessage(const QString &message)
{
    qDebug() << "Received message:" << message;
    
    // Expected format: ID:123
    if (message.startsWith("ID:")) {
        QString idEmploye = message.mid(3);
        QString poste;
        
        if (isEmployeeAuthorized(idEmploye, poste)) {
            storeAccessLog(idEmploye, true, poste);
            updateTransactionDisplay(idEmploye, true, poste);
            emit accessGranted(idEmploye, poste);
            
            // Send command to Arduino to open door
            serialPort->write("OPEN_DOOR\n");
        } else {
            storeAccessLog(idEmploye, false);
            updateTransactionDisplay(idEmploye, false);
            emit accessDenied(idEmploye);
        }
    }
}

void Arduino::storeAccessLog(const QString &idEmploye, bool granted, const QString &poste)
{
    QSqlDatabase db = Connection::getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database is not open";
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO SYRINE.TRANSACTION (IDTRANSACTION, REFERENCE, MONTANT, MODEPAIEMENT, "
                 "DATEEE, STATUTT, TYPEE, IDEMPLOYE) VALUES "
                 "(TRANSACTION_SEQ.NEXTVAL, :idEmploye, 0, 'RFID', SYSDATE, :statut, 'ACCESS', :idEmploye)");
    
    query.bindValue(":idEmploye", idEmploye);
    query.bindValue(":statut", granted ? "GRANTED" : "DENIED");

    if (!query.exec()) {
        qDebug() << "Failed to store access log:" << query.lastError().text();
    }
}

void Arduino::updateTransactionDisplay(const QString &idEmploye, bool granted, const QString &poste)
{
    if (!outputWidget) return;

    QString status = granted ? "GRANTED" : "DENIED";
    QString color = granted ? "green" : "red";
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    
    QString message;
    if (granted) {
        message = QString("<span style='color: %1'>[%2] ID: %3 (%4) - Access %5</span>")
                    .arg(color)
                    .arg(timestamp)
                    .arg(idEmploye)
                    .arg(poste)
                    .arg(status);
    } else {
        message = QString("<span style='color: %1'>[%2] ID: %3 - Access %4</span>")
                    .arg(color)
                    .arg(timestamp)
                    .arg(idEmploye)
                    .arg(status);
    }
    
    outputWidget->append(message);
} 