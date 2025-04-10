#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsTextItem>
#include <QPainter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTableWidget>
#include <QMap>
#include <QMessageBox>
#include "transaction.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "connection.h"
#include <QInputDialog>
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QSqlQueryModel>
#include <QRegularExpressionValidator>
#include <QFileDialog>
#include <QTextStream>
#include <QSqlTableModel>
#include <QDesktopServices>
#include <QUrl>
#include <QTemporaryDir>
#include <QDateTime>
#include <QBarSeries>
#include <QBarSet>
#include <QChartView>
#include <QChart>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QPieSeries>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    qDebug() << "\n=== Début de l'initialisation de MainWindow ===";
    
    // Établir la connexion à la base de données
    Connection c;
    if(!c.createConnection()) {
        qDebug() << "Erreur de connexion à la base de données";
        QMessageBox::critical(this, "Erreur", "La connexion à la base de données a échoué");
    } else {
        qDebug() << "Connexion à la base de données établie avec succès";
    }
    
    // Configuration des boutons de navigation
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
    
    // Configuration des validateurs et des combobox pour les transactions
    setupTransactionValidators();
    
    // Configuration du menu contextuel pour la table des transactions
    setupTableContextMenu();
    
    // Connecter les boutons de transaction
    connectTransactionButtons();
    
    // Connecter le bouton de statistiques
    connect(ui->pushButton_9_Transaction, &QPushButton::clicked, this, &MainWindow::on_pushButton_9_Transaction_clicked);
    
    // Rafraîchir la table des transactions
    refreshTransactionTable();
    
    qDebug() << "=== Fin de l'initialisation de MainWindow ===";
}

void MainWindow::setupTransactionValidators()
{
    qDebug() << "\n=== Configuration des validateurs de transaction ===";
    
    // Configuration du validateur pour le montant
    QDoubleValidator* montantValidator = new QDoubleValidator(0.0, 999999999.99, 2, this);
    montantValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit1Transaction->setValidator(montantValidator);
    
    // Chargement initial des clients et partenaires
    loadClientsAndPartenaires();
    
    // Connexion du signal de changement de type de transaction
    connect(ui->comboBox1Transaction, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onTransactionTypeChanged);
            
    // Configuration initiale de la visibilité
    onTransactionTypeChanged(ui->comboBox1Transaction->currentIndex());
}

void MainWindow::loadClientsAndPartenaires()
{
    qDebug() << "\n=== Chargement des clients et partenaires ===";
    
    // Vider les combobox
    ui->comboBox_8_tran_client->clear();
    ui->comboBox_7_tran_part->clear();
    
    // Ajouter l'option "Sélectionner..."
    ui->comboBox_8_tran_client->addItem("Sélectionner un client...", QVariant(0));
    ui->comboBox_7_tran_part->addItem("Sélectionner un partenaire...", QVariant(0));
    
    // Charger les clients
    QSqlQuery queryClients;
    if (queryClients.exec("SELECT IDCLIENT, NOM FROM SYRINE.CLIENT ORDER BY NOM")) {
        while (queryClients.next()) {
            int id = queryClients.value(0).toInt();
            QString nom = queryClients.value(1).toString();
            ui->comboBox_8_tran_client->addItem(nom, id);
        }
    } else {
        qDebug() << "Erreur lors du chargement des clients:" << queryClients.lastError().text();
    }
    
    // Charger les partenaires
    QSqlQuery queryPartenaires;
    if (queryPartenaires.exec("SELECT IDPARTENAIRE, NOMENTREPRISE FROM SYRINE.PARTENAIRE ORDER BY NOMENTREPRISE")) {
        while (queryPartenaires.next()) {
            int id = queryPartenaires.value(0).toInt();
            QString nom = queryPartenaires.value(1).toString();
            ui->comboBox_7_tran_part->addItem(nom, id);
        }
    } else {
        qDebug() << "Erreur lors du chargement des partenaires:" << queryPartenaires.lastError().text();
    }
}

void MainWindow::onTransactionTypeChanged(int index)
{
    qDebug() << "\n=== Changement du type de transaction ===";
    
    QString type = ui->comboBox1Transaction->currentText();
    qDebug() << "Type sélectionné:" << type;
    
    // Réinitialiser les sélections
    ui->comboBox_8_tran_client->setCurrentIndex(0);
    ui->comboBox_7_tran_part->setCurrentIndex(0);
    
    // Gérer la visibilité et l'activation des combobox en fonction du type
    if (type == "Vente") {
        ui->comboBox_8_tran_client->setEnabled(true);
        ui->comboBox_7_tran_part->setEnabled(false);
        ui->labelClient->setStyleSheet("color: red;"); // Indiquer que c'est obligatoire
        ui->labelPartenaire->setStyleSheet("");
    } else if (type == "Achat") {
        ui->comboBox_8_tran_client->setEnabled(false);
        ui->comboBox_7_tran_part->setEnabled(true);
        ui->labelClient->setStyleSheet("");
        ui->labelPartenaire->setStyleSheet("color: red;"); // Indiquer que c'est obligatoire
    } else {
        // Pour les autres types (ex: Remboursement)
        ui->comboBox_8_tran_client->setEnabled(true);
        ui->comboBox_7_tran_part->setEnabled(true);
        ui->labelClient->setStyleSheet("");
        ui->labelPartenaire->setStyleSheet("");
    }
}

bool MainWindow::validateTransaction()
{
    qDebug() << "\n=== Validation de la transaction ===";
    
    QString type = ui->comboBox1Transaction->currentText();
    int clientId = ui->comboBox_8_tran_client->currentData().toInt();
    int partenaireId = ui->comboBox_7_tran_part->currentData().toInt();
    
    // Vérifier que le montant n'est pas vide
    if (ui->lineEdit1Transaction->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le montant est obligatoire.");
        return false;
    }

    // Définir un validateur qui autorise uniquement les lettres alphabétiques et les espaces
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("^[A-Za-zÀ-ÖØ-öø-ÿ ]+$"), this);
    ui->lineEdit_3Transaction->setValidator(validator);

    // Vérifier que la date n'est pas dans le futur
    if (ui->dateEditTransaction->date() > QDate::currentDate()) {
        QMessageBox::warning(this, "Erreur", "La date ne peut pas être dans le futur.");
        return false;
    }
    
    // Validation spécifique selon le type de transaction
    if (type == "Vente" && clientId == 0) {
        QMessageBox::warning(this, "Erreur", "Vous devez sélectionner un client pour une vente.");
        return false;
    } else if (type == "Achat" && partenaireId == 0) {
        QMessageBox::warning(this, "Erreur", "Vous devez sélectionner un partenaire pour un achat.");
        return false;
    }
    
    // Validation du montant en fonction du type
    double montant = ui->lineEdit1Transaction->text().toDouble();
    if (type == "Vente" && montant <= 0) {
        QMessageBox::warning(this, "Erreur", "Le montant d'une vente doit être positif.");
        return false;
    } else if (type == "Achat" && montant <= 0) {
        QMessageBox::warning(this, "Erreur", "Le montant d'un achat doit être positif.");
        return false;
    }
    
    return true;
}

