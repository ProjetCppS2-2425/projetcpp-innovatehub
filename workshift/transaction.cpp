#include "transaction.h"

transaction::transaction() {}

transaction::transaction(double montant, QDate date, QString nomPayeur, QString type, QString statut)
    : montant(montant), date(date), nomPayeur(nomPayeur), type(type), statut(statut) {}

bool transaction::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO TRANSACTION (ID, REFERENCE, MONTANT, MODE_PAIMENT, DATE_PAIMENT, STATUT, TYPE) "
                  "VALUES (:id, :reference, :montant, :mode_paiment, :date_paiment, :statut, :type)");

    // Remplacez ces valeurs par celles que vous utilisez
    QString id = QUuid::createUuid().toString();  // Générer un ID unique
    QString reference = "REF" + QString::number(QDateTime::currentSecsSinceEpoch()); // Générer une référence unique

    query.bindValue(":id", id);
    query.bindValue(":reference", reference);
    query.bindValue(":montant", montant);
    query.bindValue(":mode_paiment", type);  // Assurez-vous que "type" correspond à "MODE_PAIMENT"
    query.bindValue(":date_paiment", date);
    query.bindValue(":statut", statut);
    query.bindValue(":type", type);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de l'ajout de la transaction :" << query.lastError().text();
        return false;
    }
    return true;
}


bool transaction::supprimer(QString reference) {
    QSqlQuery query;
    query.prepare("DELETE FROM TRANSACTION WHERE REFERENCE = :reference");
    query.bindValue(":reference", reference);
    return query.exec();
}

QSqlQueryModel *transaction::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID, REFERENCE, MONTANT, MODE_PAIMENT, DATE_PAIMENT, STATUT, TYPE FROM TRANSACTION ORDER BY DATE_PAIMENT DESC");
    return model;
}

