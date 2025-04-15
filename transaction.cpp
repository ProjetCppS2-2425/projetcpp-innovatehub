#include "transaction.h"
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QWidget>

Transaction::Transaction()
{
    reference = 0;
    montant = 0.0;
    modePaiement = "";
    dateee = QDate::currentDate();
    statutt = "En attente";
    typee = "Vente";
    idEmploye = 0;
    idClient = 0;
    idPartenaire = 0;
    lastError = "";
}

Transaction::Transaction(int reference, double montant, 
                       const QString &modePaiement, const QDate &dateee,
                       const QString &statutt, const QString &typee,
                       int idEmploye, int idClient, int idPartenaire)
{
    this->reference = reference;
    this->montant = montant;
    this->modePaiement = modePaiement;
    this->dateee = dateee;
    this->statutt = statutt;
    this->typee = typee;
    this->idEmploye = idEmploye;
    this->idClient = idClient;
    this->idPartenaire = idPartenaire;
    this->lastError = "";
}

bool Transaction::executeQuery(QSqlQuery &query)
{
    if (!query.exec()) {
        lastError = query.lastError().text();
        qDebug() << "Erreur SQL: " << lastError;
        return false;
    }
    return true;
}

int Transaction::generateReference()
{
    int newReference = 0;
    QSqlQuery query;
    query.prepare("SELECT MAX(REFERENCE) FROM SYRINE.TRANSACTION");
    
    if (query.exec() && query.next()) {
        newReference = query.value(0).toInt() + 1;
        if (newReference < 1001) {
            newReference = 1001; // Commencer à 1001 pour les références
        }
    } else {
        newReference = 1001; // Si aucune transaction n'existe encore
    }
    
    return newReference;  // Retourne une nouvelle référence unique
}

int Transaction::generateId()
{
    int newId = 0;
    QSqlQuery query;
    query.prepare("SELECT MAX(IDTRANSACTION) FROM SYRINE.TRANSACTION");
    
    if (query.exec() && query.next()) {
        newId = query.value(0).toInt() + 1;
        if (newId <= 0) {
            newId = 1; // Commencer à 1 si aucune transaction n'existe encore
        }
    } else {
        newId = 1; // Si aucune transaction n'existe encore ou en cas d'erreur
    }
    
    return newId;  // Retourne un nouvel ID unique
}

bool Transaction::ajouter()
{
    // Générer une nouvelle référence si nécessaire
    if (reference <= 0) {
        reference = generateReference();
    }
    
    // Générer un nouvel ID de transaction
    int newId = generateId();
    
    QSqlQuery query;
    query.prepare("INSERT INTO SYRINE.TRANSACTION "
                 "(IDTRANSACTION, REFERENCE, MONTANT, MODEPAIEMENT, DATEEE, STATUTT, TYPEE, IDEMPLOYE, IDCLIENT, IDPARTENAIRE) "
                 "VALUES (:id, :reference, :montant, :modePaiement, :dateee, :statutt, :typee, :idEmploye, :idClient, :idPartenaire)");
    
    query.bindValue(":id", newId);
    query.bindValue(":reference", reference);
    query.bindValue(":montant", montant);
    query.bindValue(":modePaiement", modePaiement);
    query.bindValue(":dateee", dateee);
    query.bindValue(":statutt", statutt);
    query.bindValue(":typee", typee);
    query.bindValue(":idEmploye", idEmploye > 0 ? idEmploye : QVariant(QVariant::Int));
    query.bindValue(":idClient", idClient > 0 ? idClient : QVariant(QVariant::Int));
    query.bindValue(":idPartenaire", idPartenaire > 0 ? idPartenaire : QVariant(QVariant::Int));
    
    return executeQuery(query);
}

bool Transaction::modifier()
{
    // D'abord, récupérer l'IDTRANSACTION correspondant à cette référence
    int idTransaction = 0;
    QSqlQuery idQuery;
    idQuery.prepare("SELECT IDTRANSACTION FROM SYRINE.TRANSACTION WHERE REFERENCE = :reference");
    idQuery.bindValue(":reference", reference);
    
    if (idQuery.exec() && idQuery.next()) {
        idTransaction = idQuery.value("IDTRANSACTION").toInt();
    } else {
        lastError = "Impossible de trouver la transaction avec la référence " + QString::number(reference);
        return false;
    }
    
    QSqlQuery query;
    query.prepare("UPDATE SYRINE.TRANSACTION SET "
                 "MONTANT = :montant, "
                 "MODEPAIEMENT = :modePaiement, "
                 "DATEEE = :dateee, "
                 "STATUTT = :statutt, "
                 "TYPEE = :typee, "
                 "IDEMPLOYE = :idEmploye, "
                 "IDCLIENT = :idClient, "
                 "IDPARTENAIRE = :idPartenaire "
                 "WHERE IDTRANSACTION = :idTransaction");
    
    query.bindValue(":montant", montant);
    query.bindValue(":modePaiement", modePaiement);
    query.bindValue(":dateee", dateee);
    query.bindValue(":statutt", statutt);
    query.bindValue(":typee", typee);
    query.bindValue(":idEmploye", idEmploye > 0 ? idEmploye : QVariant(QVariant::Int));
    query.bindValue(":idClient", idClient > 0 ? idClient : QVariant(QVariant::Int));
    query.bindValue(":idPartenaire", idPartenaire > 0 ? idPartenaire : QVariant(QVariant::Int));
    query.bindValue(":idTransaction", idTransaction);
    
    return executeQuery(query);
}

