#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Créer un groupe de boutons pour gérer l'exclusivité des cases à cocher
    QButtonGroup* sexeGroup = new QButtonGroup(this);
    sexeGroup->addButton(ui->checkBox_Homme);
    sexeGroup->addButton(ui->checkBox_Femme);
    sexeGroup->setExclusive(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Bouton "Valider" : Récupérer les informations et afficher un message
void MainWindow::on_btn_Valider_clicked()
{
    QString nom = ui->lineEdit_Nom->text();
    QString prenom = ui->lineEdit_Prenom->text();
    QString email = ui->lineEdit_Email->text();
    QString motDePasse = ui->lineEdit_MotDePasse->text();
    QString telephone = ui->lineEdit_Telephone->text();
    QString poste = ui->lineEdit_Poste->text();
    QString salaire = ui->lineEdit_Salaire->text();
    QString cin = ui->lineEdit_CIN->text();
    QString dateEmbauche = ui->dateEdit_Embauche->date().toString("dd/MM/yyyy");  // Assure-toi de bien formater la date
    QString role = "Employé";  // Exemple de rôle, tu peux personnaliser
    QString dateNaissance = ui->dateEdit_Naissance->date().toString("dd/MM/yyyy");

    // Vérification des champs vides
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || motDePasse.isEmpty() ||
        telephone.isEmpty() || poste.isEmpty() || salaire.isEmpty() || cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    // Création de l'employé avec le constructeur adapté
    Employe employe(0, 0, cin, nom, prenom, telephone, poste, salaire.toDouble(), email, dateEmbauche, role, motDePasse);

    // Si tu veux ajouter l'employé dans la base de données, tu peux appeler la méthode ajouter() par exemple
    if (employe.ajouter()) {
        QMessageBox::information(this, "Employé ajouté", "L'employé a bien été ajouté.");
    } else {
        QMessageBox::warning(this, "Erreur", "Une erreur est survenue lors de l'ajout.");
    }
}


// Bouton "Annuler" : Réinitialiser tous les champs du formulaire
void MainWindow::on_btn_Annuler_clicked()
{
    ui->lineEdit_Nom->clear();
    ui->lineEdit_Prenom->clear();
    ui->lineEdit_Email->clear();
    ui->lineEdit_MotDePasse->clear();
    ui->lineEdit_Telephone->clear();
    ui->lineEdit_Poste->clear();
    ui->lineEdit_Salaire->clear();
    ui->lineEdit_CIN->clear();
    ui->dateEdit_Naissance->setDate(QDate::currentDate());
    ui->dateEdit_Embauche->setDate(QDate::currentDate());
    ui->checkBox_Homme->setChecked(false);
    ui->checkBox_Femme->setChecked(false);
}
