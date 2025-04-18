#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QButtonGroup>
#include "mainwindow.h"
#include <QInputDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QApplication>
#include <QMainWindow>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QChart>      // Nécessaire pour QChart
#include <QLayoutItem>          // Pour manipuler les éléments du layout
#include <QHBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QPdfWriter>// Inclure QPrinter pour gérer l'impression PDF
#include <QFileDialog>  // Pour QFileDialog
#include <QPrinter>     // Pour QPrinter
#include <QPainter>     // Pour QPainter   // Pour QPainter
#include <QFont>        // Pour QFont
#include <QDebug>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPageSize>
#include <QTimer>
#include <QUrlQuery>

#include <QHttpMultiPart>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QFileDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QFileDialog>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QImageCapture>
#include <QMediaDevices>
#include <QVideoWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
      remplirComboBoxEmployes();
    afficherEmployes();



    Connection conn;
    if (!conn.createconnect()) {
        QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter à la base de données.");
    }

    // 🔹 Création du groupe de boutons pour le genre
    QButtonGroup *genderGroup = new QButtonGroup(this);
    genderGroup->addButton(ui->radioButtonHomme);
    genderGroup->addButton(ui->radioButtonFemme);
    genderGroup->setExclusive(true);  // Assurer qu'un seul bouton peut être sélectionné

    connect(ui->pushButton_Recherche, &QPushButton::clicked, this, &MainWindow::on_pushButton_Recherche_clicked);
    connect(ui->pushButton_4TriEmployes, &QPushButton::clicked, this, &MainWindow::on_pushButton_4TriEmployes_clicked);
    connect(ui->comboBoxTriEmployes, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBox_TriEmployes_currentIndexChanged);

    connect(ui->btnEnregistrerHeures, &QPushButton::clicked, this, &MainWindow::enregistrerHeures);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::exportSelectedContractToPDF);
    connect(ui->pushButton_ShowCamembert, &QPushButton::clicked, this, &MainWindow::afficherCamembert);
    connect(ui->btn_register, &QPushButton::clicked, this, &MainWindow::startCameraAndCapture);





}



void MainWindow::afficherCamembert()
{
    int moins40 = 0;
    int entre40et80 = 0;
    int plus80 = 0;
    int totalEmployes = 0;

    QSqlQuery query("SELECT SUM(HEURES_TRAVAILLEES) AS total_heures, IDE FROM HEURES_TRAVAIL GROUP BY IDE");
    while (query.next()) {
        int total = query.value("total_heures").toInt();
        int ide = query.value("IDE").toInt();
        qDebug() << "Employé ID:" << ide << "Heures travaillées:" << total;

        if (total < 40)
            moins40++;
        else if (total <= 80)
            entre40et80++;
        else
            plus80++;

        totalEmployes++;
    }

    // Calcul des pourcentages
    double pourcentageMoins40 = (double)moins40 / totalEmployes * 100;
    double pourcentageEntre40et80 = (double)entre40et80 / totalEmployes * 100;
    double pourcentagePlus80 = (double)plus80 / totalEmployes * 100;

    // Création du graphique camembert
    QPieSeries *series = new QPieSeries();
    QPieSlice *slice1 = series->append("Moins de 40h", moins40);
    QPieSlice *slice2 = series->append("40-80h", entre40et80);
    QPieSlice *slice3 = series->append("Plus de 80h", plus80);

    // Définir les étiquettes avec les pourcentages
    slice1->setLabel(QString("%1%").arg(pourcentageMoins40, 0, 'f', 1));
    slice2->setLabel(QString("%1%").arg(pourcentageEntre40et80, 0, 'f', 1));
    slice3->setLabel(QString("%1%").arg(pourcentagePlus80, 0, 'f', 1));

    // Crée le graphique camembert
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des heures travaillées");
    // 🔥 Animation !
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Couleurs personnalisées (optionnel)
    slice1->setBrush(QColor("#FF9999")); // Rouge clair
    slice2->setBrush(QColor("#FFCC66")); // Orange
    slice3->setBrush(QColor("#66CC99")); // Vert

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    if (ui->layoutCamembert) {
        QLayoutItem *item;
        while ((item = ui->layoutCamembert->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        ui->layoutCamembert->addWidget(chartView);
    } else {
        qDebug() << "LayoutCamembert est null";
        QMessageBox::critical(this, "Erreur", "LayoutCamembert n'est pas trouvé !");
    }
}

void MainWindow::enregistrerHeures()
{
    int idEmploye = ui->comboBox_Employe->currentData().toInt();  // ID sélectionné
    int heures = ui->spinBox_HeuresTravaillees->value();          // Heures entrées
    QDate dateJour = ui->dateEdit->date();                        // Date sélectionnée

    QSqlQuery query;
    query.prepare("INSERT INTO HEURES_TRAVAIL (IDE, HEURES_TRAVAILLEES, DATE_JOUR) "
                  "VALUES (:id, :heures, :date)");
    query.bindValue(":id", idEmploye);
    query.bindValue(":heures", heures);
    query.bindValue(":date", dateJour);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Les heures ont été enregistrées avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible d'enregistrer les heures : " + query.lastError().text());
    }
}
void MainWindow::remplirComboBoxEmployes() {
    ui->comboBox_Employe->clear();
    ui->comboBox_Employe->clear();  // <-- vide la comboBox avant remplissage
    QSqlQuery query("SELECT ide, nom FROM EMPLOYES");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString itemText = QString::number(id) + " - " + nom;
        ui->comboBox_Employe->addItem(itemText, id);
    }
}


