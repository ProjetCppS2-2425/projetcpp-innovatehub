#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsTextItem>
#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTableWidget>
#include <QMap>
#include<QMessageBox>
#include "transaction.h"
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->tableView->setModel(T.afficher());
   // ui->logo->setPixmap(QPixmap("C:\\Users\\dhibm\\Desktop\\workshift\\assets\\workshift-removebg-preview.png"));
    pageMap[ui->pushButton_4EMPLOYERS] = ui->stackedWidget_2->indexOf(ui->page_2);
    pageMap[ui->pushButton_5CLIENTS] = ui->stackedWidget_2->indexOf(ui->page_4);
    pageMap[ui->pushButton_6CONSEIL] = ui->stackedWidget_2->indexOf(ui->page_3);
    pageMap[ui->pushButton_7TRANSACTIONS] = ui->stackedWidget_2->indexOf(ui->page_5);
    pageMap[ui->pushButton_8PARTENAIRES] = ui->stackedWidget_2->indexOf(ui->page_6);

    // Connecter chaque bouton dynamiquement
    for (auto it = pageMap.begin(); it != pageMap.end(); ++it) {
        connect(it.key(), &QPushButton::clicked, this, [=]() {
            ui->stackedWidget_2->setCurrentIndex(it.value());
        });
    }
}
void MainWindow::showEmploye() {
    ui->stackedWidget_2->setCurrentIndex(0); // Page Employe
}

void MainWindow::showClient() {
    ui->stackedWidget_2->setCurrentIndex(1); // Page Client
}

void MainWindow::showConseil() {
    ui->stackedWidget_2->setCurrentIndex(2); // Page Conseil
}

void MainWindow::showTransaction() {
    ui->stackedWidget_2->setCurrentIndex(3); // Page Transaction
}

void MainWindow::showPartenaires() {
    ui->stackedWidget_2->setCurrentIndex(4); // Page Partenaires
}



void MainWindow::on_pushButton_2Transaction_clicked() {

    // Récupération des valeurs des champs
    double montant = ui->lineEdit1Transaction->text().toDouble();
    QDate date = ui->dateEditTransaction->date();
    QString nomPayeur = ui->lineEdit_3Transaction->text();
    QString type = ui->comboBox1Transaction->currentText(); // Revenues / Dépenses
    QString statut = ui->comboBox_2Transaction->currentText(); // Validé / En attente / Annulé

    transaction T(montant, date, nomPayeur, type, statut);
    if (T.ajouter()) {
        ui->tableView->setModel(T.afficher());  // Mise à jour de la table
        QMessageBox::information(this, "Succès", "Transaction ajoutée avec succès.");
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout de la transaction.");
    }
}



    MainWindow::~MainWindow()
{
    delete ui;
}