void MainWindow::on_pushButton_2Transaction_clicked()
{
    qDebug() << "\n=== Ajout d'une nouvelle transaction ===";
    
    // Validation des entrées
    if (!validateTransaction()) {
        return;
    }
    
    // Récupération des valeurs
    double montant = ui->lineEdit1Transaction->text().toDouble();
    QString modePaiement = ui->comboBox_modepaiment->currentText();
    QDate date = ui->dateEditTransaction->date();
    QString statut = ui->comboBox_2Transaction->currentText();
    QString type = ui->comboBox1Transaction->currentText();
    int clientId = ui->comboBox_8_tran_client->currentData().toInt();
    int partenaireId = ui->comboBox_7_tran_part->currentData().toInt();
    
    // Création de la transaction
    Transaction transaction(0, montant, modePaiement, date, statut, type, 0, clientId, partenaireId);
    
    // Ajout de la transaction
    if (transaction.ajouter()) {
        QMessageBox::information(this, "Succès", "Transaction ajoutée avec succès.");
        
        // Réinitialisation des champs
        ui->lineEdit1Transaction->clear();
        ui->dateEditTransaction->setDate(QDate::currentDate());
        ui->comboBox_2Transaction->setCurrentIndex(0);
        ui->comboBox1Transaction->setCurrentIndex(0);
        ui->comboBox_modepaiment->setCurrentIndex(0);
        ui->comboBox_8_tran_client->setCurrentIndex(0);
        ui->comboBox_7_tran_part->setCurrentIndex(0);
        
        // Rafraîchir la table
        refreshTransactionTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout: " + transaction.getLastError());
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

// Nouvelle méthode pour valider le montant de la transaction
bool MainWindow::validateTransactionAmount(double montant, const QString &type)
{
    if (type == "Vente" && montant <= 0) {
        QMessageBox::warning(this, "Erreur", "Le montant d'une vente doit être positif.");
        return false;
    } else if (type == "Remboursement" && montant >= 0) {
        QMessageBox::warning(this, "Erreur", "Le montant d'un remboursement doit être négatif.");
        return false;
    }
    return true;
}

void MainWindow::setupTableContextMenu()
{
    // Activer le menu contextuel
    ui->tableViewAFFICHELIST_3->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Connecter le signal pour afficher le menu
    connect(ui->tableViewAFFICHELIST_3, &QTableWidget::customContextMenuRequested,
            this, &MainWindow::showTransactionContextMenu);
            
    // Permettre la sélection de lignes
    ui->tableViewAFFICHELIST_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewAFFICHELIST_3->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // Activer le hover
    ui->tableViewAFFICHELIST_3->setMouseTracking(true);
    
    // Connecter le signal de double-clic
    connect(ui->tableViewAFFICHELIST_3, &QTableWidget::doubleClicked,
            this, &MainWindow::on_tableViewAFFICHELIST_3_doubleClicked);
}

void MainWindow::showTransactionContextMenu(const QPoint &pos)
{
    QTableWidgetItem *item = ui->tableViewAFFICHELIST_3->itemAt(pos);
    if (!item) return;
    
    // Récupérer la référence de la transaction sélectionnée
    int row = item->row();
    currentTransactionReference = ui->tableViewAFFICHELIST_3->item(row, 0)->text().toInt();
    
    qDebug() << "Transaction sélectionnée, Référence:" << currentTransactionReference;
    
    // Créer le menu contextuel
    QMenu contextMenu(this);
    
    // Ajouter les actions
    QAction *deleteAction = contextMenu.addAction("Supprimer");
    
    // Connecter les actions
    connect(deleteAction, &QAction::triggered, this, [this]() {
        deleteTransaction();
    });
    
    // Afficher le menu à la position du clic
    contextMenu.exec(ui->tableViewAFFICHELIST_3->mapToGlobal(pos));
}

void MainWindow::deleteTransaction()
{
    if (currentTransactionReference == -1) return;
    
    // Demander confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                "Voulez-vous vraiment supprimer la transaction avec la référence " + QString::number(currentTransactionReference) + " ?",
                                QMessageBox::Yes|QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        // Créer un objet Transaction pour la suppression
        Transaction transaction;
        
        if (transaction.supprimer(currentTransactionReference)) {
            QMessageBox::information(this, "Succès", "Transaction supprimée avec succès");
            refreshTransactionTable();
        } else {
            QMessageBox::critical(this, "Erreur",
                                "Erreur lors de la suppression: " + transaction.getLastError());
        }
    }
    
    currentTransactionReference = -1;
}

void MainWindow::refreshTransactionTable()
{
    qDebug() << "\n=== Rafraîchissement de la table des transactions ===";
    
    // Vider la table actuelle
    ui->tableViewAFFICHELIST_3->clearContents();
    ui->tableViewAFFICHELIST_3->setRowCount(0);
    
    // Configuration des en-têtes
    ui->tableViewAFFICHELIST_3->setColumnCount(7);
    QStringList headers;
    headers << "Référence" << "Montant" << "Mode Paiement" << "Date" << "Statut" << "Type" << "Payeur";
    ui->tableViewAFFICHELIST_3->setHorizontalHeaderLabels(headers);
    
    // Préparer la requête SQL
    QSqlQuery query;
    QString queryStr = "SELECT t.REFERENCE, t.MONTANT, t.MODEPAIEMENT, t.DATEEE, t.STATUTT, t.TYPEE, "
                      "CASE "
                      "WHEN t.IDCLIENT > 0 THEN c.NOM "
                      "WHEN t.IDPARTENAIRE > 0 THEN p.NOMENTREPRISE "
                      "ELSE 'Aucun' "
                      "END AS PAYEUR "
                      "FROM SYRINE.TRANSACTION t "
                      "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                      "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                      "ORDER BY t.DATEEE DESC";
    
    if (query.exec(queryStr)) {
        int rowCount = 0;
        
        // Remplir la table avec les résultats
        while (query.next()) {
            ui->tableViewAFFICHELIST_3->insertRow(rowCount);
            
            // Ajouter les données dans chaque colonne
            for (int col = 0; col < 7; ++col) {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                ui->tableViewAFFICHELIST_3->setItem(rowCount, col, item);
            }
            
            rowCount++;
        }
        
        // Ajuster les colonnes
        ui->tableViewAFFICHELIST_3->resizeColumnsToContents();
        ui->tableViewAFFICHELIST_3->resizeRowsToContents();
        
        qDebug() << "Table mise à jour avec" << rowCount << "lignes";
    } else {
        qDebug() << "Erreur lors de la récupération des transactions:" << query.lastError().text();
    }
}

// Implémentation de la méthode de recherche
void MainWindow::on_pushButton_3Transaction_clicked()
{
    qDebug() << "\n=== Début de la recherche de transactions ===";
    qDebug() << "Bouton de recherche cliqué!";
    
    // Récupérer le texte de recherche
    QString searchText = ui->lineEdit_5Transaction->text().trimmed();
    qDebug() << "Texte de recherche:" << searchText;
    
    if (searchText.isEmpty()) {
        QMessageBox::information(this, "Information", "Veuillez entrer un terme de recherche.");
        return;
    }
    
    // Effectuer la recherche
    searchTransactions(searchText);
}

// Méthode pour rechercher des transactions selon différents critères
void MainWindow::searchTransactions(const QString &searchText)
{
    qDebug() << "Recherche avec le terme:" << searchText;
    
    // Vider la table actuelle
    ui->tableViewAFFICHELIST_3->clearContents();
    ui->tableViewAFFICHELIST_3->setRowCount(0);
    
    // Préparer la requête SQL
    QSqlQuery query;
    QString queryStr = "SELECT t.REFERENCE, t.MONTANT, t.MODEPAIEMENT, t.DATEEE, t.STATUTT, t.TYPEE, "
                      "CASE "
                      "WHEN t.IDCLIENT > 0 THEN c.NOM "
                      "WHEN t.IDPARTENAIRE > 0 THEN p.NOMENTREPRISE "
                      "ELSE 'Aucun' "
                      "END AS PAYEUR "
                      "FROM SYRINE.TRANSACTION t "
                      "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                      "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                      "WHERE CAST(t.REFERENCE AS VARCHAR2(20)) LIKE :reference OR "
                      "UPPER(c.NOM) LIKE UPPER(:nom) OR "
                      "UPPER(p.NOMENTREPRISE) LIKE UPPER(:entreprise) OR "
                      "UPPER(t.TYPEE) LIKE UPPER(:type)";
    
    query.prepare(queryStr);
    
    // Recherche partielle pour tous les champs
    query.bindValue(":reference", "%" + searchText + "%");
    query.bindValue(":nom", "%" + searchText + "%");
    query.bindValue(":entreprise", "%" + searchText + "%");
    query.bindValue(":type", "%" + searchText + "%");
    
    if (query.exec()) {
        int rowCount = 0;
        
        // Configuration des en-têtes
        ui->tableViewAFFICHELIST_3->setColumnCount(7);
        QStringList headers;
        headers << "Référence" << "Montant" << "Mode Paiement" << "Date" << "Statut" << "Type" << "Payeur";
        ui->tableViewAFFICHELIST_3->setHorizontalHeaderLabels(headers);
        
        // Remplir la table avec les résultats
        while (query.next()) {
            ui->tableViewAFFICHELIST_3->insertRow(rowCount);
            
            // Récupérer les valeurs
            QString reference = query.value(0).toString();
            double montant = query.value(1).toDouble();
            QString modePaiement = query.value(2).toString();
            QDate date = query.value(3).toDate();
            QString statut = query.value(4).toString();
            QString type = query.value(5).toString();
            QString payeur = query.value(6).toString();
            
            // Créer les items pour chaque colonne
            QTableWidgetItem *refItem = new QTableWidgetItem(reference);
            QTableWidgetItem *montantItem = new QTableWidgetItem(QString::number(montant, 'f', 2));
            QTableWidgetItem *modePaiementItem = new QTableWidgetItem(modePaiement);
            QTableWidgetItem *dateItem = new QTableWidgetItem(date.toString("dd/MM/yyyy"));
            QTableWidgetItem *statutItem = new QTableWidgetItem(statut);
            QTableWidgetItem *typeItem = new QTableWidgetItem(type);
            QTableWidgetItem *payeurItem = new QTableWidgetItem(payeur);
            
            // Ajouter les items à la table
            ui->tableViewAFFICHELIST_3->setItem(rowCount, 0, refItem);
            ui->tableViewAFFICHELIST_3->setItem(rowCount, 1, montantItem);
            ui->tableViewAFFICHELIST_3->setItem(rowCount, 2, modePaiementItem);
            ui->tableViewAFFICHELIST_3->setItem(rowCount, 3, dateItem);
            ui->tableViewAFFICHELIST_3->setItem(rowCount, 4, statutItem);
            ui->tableViewAFFICHELIST_3->setItem(rowCount, 5, typeItem);
            ui->tableViewAFFICHELIST_3->setItem(rowCount, 6, payeurItem);
            
            rowCount++;
        }
        
        if (rowCount == 0) {
            QMessageBox::information(this, "Information", "Aucune transaction trouvée.");
        }
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de la recherche.");
    }
}

// Implémentation de la méthode de tri
void MainWindow::on_pushButton_4Transaction_clicked()
{
    qDebug() << "\n=== Début du tri des transactions ===";
    qDebug() << "Bouton de tri cliqué!";
    
    // Récupérer le critère de tri
    QString criterion = ui->comboBox_3Transaction->currentText();
    qDebug() << "Critère de tri:" << criterion;
    
    // Effectuer le tri
    sortTransactions(criterion);
}

// Méthode pour gérer le changement de critère de tri
void MainWindow::on_comboBox_3Transaction_currentIndexChanged(int index)
{
    qDebug() << "Critère de tri changé à l'index:" << index;
    qDebug() << "Nouveau critère:" << ui->comboBox_3Transaction->currentText();
}

// Méthode pour trier les transactions selon différents critères
void MainWindow::sortTransactions(const QString &criterion)
{
    qDebug() << "Tri selon le critère:" << criterion;
    
    // Déterminer la colonne SQL à utiliser pour le tri
    QString orderByColumn;
    
    if (criterion == "Date") {
        orderByColumn = "t.DATEEE";
    } else if (criterion == "Montant") {
        orderByColumn = "t.MONTANT";
    } else if (criterion == "Type") {
        orderByColumn = "t.TYPEE";
    } else if (criterion == "Statut") {
        orderByColumn = "t.STATUTT";
    } else {
        // Par défaut, trier par date
        orderByColumn = "t.DATEEE";
    }
    
    // Vider la table actuelle
    ui->tableViewAFFICHELIST_3->clearContents();
    ui->tableViewAFFICHELIST_3->setRowCount(0);
    
    // Préparer la requête SQL avec tri
    QSqlQuery query;
    QString queryStr = "SELECT t.REFERENCE, t.MONTANT, t.MODEPAIEMENT, t.DATEEE, t.STATUTT, t.TYPEE, "
                      "CASE "
                      "WHEN t.IDCLIENT > 0 THEN c.NOM "
                      "WHEN t.IDPARTENAIRE > 0 THEN p.NOMENTREPRISE "
                      "ELSE 'Aucun' "
                      "END AS PAYEUR "
                      "FROM SYRINE.TRANSACTION t "
                      "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                      "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                      "ORDER BY " + orderByColumn;
    
    if (query.exec(queryStr)) {
        int rowCount = 0;
        
        // Configuration des en-têtes
        ui->tableViewAFFICHELIST_3->setColumnCount(7);
        QStringList headers;
        headers << "Référence" << "Montant" << "Mode Paiement" << "Date" << "Statut" << "Type" << "Payeur";
        ui->tableViewAFFICHELIST_3->setHorizontalHeaderLabels(headers);
        
        // Remplir la table avec les résultats
        while (query.next()) {
            ui->tableViewAFFICHELIST_3->insertRow(rowCount);
            
            // Ajouter les données dans chaque colonne
            for (int col = 0; col < 7; ++col) {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                ui->tableViewAFFICHELIST_3->setItem(rowCount, col, item);
            }
            
            rowCount++;
        }
        
        // Ajuster les colonnes
        ui->tableViewAFFICHELIST_3->resizeColumnsToContents();
        ui->tableViewAFFICHELIST_3->resizeRowsToContents();
        
        QMessageBox::information(this, "Tri", QString("Les transactions ont été triées par %1.").arg(criterion.toLower()));
    } else {
        qDebug() << "Erreur lors du tri:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors du tri: " + query.lastError().text());
    }
}

