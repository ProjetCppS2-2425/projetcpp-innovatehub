#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include "conseil.h"
#include <QSqlDriver>
#include <QMessageBox>
#include <QDebug>
#include <QDate>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QItemDelegate>
#include <QHeaderView>
#include <QTextBoundaryFinder>
#include <QRegularExpression>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QtCharts>
#include <QTimer>

#include <QSqlQueryModel>
#include <QString>

#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLegend>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "smtpclient/smtpclient.h"
#include "smtpclient/mimemessage.h"
#include "smtpclient/emailaddress.h"
#include "smtpclient/mimetext.h"
#include "smtpclient/mimeattachment.h"

//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QAuthenticator>
//#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation du modèle de table
    model = new QSqlTableModel(this);
    model->setTable("CONSEIL");



    // Définir les en-têtes AVANT le select()
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Matricule"));
    model->setHeaderData(2, Qt::Horizontal, tr("État"));
    model->setHeaderData(3, Qt::Horizontal, tr("Type"));
    model->setHeaderData(4, Qt::Horizontal, tr("Tarif"));
    model->setHeaderData(5, Qt::Horizontal, tr("Description"));
    model->setHeaderData(6, Qt::Horizontal, tr("Date"));

    if (!model->select()) {
        qDebug() << "Erreur lors de la sélection des données : " << model->lastError().text();
    } else {
        qDebug() << "Modèle de table initialisé avec succès.";
    }

    // Appliquer le modèle à la TableView
    ui->tableViewConseil->setModel(model);

    ui->tableViewConseil->horizontalHeader()->setVisible(true);
    ui->tableViewConseil->horizontalHeader()->show();

    // Version haute visibilité
    ui->tableViewConseil->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "background-color: #1a365d ;"
        "color: white;"
        "padding: 5px;"
        "border: none;"
        "}"
        );
    // Empêcher la modification de la clé primaire (IDCONSEIL)
    //ui->tableViewConseil->setColumnHidden(model->fieldIndex("IDCONSEIL"), true);

    // Autoriser l'édition par double-clic ou touche Entrée/F2
   // ui->tableViewConseil->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    QItemDelegate *delegate = new QItemDelegate();
    ui->tableViewConseil->setItemDelegateForColumn(7, delegate);  // 4 = colonne Description

    // Appliquer un style personnalisé
    ui->tableViewConseil->setStyleSheet(
        "QTableView::item { background-color: white; color: black; }"
        "QTableView::item:selected { background-color: lightblue; color: black; }"
        "QTableView::item:!selected:hover { background-color: lightgray; }"
        );

    ui->tableViewConseil->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    // Afficher les en-têtes de colonne
    ui->tableViewConseil->resizeColumnsToContents();
    ui->tableViewConseil->horizontalHeader()->setStretchLastSection(true);

    // Sélection d'une ligne entière au lieu d'une seule cellule
    ui->tableViewConseil->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewConseil->setSelectionMode(QAbstractItemView::SingleSelection);

    peuplerComboBoxRecherche();    // Connecter les boutons et événements


    connect(model, &QSqlTableModel::dataChanged, this, &MainWindow::on_affconseil_clicked);

    connect(ui->ajoutconseil, &QPushButton::clicked, this, &MainWindow::on_ajoutconseil_clicked);

    connect(ui->suppconseil, &QPushButton::clicked, this, &MainWindow::on_suppconseil_clicked);

    connect(ui->editconseil, &QPushButton::clicked, this, &MainWindow::on_editconseil_clicked);

    connect(ui->affconseil, &QPushButton::clicked, this, &MainWindow::on_affconseil_clicked);

    connect(ui->tableViewConseil, &QTableView::clicked, this, &MainWindow::on_tableViewConseil_clicked);

    connect(ui->pdfconseil, &QPushButton::clicked, this, &MainWindow::on_pdfconseil_clicked);

    connect(ui->recherconseil, &QPushButton::clicked, this, &MainWindow::on_recherconseil_clicked);

    connect(ui->trieconseil, &QPushButton::clicked, this, &MainWindow::on_trieconseil_clicked);

    connect(ui->statconseil, &QPushButton::clicked, this, &MainWindow::on_statconseil_clicked);




}


