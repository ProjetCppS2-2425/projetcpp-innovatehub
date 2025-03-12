// partenaire.h
#ifndef PARTENAIRE_H
#define PARTENAIRE_H
#include <QSqlQuery>
#include <QString>
#include <QSqlQueryModel>
#include <QDate>

class Partenaire
{
private:
    int idPartenaire;
    QString nomEntreprise;
    QString secteurActivite;
    QDate dateDebutContrat;
    QDate dateFinContrat;
    double montantTotal;
    QString statut;

public:
    // Constructeurs
    Partenaire();
    Partenaire(int id, const QString &nom, const QString &secteur,
               const QDate &dateDebut, const QDate &dateFin,
               double montant, const QString &statut);

    // Getters
    int getIdPartenaire() const { return idPartenaire; }
    QString getNomEntreprise() const { return nomEntreprise; }
    QString getSecteurActivite() const { return secteurActivite; }
    QDate getDateDebutContrat() const { return dateDebutContrat; }
    QDate getDateFinContrat() const { return dateFinContrat; }
    double getMontantTotal() const { return montantTotal; }
    QString getStatut() const { return statut; }

    // Setters
    void setIdPartenaire(int id) { idPartenaire = id; }
    void setNomEntreprise(const QString &nom) { nomEntreprise = nom; }
    void setSecteurActivite(const QString &secteur) { secteurActivite = secteur; }
    void setDateDebutContrat(const QDate &date) { dateDebutContrat = date; }
    void setDateFinContrat(const QDate &date) { dateFinContrat = date; }
    void setMontantTotal(double montant) { montantTotal = montant; }
    void setStatut(const QString &s) { statut = s; }

    // CRUD operations
    bool ajouter();
    bool modifier(int id);
    bool supprimer(int id);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercher(const QString& critere);
    QSqlQueryModel* trier(const QString& critere, Qt::SortOrder order = Qt::AscendingOrder);
};
#endif // PARTENAIRE_H
