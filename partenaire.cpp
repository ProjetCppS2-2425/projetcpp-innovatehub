// partenaire.cpp
#include "partenaire.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QVariant>
#include <QDateTime>
#include <QSqlError>  // Include the QSqlError header

// Constructeurs
Partenaire::Partenaire() : idPartenaire(0), montantTotal(0) {
    qDebug() << "[LOG][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "][Partenaire] Default constructor called";
}

Partenaire::Partenaire(int id, const QString &nom, const QString &secteur,
                       const QDate &dateDebut, const QDate &dateFin,
                       double montant, const QString &statut)
    : idPartenaire(id), nomEntreprise(nom), secteurActivite(secteur),
    dateDebutContrat(dateDebut), dateFinContrat(dateFin),
    montantTotal(montant), statut(statut)
{
    qDebug() << "[LOG][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
    << "][Partenaire] Parameterized constructor called with data:"
    << "ID:" << id
    << "Nom:" << nom
    << "Secteur:" << secteur
    << "DateDebut:" << dateDebut.toString("yyyy-MM-dd")
    << "DateFin:" << dateFin.toString("yyyy-MM-dd")
    << "Montant:" << montant
    << "Statut:" << statut;
}

// Ajouter un partenaire
bool Partenaire::ajouter()
{
    qDebug() << "[LOG][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
    << "][Partenaire::ajouter] Adding partenaire with ID:" << idPartenaire;

    QSqlQuery query;
    query.prepare("INSERT INTO PARTENAIRE (IDPARTENAIRE, NOMENTREPRISE, SECTEURACTIVITE, "
                  "DATEDEBUTCONTRAT, DATEFINCONTRAT, MONTANTTOTAL, STATUT) "
                  "VALUES (:id, :nom, :secteur, :dateDebut, :dateFin, :montant, :statut)");

    query.bindValue(":id", idPartenaire);
    query.bindValue(":nom", nomEntreprise);
    query.bindValue(":secteur", secteurActivite);
    query.bindValue(":dateDebut", dateDebutContrat);
    query.bindValue(":dateFin", dateFinContrat);
    query.bindValue(":montant", montantTotal);
    query.bindValue(":statut", statut);

    bool result = query.exec();

    if (result) {
        qDebug() << "[LOG][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        << "][Partenaire::ajouter] Successfully added partenaire with ID:" << idPartenaire;
    } else {
        qDebug() << "[ERROR][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        << "][Partenaire::ajouter] Failed to add partenaire with ID:" << idPartenaire
        << "Error:" << query.lastError().text();
    }

    return result;
}

// Modifier un partenaire
bool Partenaire::modifier(int id)
{
    qDebug() << "[LOG][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
    << "][Partenaire::modifier] Modifying partenaire with ID:" << id
    << "New values - Nom:" << nomEntreprise
    << "Secteur:" << secteurActivite
    << "DateDebut:" << dateDebutContrat.toString("yyyy-MM-dd")
    << "DateFin:" << dateFinContrat.toString("yyyy-MM-dd")
    << "Montant:" << montantTotal
    << "Statut:" << statut;

    QSqlQuery query;
    query.prepare("UPDATE PARTENAIRE SET NOMENTREPRISE = :nom, SECTEURACTIVITE = :secteur, "
                  "DATEDEBUTCONTRAT = :dateDebut, DATEFINCONTRAT = :dateFin, "
                  "MONTANTTOTAL = :montant, STATUT = :statut WHERE IDPARTENAIRE = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nomEntreprise);
    query.bindValue(":secteur", secteurActivite);
    query.bindValue(":dateDebut", dateDebutContrat);
    query.bindValue(":dateFin", dateFinContrat);
    query.bindValue(":montant", montantTotal);
    query.bindValue(":statut", statut);

    bool result = query.exec();

    if (result) {
        qDebug() << "[LOG][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        << "][Partenaire::modifier] Successfully modified partenaire with ID:" << id;
    } else {
        qDebug() << "[ERROR][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        << "][Partenaire::modifier] Failed to modify partenaire with ID:" << id
        << "Error:" << query.lastError().text();
    }

    return result;
}

// Supprimer un partenaire
bool Partenaire::supprimer(int id)
{
    qDebug() << "[LOG][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
    << "][Partenaire::supprimer] Deleting partenaire with ID:" << id;

    QSqlQuery query;
    query.prepare("DELETE FROM PARTENAIRE WHERE IDPARTENAIRE = :id");
    query.bindValue(":id", id);

    bool result = query.exec();

    if (result) {
        qDebug() << "[LOG][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        << "][Partenaire::supprimer] Successfully deleted partenaire with ID:" << id;
    } else {
        qDebug() << "[ERROR][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        << "][Partenaire::supprimer] Failed to delete partenaire with ID:" << id
        << "Error:" << query.lastError().text();
    }

    return result;
}

// Afficher les partenaires
QSqlQueryModel* Partenaire::afficher()
{
    qDebug() << "[LOG][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
    << "][Partenaire::afficher] Displaying all partenaires";

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PARTENAIRE");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Entreprise"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Secteur Activité"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date Début Contrat"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Fin Contrat"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Montant Total"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));

    qDebug() << "[LOG][" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
             << "][Partenaire::afficher] Found" << model->rowCount() << "partenaires";

    return model;
}