MainWindow::~MainWindow() {
    delete ui; // Libération de la mémoire pour ui
    delete model; // Libération de la mémoire pour model
}




void MainWindow::peuplerComboBoxRecherche()
{
    QSqlDatabase db = QSqlDatabase::database(); // Récupérer votre connexion à la base de données
    if (db.isOpen()) {
        QSqlQuery query;

        // Récupérer les valeurs distinctes de la colonne MATRICULE
        query.exec("SELECT DISTINCT MATRICULE FROM CONSEIL");
        while (query.next()) {
            ui->rechconscob->addItem(query.value(0).toString());
        }

        // Récupérer les valeurs distinctes de la colonne TYPE
        query.exec("SELECT DISTINCT TYPE FROM CONSEIL");
        while (query.next()) {
            QString type = query.value(0).toString();
            if (ui->rechconscob->findText(type) == -1) // Éviter les doublons
                ui->rechconscob->addItem(type);
        }

        // Récupérer les valeurs distinctes de la colonne ETAT
        query.exec("SELECT DISTINCT ETAT FROM CONSEIL");
        while (query.next()) {
            QString etat = query.value(0).toString();
            if (ui->rechconscob->findText(etat) == -1) // Éviter les doublons
                ui->rechconscob->addItem(etat);
        }
    } else {
        qDebug() << "Base de données non ouverte !";
    }

    ui->triconcob->clear(); // Effacer les anciens éléments
    ui->triconcob->addItem("Type");
    ui->triconcob->addItem("Tarif");
    ui->triconcob->addItem("Date");

}




void MainWindow::on_ajoutconseil_clicked() {
    QString matricule = ui->matricule->text();
    QString etat = ui->etatconseil->currentText();
    QString type = ui->typeconseil->currentText();
    double tarif = ui->tarifconseil->text().toDouble();
    QString description = ui->desconseil->text();
    QString datee = ui->dateconseil->text();

    // Réinitialiser les styles des champs
    ui->matricule->setStyleSheet("");
    ui->etatconseil->setStyleSheet("");
    ui->typeconseil->setStyleSheet("");
    ui->tarifconseil->setStyleSheet("");
    ui->desconseil->setStyleSheet("");
    ui->dateconseil->setStyleSheet("");


    // Vérification des champs vides
    if (matricule.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le champ 'Matricule' ne peut pas être vide.");
        ui->matricule->setFocus();
        ui->matricule->setStyleSheet("border: 1px solid red;"); // Cadre rouge
        return;
    }


    if (etat.isEmpty() || etat == "Sélectionner...") { // Assurez-vous que l'élément par défaut n'est pas sélectionné
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez sélectionner un 'État'.");
        ui->etatconseil->setFocus();
        ui->etatconseil->setStyleSheet("border: 1px solid red;"); // Cadre rouge
        return;
    }

    if (type.isEmpty() || type == "Sélectionner...") { // Assurez-vous que l'élément par défaut n'est pas sélectionné
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez sélectionner un 'Type'.");
        ui->typeconseil->setFocus();
        ui->typeconseil->setStyleSheet("border: 1px solid red;"); // Cadre rouge
        return;
    }
    // Vérification du tarif
    if (tarif <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le 'Tarif' doit être un nombre positif.");
        ui->tarifconseil->setFocus();
        ui->tarifconseil->setStyleSheet("border: 1px solid red;"); // Cadre rouge
        return;
    }
    if (description.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le champ 'Description' ne peut pas être vide.");
        ui->desconseil->setFocus();
        ui->desconseil->setStyleSheet("border: 1px solid red;"); // Cadre rouge
        return;
    }

    // Vérification du format de la date
    QDate date = QDate::fromString(datee, "dd-MM-yyyy");
    if (!date.isValid()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le format de la 'Date' est invalide. Utilisez le format 'JJ-MM-AAAA'.");
        ui->dateconseil->setFocus();
        ui->dateconseil->setStyleSheet("border: 1px solid red;"); // Cadre rouge
        return;
    }

    // Si tous les champs sont valides, appliquer le style vert
    ui->matricule->setStyleSheet("border: 1px solid green;");
    ui->etatconseil->setStyleSheet("border: 1px solid green;");
    ui->typeconseil->setStyleSheet("border: 1px solid green;");
    ui->tarifconseil->setStyleSheet("border: 1px solid green;");
    ui->desconseil->setStyleSheet("border: 1px solid green;");
    ui->dateconseil->setStyleSheet("border: 1px solid green;");

    qDebug() << "Ajout d'un nouveau conseil :"
             << "Matricule:" << matricule
             << "Etat:" << etat
             << "Type:" << type
             << "Tarif:" << tarif
             << "Description:" << description
             << "Date:" << datee;

    // Créer un nouvel objet Conseil
    Conseil newConseil(matricule, etat, type, tarif, description, datee);

    // Appeler la méthode ajouter() pour tenter d'ajouter le conseil
    if (newConseil.ajouter()) {
        QMessageBox::information(this, "Succès", "Conseil ajouté avec succès !");
        on_affconseil_clicked();  // Rafraîchir la table
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du conseil.");
    }

    viderChamps();

}