// Nouvelle méthode pour connecter les boutons de transaction
void MainWindow::connectTransactionButtons()
{
    qDebug() << "\n=== Connexion des boutons de transaction ===";
    
    // Boutons d'ajout et de modification
    connect(ui->pushButton_2Transaction, &QPushButton::clicked, this, &MainWindow::on_pushButton_2Transaction_clicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::on_pushButton_5_clicked);
    connect(ui->pushButton_9_Transaction, &QPushButton::clicked, this, &MainWindow::on_pushButton_9_Transaction_clicked);
    // Bouton pour la suppression
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::on_pushButton_8_clicked);
    
    // Boutons de recherche
    connect(ui->pushButton_1Transaction, &QPushButton::clicked, this, &MainWindow::on_pushButtonRechercherTransaction_clicked);
    
    // Boutons de recherche et tri existants
    connect(ui->pushButton_3Transaction, &QPushButton::clicked, this, &MainWindow::on_pushButton_3Transaction_clicked);
    connect(ui->pushButton_4Transaction, &QPushButton::clicked, this, &MainWindow::on_pushButton_4Transaction_clicked);
    
    // Connecter le lineEdit_5Transaction pour détecter les changements de texte
    connect(ui->lineEdit_5Transaction, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (text.isEmpty()) {
            refreshTransactionTable();
        }
    });
    
    qDebug() << "Connexion des boutons terminée";
}

