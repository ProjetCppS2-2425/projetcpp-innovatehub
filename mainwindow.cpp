#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QButtonGroup>
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Vérification de la connexion à la base de données
    Connection conn;
    if (!conn.createconnect()) {
        QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter à la base de données.");
    }

    // 🔹 Création du groupe de boutons pour le genre
    QButtonGroup *genderGroup = new QButtonGroup(this);
    genderGroup->addButton(ui->radioButtonHomme);
    genderGroup->addButton(ui->radioButtonFemme);
    genderGroup->setExclusive(true);  // Assurer qu'un seul bouton peut être sélectionné
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_btn_Valider_clicked()
{
    QString nom = ui->lineEdit_Nom->text();
    QString prenom = ui->lineEdit_Prenom->text();
    QString email = ui->lineEdit_Email->text();
    QString telephone = ui->lineEdit_Telephone->text();
    QString poste = ui->lineEdit_Poste->text();
    QString salaire = ui->lineEdit_Salaire->text();
    QString cin = ui->lineEdit_CIN->text();
    QString motDePasse = ui->lineEdit_MotDePasse->text();

    // 🔹 Vérification du genre avec QRadioButton (pas CheckBox)
    QString gender;
    if (ui->radioButtonHomme->isChecked()) {
        gender = "Homme";
    } else if (ui->radioButtonFemme->isChecked()) {
        gender = "Femme";
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un genre !");
        return;
    }

    // 🔹 Vérification des champs vides
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() ||
        telephone.isEmpty() || poste.isEmpty() || salaire.isEmpty() || cin.isEmpty()
        || motDePasse.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    // 🔹 Vérification du format du CIN (8 chiffres)
    if (cin.length() != 8 || !cin.toInt()) {
        QMessageBox::warning(this, "Erreur", "Le CIN doit contenir exactement 8 chiffres !");
        return;
    }

    // 🔹 Vérification du format du salaire
    bool ok;
    double salaireDouble = salaire.toDouble(&ok);
    if (!ok || salaireDouble <= 0) {
        QMessageBox::warning(this, "Erreur", "Le salaire doit être un nombre positif !");
        return;
    }

    // 🔹 Vérification de l’email
    if (!email.contains("@") || !email.contains(".")) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un email valide !");
        return;
    }

    // 🔹 Vérification du format du numéro de téléphone
    if (telephone.length() < 8) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit contenir au moins 8 chiffres !");
        return;
    }

    if (telephone.length() < 8) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit contenir au moins 8 chiffres !");
        return;
    }

    bool phoneOk;
    long long numTel = telephone.toLongLong(&phoneOk); // Conversion du téléphone en long long
    if (!phoneOk || numTel <= 0) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone n'est pas valide !");
        return;

    }

    // 🔹 Vérification de la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Erreur", "La connexion à la base de données est fermée !");
        return;
    }

    // 🔹 Création de l'objet employé (passage du téléphone sous forme de QString)
    Employe employe(1, prenom, nom, salaireDouble, poste, cin, email, telephone, motDePasse, gender);

    // 🔹 Ajout à la base de données
    if (employe.ajouter()) {
        QMessageBox::information(this, "Succès", "L'employé a été ajouté avec succès !");
    } else {
        QMessageBox::warning(this, "Erreur", "Une erreur est survenue lors de l'ajout !");
    }
}


void MainWindow::on_btn_Annuler_clicked()
{
    // Réinitialisation des champs du formulaire
    ui->lineEdit_Nom->clear();
    ui->lineEdit_Prenom->clear();
    ui->lineEdit_Email->clear();
    ui->lineEdit_Telephone->clear();
    ui->lineEdit_Poste->clear();
    ui->lineEdit_Salaire->clear();
    ui->lineEdit_CIN->clear();
    ui->lineEdit_MotDePasse->clear();

    // Réinitialisation de la date d'embauche

    // 🔹 Décocher les boutons radio
    ui->radioButtonHomme->setAutoExclusive(false);
    ui->radioButtonFemme->setAutoExclusive(false);
    ui->radioButtonHomme->setChecked(false);
    ui->radioButtonFemme->setChecked(false);
    ui->radioButtonHomme->setAutoExclusive(true);
    ui->radioButtonFemme->setAutoExclusive(true);
}


