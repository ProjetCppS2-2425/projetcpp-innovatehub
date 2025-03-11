#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"  // Assure que la classe Client est reconnue
#include <QMessageBox>  // Pour afficher les messages


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), Etmp()  // Ajoute `Etmp()`
{
    ui->setupUi(this);  // Initialise l'interface graphique

    // Charger les données dès l'ouverture
    ui->tableViewAFFICHELIST->setModel(Etmp.afficher());
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_afficher_clicked()
{
    QSqlQueryModel *model = Etmp.afficher();
    ui->tableViewAFFICHELIST->setModel(model);
}






void MainWindow::on_pushButtonSPPRIMERCLIENT_clicked()
{
    int id = ui->lineEditID->text().toInt(); // Récupération de l'ID

    bool test = Etmp.supprimer(id); // Appel de la méthode supprimer()

    if(test)
    {
        QMessageBox::information(this, "Suppression", "Client supprimé avec succès.");
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
    }
}





void MainWindow::on_pushButtonAJOUTERCLIENT_clicked()
{
    // Récupération des informations saisies dans les champs
    int id = ui->lineEditID->text().toInt();  // Convertir l'ID en entier
    QString nom = ui->lineEditNOM->text();
    QString type = ui->lineEditTYPE->text();
    QString secteur = ui->lineEditSECTEUR->text();
    QString telephone = ui->lineEditTALEPHONE->text();
    QString email = ui->lineEditEMAIL->text();
    QString adresse = ui->lineEditADRESSE->text();


    // Instancier un objet de la classe Client    hethi chat
    QString date_creation = QDate::currentDate().toString("yyyy-MM-dd"); // Date automatique
    Client C(id, nom, type, secteur, telephone, email, adresse, date_creation);

    // Insérer le client dans la base de données
    bool test = C.ajouter();

    // Affichage d'un message de confirmation ou d'erreur
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué.\n"),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectué.\n"),
                              QMessageBox::Cancel);
    }

}


void MainWindow::on_pushButtonSPPRIMERCLIENT_2_clicked() //update
{
        int id = ui->lineEditID->text().toInt();
        QString nom = ui->lineEditNOM->text();
        QString type = ui->lineEditTYPE->text();
        QString secteur = ui->lineEditSECTEUR->text();
        QString telephone = ui->lineEditTALEPHONE->text();
        QString email = ui->lineEditEMAIL->text();
        QString adresse = ui->lineEditADRESSE->text();

        Client C;
        bool test = C.modifier(id, nom, type, secteur, telephone, email, adresse);

        if (test) {
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Modification effectuée.\n"), QMessageBox::Ok);

            // Mettre à jour l'affichage
            ui->tableViewAFFICHELIST->setModel(C.afficher());
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Modification non effectuée.\n"), QMessageBox::Cancel);
        }


}