void MainWindow::on_pushButton_5_clicked()
{
    // Vérifier si la table est vide
    if (ui->tableViewAFFICHELIST_3->model()->rowCount() == 0) {
        QMessageBox::warning(this, "Export impossible", "Aucune donnée à exporter.");
        return;
    }

    // Obtenir le modèle de la table
    QAbstractItemModel *model = ui->tableViewAFFICHELIST_3->model();

    // Chemin du fichier fixe
    QString fileName = QDir::homePath() + "/transactions_export.csv";

    // Vérifier si le fichier existe déjà
    bool fileExists = QFile::exists(fileName);

    // Créer ou mettre à jour le fichier CSV
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur d'export",
            "Impossible d'accéder au fichier CSV.");
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // Section 1: Transactions détaillées
    out << "=== Transactions détaillées ===\n\n";
    
    // Écrire les en-têtes
    QStringList headers;
    for (int col = 0; col < model->columnCount(); ++col) {
        headers << model->headerData(col, Qt::Horizontal).toString();
    }
    out << headers.join(",") << "\n";

    // Écrire les données
    for (int row = 0; row < model->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            QString data = model->data(index).toString();
            
            // Formater les nombres
            bool ok;
            double num = data.toDouble(&ok);
            if (ok) {
                data = QString::number(num, 'f', 2);
            }
            
            // Formater les dates
            QDate date = QDate::fromString(data, "yyyy-MM-dd");
            if (date.isValid()) {
                data = date.toString("dd/MM/yyyy");
            }
            
            // Échapper les virgules et guillemets dans les données
            if (data.contains(",") || data.contains("\"") || data.contains("\n")) {
                data = "\"" + data.replace("\"", "\"\"") + "\"";
            }
            
            rowData << data;
        }
        out << rowData.join(",") << "\n";
    }

    // Section 2: Statistiques mensuelles
    out << "\n\n=== Statistiques mensuelles ===\n\n";
    out << "Mois,Ventes,Achats\n";

    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(DATEEE, 'YYYY-MM') as month, "
                  "SUM(CASE WHEN TYPEE = 'Vente' THEN MONTANT ELSE 0 END) as ventes, "
                  "SUM(CASE WHEN TYPEE = 'Achat' THEN MONTANT ELSE 0 END) as achats "
                  "FROM SYRINE.TRANSACTION "
                  "GROUP BY TO_CHAR(DATEEE, 'YYYY-MM') "
                  "ORDER BY month");

    if (query.exec()) {
        while (query.next()) {
            QStringList rowData;
            rowData << query.value("month").toString();
            rowData << QString::number(query.value("ventes").toDouble(), 'f', 2);
            rowData << QString::number(query.value("achats").toDouble(), 'f', 2);
            out << rowData.join(",") << "\n";
        }
    }

    // Section 3: Distribution des types
    out << "\n\n=== Distribution des types ===\n\n";
    out << "Type,Nombre\n";

    query.prepare("SELECT TYPEE, COUNT(*) as count "
                  "FROM SYRINE.TRANSACTION "
                  "GROUP BY TYPEE");

    if (query.exec()) {
        while (query.next()) {
            QStringList rowData;
            rowData << query.value("TYPEE").toString();
            rowData << query.value("count").toString();
            out << rowData.join(",") << "\n";
        }
    }

    file.close();

    // Afficher le message uniquement lors de la première création du fichier
    if (!fileExists) {
        QMessageBox::information(this, "Export réussi", "Données exportées vers " + fileName);
    }
    
    // Ouvrir le fichier avec l'application par défaut
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void MainWindow::on_pushButtonRechercherTransaction_clicked()
{
    qDebug() << "\n=== Recherche d'une transaction par référence ===";
    
    // Demander la référence à l'utilisateur via une boîte de dialogue
    bool ok;
    int reference = QInputDialog::getInt(this, "Rechercher une transaction", 
                               "Entrez la référence de la transaction à rechercher:",
                               1001, 1001, 999999, 1, &ok);
    if (!ok) {
        return; // L'utilisateur a annulé
    }
    
    // Rechercher la transaction par référence
    QSqlQuery query;
    query.prepare("SELECT * FROM SYRINE.TRANSACTION WHERE REFERENCE = :reference");
    query.bindValue(":reference", reference);
    
    if (query.exec() && query.next()) {
        // Transaction trouvée, mettre à jour les champs
        currentTransactionReference = query.value("REFERENCE").toInt();
        
        // Remplir les champs avec les données actuelles
        ui->lineEdit1Transaction->setText(query.value("MONTANT").toString());
        ui->dateEditTransaction->setDate(query.value("DATEEE").toDate());
        ui->comboBox_2Transaction->setCurrentText(query.value("STATUTT").toString());
        ui->comboBox1Transaction->setCurrentText(query.value("TYPEE").toString());
        ui->comboBox_modepaiment->setCurrentText(query.value("MODEPAIEMENT").toString());
        
        // Sélectionner le client ou le partenaire
        int clientId = query.value("IDCLIENT").toInt();
        int partenaireId = query.value("IDPARTENAIRE").toInt();
        
        if (clientId > 0) {
            int index = ui->comboBox_8_tran_client->findData(clientId);
            if (index != -1) ui->comboBox_8_tran_client->setCurrentIndex(index);
        } else {
            ui->comboBox_8_tran_client->setCurrentIndex(0);
        }
        
        if (partenaireId > 0) {
            int index = ui->comboBox_7_tran_part->findData(partenaireId);
            if (index != -1) ui->comboBox_7_tran_part->setCurrentIndex(index);
        } else {
            ui->comboBox_7_tran_part->setCurrentIndex(0);
        }
        
        QMessageBox::information(this, "Succès", "Transaction trouvée. Vous pouvez maintenant la modifier.");
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune transaction trouvée avec cette référence.");
        // Réinitialiser les champs
        ui->lineEdit1Transaction->clear();
        ui->dateEditTransaction->setDate(QDate::currentDate());
        ui->comboBox_2Transaction->setCurrentIndex(0);
        ui->comboBox1Transaction->setCurrentIndex(0);
        ui->comboBox_modepaiment->setCurrentIndex(0);
        ui->comboBox_8_tran_client->setCurrentIndex(0);
        ui->comboBox_7_tran_part->setCurrentIndex(0);
        currentTransactionReference = -1;
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    qDebug() << "\n=== Suppression d'une transaction par référence ===";
    
    // Demander la référence à l'utilisateur via une boîte de dialogue
    bool ok;
    int reference = QInputDialog::getInt(this, "Supprimer une transaction", 
                                "Entrez la référence de la transaction à supprimer:",
                                1001, 1001, 999999, 1, &ok);
    if (!ok) {
        return; // L'utilisateur a annulé
    }
    
    // Vérifier que la transaction existe
    Transaction transaction;
    if (!transaction.verifierExistence(reference)) {
        QMessageBox::warning(this, "Erreur", "Aucune transaction trouvée avec cette référence.");
        return;
    }
    
    // Transaction trouvée, demander confirmation
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                           "Êtes-vous sûr de vouloir supprimer la transaction avec la référence " + QString::number(reference) + " ?",
                                                           QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::No) {
        return;
    }
    
    // Exécuter la suppression
    if (transaction.supprimer(reference)) {
        QMessageBox::information(this, "Succès", "Transaction supprimée avec succès.");
        
        // Rafraîchir la table des transactions
        refreshTransactionTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression: " + transaction.getLastError());
    }
}

