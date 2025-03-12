#include "employe.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QMessageBox>

// 🔹 Constructeur
// 🔹 Constructor definition in 'employe.cpp'
Employe::Employe(int idE, QString prenom, QString nom, double salaire,
                 QString poste, QString cin, QString email,
                 QString numeroDeTelephone, QString motDePasse, QString gender)
{
    this->idE = idE;
    this->prenom = prenom;
    this->nom = nom;

    this->salaire = salaire;
    this->poste = poste;
    this->cin = cin;
    this->email = email;

    this->numeroDeTelephone = numeroDeTelephone;
    this->motDePasse = motDePasse;
    this->gender = gender;  // Add this line to initialize gender
}



bool Employe::ajouter() {
    // Vérification des données de l'employé avant d'ajouter
    if (cin.length() != 8) {
        qDebug() << "Erreur: CIN doit avoir 8 chiffres.";
        return false;
    }

    if (!email.contains("@")) {
        qDebug() << "Erreur: Email invalide.";
        return false;
    }

    QSqlQuery query;

    // Vérifier si le CIN existe déjà dans la base de données
    query.prepare("SELECT COUNT(*) FROM EMPLOYES WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        // Passer nullptr comme parent ou utiliser un widget parent valide
        QMessageBox::warning(nullptr, "Erreur", "Erreur lors de la vérification du CIN dans la base de données !");
        return false;
    }

    query.next();
    int count = query.value(0).toInt();

    if (count > 0) {
        QMessageBox::warning(nullptr, "Erreur", "Ce CIN existe déjà dans la base de données !");
        return false;
    }

    // Vérification de la connexion à la base de données
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Erreur de connexion à la base de données!";
        return false;  // Retourner false si la connexion échoue
    }

    // Préparer la requête pour insérer l'employé dans la base de données
    query.prepare("INSERT INTO EMPLOYES (IDE, PRENOM, NOM, SALAIRE, POSTE, CIN, EMAIL, NUMERODETELEPHONE, MOTDEPASSE, GENDER) "
                  "VALUES (SYRINE.EMPLOYES_SEQ.NEXTVAL, :prenom, :nom, :salaire, :poste, :cin, :email, :numeroDeTelephone, :motDePasse, :gender)");

    // Lier les valeurs aux paramètres de la requête
    query.bindValue(":idE", this->idE);
    query.bindValue(":prenom", this->prenom);
    query.bindValue(":nom", this->nom);
    query.bindValue(":salaire", this->salaire);
    query.bindValue(":poste", this->poste);
    query.bindValue(":cin", this->cin);
    query.bindValue(":email", this->email);
    query.bindValue(":numeroDeTelephone", this->numeroDeTelephone); // Assurez-vous que c'est un nombre
    query.bindValue(":motDePasse", this->motDePasse);
    query.bindValue(":gender", this->gender);

    // Exécution de la requête
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de l'employé :" << query.lastError();
        return false;  // Retourner false si l'ajout échoue
    }

    return true;  // Retourner true si l'ajout réussit
}

QSqlQueryModel* Employe::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    // Création et préparation de la requête
    QSqlQuery query;
    query.prepare("SELECT IDE, PRENOM, NOM, SALAIRE, POSTE, CIN, EMAIL, NUMERODETELEPHONE, GENDER FROM EMPLOYES ORDER BY IDE ASC");

    // Exécution de la requête
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError();
        delete model;  // Assurez-vous de supprimer le modèle en cas d'erreur
        return nullptr;
    }

    // Passer la requête par mouvement (utiliser std::move)
    model->setQuery(std::move(query));  // Utilisation de std::move

    // Si tu as un endroit où le modèle est utilisé (comme une table ou une vue), c'est ici que tu peux l'afficher.
    return model;  // Retourne le modèle pour utilisation ailleurs
}


bool Employe::supprimer(const QString &ide)
{
    // Prepare the DELETE SQL query
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYES WHERE IDE = :ide");
    query.bindValue(":ide", ide);  // Bind the IDE parameter

    // Execute the query
    if (query.exec()) {
        qDebug() << "L'employé avec l'IDE" << ide << "a été supprimé.";
        return true;  // Return true if the deletion was successful
    } else {
        qDebug() << "Erreur lors de la suppression de l'employé :" << query.lastError().text();
        return false;  // Return false if there was an error
    }
}

bool Employe::modifier(const QString &ide, const QString &prenom, const QString &nom, const QString &salaire,
                       const QString &poste, const QString &cin, const QString &email, const QString &telephone, const QString &gender) {
    // Création de la requête SQL pour mettre à jour l'employé
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYES SET PRENOM = :prenom, NOM = :nom, SALAIRE = :salaire, POSTE = :poste, "
                  "CIN = :cin, EMAIL = :email, NUMERODETELEPHONE = :telephone, GENDER = :gender WHERE IDE = :ide");

    // Lier les paramètres à la requête
    query.bindValue(":prenom", prenom);
    query.bindValue(":nom", nom);
    query.bindValue(":salaire", salaire);
    query.bindValue(":poste", poste);
    query.bindValue(":cin", cin);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":gender", gender);
    query.bindValue(":ide", ide);  // On lie l'IDE de l'employé que l'on souhaite modifier

    // Exécuter la requête
    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification de l'employé : " << query.lastError();
        return false;  // Retourner false en cas d'erreur
    }

    return true;  // Retourner true si la modification s'est bien passée
}
