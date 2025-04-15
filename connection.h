#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class Connection
{
public:
    static bool createConnection();
    static void closeConnection();
    static QSqlDatabase getDatabase() { return db; }

private:
    static QSqlDatabase db;
};

#endif // CONNECTION_H
