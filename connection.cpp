#include "connection.h"
#include <QDebug>  // N'oubliez pas d'inclure cette ligne

Connection::Connection()
{
    qDebug() << "Constructeur de Connection appelé.";
}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A"); // Insérer le nom de la source de données
    db.setUserName("syrine");                // Insérer le nom de l'utilisateur
    db.setPassword("esprit18");               // Insérer le mot de passe de cet utilisateur

    qDebug() << "Tentative de connexion à la base de données avec les informations suivantes :";
    qDebug() << "Nom de la base de données :" << db.databaseName();
    qDebug() << "Nom d'utilisateur :" << db.userName();

    if (db.open()) {
        test = true;
        qDebug() << "Connexion à la base de données réussie.";
    } else {
        qDebug() << "Échec de la connexion à la base de données :" << db.lastError().text();
    }

    return test;
}