void MainWindow::on_pushButton_8_Transaction_clicked()
{
    qDebug() << "\n=== Début de la modification d'une transaction depuis la liste ===";
    
    // Si aucune transaction n'est sélectionnée, on ne fait rien
    if (currentTransactionReference == -1) {
        QMessageBox::warning(this, "Attention", "Veuillez d'abord sélectionner une transaction à modifier");
        return;
    }
    
    // Validation des entrées
    if (ui->lineEdit1Transaction->text().isEmpty()) {
        qDebug() << "Erreur: Montant vide";
        QMessageBox::warning(this, "Erreur", "Le montant est obligatoire");
        return;
    }

    if (ui->dateEditTransaction->date() > QDate::currentDate()) {
        qDebug() << "Erreur: Date future";
        QMessageBox::warning(this, "Erreur", "La date ne peut pas être future");
        return;
    }

    // Récupération des IDs client et partenaire
    int clientId = ui->comboBox_8_tran_client->currentData().toInt();
    int partenaireId = ui->comboBox_7_tran_part->currentData().toInt();
    
    // Configuration des valeurs
    double montant = ui->lineEdit1Transaction->text().toDouble();
    QDate date = ui->dateEditTransaction->date();
    QString statut = ui->comboBox_2Transaction->currentText();
    QString type = ui->comboBox1Transaction->currentText();
    
    // Validation du montant en fonction du type de transaction
    if (!validateTransactionAmount(montant, type)) {
        return;
    }
    
    QString modePaiement = ui->comboBox_modepaiment->currentText();
    
    // Vérification du payeur (client ou partenaire)
    // Pour les transactions de type "Vente" ou "Remboursement", un payeur est obligatoire
    if ((type == "Vente" || type == "Remboursement") && clientId == 0 && partenaireId == 0) {
        qDebug() << "Erreur: Aucun payeur sélectionné pour une " << type;
        QMessageBox::warning(this, "Erreur", "Vous devez sélectionner un client ou un partenaire pour une transaction de type '" + type + "'");
        return;
    }
    
    qDebug() << "Données de la transaction à modifier:";
    qDebug() << "- Référence:" << currentTransactionReference;
    qDebug() << "- Montant:" << montant;
    qDebug() << "- Date:" << date.toString("yyyy-MM-dd");
    qDebug() << "- Statut:" << statut;
    qDebug() << "- Type:" << type;
    qDebug() << "- Mode de paiement:" << modePaiement;
    qDebug() << "- Client ID:" << clientId;
    qDebug() << "- Partenaire ID:" << partenaireId;
    
    // Créer un objet Transaction avec les nouvelles valeurs
    Transaction transaction(currentTransactionReference, montant, modePaiement, date, statut, type, 0, clientId, partenaireId);
    
    // Modifier la transaction
    if (transaction.modifier()) {
        QMessageBox::information(this, "Succès", "Transaction modifiée avec succès");
        
        // Nettoyage des champs
        ui->lineEdit1Transaction->clear();
        ui->dateEditTransaction->setDate(QDate::currentDate());
        ui->comboBox_2Transaction->setCurrentIndex(0);
        ui->comboBox1Transaction->setCurrentIndex(0);
        ui->comboBox_modepaiment->setCurrentIndex(0);
        ui->comboBox_8_tran_client->setCurrentIndex(0);
        ui->comboBox_7_tran_part->setCurrentIndex(0);
        
        // Réinitialiser la référence
        currentTransactionReference = -1;
        
        // Rafraîchir la table des transactions
        refreshTransactionTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification: " + transaction.getLastError());
    }
}