bool Transaction::supprimer(int reference)
{
    // D'abord, récupérer l'IDTRANSACTION correspondant à cette référence
    int idTransaction = 0;
    QSqlQuery idQuery;
    idQuery.prepare("SELECT IDTRANSACTION FROM SYRINE.TRANSACTION WHERE REFERENCE = :reference");
    idQuery.bindValue(":reference", reference);
    
    if (idQuery.exec() && idQuery.next()) {
        idTransaction = idQuery.value("IDTRANSACTION").toInt();
    } else {
        lastError = "Impossible de trouver la transaction avec la référence " + QString::number(reference);
        return false;
    }
    
    QSqlQuery query;
    query.prepare("DELETE FROM SYRINE.TRANSACTION WHERE IDTRANSACTION = :idTransaction");
    query.bindValue(":idTransaction", idTransaction);
    
    return executeQuery(query);
}

QSqlQueryModel* Transaction::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    
    model->setQuery("SELECT t.REFERENCE, t.MONTANT, t.MODEPAIEMENT, "
                   "t.DATEEE, t.STATUTT, t.TYPEE, "
                   "CASE "
                   "WHEN t.IDCLIENT IS NOT NULL THEN (SELECT NOM FROM SYRINE.CLIENT WHERE IDCLIENT = t.IDCLIENT) "
                   "WHEN t.IDPARTENAIRE IS NOT NULL THEN (SELECT NOMENTREPRISE FROM SYRINE.PARTENAIRE WHERE IDPARTENAIRE = t.IDPARTENAIRE) "
                   "ELSE 'Non spécifié' "
                   "END as PAYEUR "
                   "FROM SYRINE.TRANSACTION t "
                   "ORDER BY t.REFERENCE DESC");
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur SQL dans afficher(): " << model->lastError().text();
        delete model;
        return nullptr;
    }
    
    // Définir les en-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Référence"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Montant"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Mode Paiement"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Payeur"));
    
    return model;
}

Transaction Transaction::rechercher(int reference)
{
    Transaction transaction;
    QSqlQuery query;
    query.prepare("SELECT * FROM SYRINE.TRANSACTION WHERE REFERENCE = :reference");
    query.bindValue(":reference", reference);
    
    if (query.exec() && query.next()) {
        transaction.setReference(query.value("REFERENCE").toInt());
        transaction.setMontant(query.value("MONTANT").toDouble());
        transaction.setModePaiement(query.value("MODEPAIEMENT").toString());
        transaction.setDateee(query.value("DATEEE").toDate());
        transaction.setStatutt(query.value("STATUTT").toString());
        transaction.setTypee(query.value("TYPEE").toString());
        transaction.setIdEmploye(query.value("IDEMPLOYE").toInt());
        transaction.setIdClient(query.value("IDCLIENT").toInt());
        transaction.setIdPartenaire(query.value("IDPARTENAIRE").toInt());
    } else {
        transaction.lastError = query.lastError().text();
    }
    
    return transaction;
}

bool Transaction::verifierExistence(int reference)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM SYRINE.TRANSACTION WHERE REFERENCE = :reference");
    query.bindValue(":reference", reference);
    
    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    
    lastError = query.lastError().text();
    return false;
}

QSqlQueryModel* Transaction::rechercherParCriteres(const QString &searchText)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    
    // Vérifier si le terme de recherche est un nombre (référence)
    bool isNumber;
    int reference = searchText.toInt(&isNumber);
    
    QString queryStr = "SELECT t.REFERENCE, t.MONTANT, t.MODEPAIEMENT, t.DATEEE, t.STATUTT, t.TYPEE, "
                      "CASE "
                      "WHEN t.IDCLIENT > 0 THEN c.NOM "
                      "WHEN t.IDPARTENAIRE > 0 THEN p.NOMENTREPRISE "
                      "ELSE 'Aucun' "
                      "END AS PAYEUR "
                      "FROM SYRINE.TRANSACTION t "
                      "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                      "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                      "WHERE ";
    
    if (isNumber) {
        queryStr += "t.REFERENCE = " + QString::number(reference) + " OR ";
    }
    
    queryStr += "c.NOM LIKE '%" + searchText + "%' OR "
               "p.NOMENTREPRISE LIKE '%" + searchText + "%' OR "
               "t.STATUTT LIKE '%" + searchText + "%'";
    
    model->setQuery(queryStr);
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur SQL dans rechercherParCriteres(): " << model->lastError().text();
        delete model;
        return nullptr;
    }
    
    // Définir les en-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Référence"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Montant"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Mode Paiement"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Payeur"));
    
    return model;
}