void MainWindow::on_affconseil_clicked()
{
    Conseil conseil;
    QSqlQueryModel *newModel = conseil.afficher();

    if(newModel) {
        // Définir les en-têtes
        newModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        newModel->setHeaderData(1, Qt::Horizontal, tr("Matricule"));
        // ... autres colonnes ...

        ui->tableViewConseil->setModel(newModel);

        // FORCER l'affichage
        ui->tableViewConseil->horizontalHeader()->setVisible(true);
        ui->tableViewConseil->horizontalHeader()->show();

        // Redimensionner
        ui->tableViewConseil->resizeColumnsToContents();

        // Si les en-têtes disparaissent encore, ajoutez :
        QTimer::singleShot(0, [this]() {
            ui->tableViewConseil->horizontalHeader()->setVisible(true);
            ui->tableViewConseil->horizontalHeader()->show();
        });
    }
    viderChamps();

}



void MainWindow::on_suppconseil_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableViewConseil->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Suppression", "Veuillez sélectionner une ligne à supprimer.");
        return;
    }

    // Obtenir l'ID du conseil depuis la ligne sélectionnée
    QModelIndex selectedIndex = selectedRows.first();
    int row = selectedIndex.row();
    int idConseil = ui->tableViewConseil->model()->data(ui->tableViewConseil->model()->index(row, 0)).toInt(); // 0 = colonne IDCONSEIL

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer ce conseil ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Conseil c;
        if (c.supprimer(idConseil)) {
            QMessageBox::information(this, "Succès", "Conseil supprimé avec succès.");
            on_affconseil_clicked(); // Rafraîchir la table
        } else {
            QMessageBox::critical(this, "Erreur", "La suppression a échoué.");
        }
    }
}


