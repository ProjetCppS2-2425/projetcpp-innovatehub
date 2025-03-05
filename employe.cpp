#include "employe.h"

// Constructeur
Employe::Employe(int idEmploye, int idConseil, QString cin, QString nom, QString prenom, QString numeroTelephone,
                 QString poste, double salaire, QString email, QString dateEmbauche, QString role, QString motDePasse) {
    this->idEmploye = idEmploye;
    this->idConseil = idConseil;
    this->cin = cin;
    this->nom = nom;
    this->prenom = prenom;
    this->numeroTelephone = numeroTelephone;
    this->poste = poste;
    this->salaire = salaire;
    this->email = email;
    this->dateEmbauche = dateEmbauche;
    this->role = role;
    this->motDePasse = motDePasse;
}

// Ajouter un employé
bool Employe::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO employes (idEmploye, idConseil, cin, nom, prenom, numeroTelephone, poste, salaire, email, dateEmbauche, role, motDePasse) "
                  "VALUES (:idEmploye, :idConseil, :cin, :nom, :prenom, :numeroTelephone, :poste, :salaire, :email, :dateEmbauche, :role, :motDePasse)");
    query.bindValue(":idEmploye", idEmploye);
    query.bindValue(":idConseil", idConseil);
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":numeroTelephone", numeroTelephone);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":email", email);
    query.bindValue(":dateEmbauche", dateEmbauche);
    query.bindValue(":role", role);
    query.bindValue(":motDePasse", motDePasse);
    return query.exec();
}

// Afficher la liste des employés
QSqlQueryModel* Employe::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employes");
    return model;
}

// Supprimer un employé
bool Employe::supprimer(int idEmploye) {
    QSqlQuery query;
    query.prepare("DELETE FROM employes WHERE idEmploye = :idEmploye");
    query.bindValue(":idEmploye", idEmploye);
    return query.exec();
}

// Modifier un employé
bool Employe::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE employes SET idConseil=:idConseil, cin=:cin, nom=:nom, prenom=:prenom, numeroTelephone=:numeroTelephone, "
                  "poste=:poste, salaire=:salaire, email=:email, dateEmbauche=:dateEmbauche, role=:role, motDePasse=:motDePasse "
                  "WHERE idEmploye=:idEmploye");
    query.bindValue(":idEmploye", idEmploye);
    query.bindValue(":idConseil", idConseil);
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":numeroTelephone", numeroTelephone);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":email", email);
    query.bindValue(":dateEmbauche", dateEmbauche);
    query.bindValue(":role", role);
    query.bindValue(":motDePasse", motDePasse);
    return query.exec();
}

