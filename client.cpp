#include "client.h"

// Constructeur
Client::Client(int id, QString nom, QString type, QString secteur, QString tel, QString email, QString adresse)
{
    this->idClient = id;
    this->nomClient = nom;
    this->typeClient = type;
    this->secteurClient = secteur;
    this->telephoneClient = tel;
    this->emailClient = email;
    this->adresseClient = adresse;
}

// Ajouter un client à la base de données
bool Client::ajouter()
{
    QSqlQuery query;
    QString res = QString::number(idClient);

    // Préparation de la requête SQL pour insérer un client
    query.prepare("INSERT INTO CLIENT (IDCLIENT, NOMCLIENT, TYPECLIENT, SECTEURCLIENT, TELEPHONECLIENT, EMAILCLIENT, ADRESSECLIENT, DATECRÉATION) "
                  "VALUES (:id, :nom, :type, :secteur, :telephone, :email, :adresse, :dateCreation)");

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


bool Client::supprimer(int id)
{
    QSqlQuery query;
    QString res = QString::number(id);

    query.prepare("DELETE FROM CLIENT WHERE IDCLIENT = :id");
    query.bindValue(":id", res);

    return query.exec();
}