void MainWindow::on_editconseil_clicked() {
    // Créer un objet Conseil avec les nouvelles valeurs du formulaire
    Conseil c;
    c.setIdConseil(selectedId);
    c.setMatricule(ui->matricule->text());
    c.setEtat(ui->etatconseil->currentText());
    c.setType(ui->typeconseil->currentText());
    c.setTarif(ui->tarifconseil->text().toDouble());
    c.setDescription(ui->desconseil->text());
    c.setDatee(ui->dateconseil->text());

    // Vérification des champs avant de procéder à la modification
    qDebug() << "Matricule:" << c.getMatricule();  // Affiche le matricule dans la console
    qDebug() << "État:" << c.getEtat();            // Affiche l'état dans la console
    qDebug() << "Type:" << c.getType();            // Affiche le type dans la console
    qDebug() << "Description:" << c.getDescription();  // Affiche la description dans la console
    qDebug() << "Tarif:" << c.getTarif();          // Affiche le tarif dans la console
    qDebug() << "Date:" << c.getDatee();           // Affiche la date dans la console

    // Vérification des champs avant de procéder à la modification
    if (c.getMatricule().isEmpty() || c.getEtat().isEmpty() || c.getType().isEmpty() ||
        c.getDescription().isEmpty() || c.getTarif() <= 0 || !QDate::fromString(c.getDatee(), "dd-MM-yyyy").isValid()) {

        // Si un champ est incorrect, afficher le message d'erreur
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs correctement.");
        return; // Arrêter la fonction si un champ est vide ou invalide
    }

    // Si tous les champs sont valides, effectuer la modification
    if (c.update()) {
        QMessageBox::information(this, "Succès", "Conseil modifié avec succès.");

        // Rafraîchir l'affichage
        Conseil temp;
        QSqlQueryModel *model = temp.afficher();
        ui->tableViewConseil->setModel(model);
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }

    // Réinitialiser les champs
    viderChamps();
}



void MainWindow::on_tableViewConseil_clicked(const QModelIndex &index) {
    if (!index.isValid()) {
        return; // Vérifiez si l'index est valide
    }

    QSqlQueryModel *model = qobject_cast<QSqlQueryModel *>(ui->tableViewConseil->model());
    if (!model) {
        qDebug() << "Échec du cast du modèle.";
        return;
    }

    QSqlRecord record = model->record(index.row());
    selectedId = record.value("IDCONSEIL").toInt(); // Assurez-vous que l'ID est correctement récupéré

    // Remplir les champs du formulaire
    ui->matricule->setText(record.value("MATRICULE").toString());
    ui->etatconseil->setCurrentText(record.value("ETAT").toString());
    ui->typeconseil->setCurrentText(record.value("TYPE").toString());
    ui->tarifconseil->setText(record.value("TARIF").toString());
    ui->desconseil->setText(record.value("DESCRIPTION").toString());
    QDate date = record.value("DATEE").toDate();
    ui->dateconseil->setText(date.toString("dd-MM-yyyy"));


}


void MainWindow::viderChamps() {
    // Clear all input fields
    ui->matricule->clear();
    ui->etatconseil->setCurrentIndex(0); // Reset combo box to the first item
    ui->tarifconseil->clear();
    ui->desconseil->clear();
    ui->dateconseil->clear();
    selectedId = -1;

}



