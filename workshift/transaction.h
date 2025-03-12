#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDateTime>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include "connection.h"

// Forward declarations pour les classes Qt utilisées
class QWidget;
class QLineEdit;
class QDateEdit;
class QComboBox;

class Transaction
{
public:
    Transaction();
    Transaction(int reference, double montant, 
               const QString &modePaiement, const QDate &dateee,
               const QString &statutt, const QString &typee,
               int idEmploye, int idClient, int idPartenaire);

    // Getters
    int getReference() const { return reference; }
    double getMontant() const { return montant; }
    QString getModePaiement() const { return modePaiement; }
    QDate getDateee() const { return dateee; }
    QString getStatutt() const { return statutt; }
    QString getTypee() const { return typee; }
    int getIdEmploye() const { return idEmploye; }
    int getIdClient() const { return idClient; }
    int getIdPartenaire() const { return idPartenaire; }
    QString getLastError() const { return lastError; }

    // Setters
    void setReference(int value) { reference = value; }
    void setMontant(double value) { montant = value; }
    void setModePaiement(const QString &value) { modePaiement = value; }
    void setDateee(const QDate &value) { dateee = value; }
    void setStatutt(const QString &value) { statutt = value; }
    void setTypee(const QString &value) { typee = value; }
    void setIdEmploye(int value) { idEmploye = value; }
    void setIdClient(int value) { idClient = value; }
    void setIdPartenaire(int value) { idPartenaire = value; }

    // CRUD Operations
    bool ajouter();
    bool supprimer(int reference);
    bool modifier();
    static QSqlQueryModel* afficher();
    static Transaction rechercher(int reference);
    bool verifierExistence(int reference);
    
    // Nouvelles méthodes pour la recherche et le tri
    static QSqlQueryModel* rechercherParCriteres(const QString &searchText);
    static QSqlQueryModel* rechercherAvance(int reference, const QString &statut, const QString &type, 
                                          const QString &payeur, const QDate &dateDebut, const QDate &dateFin);
    static QSqlQueryModel* trierParCritere(const QString &criterion);
    
    // Méthode pour charger une transaction dans les champs du formulaire
    static void chargerDansFormulaire(QWidget* parent, int reference, 
                                     QLineEdit* montantEdit, QDateEdit* dateEdit,
                                     QComboBox* statutCombo, QComboBox* typeCombo,
                                     QComboBox* modeCombo, QComboBox* clientCombo,
                                     QComboBox* partenaireCombo);

private:
    int reference;  // Référence unique de la transaction (identifiant visible pour l'utilisateur)
    double montant;
    QString modePaiement;
    QDate dateee;
    QString statutt;
    QString typee;
    int idEmploye;
    int idClient;
    int idPartenaire;
    QString lastError;

    bool executeQuery(QSqlQuery &query);
    static int generateReference();  // Génère une nouvelle référence unique
    static int generateId();  // Génère un nouvel ID interne pour la base de données
};

#endif // TRANSACTION_H
