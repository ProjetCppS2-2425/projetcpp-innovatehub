// connection.h
#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Connection
{
private:
    QSqlDatabase db;
    static Connection* instance;

public:
    Connection();
    ~Connection();
    bool createconnect();
    void closeConnection();
    static Connection* getInstance();
};
#endif // CONNECTION_H

// connection.cpp
#include "connection.h"

Connection* Connection::instance = nullptr;

Connection::Connection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("syrine");
    db.setPassword("esprit18");
}

Connection::~Connection()
{
    closeConnection();
}

bool Connection::createconnect()
{
    return db.open();
}

void Connection::closeConnection()
{
    if(db.isOpen())
        db.close();
}

Connection* Connection::getInstance()
{
    if(instance == nullptr)
        instance = new Connection();
    return instance;
}