void MainWindow::on_pdfconseil_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "*.pdf");
    if (fileName.isEmpty())
        return;

    if (QFileInfo(fileName).suffix().isEmpty())
        fileName.append(".pdf");

    QPdfWriter pdf(fileName);
    pdf.setResolution(300); // 300 DPI pour une bonne qualité
    pdf.setPageMargins(QMarginsF(30, 30, 30, 30)); // Marges de 30 pixels

    // Taille A4 en pixels (300 DPI)
    const int pageWidth = pdf.width();
    const int pageHeight = pdf.height();

    QPainter painter(&pdf);
    painter.setRenderHint(QPainter::Antialiasing);

    // Paramètres de mise en page
    const int leftMargin = 100;
    const int topMargin = 150;
    const int bottomMargin = 100;
    int currentY = topMargin;

    // 1. Titre centré en haut de page
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    QRect titleRect(0, 50, pageWidth, 60);
    painter.drawText(titleRect, Qt::AlignCenter, "Rapport détaillé des Conseils");
    currentY += 100;

    QAbstractItemModel* model = ui->tableViewConseil->model();
    if (!model || model->rowCount() == 0) {
        painter.drawText(leftMargin, currentY, "Aucune donnée à afficher");
        painter.end();
        return;
    }

    // Configuration du tableau
    const int columnCount = model->columnCount();
    const int rowCount = model->rowCount();
    const int headerHeight = 50;  // Augmenter la hauteur de l'en-tête
    const int rowHeight = 50;  // Augmenter la hauteur des lignes
    const int cellPadding = 10;  // Augmenter le padding des cellules pour plus d'espace

    // Calcul des largeurs de colonnes
    QVector<int> columnWidths(columnCount);
    int totalWidth = 0;

    painter.setFont(QFont("Arial", 10, QFont::Bold));
    for (int col = 0; col < columnCount; ++col) {
        QString header = model->headerData(col, Qt::Horizontal).toString();
        columnWidths[col] = painter.fontMetrics().horizontalAdvance(header) + cellPadding * 2;

        // Vérifier la largeur des données
        painter.setFont(QFont("Arial", 10));
        for (int row = 0; row < rowCount; ++row) {
            QString data = model->data(model->index(row, col)).toString();
            columnWidths[col] = qMax(columnWidths[col],
                                     painter.fontMetrics().horizontalAdvance(data) + cellPadding * 2);
        }

        // Largeur minimale de 100 pixels pour plus d'espace
        columnWidths[col] = qMax(columnWidths[col], 100);
        totalWidth += columnWidths[col];
    }

    // Ajuster si trop large
    if (totalWidth > pageWidth - 2 * leftMargin) {
        double ratio = (pageWidth - 2 * leftMargin) / (double)totalWidth;
        for (int col = 0; col < columnCount; ++col) {
            columnWidths[col] *= ratio;
        }
    }

    // Centrer le tableau horizontalement
    int tableLeft = (pageWidth - totalWidth) / 2;

    // Dessiner l'en-tête du tableau
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.setPen(QPen(Qt::black, 1.5));
    painter.setBrush(QBrush(QColor(70, 130, 180))); // Bleu acier

    int currentX = tableLeft;
    for (int col = 0; col < columnCount; ++col) {
        // Rectangle d'en-tête
        painter.drawRect(currentX, currentY, columnWidths[col], headerHeight);

        // Texte centré
        QString header = model->headerData(col, Qt::Horizontal).toString();
        painter.setPen(Qt::white);
        painter.drawText(currentX, currentY, columnWidths[col], headerHeight,
                         Qt::AlignCenter, header);
        painter.setPen(Qt::black);

        currentX += columnWidths[col];
    }
    currentY += headerHeight;

    // Dessiner les données du tableau
    painter.setFont(QFont("Arial", 10));
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(Qt::NoBrush);

    for (int row = 0; row < rowCount; ++row) {
        currentX = tableLeft;

        // Dessiner les cellules
        for (int col = 0; col < columnCount; ++col) {
            // Alternance de couleurs pour les lignes
            if (row % 2 == 0) {
                painter.setBrush(QBrush(QColor(240, 248, 255))); // Alice blue
            } else {
                painter.setBrush(QBrush(Qt::white));
            }

            // Dessiner la cellule
            painter.drawRect(currentX, currentY, columnWidths[col], rowHeight);

            // Dessiner le texte
            QString text = model->data(model->index(row, col)).toString();
            // Formater la date si c'est la colonne DATEE
            if (model->headerData(col, Qt::Horizontal).toString() == "DATEE") {
                QDateTime date = QDateTime::fromString(text, Qt::ISODate);
                text = date.toString("ddd MMM d.yyyy");
            }

            painter.drawText(currentX + cellPadding, currentY,
                             columnWidths[col] - 2 * cellPadding, rowHeight,
                             Qt::AlignLeft | Qt::AlignVCenter, text);

            currentX += columnWidths[col];
        }

        currentY += rowHeight;

        // Vérifier si on dépasse la page
        if (currentY > pageHeight - bottomMargin) {
            // Ajouter la date en bas de page avant de changer
            painter.setFont(QFont("Arial", 10));
            painter.drawText(QRect(0, pageHeight - 50, pageWidth, 30),
                             Qt::AlignCenter,
                             "Date de génération du rapport : " + QDate::currentDate().toString("ddd MMM d.yyyy"));

            // Créer une nouvelle page si nécessaire
            pdf.newPage();
            currentY = topMargin;

            // Redessiner l'en-tête du tableau sur la nouvelle page
            currentX = tableLeft;
            painter.setFont(QFont("Arial", 10, QFont::Bold));
            painter.setPen(QPen(Qt::black, 1.5));
            painter.setBrush(QBrush(QColor(70, 130, 180)));

            for (int col = 0; col < columnCount; ++col) {
                painter.drawRect(currentX, currentY, columnWidths[col], headerHeight);
                QString header = model->headerData(col, Qt::Horizontal).toString();
                painter.setPen(Qt::white);
                painter.drawText(currentX, currentY, columnWidths[col], headerHeight,
                                 Qt::AlignCenter, header);
                painter.setPen(Qt::black);
                currentX += columnWidths[col];
            }
            currentY += headerHeight;

            painter.setFont(QFont("Arial", 10));
            painter.setPen(QPen(Qt::black, 1));
        }
    }

    // Date en bas de la dernière page
    painter.setFont(QFont("Arial", 10));
    painter.drawText(QRect(0, pageHeight - 50, pageWidth, 30),
                     Qt::AlignCenter,
                     "Date de génération du rapport : " + QDate::currentDate().toString("ddd MMM d.yyyy"));

    painter.end();
    QMessageBox::information(this, "Export PDF", "Le rapport a été généré avec succès !");
}




