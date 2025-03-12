#include "mainwindow.h"
#include <QApplication>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Vérifier la connexion à la base de données
    Connection conn;
    if (!conn.createconnect()) {
        qDebug() << "Erreur de connexion à la base de données !";
        return -1;  // Quitter si la connexion échoue
    }

    MainWindow w;
    w.show();
    return a.exec();
}
