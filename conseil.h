#ifndef CONSEIL_H
#define CONSEIL_H
#include <QPushButton>
#include <QTableView>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Conseil {
private:
    int idConseil;  // La base de données s'en charge
    QString matricule;
    QString etat;
    QString type;
    double tarif;
    QString description;
    QString datee;

public:
    Conseil();  // Constructeur sans idConseil
    Conseil(QString matricule, QString etat, QString type, double tarif, QString description, QString datee);  // Constructeur avec paramètres

    // Getters
    int getIdConseil() const { return idConseil; }
    QString getMatricule() const { return matricule; }
    QString getEtat() const { return etat; }
    QString getType() const { return type; }
    double getTarif() const { return tarif; }
    QString getDescription() const { return description; }
    QString getDatee() const { return datee; }

    // Setters
    void setIdConseil(int idConseil) { this->idConseil = idConseil; }
    void setMatricule(const QString &mat) { matricule = mat; }
    void setEtat(const QString &e) { etat = e; }
    void setType(const QString &t) { type = t; }
    void setTarif(double t) { tarif = t; }
    void setDescription(const QString &desc) { description = desc; }
    void setDatee(const QString &date) { datee = date; }

    // Méthodes de base
    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(int idConseil);
    bool update();
    void rechercher(QTableView *table, QString cas);
    static bool existe(QString matricule);

};

#endif // CONSEIL_H