// Fonction pour supprimer les accents
QString enleverAccents(const QString& texte) {
    QString normalised = texte.normalized(QString::NormalizationForm_D);
    return normalised.remove(QRegularExpression("[\\u0300-\\u036f]"));
}


void MainWindow::on_recherconseil_clicked()
{
    QString critere = ui->rechconscob->currentText().trimmed();
    QString recherche = ui->lineEditRecherche->text().trimmed();

    if (critere.isEmpty()) {
        QMessageBox::information(this, "Recherche", "Veuillez sélectionner un critère de recherche.");
        return;
    }

    if (recherche.isEmpty()) {
        QMessageBox::information(this, "Recherche", "Veuillez entrer un terme à rechercher.");
        return;
    }

    QString rechercheNormalisee = enleverAccents(recherche).toLower();
    QSqlQueryModel *modelRecherche = new QSqlQueryModel();
    QSqlQuery query;

    QString requete = "SELECT IDCONSEIL, MATRICULE, ETAT, TYPE, TARIF, DESCRIPTION, DATEE FROM CONSEIL WHERE ";

    if (critere == "Matricule") {
        requete += "LOWER(MATRICULE) LIKE :recherche";
        query.prepare(requete);
        query.bindValue(":recherche", "%" + rechercheNormalisee + "%");

    } else if (critere == "Etats") {
        requete += "LOWER(ETAT) LIKE :recherche";  // Normalisation en minuscule
        query.prepare(requete);
        query.bindValue(":recherche", "%" + rechercheNormalisee + "%");

    } else if (critere == "Date") {
        // Accepte : dd-MM-yyyy, dd/MM/yyyy, yyyy-MM-dd
        QString dateInput = recherche;
        dateInput.replace("/", "-");

        QDate date = QDate::fromString(dateInput, "dd-MM-yyyy");
        if (!date.isValid()) {
            date = QDate::fromString(dateInput, "yyyy-MM-dd");
        }
        if (!date.isValid()) {
            QMessageBox::warning(this, "Format incorrect", "Veuillez entrer une date au format JJ-MM-AAAA ou AAAA-MM-JJ.");
            delete modelRecherche;
            return;
        }

        QString dateSql = date.toString("yyyy-MM-dd");
        requete += "TRUNC(DATEE) = TO_DATE(:recherche, 'YYYY-MM-DD')";
        query.prepare(requete);
        query.bindValue(":recherche", dateSql);

    } else {
        QMessageBox::warning(this, "Recherche", "Critère de recherche non reconnu.");
        delete modelRecherche;
        return;
    }

    qDebug() << "Requête exécutée : " << requete << " avec : " << recherche;

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'exécution de la recherche : " + query.lastError().text());
        delete modelRecherche;
        return;
    }

    modelRecherche->setQuery(query);

    if (modelRecherche->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des résultats : " + modelRecherche->lastError().text());
        delete modelRecherche;
        return;
    }

    if (modelRecherche->rowCount() == 0) {
        QMessageBox::information(this, "Résultat", "Aucun résultat trouvé pour : " + recherche + " dans le critère : " + critere);
    } else {
        modelRecherche->setHeaderData(0, Qt::Horizontal, "ID");
        modelRecherche->setHeaderData(1, Qt::Horizontal, "Matricule");
        modelRecherche->setHeaderData(2, Qt::Horizontal, "ETAT");
        modelRecherche->setHeaderData(3, Qt::Horizontal, "TYPE");
        modelRecherche->setHeaderData(4, Qt::Horizontal, "TARIF");
        modelRecherche->setHeaderData(5, Qt::Horizontal, "DESCRIPTION");
        modelRecherche->setHeaderData(6, Qt::Horizontal, "DATEE");
    }

    ui->tableViewConseil->setModel(modelRecherche);
    ui->tableViewConseil->resizeColumnsToContents();
    ui->tableViewConseil->resizeRowsToContents();
    ui->tableViewConseil->horizontalHeader()->setStretchLastSection(true);

    connect(modelRecherche, &QSqlQueryModel::destroyed, modelRecherche, &QObject::deleteLater);
}


