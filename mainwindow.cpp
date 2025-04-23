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
#include "arduino.h"




#include <QProcess>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation du modèle de table
    model = new QSqlTableModel(this);
    model->setTable("CONSEIL");



    // Définir les en-têtes AVANT le select()
    model->setHeaderData(0, Qt::Horizontal, tr("IDConseil"));
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

    // Vérification des champs vides
    if (Conseil::existe(matricule)) {
        QMessageBox::warning(this, "Erreur", "Ce matricule existe déjà. Veuillez en saisir un autre.");
        ui->matricule->setFocus();
        ui->matricule->setStyleSheet("border: 1px solid red;");
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
        viderChamps();
        ui->ajoutconseil->setFocus();

    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du conseil.");
    }


}



void MainWindow::on_affconseil_clicked()
{
    Conseil conseil;
    QSqlQueryModel *newModel = conseil.afficher();

    if(newModel) {
        // Définir les en-têtes
        newModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        newModel->setHeaderData(1, Qt::Horizontal, tr("Matricule"));

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
    // Liste des types disponibles : traitee, en cours, rejetee, en attente
    QStringList typesDisponibles = {"traitee", "en cours", "rejetee", "en attente"};
    QString baseDir = QFileDialog::getExistingDirectory(this, "Choisissez un dossier de sauvegarde");
    if (baseDir.isEmpty()) return;

    QStringList headers = {"Matricule", "Type", "Description", "Date"};
    QVector<int> columnWidths = {200, 200, 500, 250};
    int columnCount = headers.size();
    int rowHeight = 45;
    int headerHeight = 50;

    // Fonction pour exporter le PDF
    auto exporterPDF = [&](const QString &type, const QList<QVector<QString>> &dataRows, const QString &filePath) {
        QPdfWriter pdf(filePath);
        pdf.setPageMargins(QMarginsF(30, 30, 30, 30));
        pdf.setResolution(300);
        QPainter painter(&pdf);
        painter.setRenderHint(QPainter::Antialiasing);

        int pageWidth = pdf.width();
        int pageHeight = pdf.height();
        int topMargin = 120;
        int currentY = 240;
        int bottomMargin = 80;
        int tableWidth = std::accumulate(columnWidths.begin(), columnWidths.end(), 0);
        int tableLeft = (pageWidth - tableWidth) / 2;

        // Ajouter le logo
        QImage logo("logo.png");
        if (!logo.isNull()) {
            painter.drawImage(QRect(0, 0, 250, 250), logo);
        }

        // Titre du rapport
        painter.setFont(QFont("Arial", 16, QFont::Bold));
        painter.drawText(QRect(0, 150, pageWidth, 60), Qt::AlignCenter, "Rapport des conseils - Type : " + type);

        // En-tête du tableau
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        painter.setBrush(QColor(70, 130, 180));
        painter.setPen(Qt::black);

        int currentX = tableLeft;
        for (int i = 0; i < columnCount; ++i) {
            painter.drawRect(currentX, currentY, columnWidths[i], headerHeight);
            painter.setPen(Qt::white);
            painter.drawText(currentX, currentY, columnWidths[i], headerHeight, Qt::AlignCenter, headers[i]);
            painter.setPen(Qt::black);
            currentX += columnWidths[i];
        }
        currentY += headerHeight;

        // Dessiner les données
        painter.setFont(QFont("Arial", 9));
        int rowIndex = 0;
        for (const QVector<QString>& row : dataRows) {
            currentX = tableLeft;
            painter.setBrush((rowIndex % 2 == 0) ? QColor(245, 245, 245) : Qt::white);

            for (int i = 0; i < columnCount; ++i) {
                QString text = row[i];
                if (headers[i] == "Date") {
                    QDateTime date = QDateTime::fromString(text, Qt::ISODate);
                    text = date.toString("dd MMM yyyy");
                }
                painter.drawRect(currentX, currentY, columnWidths[i], rowHeight);
                painter.drawText(currentX + 5, currentY, columnWidths[i] - 10, rowHeight, Qt::AlignLeft | Qt::AlignVCenter, text);
                currentX += columnWidths[i];
            }

            currentY += rowHeight;
            rowIndex++;

            // Si on atteint la fin de la page, passer à la page suivante
            if (currentY + rowHeight + bottomMargin > pageHeight) {
                painter.drawText(QRect(0, pageHeight - 40, pageWidth, 30),
                                 Qt::AlignCenter,
                                 "Date du rapport : " + QDate::currentDate().toString("dd MMM yyyy"));
                pdf.newPage();
                currentY = topMargin;
            }
        }

        // Footer date
        painter.setFont(QFont("Arial", 9));
        painter.drawText(QRect(0, pageHeight - 40, pageWidth, 30),
                         Qt::AlignCenter,
                         "Date du rapport : " + QDate::currentDate().toString("dd MMM yyyy"));
        painter.end();
    };

    int pdfCount = 0;

    // Pour chaque type de conseil (4 types)
    for (const QString& type : typesDisponibles) {
        qDebug() << "Exécution de la requête pour le type:" << type;

        QSqlQuery query;
        query.prepare("SELECT matricule, type, description, datee FROM conseil WHERE type = :type");
        query.bindValue(":type", type);
        if (!query.exec()) {
            qDebug() << "Erreur d'exécution de la requête:" << query.lastError();
            continue;
        }

        QList<QVector<QString>> rows;
        while (query.next()) {
            QVector<QString> row;
            for (int i = 0; i < columnCount; ++i)
                row.append(query.value(i).toString());
            rows.append(row);
        }

        qDebug() << "Nombre de lignes pour le type" << type << ":" << rows.size();

        // Si des lignes ont été récupérées, générer le PDF
        if (!rows.isEmpty()) {
            QString fileName = baseDir + "/rapport_" + type + ".pdf";
            qDebug() << "Génération du fichier PDF pour le type:" << type << "fichier:" << fileName;
            exporterPDF(type, rows, fileName);
            pdfCount++;
        }
    }

    // Vérification du nombre de fichiers générés
    if (pdfCount == 0)
        QMessageBox::information(this, "Information", "Aucun rapport PDF n’a été généré (aucune donnée).");
    else
        QMessageBox::information(this, "Succès", QString("%1 rapport(s) PDF ont été générés !").arg(pdfCount));
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
    if (modelRecherche->rowCount() > 0) {
        QString matricule = modelRecherche->data(modelRecherche->index(0, 1)).toString(); // colonne 1 = MATRICULE
        Arduino arduino;
        if (!arduino.envoyerTexte(matricule)) {
            qDebug() << "Erreur Arduino:" << arduino.getLastError();
        } else {
            qDebug() << "Matricule envoyé à l'Arduino :" << matricule;
        }
    }

}


void MainWindow::on_trieconseil_clicked()
{
    QString critere = ui->triconcob->currentText();
    QString requete;

    // Écris les colonnes dans l'ordre souhaité (pas de SELECT *)
    QString colonnes = "IDCONSEIL, MATRICULE, ETAT, TYPE, TARIF, DESCRIPTION, DATEE";

    if (critere == "Tarif") {
        requete = "SELECT " + colonnes + " FROM conseil ORDER BY tarif ASC";
    } else if (critere == "Date") {
        requete = "SELECT " + colonnes + " FROM conseil ORDER BY datee ASC";
    } else if (critere == "Type") {
        requete = "SELECT " + colonnes + " FROM conseil ORDER BY type ASC";
    } else {
        requete = "SELECT " + colonnes + " FROM conseil ORDER BY IDCONSEIL ASC";
    }

    QSqlQueryModel *modelTrie = new QSqlQueryModel();
    modelTrie->setQuery(requete);

    if (modelTrie->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur SQL", modelTrie->lastError().text());
        return;
    }

    // Définir les bons en-têtes
    modelTrie->setHeaderData(0, Qt::Horizontal, "ID");
    modelTrie->setHeaderData(1, Qt::Horizontal, "Matricule");
    modelTrie->setHeaderData(2, Qt::Horizontal, "État");
    modelTrie->setHeaderData(3, Qt::Horizontal, "Type");
    modelTrie->setHeaderData(4, Qt::Horizontal, "Tarif");
    modelTrie->setHeaderData(5, Qt::Horizontal, "Description");
    modelTrie->setHeaderData(6, Qt::Horizontal, "Date");

    ui->tableViewConseil->setModel(modelTrie);
}



void MainWindow::on_statconseil_clicked()
{
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

    // Création du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des conseils par type");
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible();
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setLabel(QString("%1 (%2%)").arg(slice->label()).arg(QString::number(100.0 * slice->percentage(), 'f', 1)));
        QFont sliceFont = slice->labelFont();
        sliceFont.setPointSize(6);
        slice->setLabelFont(sliceFont);
    }

    chart->layout()->setContentsMargins(10, 10, 10, 10);

    QFont font = chart->legend()->font();
    font.setPointSize(7);
    chart->legend()->setFont(font);

    // Création de la vue du graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    //  Création d'une nouvelle fenêtre (QDialog)
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Statistiques des conseils");
    dialog->resize(700, 500);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(chartView);
    dialog->setLayout(layout);

    dialog->exec(); // Utiliser show() si tu veux que ce soit non modal
}



