#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Client {

    private:
        int idClient;
        QString nomClient, typeClient, secteurClient, telephoneClient, emailClient, adresseClient;
        QString date_creation; // Ajoute cette ligne


    public:
        Client();  // Déclaration du constructeur par défaut
        Client(int, QString, QString, QString, QString, QString, QString, QString);  // Autre constructeur

        bool modifier(int id, QString nom, QString type, QString secteur, QString tel, QString email, QString adresse);






    // Getters
    int getIdClient() { return idClient; }
    QString getNomClient() { return nomClient; }
    QString getTypeClient() { return typeClient; }
    QString getSecteurClient() { return secteurClient; }
    QString getTelephoneClient() { return telephoneClient; }
    QString getEmailClient() { return emailClient; }
    QString getAdresseClient() { return adresseClient; }

    // Setters
    void setIdClient(int id) { idClient = id; }
    void setNomClient(QString nom) { nomClient = nom; }
    void setTypeClient(QString type) { typeClient = type; }
    void setSecteurClient(QString secteur) { secteurClient = secteur; }
    void setTelephoneClient(QString tel) { telephoneClient = tel; }
    void setEmailClient(QString email) { emailClient = email; }
    void setAdresseClient(QString adresse) { adresseClient = adresse; }

    // Fonctionnalités de Base
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
};

#endif // CLIENT_H