void MainWindow::on_btn_Valider_clicked()
{
    QString nom = ui->lineEdit_Nom->text();
    QString prenom = ui->lineEdit_Prenom->text();
    QString email = ui->lineEdit_Email->text();
    QString telephone = ui->lineEdit_Telephone->text();
    QString poste = ui->comboBox_Poste->currentText();

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
    ui->comboBox_Poste->clear();
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
void MainWindow::afficherEmployes()
{
    qDebug() << "La fonction afficherEmployes a été appelée";  // Débogage

    // Vérifier si la connexion à la base de données est ouverte
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Erreur", "La connexion à la base de données n'est pas ouverte.");
        return;
    }

    // Exécuter la requête sans sélectionner l'ID
    QSqlQuery query;
    query.prepare("SELECT PRENOM, NOM, SALAIRE, POSTE, CIN, EMAIL, NUMERODETELEPHONE, GENDER FROM EMPLOYES");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible d'exécuter la requête : " + query.lastError().text());
        return;
    }

    // Vider le tableau avant de charger les nouveaux résultats
    ui->tableWidgetEmployes->clearContents();
    ui->tableWidgetEmployes->setRowCount(0);
    ui->tableWidgetEmployes->setColumnCount(8); // 9 colonnes au lieu de 10

    // Remplir le tableau avec les résultats
    int row = 0;
    while (query.next()) {
        ui->tableWidgetEmployes->insertRow(row);
        ui->tableWidgetEmployes->setItem(row, 0, new QTableWidgetItem(query.value("PRENOM").toString()));
        ui->tableWidgetEmployes->setItem(row, 1, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tableWidgetEmployes->setItem(row, 2, new QTableWidgetItem(query.value("SALAIRE").toString()));
        ui->tableWidgetEmployes->setItem(row, 3, new QTableWidgetItem(query.value("POSTE").toString()));
        ui->tableWidgetEmployes->setItem(row, 4, new QTableWidgetItem(query.value("CIN").toString()));
        ui->tableWidgetEmployes->setItem(row, 5, new QTableWidgetItem(query.value("EMAIL").toString()));
        ui->tableWidgetEmployes->setItem(row, 6, new QTableWidgetItem(query.value("NUMERODETELEPHONE").toString()));
        ui->tableWidgetEmployes->setItem(row, 7, new QTableWidgetItem(query.value("GENDER").toString()));
        row++;
    }

    // Ajuster la taille des colonnes pour une meilleure lisibilité
    ui->tableWidgetEmployes->horizontalHeader()->setStretchLastSection(true);

    // Si aucun employé trouvé, afficher un message
    if (row == 0) {
        QMessageBox::information(this, "Aucun employé", "Il n'y a pas d'employé à afficher.");
    }
}