// Dans votre méthode (par exemple, un slot qui est appelé lorsque vous souhaitez afficher les employés)

void MainWindow::on_pushButtonAfficher_clicked()
{
    qDebug() << "La fonction afficherEmployes a été appelée";  // Débogage

    // Vérification si la connexion à la base de données est ouverte
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Erreur", "La connexion à la base de données n'est pas ouverte.");
        return;  // Quitte la fonction si la connexion n'est pas ouverte
    }



    // Créer une requête SQL pour récupérer les employés
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYES");

    // Exécuter la requête
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible d'exécuter la requête : " + query.lastError().text());
        return;  // Si la requête échoue, un message d'erreur sera affiché et la fonction s'arrête
    }

    // Vider le QTableWidget avant d'afficher les nouveaux résultats
    ui->tableWidgetEmployes->clearContents();
    ui->tableWidgetEmployes->setRowCount(0);  // Remettre le nombre de lignes à 0

    // Parcourir les résultats de la requête et ajouter les données dans le QTableWidget
    int row = 0;
    while (query.next()) {
        ui->tableWidgetEmployes->insertRow(row);  // Insérer une nouvelle ligne dans le QTableWidget

        // Ajouter les données dans chaque cellule de la ligne
        ui->tableWidgetEmployes->setItem(row, 0, new QTableWidgetItem(query.value("IDE").toString()));
        ui->tableWidgetEmployes->setItem(row, 1, new QTableWidgetItem(query.value("PRENOM").toString()));
        ui->tableWidgetEmployes->setItem(row, 2, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tableWidgetEmployes->setItem(row, 3, new QTableWidgetItem(query.value("SALAIRE").toString()));
        ui->tableWidgetEmployes->setItem(row, 4, new QTableWidgetItem(query.value("POSTE").toString()));
        ui->tableWidgetEmployes->setItem(row, 5, new QTableWidgetItem(query.value("CIN").toString()));
        ui->tableWidgetEmployes->setItem(row, 6, new QTableWidgetItem(query.value("EMAIL").toString()));
        ui->tableWidgetEmployes->setItem(row, 7, new QTableWidgetItem(query.value("NUMERODETELEPHONE").toString()));
        ui->tableWidgetEmployes->setItem(row, 8, new QTableWidgetItem(query.value("MOTDEPASSE").toString()));
        ui->tableWidgetEmployes->setItem(row, 9, new QTableWidgetItem(query.value("GENDER").toString()));

        row++;
    }
 connect(ui->pushButtonAfficher, &QPushButton::clicked, this, &MainWindow::on_pushButtonAfficher_clicked);
    // Vérifier si le tableau est vide
    if (row == 0) {
        QMessageBox::information(this, "Aucun employé", "Il n'y a pas d'employé à afficher.");
    }
}