void MainWindow::on_historique_clicked()
{
    QSqlQuery query;
    if (!query.exec("SELECT idconseil, matricule, etat, description, datee FROM conseil WHERE type = 'traitee'")) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des conseils traités : " + query.lastError().text());
        return;
    }

    QString historique;

    while (query.next()) {
        QString matricule = query.value(1).toString();
        QString etat = query.value(2).toString();
        QString description = query.value(3).toString();
        QString datee = query.value(4).toString();

        historique += QString("🔹 Matricule: %1\nÉtat: %2\nDescription: %3\nDate: %4\n\n")
                          .arg(matricule, etat, description, datee);
    }

    if (historique.isEmpty()) {
        QMessageBox::information(this, "Historique", "Aucun conseil traité trouvé.");
        return;
    }

    // Affichage dans une fenêtre modale
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Historique des conseils traités");
    dialog->resize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QTextEdit *textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(historique);

    layout->addWidget(textEdit);
    dialog->setLayout(layout);
    dialog->exec();

    // Sauvegarde dans un fichier texte
    QFile file("historique_conseils.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << historique;
        file.close();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'écrire dans le fichier historique_conseils.txt");
    }

    // ❌ Suppression désactivée
    /*
    QSqlQuery deleteQuery;
    for (const QString &idconseil : idsASupprimer) {
        deleteQuery.prepare("DELETE FROM conseil WHERE idconseil = :idconseil");
        deleteQuery.bindValue(":idconseil", idconseil);
        if (!deleteQuery.exec()) {
            QMessageBox::warning(this, "Erreur de suppression", "Impossible de supprimer le conseil ID " + idconseil + " : " + deleteQuery.lastError().text());
        }
    }
    */

    QMessageBox::information(this, "Succès", "Historique généré avec succès. Les conseils restent enregistrés.");
}