void MainWindow::on_deleteButton_clicked()
{
    // Vérifier si un employé est sélectionné dans le tableau
    int row = ui->tableWidgetEmployes->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Sélection incorrecte", "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    // Récupérer le CIN de l'employé sélectionné (supposons qu'il est dans la 4e colonne, index 3)
    QString cin = ui->tableWidgetEmployes->item(row, 3)->text();  // Vérifie bien l'index correct !

    // Demander confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer cet employé ?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }

    // Supprimer l'employé de la base de données en utilisant CIN
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYES WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "L'employé a été supprimé avec succès.");
        ui->tableWidgetEmployes->removeRow(row);  // Supprimer la ligne de l'interface
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de supprimer l'employé : " + query.lastError().text());
    }
}

void MainWindow::on_tableWidgetEmployes_itemSelectionChanged()
{
    int row = ui->tableWidgetEmployes->currentRow();
    if (row == -1) {
        return; // Si aucune ligne n'est sélectionnée, ne rien faire
    }

    // Récupérer le CIN de l'employé sélectionné (colonne où il est affiché, adapte si nécessaire)
    QString cin = ui->tableWidgetEmployes->item(row, 4)->text();

    // Créer une requête pour récupérer les informations de l'employé à partir de la base de données
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYES WHERE CIN = :cin");  // Utilisation du CIN à la place de l'IDE
    query.bindValue(":cin", cin);

    // Exécuter la requête
    if (query.exec() && query.next()) {
        // Remplir les champs du formulaire
        ui->lineEdit_Nom->setText(query.value("NOM").toString());
        ui->lineEdit_Prenom->setText(query.value("PRENOM").toString());
        ui->lineEdit_Email->setText(query.value("EMAIL").toString());
        ui->lineEdit_Telephone->setText(query.value("NUMERODETELEPHONE").toString());
        ui->comboBox_Poste->setCurrentText(query.value("POSTE").toString());  // ✅ Correct

        ui->lineEdit_Salaire->setText(query.value("SALAIRE").toString());
        ui->lineEdit_CIN->setText(query.value("CIN").toString());

        // Vérifier le genre et cocher le bon bouton radio
        QString gender = query.value("GENDER").toString();
        ui->radioButtonHomme->setChecked(gender == "Homme");
        ui->radioButtonFemme->setChecked(gender == "Femme");
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer les informations de l'employé.");
    }
}


void MainWindow::on_modifierButton_clicked()
{
    int row = ui->tableWidgetEmployes->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Sélection incorrecte", "Veuillez sélectionner un employé à modifier.");
        return;
    }

    // Récupérer le CIN à partir du tableau (colonne 4 ici, vérifie si c'est correct)
    QString cin = ui->tableWidgetEmployes->item(row, 4)->text().trimmed();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer le CIN de l'employé.");
        return;
    }

    // Récupérer les nouvelles valeurs des champs
    QString prenom = ui->lineEdit_Prenom->text().trimmed();
    QString nom = ui->lineEdit_Nom->text().trimmed();
    QString email = ui->lineEdit_Email->text().trimmed();
    QString telephone = ui->lineEdit_Telephone->text().trimmed();
    QString poste = ui->comboBox_Poste->currentText().trimmed();  // ✅ Correct

    QString salaireStr = ui->lineEdit_Salaire->text().trimmed();
    QString gender;

    if (ui->radioButtonHomme->isChecked())
    {
        gender = "Homme";
    } else if (ui->radioButtonFemme->isChecked()) {
        gender = "Femme";
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un genre !");
        return;
    }

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty() ||
        poste.isEmpty() || salaireStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    bool ok;
    double salaire = salaireStr.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "Le salaire doit être un nombre valide !");
        return;
    }

    // Modifier l'employé par son CIN
    bool success = Employe::modifier(cin, prenom, nom, salaire, poste, email, telephone, gender);
    if (success) {
        QMessageBox::information(this, "Succès", "L'employé a été modifié avec succès.");
        afficherEmployes(); // Rafraîchir la liste
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de modifier l'employé.");
    }
}





void MainWindow::on_pushButton_Recherche_clicked() {
    qDebug() << "\n=== Début de la recherche d'employés ===";
    qDebug() << "Bouton de recherche cliqué!";

    QString searchText = ui->lineEdit_Recherche->text().trimmed(); // récupère le texte de recherche
    qDebug() << "Texte de recherche:" << searchText;

    if (searchText.isEmpty()) {
        QMessageBox::information(this, "Information", "Veuillez entrer un terme de recherche.");
        return;
    }

    searchEmployes(searchText);
}

