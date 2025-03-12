#include "connection.h"
#include <QDebug>  // Pour afficher des messages d'erreur

Connection::Connection()
{
    // Constructeur vide, tout se passe dans la méthode createconnect
}

bool Connection::createconnect()
{
    bool test = false;

    // Utilisation de l'objet db comme membre de la classe
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");  // Nom de la source de données ODBC
    db.setUserName("syrine");               // Nom de l'utilisateur
    db.setPassword("esprit18");             // Mot de passe de l'utilisateur

    // Vérification de la connexion
    if (db.open()) {
        test = true;
        qDebug() << "Connexion réussie à la base de données.";
    } else {
        qDebug() << "Erreur de connexion : " << db.lastError().text();
    }

    return test;  // Retourne true si la connexion est réussie, sinon false
}
