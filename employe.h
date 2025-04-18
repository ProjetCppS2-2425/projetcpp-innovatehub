#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QMessageBox>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Employe {
private:
    int idE;
    QString prenom;
    QString nom;

    double salaire;
    QString poste;
    QString cin;
    QString email;
 QMap<int, int> employeIds;
    QString numeroDeTelephone;
    QString motDePasse;

public:
    // 🔹 Constructeurs
    // 🔹 Constructor declaration in 'employe.h'
    Employe();
    Employe(int idE, QString prenom, QString nom, double salaire,
            QString poste, QString cin, QString email,
            QString numeroDeTelephone, QString motDePasse, QString gender);

    // 🔹 Méthodes CRUD
    bool ajouter();
    bool supprimer(int cin);
    bool modifier();
    QSqlQueryModel* afficher();
static bool supprimer(const QString &ide);

    // Déclaration de la méthode statique pour modifier un employé
static bool modifier(QString cin, QString prenom, QString nom, double salaire,
                     QString poste, QString email, QString telephone, QString gender);
    // 🔹 Getters
    int getId() const { return idE; }
    QString getPrenom() const { return prenom; }
    QString getNom() const { return nom; }

    double getSalaire() const { return salaire; }
    QString getPoste() const { return poste; }
    QString getCin() const { return cin; }
    QString getEmail() const { return email; }
    QString getNumeroDeTelephone() const { return numeroDeTelephone; }
    QString getMotDePasse() const { return motDePasse; }
    QString gender;
    QString getGender() const { return gender; }


    // 🔹 Setters
    void setId(int id) { idE = id; }
    void setPrenom(const QString &p) { prenom = p; }
    void setNom(const QString &n) { nom = n; }

    void setSalaire(double s) { salaire = s; }
    void setPoste(const QString &p) { poste = p; }
    void setCin(const QString &c) { cin = c; }
    void setEmail(const QString &e) { email = e; }

    void setNumeroDeTelephone(const QString &num) { numeroDeTelephone = num; }
    void setMotDePasse(const QString &mdp) { motDePasse = mdp; }
    void setGender(const QString &g) { gender = g; }
};
#endif // EMPLOYE_H