void MainWindow::searchEmployes(const QString &searchText) {
    qDebug() << "Recherche avec le terme:" << searchText;

    ui->tableWidgetEmployes->clearContents();  // Réinitialiser la table
    ui->tableWidgetEmployes->setRowCount(0);   // Réinitialiser les lignes de la table

    QSqlQuery query;
    QString queryStr = "SELECT NOM, PRENOM, POSTE, CIN, EMAIL, NUMERODETELEPHONE, GENDER, SALAIRE "
                       "FROM EMPLOYES "
                       "WHERE UPPER(NOM) LIKE UPPER(:nom) OR "
                       "UPPER(PRENOM) LIKE UPPER(:prenom) OR "
                       "UPPER(POSTE) LIKE UPPER(:poste) OR "
                       "CIN LIKE :cin OR "
                       "UPPER(EMAIL) LIKE UPPER(:email)";

    query.prepare(queryStr);

    QString wildcard = "%" + searchText + "%";  // Ajouter des caractères génériques

    query.bindValue(":nom", wildcard);
    query.bindValue(":prenom", wildcard);
    query.bindValue(":poste", wildcard);
    query.bindValue(":cin", wildcard);
    query.bindValue(":email", wildcard);

    if (query.exec()) {
        int rowCount = 0;

        ui->tableWidgetEmployes->setColumnCount(8);  // Définir le nombre de colonnes
        QStringList headers = {"Nom", "Prénom", "Poste", "CIN", "Email", "Téléphone", "Genre", "Salaire"};
        ui->tableWidgetEmployes->setHorizontalHeaderLabels(headers);  // Définir les en-têtes

        while (query.next()) {
            ui->tableWidgetEmployes->insertRow(rowCount);  // Insérer une nouvelle ligne

            // Remplir les colonnes avec les données
            for (int col = 0; col < 8; ++col) {
                QString value = query.value(col).toString();
                QTableWidgetItem *item = new QTableWidgetItem(value);
                ui->tableWidgetEmployes->setItem(rowCount, col, item);
            }

            rowCount++;
        }

        if (rowCount == 0) {
            QMessageBox::information(this, "Information", "Aucun employé trouvé.");
        }
    } else {
        qDebug() << "Erreur SQL:" << query.lastError().text();  // Afficher l'erreur SQL dans le debug
        QMessageBox::critical(this, "Erreur", "Échec lors de la recherche.");
    }
}
void MainWindow::on_pushButton_4TriEmployes_clicked()
{
    qDebug() << "\n=== Début du tri des employés ===";
    qDebug() << "Bouton de tri cliqué!";

    // Récupérer le critère de tri
    QString criterion = ui->comboBoxTriEmployes->currentText();
    qDebug() << "Critère de tri:" << criterion;

    // Effectuer le tri des employés
    sortEmployes(criterion);
}
void MainWindow::on_comboBox_TriEmployes_currentIndexChanged(int index)
{
    qDebug() << "Critère de tri changé à l'index:" << index;
    qDebug() << "Nouveau critère:" << ui->comboBoxTriEmployes->currentText();
}


void MainWindow::sortEmployes(const QString &criterion)
{
    qDebug() << "Tri des employés selon le critère:" << criterion;

    // Déterminer la colonne SQL à utiliser pour le tri
    QString orderByColumn;

    if (criterion == "Nom") {
        orderByColumn = "NOM";
    } else if (criterion == "Genre") {  // Remplace "CIN" par "Genre"
        orderByColumn = "GENDER";
    } else if (criterion == "Poste") {
        orderByColumn = "POSTE";
    } else {
        // Par défaut, trier par Nom
        orderByColumn = "NOM";
    }

    // Vider la table actuelle
    ui->tableWidgetEmployes->clearContents();
    ui->tableWidgetEmployes->setRowCount(0);

    // Préparer la requête SQL avec tri
    QSqlQuery query;
    QString queryStr = "SELECT NOM, PRENOM, POSTE, CIN, EMAIL, NUMERODETELEPHONE, GENDER, SALAIRE "
                       "FROM EMPLOYES "
                       "ORDER BY " + orderByColumn;

    if (query.exec(queryStr)) {
        int rowCount = 0;

        // Configuration des en-têtes
        ui->tableWidgetEmployes->setColumnCount(8);
        QStringList headers;
        headers << "Nom" << "Prénom" << "Poste" << "CIN" << "Email" << "Téléphone" << "Genre" << "Salaire";
        ui->tableWidgetEmployes->setHorizontalHeaderLabels(headers);

        // Remplir la table avec les résultats
        while (query.next()) {
            ui->tableWidgetEmployes->insertRow(rowCount);

            for (int col = 0; col < 8; ++col) {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
                ui->tableWidgetEmployes->setItem(rowCount, col, item);
            }

            rowCount++;
        }

        // Ajuster les colonnes
        ui->tableWidgetEmployes->resizeColumnsToContents();
        ui->tableWidgetEmployes->resizeRowsToContents();

        QMessageBox::information(this, "Tri", QString("Les employés ont été triés par %1.").arg(criterion.toLower()));
    } else {
        qDebug() << "Erreur lors du tri:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors du tri: " + query.lastError().text());
    }
}

