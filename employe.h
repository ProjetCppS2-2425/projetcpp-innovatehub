#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Employe {
private:
    int idEmploye;
    int idConseil;
    QString cin;
    QString nom;
    QString prenom;
    QString numeroTelephone;
    QString poste;
    double salaire;
    QString email;
    QString dateEmbauche;
    QString role;
    QString motDePasse;

public:
    // Constructeurs
    Employe() {};
    Employe(int, int, QString, QString, QString, QString, QString, double, QString, QString, QString, QString);

    // Méthodes CRUD
    bool ajouter();
    bool supprimer(int);
    bool modifier();
    QSqlQueryModel* afficher();
};

#endif // EMPLOYE_H


