#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>  // Ajouté pour une meilleure gestion des chaînes de caractères

class Connection
{
public:
    Connection();         // Constructeur
    bool createconnect(); // Méthode pour établir la connexion à la base de données

private:
    QSqlDatabase db; // Objet de la base de données pour gérer la connexion
};

#endif // CONNECTION_H