void MainWindow::on_pushButtonRechercherParReference_clicked()
{
    qDebug() << "\n=== Recherche par référence ===";
    
    // Demander la référence à l'utilisateur via une boîte de dialogue
    bool ok;
    int reference = QInputDialog::getInt(this, "Rechercher une transaction", 
                               "Entrez la référence de la transaction à rechercher:",
                               1001, 1001, 999999, 1, &ok);
    if (!ok) {
        return; // L'utilisateur a annulé
    }
    
    // Rechercher la transaction par référence
    Transaction transaction = Transaction::rechercher(reference);
    
    if (transaction.getReference() > 0) {
        // Transaction trouvée, mettre à jour les champs
        currentTransactionReference = transaction.getReference();
        
        // Remplir les champs avec les données actuelles
        ui->lineEdit1Transaction->setText(QString::number(transaction.getMontant()));
        ui->dateEditTransaction->setDate(transaction.getDateee());
        ui->comboBox_2Transaction->setCurrentText(transaction.getStatutt());
        ui->comboBox1Transaction->setCurrentText(transaction.getTypee());
        ui->comboBox_modepaiment->setCurrentText(transaction.getModePaiement());
        
        // Sélectionner le client ou le partenaire
        int clientId = transaction.getIdClient();
        int partenaireId = transaction.getIdPartenaire();
        
        if (clientId > 0) {
            int index = ui->comboBox_8_tran_client->findData(clientId);
            if (index != -1) ui->comboBox_8_tran_client->setCurrentIndex(index);
        } else {
            ui->comboBox_8_tran_client->setCurrentIndex(0);
        }
        
        if (partenaireId > 0) {
            int index = ui->comboBox_7_tran_part->findData(partenaireId);
            if (index != -1) ui->comboBox_7_tran_part->setCurrentIndex(index);
        } else {
            ui->comboBox_7_tran_part->setCurrentIndex(0);
        }
        
        // Afficher un message d'information
        QMessageBox::information(this, "Succès", "Transaction trouvée. Vous pouvez maintenant la modifier.");
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune transaction trouvée avec cette référence.");
    }
}

void MainWindow::on_pushButtonRechercherAvance_clicked()
{
    qDebug() << "\n=== Recherche avancée ===";
    
    // Créer une boîte de dialogue pour la recherche avancée
    QDialog dialog(this);
    dialog.setWindowTitle("Recherche avancée de transactions");
    
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    
    // Champs de recherche
    QFormLayout* formLayout = new QFormLayout();
    
    QSpinBox* referenceSpinBox = new QSpinBox(&dialog);
    referenceSpinBox->setMinimum(0);
    referenceSpinBox->setMaximum(999999);
    referenceSpinBox->setSpecialValueText("Toutes");
    formLayout->addRow("Référence:", referenceSpinBox);
    
    QComboBox* statutCombo = new QComboBox(&dialog);
    statutCombo->addItem("Tous");
    statutCombo->addItems({"En attente", "Validé", "Annulé"});
    formLayout->addRow("Statut:", statutCombo);
    
    QComboBox* typeCombo = new QComboBox(&dialog);
    typeCombo->addItem("Tous");
    typeCombo->addItems({"Vente", "Achat", "Remboursement"});
    formLayout->addRow("Type:", typeCombo);
    
    QLineEdit* payeurEdit = new QLineEdit(&dialog);
    formLayout->addRow("Payeur:", payeurEdit);
    
    QDateEdit* dateDebutEdit = new QDateEdit(&dialog);
    dateDebutEdit->setCalendarPopup(true);
    dateDebutEdit->setDate(QDate::currentDate().addMonths(-1));
    formLayout->addRow("Date début:", dateDebutEdit);
    
    QDateEdit* dateFinEdit = new QDateEdit(&dialog);
    dateFinEdit->setCalendarPopup(true);
    dateFinEdit->setDate(QDate::currentDate());
    formLayout->addRow("Date fin:", dateFinEdit);
    
    layout->addLayout(formLayout);
    
    // Boutons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* cancelButton = new QPushButton("Annuler", &dialog);
    QPushButton* searchButton = new QPushButton("Rechercher", &dialog);
    searchButton->setDefault(true);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(searchButton);
    
    layout->addLayout(buttonLayout);
    
    // Connecter les boutons
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(searchButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    
    // Afficher la boîte de dialogue
    if (dialog.exec() == QDialog::Accepted) {
        // Récupérer les valeurs des champs
        int reference = referenceSpinBox->value();
        QString statut = statutCombo->currentIndex() > 0 ? statutCombo->currentText() : "";
        QString type = typeCombo->currentIndex() > 0 ? typeCombo->currentText() : "";
        QString payeur = payeurEdit->text().trimmed();
        QDate dateDebut = dateDebutEdit->date();
        QDate dateFin = dateFinEdit->date();
        
        // Exécuter la recherche avancée
        QSqlQueryModel* model = Transaction::rechercherAvance(reference, statut, type, payeur, dateDebut, dateFin);
        
        if (!model) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche avancée");
            return;
        }
        
        // Afficher les résultats
        // Vider la table actuelle
        ui->tableViewAFFICHELIST_3->clearContents();
        ui->tableViewAFFICHELIST_3->setRowCount(0);
        
        // Configuration des en-têtes
        ui->tableViewAFFICHELIST_3->setColumnCount(7);
        QStringList headers;
        headers << "Référence" << "Montant" << "Mode Paiement" << "Date" << "Statut" << "Type" << "Payeur";
        ui->tableViewAFFICHELIST_3->setHorizontalHeaderLabels(headers);
        
        // Remplir la table avec les résultats
        int rowCount = 0;
        for(int row = 0; row < model->rowCount(); ++row) {
            ui->tableViewAFFICHELIST_3->insertRow(rowCount);
            
            // Ajouter les données dans chaque colonne
            for (int col = 0; col < 7; ++col) {
                QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
                ui->tableViewAFFICHELIST_3->setItem(rowCount, col, item);
            }
            
            rowCount++;
        }
        
        // Ajuster les colonnes
        ui->tableViewAFFICHELIST_3->resizeColumnsToContents();
        ui->tableViewAFFICHELIST_3->resizeRowsToContents();
        
        // Nettoyage
        delete model;
        
        if (rowCount == 0) {
            QMessageBox::information(this, "Résultat", "Aucune transaction trouvée avec ces critères de recherche.");
            // Rafraîchir la table pour afficher toutes les transactions
            refreshTransactionTable();
        } else {
            QMessageBox::information(this, "Résultat", QString("%1 transaction(s) trouvée(s).").arg(rowCount));
        }
    }
}

