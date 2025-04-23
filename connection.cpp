#include "connection.h"

QSqlDatabase Connection::db = QSqlDatabase();

bool Connection::createConnection()
{
    qDebug() << "Tentative de connexion à la base de données...";
    
    // Vérifier si une connexion existe déjà
    if(db.isValid() && db.isOpen()) {
        qDebug() << "Une connexion existe déjà";
        return true;
    }

    // Liste des pilotes disponibles
    qDebug() << "Pilotes disponibles:" << QSqlDatabase::drivers();

    db = QSqlDatabase::addDatabase("QODBC");
    if (!db.isValid()) {
        qDebug() << "Erreur: Le pilote QODBC n'est pas disponible";
        return false;
    }

    // Configuration de la connexion
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("syrine");
    db.setPassword("esprit18");

    // Tentative de connexion
    if (!db.open()) {
        qDebug() << "Erreur de connexion:";
        qDebug() << "Type:" << db.lastError().type();
        qDebug() << "Message:" << db.lastError().text();
        qDebug() << "Driver Text:" << db.lastError().driverText();
        qDebug() << "Database Text:" << db.lastError().databaseText();
        return false;
    }

    qDebug() << "Connexion réussie à la base de données";
    qDebug() << "Nom de la base:" << db.databaseName();
    qDebug() << "Utilisateur:" << db.userName();
    qDebug() << "Pilote:" << db.driverName();

    return true;
}

void Connection::closeConnection()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "Connexion à la base de données fermée";
    }
}