QSqlQueryModel* Transaction::rechercherAvance(int reference, const QString &statut, const QString &type, 
                                            const QString &payeur, const QDate &dateDebut, const QDate &dateFin)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    
    QString queryStr = "SELECT t.REFERENCE, t.MONTANT, t.MODEPAIEMENT, t.DATEEE, t.STATUTT, t.TYPEE, "
                      "CASE "
                      "WHEN t.IDCLIENT > 0 THEN c.NOM "
                      "WHEN t.IDPARTENAIRE > 0 THEN p.NOMENTREPRISE "
                      "ELSE 'Aucun' "
                      "END AS PAYEUR "
                      "FROM SYRINE.TRANSACTION t "
                      "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                      "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                      "WHERE 1=1 "; // Condition toujours vraie pour faciliter la construction de la requête
    
    // Ajouter les conditions de filtrage
    if (reference > 0) {
        queryStr += "AND t.REFERENCE = " + QString::number(reference) + " ";
    }
    
    if (!statut.isEmpty() && statut != "Tous") {
        queryStr += "AND t.STATUTT = '" + statut + "' ";
    }
    
    if (!type.isEmpty() && type != "Tous") {
        queryStr += "AND t.TYPEE = '" + type + "' ";
    }
    
    if (!payeur.isEmpty()) {
        queryStr += "AND (c.NOM LIKE '%" + payeur + "%' OR p.NOMENTREPRISE LIKE '%" + payeur + "%') ";
    }
    
    // Filtrage par date
    queryStr += "AND t.DATEEE BETWEEN '" + dateDebut.toString("yyyy-MM-dd") + "' AND '" + dateFin.toString("yyyy-MM-dd") + "' ";
    
    // Tri par date décroissante
    queryStr += "ORDER BY t.DATEEE DESC";
    
    model->setQuery(queryStr);
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur SQL dans rechercherAvance(): " << model->lastError().text();
        delete model;
        return nullptr;
    }
    
    // Définir les en-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Référence"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Montant"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Mode Paiement"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Payeur"));
    
    return model;
}

QSqlQueryModel* Transaction::trierParCritere(const QString &criterion)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    
    // Déterminer la colonne SQL à utiliser pour le tri
    QString orderByColumn;
    
    if (criterion == "Date") {
        orderByColumn = "t.DATEEE";
    } else if (criterion == "Montant") {
        orderByColumn = "t.MONTANT";
    } else if (criterion == "Type") {
        orderByColumn = "t.TYPEE";
    } else if (criterion == "Statut") {
        orderByColumn = "t.STATUTT";
    } else if (criterion == "Référence") {
        orderByColumn = "t.REFERENCE";
    } else {
        // Par défaut, trier par date
        orderByColumn = "t.DATEEE";
    }
    
    QString queryStr = "SELECT t.REFERENCE, t.MONTANT, t.MODEPAIEMENT, t.DATEEE, t.STATUTT, t.TYPEE, "
                      "CASE "
                      "WHEN t.IDCLIENT > 0 THEN c.NOM "
                      "WHEN t.IDPARTENAIRE > 0 THEN p.NOMENTREPRISE "
                      "ELSE 'Aucun' "
                      "END AS PAYEUR "
                      "FROM SYRINE.TRANSACTION t "
                      "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                      "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                      "ORDER BY " + orderByColumn + " DESC";
    
    model->setQuery(queryStr);
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur SQL dans trierParCritere(): " << model->lastError().text();
        delete model;
        return nullptr;
    }
    
    // Définir les en-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Référence"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Montant"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Mode Paiement"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Payeur"));
    
    return model;
}

void Transaction::chargerDansFormulaire(QWidget* parent, int reference, 
                                       QLineEdit* montantEdit, QDateEdit* dateEdit,
                                       QComboBox* statutCombo, QComboBox* typeCombo,
                                       QComboBox* modeCombo, QComboBox* clientCombo,
                                       QComboBox* partenaireCombo)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM SYRINE.TRANSACTION WHERE REFERENCE = :reference");
    query.bindValue(":reference", reference);
    
    if (query.exec() && query.next()) {
        // Remplir les champs avec les données actuelles
        montantEdit->setText(query.value("MONTANT").toString());
        dateEdit->setDate(query.value("DATEEE").toDate());
        statutCombo->setCurrentText(query.value("STATUTT").toString());
        typeCombo->setCurrentText(query.value("TYPEE").toString());
        modeCombo->setCurrentText(query.value("MODEPAIEMENT").toString());
        
        // Sélectionner le client ou le partenaire
        int clientId = query.value("IDCLIENT").toInt();
        int partenaireId = query.value("IDPARTENAIRE").toInt();
        
        if (clientId > 0) {
            int index = clientCombo->findData(clientId);
            if (index != -1) clientCombo->setCurrentIndex(index);
        } else {
            clientCombo->setCurrentIndex(0);
        }
        
        if (partenaireId > 0) {
            int index = partenaireCombo->findData(partenaireId);
            if (index != -1) partenaireCombo->setCurrentIndex(index);
        } else {
            partenaireCombo->setCurrentIndex(0);
        }
    } else {
        QMessageBox::warning(parent, "Erreur", "Aucune transaction trouvée avec cette référence.");
    }
}

