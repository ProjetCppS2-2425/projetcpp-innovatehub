#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "qdatetime.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "connection.h"
#include <QUuid>

class transaction
{
public:
    transaction();
    transaction(double montant, QDate date, QString nomPayeur, QString type, QString statut );

    bool ajouter();
    bool supprimer(QString reference);
    QSqlQueryModel *afficher();
    bool modifier(QString reference);
    QString getLastError() const { return lastError; }

private:
    double montant;
    QDate date;
    QString nomPayeur, type, statut;
    QString lastError;
    
    bool executeQuery(QSqlQuery &query);
};

#endif // TRANSACTION_H