void MainWindow::on_deleteButton_clicked()
{
    // Get the selected employee's ID from the QTableWidget (assuming you select a row to delete)
    int row = ui->tableWidgetEmployes->currentRow();  // Get the currently selected row
    if (row == -1) {
        QMessageBox::warning(this, "Sélection incorrecte", "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    // Get the employee's IDE (assuming it's in the first column)
    QString ide = ui->tableWidgetEmployes->item(row, 0)->text();

    // Create the SQL query to delete the employee by IDE
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYES WHERE IDE = :ide");
    query.bindValue(":ide", ide);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::on_deleteButton_clicked);

    // Execute the delete query
    if (query.exec()) {
        QMessageBox::information(this, "Succès", "L'employé a été supprimé avec succès.");
        ui->tableWidgetEmployes->removeRow(row);  // Remove the row from the UI
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de supprimer l'employé : " + query.lastError().text());
    }
}
void MainWindow::on_tableWidgetEmployes_itemSelectionChanged()
{
    // Vérifier si un employé est sélectionné dans le tableau
    int row = ui->tableWidgetEmployes->currentRow();
    if (row == -1) {
        return; // Si aucune ligne n'est sélectionnée, ne rien faire
    }

    // Récupérer l'IDE de l'employé sélectionné
    QString ide = ui->tableWidgetEmployes->item(row, 0)->text();  // Supposons que l'IDE est dans la première colonne

    // Créer une requête pour récupérer les informations de l'employé à partir de la base de données
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYES WHERE IDE = :ide");
    query.bindValue(":ide", ide);

    // Exécuter la requête
    if (query.exec() && query.next()) {
        // Récupérer les valeurs des colonnes et remplir le formulaire
        ui->lineEdit_Nom->setText(query.value("NOM").toString());
        ui->lineEdit_Prenom->setText(query.value("PRENOM").toString());
        ui->lineEdit_Email->setText(query.value("EMAIL").toString());
        ui->lineEdit_Telephone->setText(query.value("NUMERODETELEPHONE").toString());
        ui->lineEdit_Poste->setText(query.value("POSTE").toString());
        ui->lineEdit_Salaire->setText(query.value("SALAIRE").toString());
        ui->lineEdit_CIN->setText(query.value("CIN").toString());

        QString gender = query.value("GENDER").toString();
        if (gender == "Homme") {
            ui->radioButtonHomme->setChecked(true);
        } else if (gender == "Femme") {
            ui->radioButtonFemme->setChecked(true);
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer les informations de l'employé.");
    }
}

void MainWindow::on_modifierButton_clicked()
{
    // Vérifier si un employé est sélectionné dans le tableau
    int row = ui->tableWidgetEmployes->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Sélection incorrecte", "Veuillez sélectionner un employé à modifier.");
        return;
    }

    // Récupérer l'IDE de l'employé sélectionné
    QString ide = ui->tableWidgetEmployes->item(row, 0)->text();  // Supposons que l'IDE est dans la première colonne

    // Récupérer les nouvelles valeurs des champs de modification
    QString nom = ui->lineEdit_Nom->text();
    QString prenom = ui->lineEdit_Prenom->text();
    QString email = ui->lineEdit_Email->text();
    QString telephone = ui->lineEdit_Telephone->text();
    QString poste = ui->lineEdit_Poste->text();
    QString salaire = ui->lineEdit_Salaire->text();
    QString cin = ui->lineEdit_CIN->text();
    QString gender;
    if (ui->radioButtonHomme->isChecked()) {
        gender = "Homme";
    } else if (ui->radioButtonFemme->isChecked()) {
        gender = "Femme";
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un genre !");
        return;
    }

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty() ||
        poste.isEmpty() || salaire.isEmpty() || cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;  // Ne pas continuer si des champs sont vides
    }

    // Appeler la fonction modifier() dans la classe Employe pour mettre à jour l'employé dans la base de données
    bool success = Employe::modifier(ide, prenom, nom, salaire, poste, cin, email, telephone, gender);

    if (success) {
        QMessageBox::information(this, "Succès", "L'employé a été modifié avec succès.");

        // Mettre à jour la ligne correspondante dans le QTableWidget
        ui->tableWidgetEmployes->item(row, 1)->setText(prenom);  // Mettre à jour le prénom
        ui->tableWidgetEmployes->item(row, 2)->setText(nom);      // Mettre à jour le nom
        ui->tableWidgetEmployes->item(row, 3)->setText(salaire);  // Mettre à jour le salaire
        ui->tableWidgetEmployes->item(row, 4)->setText(poste);    // Mettre à jour le poste
        ui->tableWidgetEmployes->item(row, 5)->setText(cin);      // Mettre à jour le CIN
        ui->tableWidgetEmployes->item(row, 6)->setText(email);    // Mettre à jour l'email
        ui->tableWidgetEmployes->item(row, 7)->setText(telephone);// Mettre à jour le téléphone
        ui->tableWidgetEmployes->item(row, 8)->setText(gender);   // Mettre à jour le genre
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de modifier l'employé.");
    }
}
