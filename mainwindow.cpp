
// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "partenaire.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup page mapping
    pageMap[ui->pushButton_4EMPLOYERS] = ui->stackedWidget_2->indexOf(ui->page_2);
    pageMap[ui->pushButton_5CLIENTS] = ui->stackedWidget_2->indexOf(ui->page_4);
    pageMap[ui->pushButton_6CONSEIL] = ui->stackedWidget_2->indexOf(ui->page_3);
    pageMap[ui->pushButton_7TRANSACTIONS] = ui->stackedWidget_2->indexOf(ui->page_5);
    pageMap[ui->pushButton_8PARTENAIRES] = ui->stackedWidget_2->indexOf(ui->page_6);

    // Connect buttons to page changes
    for (auto it = pageMap.begin(); it != pageMap.end(); ++it) {
        connect(it.key(), &QPushButton::clicked, this, [=]() {
            ui->stackedWidget_2->setCurrentIndex(it.value());
        });
    }

    ui->dateEdit_debutd->setDate(QDate::currentDate());
    ui->dateEdit_find->setDate(QDate::currentDate().addMonths(1));

    // Initialize partenaire view
    refreshPartenaireTable();



    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEmploye() {
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::showClient() {
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::showConseil() {
    ui->stackedWidget_2->setCurrentIndex(2);
}

void MainWindow::showTransaction() {
    ui->stackedWidget_2->setCurrentIndex(3);
}

void MainWindow::showPartenaires() {
    ui->stackedWidget_2->setCurrentIndex(4);
    refreshPartenaireTable();
}

void MainWindow::refreshPartenaireTable()
{
    Partenaire p;
    ui->tableView->setModel(p.afficher());
}

void MainWindow::setupConnections()
{
    // Connect tableView selection to form fields
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=]() {
        QModelIndex index = ui->tableView->currentIndex();
        if (index.isValid()) {
            ui->lineEdit_idp->setText(ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toString());
            ui->lineEdit_nome->setText(ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 1)).toString());
            ui->lineEdit_secteur->setText(ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 2)).toString());
            ui->dateEdit_debutd->setDate(ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 3)).toDate());
            ui->dateEdit_find->setDate(ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 4)).toDate());
            ui->lineEdit_montant->setText(ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 5)).toString());

            QString status = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 6)).toString();
            int statusIndex = 0;
            if (status == "valide") statusIndex = 1;
            else if (status == "annule") statusIndex = 2;
            ui->comboBox_statut->setCurrentIndex(statusIndex);
        }
    });
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    // Validate required fields first
    if (ui->lineEdit_idp->text().isEmpty() || ui->lineEdit_nome->text().isEmpty() ||
        ui->lineEdit_secteur->text().isEmpty() || ui->lineEdit_montant->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "All fields must be filled");
        return;
    }

    // Validate ID and Montant
    if (!validateId() || !validateMontant()) {
        return;
    }

    int id = ui->lineEdit_idp->text().toInt();
    QString nom = ui->lineEdit_nome->text();
    QString secteur = ui->lineEdit_secteur->text();
    QDate dateDebut = ui->dateEdit_debutd->date();
    QDate dateFin = ui->dateEdit_find->date();
    double montant = ui->lineEdit_montant->text().toDouble();

    // Get status from combobox
    QString statut;
    switch(ui->comboBox_statut->currentIndex()) {
    case 0: statut = "En attente"; break;
    case 1: statut = "valide"; break;
    case 2: statut = "annule"; break;
    default: statut = "En attente";
    }

    Partenaire p(id, nom, secteur, dateDebut, dateFin, montant, statut);
    bool test = p.ajouter();

    if (test) {
        QMessageBox::information(this, "Success", "Partenaire ajouté avec succès");
        refreshPartenaireTable();
        clearPartenaireFields();
    } else {
        QMessageBox::critical(this, "Error", "Erreur lors de l'ajout du partenaire");
    }
}

void MainWindow::on_pushButton_annuller_clicked()
{
    clearPartenaireFields();
}

void MainWindow::clearPartenaireFields()
{
    ui->lineEdit_idp->clear();
    ui->lineEdit_nome->clear();
    ui->lineEdit_secteur->clear();
    ui->dateEdit_debutd->setDate(QDate::currentDate());
    ui->dateEdit_find->setDate(QDate::currentDate());
    ui->lineEdit_montant->clear();
    ui->comboBox_statut->setCurrentIndex(0);
}

// Add these methods to the MainWindow class

bool MainWindow::validateId()
{
    QString idText = ui->lineEdit_idp->text();
    bool ok;
    int id = idText.toInt(&ok);

    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Validation Error", "L'ID doit être un nombre entier positif");
        ui->lineEdit_idp->setFocus();
        return false;
    }
    return true;
}

bool MainWindow::validateMontant()
{
    QString montantText = ui->lineEdit_montant->text();
    bool ok;
    double montant = montantText.toDouble(&ok);

    if (!ok || montant < 0) {
        QMessageBox::warning(this, "Validation Error", "Le montant doit être un nombre décimal positif");
        ui->lineEdit_montant->setFocus();
        return false;
    }
    return true;
}

void MainWindow::on_pushButton_modifier_clicked()
{
    if (ui->lineEdit_idp->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Veuillez sélectionner un partenaire à modifier");
        return;
    }

    // Validate ID and Montant
    if (!validateId() || !validateMontant()) {
        return;
    }

    int id = ui->lineEdit_idp->text().toInt();
    QString nom = ui->lineEdit_nome->text();
    QString secteur = ui->lineEdit_secteur->text();
    QDate dateDebut = ui->dateEdit_debutd->date();
    QDate dateFin = ui->dateEdit_find->date();
    double montant = ui->lineEdit_montant->text().toDouble();

    QString statut;
    switch(ui->comboBox_statut->currentIndex()) {
    case 0: statut = "En attente"; break;
    case 1: statut = "valide"; break;
    case 2: statut = "annule"; break;
    default: statut = "En attente";
    }

    Partenaire p(id, nom, secteur, dateDebut, dateFin, montant, statut);
    bool test = p.modifier(id);

    if (test) {
        QMessageBox::information(this, "Success", "Partenaire modifié avec succès");
        refreshPartenaireTable();
        clearPartenaireFields();
    } else {
        QMessageBox::critical(this, "Error", "Erreur lors de la modification du partenaire");
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Error", "Veuillez sélectionner un partenaire à supprimer");
        return;
    }

    int id = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              "Êtes-vous sûr de vouloir supprimer ce partenaire?",
                                                              QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Partenaire p;
        bool test = p.supprimer(id);

        if (test) {
            QMessageBox::information(this, "Success", "Partenaire supprimé avec succès");
            refreshPartenaireTable();
            clearPartenaireFields();
        } else {
            QMessageBox::critical(this, "Error", "Erreur lors de la suppression du partenaire");
        }
    }
}