void MainWindow::exportSelectedContractToPDF()
{
    int row = ui->tableWidgetEmployes->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un contrat à exporter.");
        return;
    }

    QString nom = ui->tableWidgetEmployes->item(row, 0)->text();
    QString prenom = ui->tableWidgetEmployes->item(row, 1)->text();
    QString salaire = ui->tableWidgetEmployes->item(row, 2)->text();
    QString poste = ui->tableWidgetEmployes->item(row, 3)->text();
    QString cin = ui->tableWidgetEmployes->item(row, 4)->text();
    QString email = ui->tableWidgetEmployes->item(row, 5)->text();
    QString telephone = ui->tableWidgetEmployes->item(row, 6)->text();
    QString genre = ui->tableWidgetEmployes->item(row, 7)->text();

    QString dateActuelle = QDate::currentDate().toString("dd/MM/yyyy");

    QString htmlContent = R"(
    <html>
    <head>
        <style>
            body { font-family: 'Arial'; color: #333; }
            h1 { text-align: center; color: #2c3e50; }
            .header {
                text-align: center;
                font-size: 22px;
                font-weight: bold;
                color: #2980b9;
                margin-bottom: 30px;
            }
            .info-table { width: 100%; border-collapse: collapse; margin-top: 20px; }
            .info-table td { padding: 8px 12px; border-bottom: 1px solid #ccc; }
            .label { font-weight: bold; width: 30%; background-color: #f0f0f0; }
            .footer { margin-top: 50px; }
            .signature { margin-top: 40px; }
        </style>
    </head>
    <body>
        <h1>Contrat de Travail</h1>
        <div class='header'>WorkShift - Plateforme de consulting</div>

        <p><b>Date d'exportation :</b> )" + dateActuelle + R"(</p>

        <table class='info-table'>
            <tr><td class='label'>Nom</td><td>)" + nom + R"(</td></tr>
            <tr><td class='label'>Prénom</td><td>)" + prenom + R"(</td></tr>
            <tr><td class='label'>Poste</td><td>)" + poste + R"(</td></tr>
            <tr><td class='label'>Email</td><td>)" + email + R"(</td></tr>
            <tr><td class='label'>Téléphone</td><td>)" + telephone + R"(</td></tr>
            <tr><td class='label'>CIN</td><td>)" + cin + R"(</td></tr>
            <tr><td class='label'>Genre</td><td>)" + genre + R"(</td></tr>
            <tr><td class='label'>Salaire</td><td>)" + salaire + R"( DT</td></tr>
        </table>

        <div class='footer'>
            <div class='signature'>
                <p><b>Signature de l'employé</b></p>
                <p>_______________________</p>
            </div>
        </div>
    </body>
    </html>
)";


    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QTextDocument doc;
    doc.setHtml(htmlContent);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);

    doc.print(&printer);

    QMessageBox::information(this, "Succès", "Le contrat a été exporté avec succès !");
}
void MainWindow::updateFaceIdAndToken(const QString &faceId, const QString &faceToken, int currentEmployeeId)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYES SET FACE_ID = :face_id, FACE_TOKEN = :face_token WHERE IDE = :ide");
    query.bindValue(":face_id", faceId);
    query.bindValue(":face_token", faceToken);
    query.bindValue(":ide", currentEmployeeId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'enregistrement du face_id.\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Succès", "Face ID et Face Token enregistrés dans la base de données.");
    }
}
void MainWindow::enregistrerFaceTokenDansBDD(const QString &faceToken)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYES SET face_token = :face_token WHERE IDE = :ide");

    // Si le faceToken est vide, on le remplace par un QVariant::Null
    if (faceToken.isEmpty()) {
        query.prepare("UPDATE EMPLOYES SET \"face_token\" = :face_token WHERE IDE = :ide");
    } else {
        query.bindValue(":face_token", faceToken);
    }

    query.bindValue(":ide", currentEmployeeId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'enregistrement du face_token.\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Succès", "Face token enregistré avec succès dans la base de données.");
    }
}