void MainWindow::on_trieconseil_clicked()
{
    QString critere = ui->triconcob->currentText();  // ex: "tarif", "date", "type"
    QString requete;

    // Construction de la requête SQL en fonction du critère choisi
    if (critere == "Tarif") {
        requete = "SELECT * FROM conseil ORDER BY tarif ASC";
    } else if (critere == "Date") {
        requete = "SELECT * FROM conseil ORDER BY datee ASC";
    } else if (critere == "Type") {
        requete = "SELECT * FROM conseil ORDER BY type ASC";
    } else {
        // Par défaut, on trie par ID
        requete = "SELECT * FROM conseil ORDER BY id ASC";
    }

    // Création et configuration du modèle
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(requete);

    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur SQL", model->lastError().text());
        return;
    }

    // Lier le modèle à la table view
    ui->tableViewConseil->setModel(model);
}



void MainWindow::on_statconseil_clicked()
{
    // Créer les données pour le graphique circulaire basé sur le type des conseils
    QPieSeries *series = new QPieSeries();

    QSqlQuery query;
    if (query.exec("SELECT type, COUNT(*) FROM conseil GROUP BY type")) {
        while (query.next()) {
            QString typeConseil = query.value(0).toString();
            int count = query.value(1).toInt();
            series->append(typeConseil, count);
        }
    } else {
        QMessageBox::critical(this, "Erreur de base de données", "Erreur lors de la récupération des statistiques de type : " + query.lastError().text());
        delete series;
        return;
    }

    if (series->count() == 0) {
        QMessageBox::information(this, "Statistiques", "Aucun conseil trouvé pour générer les statistiques de type.");
        delete series;
        return;
    }

    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des conseils par type");
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // **Label Adjustments (Prioritize these)**
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible();
        // **Try setting the label position to "outside"**
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setLabel(QString("%1 (%2%)").arg(slice->label()).arg(QString::number(100.0 * slice->percentage(), 'f', 1)));
    }

    // **Adjust Chart Margins (Give more space around the pie)**
    chart->layout()->setContentsMargins(10, 10, 10, 10); // Increase the margins

    // **Reduce Label Font Size (If outside positioning isn't enough)**
    QFont font = chart->legend()->font();
    font.setPointSize(5); // Try a smaller font size
    chart->legend()->setFont(font);

    for (QPieSlice *slice : series->slices()) {
        QFont sliceFont = slice->labelFont();
        sliceFont.setPointSize(5); // Try a smaller font size
        slice->setLabelFont(sliceFont);
    }

    // Créer la vue du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // **Increase the fixed size of the widget (Give more overall space)**
   // QSize fixedSize(700, 500); // Try larger dimensions
    //ui->chartViewConseil->setFixedSize(fixedSize);

    // Nettoyer l'ancien layout
    QLayout *oldLayout = ui->chartViewConseil->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    // Nouveau layout
    QVBoxLayout *layout = new QVBoxLayout(ui->chartViewConseil);
    layout->addWidget(chartView);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Afficher la page
    ui->chartViewConseil->setLayout(layout);
}