void MainWindow::on_pushButtonSupprimer_clicked()
{
    qDebug() << "\n=== Suppression de transaction ===";
    
    // Demander la référence à l'utilisateur via une boîte de dialogue
    bool ok;
    int reference = QInputDialog::getInt(this, "Supprimer une transaction", 
                                "Entrez la référence de la transaction à supprimer:",
                                1001, 1001, 999999, 1, &ok);
    if (!ok) {
        return; // L'utilisateur a annulé
    }
    
    // Vérifier que la transaction existe
    Transaction transaction;
    if (!transaction.verifierExistence(reference)) {
        QMessageBox::warning(this, "Erreur", "Aucune transaction trouvée avec cette référence.");
        return;
    }
    
    // Transaction trouvée, demander confirmation
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                           "Êtes-vous sûr de vouloir supprimer la transaction avec la référence " + QString::number(reference) + " ?",
                                                           QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::No) {
        return;
    }
    
    // Exécuter la suppression
    if (transaction.supprimer(reference)) {
        QMessageBox::information(this, "Succès", "Transaction supprimée avec succès.");
        
        // Rafraîchir la table des transactions
        refreshTransactionTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression: " + transaction.getLastError());
    }
}

void MainWindow::on_pushButtonAjouter_clicked()
{
    qDebug() << "\n=== Ajout d'une transaction ===";
    
    // Appeler la méthode existante pour ajouter une transaction
    on_pushButton_2Transaction_clicked();
}

void MainWindow::on_pushButtonModifier_clicked()
{
    qDebug() << "\n=== Modification d'une transaction ===";
    
    // Vérifier si une transaction est sélectionnée
    if (currentTransactionReference <= 0) {
        QMessageBox::warning(this, "Erreur", "Aucune transaction sélectionnée. Veuillez d'abord rechercher ou sélectionner une transaction.");
        return;
    }
    
    // Appeler la méthode existante pour modifier une transaction
    on_pushButton_8_Transaction_clicked();
}

void MainWindow::on_pushButtonReinitialiser_clicked()
{
    qDebug() << "\n=== Réinitialisation du formulaire ===";
    
    // Réinitialiser les champs du formulaire
    ui->lineEdit1Transaction->clear();
    ui->dateEditTransaction->setDate(QDate::currentDate());
    ui->comboBox_2Transaction->setCurrentIndex(0);
    ui->comboBox1Transaction->setCurrentIndex(0);
    ui->comboBox_modepaiment->setCurrentIndex(0);
    ui->comboBox_8_tran_client->setCurrentIndex(0);
    ui->comboBox_7_tran_part->setCurrentIndex(0);
    
    // Réinitialiser la référence courante
    currentTransactionReference = -1;
}

void MainWindow::on_pushButtonTrier_clicked()
{
    qDebug() << "\n=== Tri des transactions ===";
    
    // Récupérer le critère de tri
    QString criterion = ui->comboBox_3Transaction->currentText();
    
    // Trier les transactions
    sortTransactions(criterion);
}

void MainWindow::on_tableViewAFFICHELIST_3_doubleClicked(const QModelIndex &index)
{
    qDebug() << "\n=== Double-clic sur une transaction ===";
    
    // Récupérer la référence de la transaction sélectionnée
    int row = index.row();
    currentTransactionReference = ui->tableViewAFFICHELIST_3->item(row, 0)->text().toInt();
    
    qDebug() << "Transaction sélectionnée, Référence:" << currentTransactionReference;
    
    // Créer une boîte de dialogue pour la modification
    QDialog dialog(this);
    dialog.setWindowTitle("Modifier la transaction");
    dialog.setMinimumWidth(400);
    
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    
    // Formulaire de modification
    QFormLayout* formLayout = new QFormLayout();
    
    // Montant
    QLineEdit* montantEdit = new QLineEdit(&dialog);
    QDoubleValidator* montantValidator = new QDoubleValidator(0.0, 999999999.99, 2, &dialog);
    montantValidator->setNotation(QDoubleValidator::StandardNotation);
    montantEdit->setValidator(montantValidator);
    formLayout->addRow("Montant:", montantEdit);
    
    // Date
    QDateEdit* dateEdit = new QDateEdit(&dialog);
    dateEdit->setCalendarPopup(true);
    dateEdit->setMaximumDate(QDate::currentDate());
    formLayout->addRow("Date:", dateEdit);
    
    // Statut
    QComboBox* statutCombo = new QComboBox(&dialog);
    statutCombo->addItems({"En attente", "Validé", "Annulé"});
    formLayout->addRow("Statut:", statutCombo);
    
    // Type
    QComboBox* typeCombo = new QComboBox(&dialog);
    typeCombo->addItems({"Vente", "Achat", });
    formLayout->addRow("Type:", typeCombo);
    
    // Mode de paiement
    QComboBox* modeCombo = new QComboBox(&dialog);
    modeCombo->addItems({"Especes", "Cheque", "Virement"});
    formLayout->addRow("Mode de paiement:", modeCombo);
    
    // Client
    QComboBox* clientCombo = new QComboBox(&dialog);
    clientCombo->addItem("Aucun client", QVariant(0));
    
    // Partenaire
    QComboBox* partenaireCombo = new QComboBox(&dialog);
    partenaireCombo->addItem("Aucun partenaire", QVariant(0));
    
    // Charger les clients
    QSqlQuery queryClients;
    if (queryClients.exec("SELECT IDCLIENT, NOM FROM SYRINE.CLIENT ORDER BY NOM")) {
        while (queryClients.next()) {
            int id = queryClients.value(0).toInt();
            QString nom = queryClients.value(1).toString();
            clientCombo->addItem(nom, id);
        }
    }
    
    // Charger les partenaires
    QSqlQuery queryPartenaires;
    if (queryPartenaires.exec("SELECT IDPARTENAIRE, NOMENTREPRISE FROM SYRINE.PARTENAIRE ORDER BY NOMENTREPRISE")) {
        while (queryPartenaires.next()) {
            int id = queryPartenaires.value(0).toInt();
            QString nom = queryPartenaires.value(1).toString();
            partenaireCombo->addItem(nom, id);
        }
    }
    
    formLayout->addRow("Client:", clientCombo);
    formLayout->addRow("Partenaire:", partenaireCombo);
    
    // Connecter les signaux pour la sélection mutuelle exclusive
    connect(clientCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        &dialog, [partenaireCombo](int index) {
            if (index > 0) {
                partenaireCombo->setCurrentIndex(0);
            }
        });

    connect(partenaireCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        &dialog, [clientCombo](int index) {
            if (index > 0) {
                clientCombo->setCurrentIndex(0);
            }
        });
    
    layout->addLayout(formLayout);
    
    // Boutons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* cancelButton = new QPushButton("Annuler", &dialog);
    QPushButton* saveButton = new QPushButton("Enregistrer", &dialog);
    saveButton->setDefault(true);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);
    
    layout->addLayout(buttonLayout);
    
    // Rechercher la transaction par référence pour remplir les champs
    QSqlQuery query;
    query.prepare("SELECT * FROM SYRINE.TRANSACTION WHERE REFERENCE = :reference");
    query.bindValue(":reference", currentTransactionReference);
    
    if (query.exec() && query.next()) {
        // Remplir les champs avec les données actuelles
        montantEdit->setText(query.value("MONTANT").toString());
        dateEdit->setDate(query.value("DATEEE").toDate());
        statutCombo->setCurrentText(query.value("STATUTT").toString());
        typeCombo->setCurrentText(query.value("TYPEE").toString());
        modeCombo->setCurrentText(query.value("MODEPAIEMENT").toString());
        
        // Sélectionner le client ou le partenaire
        int clientId = query.value("IDCLIENT").toInt();
        int partenaireId = query.value("IDPARTENAIRE").toInt();
        
        if (clientId > 0) {
            int index = clientCombo->findData(clientId);
            if (index != -1) clientCombo->setCurrentIndex(index);
        } else {
            clientCombo->setCurrentIndex(0);
        }
        
        if (partenaireId > 0) {
            int index = partenaireCombo->findData(partenaireId);
            if (index != -1) partenaireCombo->setCurrentIndex(index);
        } else {
            partenaireCombo->setCurrentIndex(0);
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les données de la transaction.");
        return;
    }
    
    // Connecter les boutons
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    connect(saveButton, &QPushButton::clicked, [&]() {
        // Validation des entrées
        if (montantEdit->text().isEmpty()) {
            QMessageBox::warning(&dialog, "Erreur", "Le montant est obligatoire");
            return;
        }

        if (dateEdit->date() > QDate::currentDate()) {
            QMessageBox::warning(&dialog, "Erreur", "La date ne peut pas être future");
            return;
        }

        // Récupération des IDs client et partenaire
        int clientId = clientCombo->currentData().toInt();
        int partenaireId = partenaireCombo->currentData().toInt();
        
        // Configuration des valeurs
        double montant = montantEdit->text().toDouble();
        QDate date = dateEdit->date();
        QString statut = statutCombo->currentText();
        QString type = typeCombo->currentText();
        QString modePaiement = modeCombo->currentText();
        
        // Validation du montant en fonction du type de transaction
        if (type == "Vente" && montant <= 0) {
            QMessageBox::warning(&dialog, "Erreur", "Le montant d'une vente doit être positif.");
            return;
        } else if (type == "Remboursement" && montant >= 0) {
            QMessageBox::warning(&dialog, "Erreur", "Le montant d'un remboursement doit être négatif.");
            return;
        }
        
        // Vérification du payeur (client ou partenaire)
        // Pour les transactions de type "Vente" ou "Remboursement", un payeur est obligatoire
        if ((type == "Vente" || type == "Remboursement") && clientId == 0 && partenaireId == 0) {
            QMessageBox::warning(&dialog, "Erreur", "Vous devez sélectionner un client ou un partenaire pour une transaction de type '" + type + "'");
            return;
        }
        
        // Créer un objet Transaction avec les nouvelles valeurs
        Transaction transaction(currentTransactionReference, montant, modePaiement, date, statut, type, 0, clientId, partenaireId);
        
        // Modifier la transaction
        if (transaction.modifier()) {
            QMessageBox::information(&dialog, "Succès", "Transaction modifiée avec succès");
            dialog.accept();
        } else {
            QMessageBox::critical(&dialog, "Erreur", "Erreur lors de la modification: " + transaction.getLastError());
        }
    });
    
    // Afficher la boîte de dialogue
    if (dialog.exec() == QDialog::Accepted) {
        // Rafraîchir la table des transactions
        refreshTransactionTable();
    }
    
    // Réinitialiser la référence
    currentTransactionReference = -1;
}

