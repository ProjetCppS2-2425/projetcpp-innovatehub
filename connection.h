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
