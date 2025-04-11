#include "conseil.h"
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>

Conseil::Conseil() : tarif(0.0) {
    qDebug() << "Constructeur de Conseil appelé.";
}

Conseil::Conseil(QString matricule, QString etat, QString type, double tarif, QString description, QString datee)
    : matricule(matricule), etat(etat), type(type), tarif(tarif), description(description), datee(datee) {
    qDebug() << "Constructeur de Conseil avec paramètres appelé :"
             << "Matricule:" << matricule
             << "État:" << etat
             << "Type:" << type
             << "Tarif:" << tarif
             << "Description:" << description
             << "Date:" << datee;
}



bool Conseil::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO CONSEIL (MATRICULE, ETAT, TYPE, TARIF, DESCRIPTION, DATEE) "
                  "VALUES (:matricule, :etat, :type, :tarif, :description, :datee)");
    query.bindValue(":matricule", matricule);
    query.bindValue(":etat", etat);
    query.bindValue(":type", type);
    query.bindValue(":tarif", tarif);
    query.bindValue(":description", description);
    query.bindValue(":datee", datee);
    return query.exec();
}



QSqlQueryModel* Conseil::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;

    // Préparer la requête
    query.prepare("SELECT IDCONSEIL, MATRICULE, ETAT, TYPE, TARIF, DESCRIPTION, DATEE FROM CONSEIL");

    if (!query.exec()) {
        qDebug() << "Erreur dans l'exécution de la requête : " << query.lastError();
        return nullptr; // Retourner null en cas d'erreur
    }

    model->setQuery(query);

    // Déboguer le nom des colonnes
    for (int i = 0; i < model->columnCount(); ++i) {
        qDebug() << "Colonne " << i << ": " << model->headerData(i, Qt::Horizontal).toString();
    }

    return model;
}




bool Conseil::supprimer(int idConseil) {
    QSqlQuery query;
    query.prepare("DELETE FROM conseil WHERE IDCONSEIL = :idconseil");
    query.bindValue(":idconseil", idConseil);

    qDebug() << "Tentative de suppression du conseil avec ID:" << idConseil;

    // EXÉCUTION de la requête
    if (!query.exec()) {
        qDebug() << "Erreur SQL (supprimer conseil) : " << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression du conseil : " + query.lastError().text());
        return false;
    }

    qDebug() << "Conseil supprimé avec succès.";
    return true;
}

bool Conseil::update() {
    QSqlQuery query;
    query.prepare("UPDATE CONSEIL SET MATRICULE = :matricule, ETAT = :etat, TYPE = :type, "
                  "TARIF = :tarif, DESCRIPTION = :description, DATEE = :datee "
                  "WHERE IDCONSEIL = :idconseil");

    query.bindValue(":matricule", matricule);
    query.bindValue(":etat", etat);
    query.bindValue(":type", type);
    query.bindValue(":tarif", tarif);
    query.bindValue(":description", description);
    query.bindValue(":datee", datee);
    query.bindValue(":idconseil", idConseil); // Assurez-vous que idConseil est correctement assigné

    if (!query.exec()) {
        qDebug() << "Erreur lors de la mise à jour :" << query.lastError().text();
        return false;
    }
    return true;
}
