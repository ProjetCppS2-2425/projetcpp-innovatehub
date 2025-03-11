#include "client.h"
#include <QSqlError>  // ✅ Ajout de l'en-tête

Client::Client() {
    // Initialisation des attributs si nécessaire
}
// Constructeur
Client::Client(int id, QString nom, QString type, QString secteur, QString tel, QString email, QString adresse, QString date_creation) {
    this->idClient = id;
    this->nomClient = nom;
    this->typeClient = type;
    this->secteurClient = secteur;
    this->telephoneClient = tel;
    this->emailClient = email;
    this->adresseClient = adresse;
    this->date_creation = date_creation; // Ajoute cette ligne
}



// Ajouter un client à la base de données
bool Client::ajouter()
{
    QSqlQuery query;
    QString res = QString::number(idClient);

    // Préparation de la requête SQL pour insérer un client
    //
    query.prepare("INSERT INTO CLIENT (IDCLIENT, NOM, EMAIL, TELEPHONE, TYPE, ADRESSE, SECTEUR, DATECREATION) "
                  "VALUES (:id, :nom, :email, :telephone, :type, :adresse, :secteur, :date_creation)");

    query.bindValue(":date_creation", date_creation);

    // Création des variables liées
    query.bindValue(":id", res);
    query.bindValue(":nom", nomClient);
    query.bindValue(":type", typeClient);
    query.bindValue(":secteur", secteurClient);
    query.bindValue(":telephone", telephoneClient);
    query.bindValue(":email", emailClient);
    query.bindValue(":adresse", adresseClient);

    // Exécuter la requête
    return query.exec();
}
QSqlQueryModel* Client::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT IDCLIENT, NOMCLIENT, TYPECLIENT, SECTEURCLIENT, TELEPHONECLIENT, EMAILCLIENT, ADRESSECLIENT FROM CLIENT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Secteur"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Adresse"));

    return model;
}


bool Client::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM CLIENT WHERE IDCLIENT = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        return true;  // Suppression réussie
    } else {
        return false; // Échec de suppression
    }
}

bool Client::modifier(int id, QString nom, QString type, QString secteur, QString tel, QString email, QString adresse) {
    QSqlQuery query;
    query.prepare("UPDATE CLIENT SET NOM=:nom, TYPE=:type, SECTEUR=:secteur, TELEPHONE=:telephone, EMAIL=:email, ADRESSE=:adresse WHERE IDCLIENT=:id");

    // Lier les valeurs aux variables de la requête
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":secteur", secteur);
    query.bindValue(":telephone", tel);
    query.bindValue(":email", email);
    query.bindValue(":adresse", adresse);

    return query.exec();  // Retourne `true` si la requête a réussi, sinon `false`
}