void MainWindow::on_historique_clicked()
{
    Conseil conseil;
    QSqlQueryModel *model = conseil.afficher(); // récupère tous les conseils

    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "Historique", "Aucun conseil trouvé.");
        return;
    }

    QString historique;

    // Parcourir les lignes du modèle
    for (int row = 0; row < model->rowCount(); ++row) {
        QString id = model->data(model->index(row, 0)).toString();         // colonne 0 : ID
        QString type = model->data(model->index(row, 1)).toString();       // colonne 1 : Type
        QString description = model->data(model->index(row, 2)).toString();// colonne 2 : Description
        QString date = model->data(model->index(row, 3)).toString();       // colonne 3 : Date (si elle existe)

        historique += QString("🔹 ID: %1\nType: %2\nDescription: %3\nDate: %4\n\n")
                          .arg(id, type, description, date);
    }

    ui->textEditHistorique->setPlainText(historique);
}


/*void MainWindow::on_mailconseil_clicked() {
    // Paramètres SMTP
    QString smtpServer = "smtp.gmail.com";  // Utilisez le serveur SMTP de votre fournisseur
    int smtpPort = 587;  // Port pour TLS
    QString fromEmail = "votre_email@gmail.com";  // Votre email
    QString toEmail = "destinataire@example.com";  // Destinataire
    QString subject = "Sujet de l'email";
    QString body = "Voici le corps de l'email.";
    QString attachmentFile = "chemin/vers/le/fichier.pdf"; // Si vous avez un fichier joint

    // Préparer le corps du message
    QByteArray emailData;
    emailData.append("From: " + fromEmail.toUtf8() + "\r\n");
    emailData.append("To: " + toEmail.toUtf8() + "\r\n");
    emailData.append("Subject: " + subject.toUtf8() + "\r\n");
    emailData.append("\r\n");  // Séparation entre en-tête et corps
    emailData.append(body.toUtf8());

    // Ajouter une pièce jointe, si nécessaire
    if (!attachmentFile.isEmpty()) {
        QFile file(attachmentFile);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray fileData = file.readAll();
            file.close();
            emailData.append("\r\n--boundary\r\n");
            emailData.append("Content-Type: application/pdf; name=\"" + QFileInfo(attachmentFile).fileName().toUtf8() + "\"\r\n");
            emailData.append("Content-Disposition: attachment; filename=\"" + QFileInfo(attachmentFile).fileName().toUtf8() + "\"\r\n");
            emailData.append("Content-Transfer-Encoding: base64\r\n");
            emailData.append("\r\n");
            emailData.append(fileData.toBase64());
        }
    }

    // Créer une requête SMTP
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("smtp://" + smtpServer + ":" + QString::number(smtpPort));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");

    // Authentification SMTP (si nécessaire)
    QAuthenticator *authenticator = new QAuthenticator();
    authenticator->setUser("votre_email@gmail.com");  // Votre email
    authenticator->setPassword("votre_mot_de_passe");  // Votre mot de passe
    manager->authenticationRequired.connect([=](QNetworkReply *reply, QAuthenticator *auth) {
        auth->setUser("votre_email@gmail.com");
        auth->setPassword("votre_mot_de_passe");
    });

    // Envoyer la requête
    QNetworkReply *reply = manager->post(request, emailData);

    // Connexion au signal de réponse
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(nullptr, "Succès", "L'email a été envoyé avec succès.");
        } else {
            QMessageBox::warning(nullptr, "Erreur", "Erreur lors de l'envoi de l'email : " + reply->errorString());
        }
        reply->deleteLater();
    });
}*/
