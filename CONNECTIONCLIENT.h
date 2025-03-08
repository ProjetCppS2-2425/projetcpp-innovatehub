#ifndef CONNECTIONCLIENT_H
#define CONNECTIONCLIENT_H


#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Connection
{
    QSqlDatabase db;
public:
    Connection();
    bool createconnect();
    void closeConnection();
};



#endif // CONNECTIONCLIENT_H