void MainWindow::registerFace(const QString &imagePath)
{
    QString apiKey = "jk5KjSk7vXY6Ueo148Kcr9xXHaYvMzZl";
    QString apiSecret = "Dh3r4LeDVTyxnotaF4CotC5FtPJwLrRR";
    QString url = "https://api-us.faceplusplus.com/facepp/v3/detect";

    QUrl qUrl(url);
    QNetworkRequest request(qUrl);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // Clés API
    QHttpPart keyPart;
    keyPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"api_key\""));
    keyPart.setBody(apiKey.toUtf8());

    QHttpPart secretPart;
    secretPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"api_secret\""));
    secretPart.setBody(apiSecret.toUtf8());

    // Image
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image_file\"; filename=\"face.jpg\""));
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));

    QFile *file = new QFile(imagePath);
    if (!file->open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier image.");
        return;
    }
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);

    multiPart->append(keyPart);
    multiPart->append(secretPart);
    multiPart->append(imagePart);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray response = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response);
        QJsonObject obj = json.object();

        if (!json.isNull() && obj.contains("faces") && !obj["faces"].toArray().isEmpty()) {
            QString faceToken = obj["faces"].toArray()[0].toObject()["face_token"].toString();
            qDebug() << "Face token:" << faceToken;

            QMessageBox::information(this, "Succès", "Visage détecté ! Face Token : " + faceToken);

            // Enregistrer dans la BDD
            enregistrerFaceTokenDansBDD(faceToken);
        } else {
            QMessageBox::warning(this, "Erreur", "Aucun visage détecté ou réponse invalide.");
        }

        reply->deleteLater();
    });
}

void MainWindow::getFaceId(const QString &faceToken)
{
    QString apiKey = "jk5KjSk7vXY6Ueo148Kcr9xXHaYvMzZl";  // Remplace avec ta vraie clé API
    QString apiSecret = "Dh3r4LeDVTyxnotaF4CotC5FtPJwLrRR"; // Remplace avec ton vrai secret API
    QString url = "https://api-us.faceplusplus.com/facepp/v3/face/getfaceid";

    QUrl qUrl(url);
    QNetworkRequest request(qUrl);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // Ajouter les clés API
    QHttpPart keyPart;
    keyPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"api_key\""));
    keyPart.setBody(apiKey.toUtf8());

    QHttpPart secretPart;
    secretPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"api_secret\""));
    secretPart.setBody(apiSecret.toUtf8());

    // Ajouter le face_token pour récupérer le face_id
    QHttpPart faceTokenPart;
    faceTokenPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"face_token\""));
    faceTokenPart.setBody(faceToken.toUtf8());

    // Ajouter tout à la requête
    multiPart->append(keyPart);
    multiPart->append(secretPart);
    multiPart->append(faceTokenPart);

    // Envoi de la requête
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray response = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response);

        if (!json.isNull()) {
            QJsonObject obj = json.object();
            if (obj.contains("face_id")) {
                QString faceId = obj["face_id"].toString();
                qDebug() << "Face ID:" << faceId;

                // Enregistrer face_id dans la base de données
                QSqlQuery query;
                query.prepare("UPDATE EMPLOYES SET FACE_ID = :face_id WHERE IDE = :ide");
                query.bindValue(":face_id", faceId);  // Valeur du face_id
                query.bindValue(":ide", currentEmployeeId);  // Assure-toi que currentEmployeeId est bien défini

                if (!query.exec()) {
                    QMessageBox::warning(this, "Erreur", "Erreur lors de l'enregistrement du face_id.\n" + query.lastError().text());
                } else {
                    QMessageBox::information(this, "Succès", "Face ID enregistré dans la base de données.");
                }
            } else {
                QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération du face_id.");
            }
        } else {
            QMessageBox::warning(this, "Erreur", "Réponse invalide de Face++");
        }
        reply->deleteLater();
    });
}


