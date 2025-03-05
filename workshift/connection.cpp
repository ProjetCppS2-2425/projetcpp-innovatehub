#include "connection.h"

connection::connection() {db = QSqlDatabase::addDatabase("QODBC");}

bool connection::createconnection()

{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");//inserer le nom de la source de données
    db.setUserName("transaction_admin");//inserer nom de l'utilisateur
    db.setPassword("123456");//inserer mot de passe de cet utilisateur

    if (db.open()) test=true;
     return  test;
}
void connection::closeconection() { db.close() ;}