#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QMessageBox>
#include <QRegularExpression>


void MainWindow::on_mailconseil_clicked()
{
    // 1. Demander le matricule à l'utilisateur
    bool ok;
    QString matriculeSaisi = QInputDialog::getText(this, "Entrer le matricule", "Matricule:", QLineEdit::Normal, "", &ok);

    if (!ok || matriculeSaisi.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Matricule invalide ou vide.");
        return;
    }

    // 2. Récupérer les conseils traités correspondant au matricule
    QSqlQuery query;
    query.prepare("SELECT idconseil, matricule, etat, description, datee FROM conseil WHERE type = 'traitee' AND matricule = :matricule");
    query.bindValue(":matricule", matriculeSaisi);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des conseils traités : " + query.lastError().text());
        return;
    }

    QString historique;

    while (query.next()) {
        QString matricule = query.value(1).toString();
        QString etat = query.value(2).toString();
        QString description = query.value(3).toString();
        QString datee = query.value(4).toString();

        historique += QString("🔹 Matricule: %1\nÉtat: %2\nDescription: %3\nDate: %4\n\n")
                          .arg(matricule, etat, description, datee);
    }

    if (historique.isEmpty()) {
        QMessageBox::information(this, "Historique", "Aucun conseil traité trouvé pour ce matricule.");
        return;
    }

    // 3. Sauvegarder l'historique dans un fichier texte
    QString fileName = QString("historique_%1.txt").arg(matriculeSaisi);
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << historique;
        file.close();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible d'écrire dans le fichier " + fileName);
        return;
    }

    // 4. Demander l'e-mail du destinataire
    QString email = QInputDialog::getText(this, "Entrer l'adresse email", "Email:", QLineEdit::Normal, "", &ok);
    if (!ok || !email.contains("@")) {
        QMessageBox::warning(this, "Erreur", "Adresse email invalide.");
        return;
    }

    // 5. Convertir le fichier en base64
    QFile fichierAjoute(fileName);
    if (!fichierAjoute.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier pour l'envoi.");
        return;
    }
    QByteArray fileData = fichierAjoute.readAll();
    fichierAjoute.close();
    QString base64File = QString::fromLatin1(fileData.toBase64());

    // 6. Envoi via l'API MailerSend avec pièce jointe
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("https://api.mailersend.com/v1/email");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer mlsn.c2515662441bcd9ec81b7037807f04976a04c6b696ec19a98998bbe046025993");

    QJsonObject fromObj;
    fromObj["email"] = "MS_iyAMjV@test-xkjn41mmz2q4z781.mlsender.net";
    fromObj["name"] = "Qt Application";

    QJsonObject toObj;
    toObj["email"] = email;
    QJsonArray toArray;
    toArray.append(toObj);

    QJsonObject attachment;
    attachment["content"] = base64File;
    attachment["type"] = "text/plain";
    attachment["filename"] = fileName;

    QJsonArray attachments;
    attachments.append(attachment);

    QJsonObject json;
    json["from"] = fromObj;
    json["to"] = toArray;
    json["subject"] = "Historique des conseils traités";
    json["text"] = "Veuillez trouver ci-joint l'historique des conseils traités pour le matricule : " + matriculeSaisi;
    json["attachments"] = attachments;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray response = reply->readAll();
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "Succès", "Email envoyé avec succès !");
            qDebug() << "Réponse MailerSend:" << response;
        } else {
            qDebug() << "Erreur MailerSend:" << reply->errorString() << response;
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'envoi : " + reply->errorString() + "\n" + QString::fromUtf8(response));
        }
        reply->deleteLater();
    });

    // 7. Supprimer le fichier après envoi
    QFile::remove(fileName);
}
