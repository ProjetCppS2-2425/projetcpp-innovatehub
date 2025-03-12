#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize database connection
    Connection* c = Connection::getInstance();
    bool test = c->createconnect();



    if(test) {
        MainWindow w;
        w.show();
     //   QMessageBox::information(nullptr, QObject::tr("Database Connection"),
     //                            QObject::tr("Connection successful."), QMessageBox::Ok);
        return a.exec();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Connection failed."), QMessageBox::Cancel);
        return 1;
    }

}