void MainWindow::on_pushButton_9_Transaction_clicked()
{
    // Create a new dialog to display the charts
    QDialog *chartDialog = new QDialog(this);
    chartDialog->setWindowTitle("Statistiques des transactions");
    chartDialog->setMinimumSize(1200, 600);

    // Create a horizontal layout for the charts
    QHBoxLayout *mainLayout = new QHBoxLayout(chartDialog);

    // Create the bar chart
    QChart *barChart = new QChart();
    barChart->setTitle("Statistiques par type de transaction");
    barChart->setAnimationOptions(QChart::AllAnimations);

    // Create series for transaction type statistics
    QBarSeries *typeSeries = new QBarSeries();
    QBarSet *ventesSet = new QBarSet("Ventes");
    QBarSet *achatsSet = new QBarSet("Achats");
    
    // Query transaction type statistics
    QSqlQuery query;
    query.prepare("SELECT TYPEE, "
                  "SUM(CASE WHEN TYPEE = 'Vente' THEN MONTANT ELSE 0 END) as ventes, "
                  "SUM(CASE WHEN TYPEE = 'Achat' THEN MONTANT ELSE 0 END) as achats "
                  "FROM SYRINE.TRANSACTION "
                  "GROUP BY TYPEE "
                  "ORDER BY TYPEE");

    QStringList categories;
    if (query.exec()) {
        while (query.next()) {
            categories << query.value("TYPEE").toString();
            *ventesSet << query.value("ventes").toDouble();
            *achatsSet << query.value("achats").toDouble();
        }
    }

    typeSeries->append(ventesSet);
    typeSeries->append(achatsSet);
    barChart->addSeries(typeSeries);

    // Create axis for type chart
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    barChart->addAxis(axisX, Qt::AlignBottom);
    typeSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    barChart->addAxis(axisY, Qt::AlignLeft);
    typeSeries->attachAxis(axisY);

    // Create the pie chart
    QChart *pieChart = new QChart();
    pieChart->setTitle("Distribution des payeurs");
    pieChart->setAnimationOptions(QChart::AllAnimations);

    // Create pie chart for payer distribution
    QPieSeries *payerSeries = new QPieSeries();
    
    // Get payer information from the table view
    QMap<QString, int> payerCounts;
    QAbstractItemModel *model = ui->tableViewAFFICHELIST_3->model();
    
    // Assuming the payer column is the last column (index 6)
    int payerColumn = 6;
    
    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex index = model->index(row, payerColumn);
        QString payer = model->data(index).toString();
        if (payer.isEmpty()) {
            payer = "Non spécifié";
        }
        payerCounts[payer]++;
    }
    
    // Add data to the pie series
    for (auto it = payerCounts.begin(); it != payerCounts.end(); ++it) {
        payerSeries->append(it.key(), it.value());
    }

    // Add pie chart to the main chart
    pieChart->addSeries(payerSeries);

    // Create chart views
    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->setMinimumSize(600, 400);

    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieChartView->setMinimumSize(600, 400);

    // Add chart views to the layout
    mainLayout->addWidget(barChartView);
    mainLayout->addWidget(pieChartView);

    // Show the dialog
    chartDialog->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