void MainWindow::verifyFace(const QString &imagePath, const QString &faceToken)
{
    QString apiKey = "jk5KjSk7vXY6Ueo148Kcr9xXHaYvMzZl";   // Remplace avec ta vraie clé API
    QString apiSecret = "Dh3r4LeDVTyxnotaF4CotC5FtPJwLrRR";  // Remplace avec ton vrai secret API
    QString url = "https://api-us.faceplusplus.com/facepp/v3/compare";

    // Créer la requête
    QUrl apiUrl(url);
    QNetworkRequest request(apiUrl);

    // Créer le multipart pour la requête
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // Ajouter les clés API
    QHttpPart keyPart;
    keyPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"api_key\""));
    keyPart.setBody(apiKey.toUtf8());

    QHttpPart secretPart;
    secretPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"api_secret\""));
    secretPart.setBody(apiSecret.toUtf8());

    // Ajouter le Face Token
    QHttpPart faceTokenPart;
    faceTokenPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"face_token1\""));
    faceTokenPart.setBody(faceToken.toUtf8());

    // Ajouter l'image à comparer
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image_file2\"; filename=\"compare_face.jpg\""));
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));

    QFile *file = new QFile(imagePath);
    if (!file->open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier image.");
        return;
    }
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);  // Gère la suppression automatique de file avec multiPart

    // Ajouter tout à la requête multipart
    multiPart->append(keyPart);
    multiPart->append(secretPart);
    multiPart->append(faceTokenPart);
    multiPart->append(imagePart);

    // Envoi de la requête POST avec les données multipart
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);  // Gère la mémoire de multiPart automatiquement

    // Connexion pour recevoir la réponse
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        QByteArray response = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(response);

        if (!json.isNull()) {
            QJsonObject obj = json.object();
            if (obj.contains("confidence")) {
                double confidence = obj["confidence"].toDouble();
                if (confidence > 80.0) {  // Seuil de confiance
                    QMessageBox::information(nullptr, "Succès", "Les visages correspondent avec une confiance de " + QString::number(confidence));
                } else {
                    QMessageBox::warning(nullptr, "Erreur", "Les visages ne correspondent pas.");
                }
            } else {
                QMessageBox::warning(nullptr, "Erreur", "Réponse invalide de Face++.");
            }
        } else {
            QMessageBox::warning(nullptr, "Erreur", "Réponse invalide de Face++.");
        }
        reply->deleteLater();
    });
}


void MainWindow::startCameraAndCapture()
{
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    if (cameras.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucune caméra n'est disponible !");
        return;
    }

    QCamera *camera = new QCamera(cameras.first(), this);
    QMediaCaptureSession *captureSession = new QMediaCaptureSession(this);
    QImageCapture *imageCapture = new QImageCapture(this);
    QVideoWidget *viewfinder = new QVideoWidget(this);

    ui->cameraLayout->addWidget(viewfinder);

    captureSession->setCamera(camera);
    captureSession->setVideoOutput(viewfinder);
    captureSession->setImageCapture(imageCapture);

    viewfinder->show();
    camera->start();

    // Capture automatique après 1,5 secondes
    QTimer::singleShot(1500, imageCapture, [=]() {
        imageCapture->captureToFile();
    });

    connect(imageCapture, &QImageCapture::imageCaptured, this, [=](int id, const QImage &image) {
        QString imagePath = QFileDialog::getSaveFileName(this, "Enregistrer l'image", "", "Images (*.jpg *.png)");
        if (!imagePath.isEmpty()) {
            image.save(imagePath);
            registerFace(imagePath);
        }

        camera->stop();
        ui->cameraLayout->removeWidget(viewfinder);
        viewfinder->deleteLater();
        camera->deleteLater();
        imageCapture->deleteLater();
        captureSession->deleteLater();
    });
}



MainWindow::~MainWindow()
{
    delete ui;

}




