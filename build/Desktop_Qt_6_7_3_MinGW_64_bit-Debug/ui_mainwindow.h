/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page1;
    QWidget *logocontainer;
    QLabel *logo;
    QGroupBox *groupBox;
    QLineEdit *lineEdit;
    QLabel *id_2;
    QLabel *id_3;
    QLineEdit *lineEdit_2;
    QPushButton *Bemploye_16;
    QLabel *label;
    QWidget *page_2;
    QStackedWidget *stackedWidget_2;
    QWidget *page;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox_3;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_8;
    QLineEdit *lineEdit_Nom;
    QLineEdit *lineEdit_Prenom;
    QLineEdit *lineEdit_Salaire;
    QLabel *label_;
    QLineEdit *lineEdit_Email;
    QLabel *label_10;
    QLineEdit *lineEdit_MotDePasse;
    QLineEdit *lineEdit_Telephone;
    QLabel *label_11;
    QPushButton *btn_Valider;
    QLineEdit *lineEdit_CIN;
    QLabel *label_13;
    QPushButton *btn_Annuler;
    QRadioButton *radioButtonFemme;
    QRadioButton *radioButtonHomme;
    QComboBox *comboBox_Poste;
    QPushButton *pushButton_Recherche;
    QComboBox *comboBoxTriEmployes;
    QTableWidget *tableWidgetEmployes;
    QComboBox *comboBoxFiltre;
    QPushButton *deleteButton;
    QPushButton *modifierButton;
    QLineEdit *lineEdit_Recherche;
    QPushButton *pushButton_4TriEmployes;
    QPushButton *exportButton;
    QWidget *tab_2;
    QWidget *widget_2;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *layoutCamembert;
    QPushButton *pushButton_ShowCamembert;
    QGroupBox *groupBox_4;
    QSpinBox *spinBox_HeuresTravaillees;
    QComboBox *comboBox_Employe;
    QLabel *label_2;
    QLabel *label_12;
    QPushButton *btnEnregistrerHeures;
    QDateEdit *dateEdit;
    QLabel *label_14;
    QWidget *tab_3;
    QPushButton *Bemploye_10;
    QPushButton *Bemploye_11;
    QPushButton *Bemploye_9;
    QWidget *widget;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *label_28;
    QWidget *page_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1299, 710);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 10, 1151, 671));
        stackedWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(27, 26, 49);"));
        page1 = new QWidget();
        page1->setObjectName("page1");
        logocontainer = new QWidget(page1);
        logocontainer->setObjectName("logocontainer");
        logocontainer->setGeometry(QRect(10, 190, 711, 271));
        logo = new QLabel(logocontainer);
        logo->setObjectName("logo");
        logo->setGeometry(QRect(0, 10, 761, 251));
        logo->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/473633314_984267386887376_925439099489325601_n.png")));
        groupBox = new QGroupBox(page1);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(780, 70, 321, 491));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(190, 130, 113, 26));
        lineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        id_2 = new QLabel(groupBox);
        id_2->setObjectName("id_2");
        id_2->setGeometry(QRect(20, 130, 171, 31));
        id_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
""));
        id_3 = new QLabel(groupBox);
        id_3->setObjectName("id_3");
        id_3->setGeometry(QRect(40, 210, 171, 31));
        id_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
""));
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(190, 210, 113, 26));
        lineEdit_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        Bemploye_16 = new QPushButton(groupBox);
        Bemploye_16->setObjectName("Bemploye_16");
        Bemploye_16->setGeometry(QRect(90, 308, 131, 41));
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("user-available")));
        Bemploye_16->setIcon(icon);
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(120, 40, 91, 21));
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 9pt \"Stencil\";"));
        stackedWidget->addWidget(page1);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget_2 = new QStackedWidget(page_2);
        stackedWidget_2->setObjectName("stackedWidget_2");
        stackedWidget_2->setGeometry(QRect(20, 40, 1131, 571));
        page = new QWidget();
        page->setObjectName("page");
        tabWidget = new QTabWidget(page);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(10, 10, 1111, 561));
        tabWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 53);\\ncolor: rgb(255, 255, 255);\\n\\nselection-background-color: rgb(255, 255, 255);"));
        tab = new QWidget();
        tab->setObjectName("tab");
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(10, 10, 401, 511));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(30, 30, 63, 20));
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(30, 100, 63, 20));
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(220, 240, 63, 20));
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(30, 240, 63, 20));
        label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineEdit_Nom = new QLineEdit(groupBox_3);
        lineEdit_Nom->setObjectName("lineEdit_Nom");
        lineEdit_Nom->setGeometry(QRect(30, 60, 113, 26));
        lineEdit_Nom->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        lineEdit_Prenom = new QLineEdit(groupBox_3);
        lineEdit_Prenom->setObjectName("lineEdit_Prenom");
        lineEdit_Prenom->setGeometry(QRect(30, 130, 113, 26));
        lineEdit_Prenom->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        lineEdit_Salaire = new QLineEdit(groupBox_3);
        lineEdit_Salaire->setObjectName("lineEdit_Salaire");
        lineEdit_Salaire->setGeometry(QRect(30, 270, 113, 26));
        lineEdit_Salaire->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_ = new QLabel(groupBox_3);
        label_->setObjectName("label_");
        label_->setGeometry(QRect(220, 30, 63, 20));
        label_->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineEdit_Email = new QLineEdit(groupBox_3);
        lineEdit_Email->setObjectName("lineEdit_Email");
        lineEdit_Email->setGeometry(QRect(220, 60, 113, 26));
        lineEdit_Email->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(220, 100, 111, 31));
        label_10->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        lineEdit_MotDePasse = new QLineEdit(groupBox_3);
        lineEdit_MotDePasse->setObjectName("lineEdit_MotDePasse");
        lineEdit_MotDePasse->setGeometry(QRect(220, 140, 113, 26));
        lineEdit_MotDePasse->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        lineEdit_Telephone = new QLineEdit(groupBox_3);
        lineEdit_Telephone->setObjectName("lineEdit_Telephone");
        lineEdit_Telephone->setGeometry(QRect(220, 200, 113, 26));
        lineEdit_Telephone->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(220, 170, 111, 20));
        label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        btn_Valider = new QPushButton(groupBox_3);
        btn_Valider->setObjectName("btn_Valider");
        btn_Valider->setGeometry(QRect(60, 450, 93, 29));
        btn_Valider->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        QIcon icon1(QIcon::fromTheme(QString::fromUtf8("contact-new")));
        btn_Valider->setIcon(icon1);
        lineEdit_CIN = new QLineEdit(groupBox_3);
        lineEdit_CIN->setObjectName("lineEdit_CIN");
        lineEdit_CIN->setGeometry(QRect(30, 200, 113, 26));
        lineEdit_CIN->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(30, 170, 63, 20));
        label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        btn_Annuler = new QPushButton(groupBox_3);
        btn_Annuler->setObjectName("btn_Annuler");
        btn_Annuler->setGeometry(QRect(180, 450, 93, 29));
        btn_Annuler->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        QIcon icon2(QIcon::fromTheme(QString::fromUtf8("edit-clear")));
        btn_Annuler->setIcon(icon2);
        radioButtonFemme = new QRadioButton(groupBox_3);
        radioButtonFemme->setObjectName("radioButtonFemme");
        radioButtonFemme->setGeometry(QRect(210, 350, 110, 24));
        radioButtonFemme->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        radioButtonHomme = new QRadioButton(groupBox_3);
        radioButtonHomme->setObjectName("radioButtonHomme");
        radioButtonHomme->setGeometry(QRect(30, 350, 110, 24));
        radioButtonHomme->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        comboBox_Poste = new QComboBox(groupBox_3);
        comboBox_Poste->addItem(QString());
        comboBox_Poste->addItem(QString());
        comboBox_Poste->addItem(QString());
        comboBox_Poste->addItem(QString());
        comboBox_Poste->setObjectName("comboBox_Poste");
        comboBox_Poste->setGeometry(QRect(200, 270, 121, 26));
        comboBox_Poste->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        pushButton_Recherche = new QPushButton(tab);
        pushButton_Recherche->setObjectName("pushButton_Recherche");
        pushButton_Recherche->setGeometry(QRect(580, 120, 93, 31));
        pushButton_Recherche->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 85, 255);"));
        QIcon icon3(QIcon::fromTheme(QString::fromUtf8("system-search")));
        pushButton_Recherche->setIcon(icon3);
        comboBoxTriEmployes = new QComboBox(tab);
        comboBoxTriEmployes->addItem(QString());
        comboBoxTriEmployes->addItem(QString());
        comboBoxTriEmployes->addItem(QString());
        comboBoxTriEmployes->setObjectName("comboBoxTriEmployes");
        comboBoxTriEmployes->setGeometry(QRect(960, 120, 141, 31));
        comboBoxTriEmployes->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tableWidgetEmployes = new QTableWidget(tab);
        if (tableWidgetEmployes->columnCount() < 8)
            tableWidgetEmployes->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetEmployes->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetEmployes->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidgetEmployes->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidgetEmployes->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidgetEmployes->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidgetEmployes->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidgetEmployes->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidgetEmployes->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tableWidgetEmployes->setObjectName("tableWidgetEmployes");
        tableWidgetEmployes->setGeometry(QRect(430, 180, 581, 301));
        tableWidgetEmployes->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        comboBoxFiltre = new QComboBox(tab);
        comboBoxFiltre->addItem(QString());
        comboBoxFiltre->addItem(QString());
        comboBoxFiltre->addItem(QString());
        comboBoxFiltre->setObjectName("comboBoxFiltre");
        comboBoxFiltre->setGeometry(QRect(430, 120, 141, 31));
        comboBoxFiltre->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        deleteButton = new QPushButton(tab);
        deleteButton->setObjectName("deleteButton");
        deleteButton->setGeometry(QRect(1020, 260, 81, 31));
        modifierButton = new QPushButton(tab);
        modifierButton->setObjectName("modifierButton");
        modifierButton->setGeometry(QRect(1022, 310, 81, 29));
        lineEdit_Recherche = new QLineEdit(tab);
        lineEdit_Recherche->setObjectName("lineEdit_Recherche");
        lineEdit_Recherche->setGeometry(QRect(690, 120, 141, 31));
        lineEdit_Recherche->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        pushButton_4TriEmployes = new QPushButton(tab);
        pushButton_4TriEmployes->setObjectName("pushButton_4TriEmployes");
        pushButton_4TriEmployes->setGeometry(QRect(850, 120, 93, 29));
        pushButton_4TriEmployes->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 85, 255);"));
        exportButton = new QPushButton(tab);
        exportButton->setObjectName("exportButton");
        exportButton->setGeometry(QRect(920, 490, 171, 29));
        exportButton->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 85, 255);"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        widget_2 = new QWidget(tab_2);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(10, 30, 451, 341));
        widget_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        horizontalLayoutWidget = new QWidget(widget_2);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(10, 0, 411, 321));
        layoutCamembert = new QHBoxLayout(horizontalLayoutWidget);
        layoutCamembert->setObjectName("layoutCamembert");
        layoutCamembert->setContentsMargins(0, 0, 0, 0);
        pushButton_ShowCamembert = new QPushButton(tab_2);
        pushButton_ShowCamembert->setObjectName("pushButton_ShowCamembert");
        pushButton_ShowCamembert->setGeometry(QRect(180, 410, 93, 29));
        pushButton_ShowCamembert->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 85, 255);"));
        groupBox_4 = new QGroupBox(tab_2);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(570, 20, 481, 411));
        spinBox_HeuresTravaillees = new QSpinBox(groupBox_4);
        spinBox_HeuresTravaillees->setObjectName("spinBox_HeuresTravaillees");
        spinBox_HeuresTravaillees->setGeometry(QRect(320, 170, 42, 26));
        spinBox_HeuresTravaillees->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        comboBox_Employe = new QComboBox(groupBox_4);
        comboBox_Employe->setObjectName("comboBox_Employe");
        comboBox_Employe->setGeometry(QRect(290, 110, 76, 26));
        comboBox_Employe->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_2 = new QLabel(groupBox_4);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(70, 110, 161, 31));
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(70, 160, 161, 31));
        label_12->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        btnEnregistrerHeures = new QPushButton(groupBox_4);
        btnEnregistrerHeures->setObjectName("btnEnregistrerHeures");
        btnEnregistrerHeures->setGeometry(QRect(190, 290, 93, 29));
        btnEnregistrerHeures->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 85, 255);"));
        dateEdit = new QDateEdit(groupBox_4);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setGeometry(QRect(260, 220, 110, 26));
        dateEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(80, 220, 131, 31));
        label_14->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        Bemploye_10 = new QPushButton(tab_3);
        Bemploye_10->setObjectName("Bemploye_10");
        Bemploye_10->setGeometry(QRect(220, 440, 151, 29));
        Bemploye_11 = new QPushButton(tab_3);
        Bemploye_11->setObjectName("Bemploye_11");
        Bemploye_11->setGeometry(QRect(20, 440, 181, 29));
        Bemploye_9 = new QPushButton(tab_3);
        Bemploye_9->setObjectName("Bemploye_9");
        Bemploye_9->setGeometry(QRect(390, 440, 211, 29));
        widget = new QWidget(tab_3);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(10, 20, 621, 381));
        label_26 = new QLabel(widget);
        label_26->setObjectName("label_26");
        label_26->setGeometry(QRect(-60, -10, 691, 391));
        label_26->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/Baixar \303\255cone de reconhecimento facial, vetor de linha de varredura biom\303\251trica de rosto gratuitamente.jpeg")));
        label_27 = new QLabel(tab_3);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(680, 210, 151, 20));
        label_27->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_28 = new QLabel(tab_3);
        label_28->setObjectName("label_28");
        label_28->setGeometry(QRect(680, 120, 141, 20));
        label_28->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tabWidget->addTab(tab_3, QString());
        stackedWidget_2->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        stackedWidget_2->addWidget(page_3);
        stackedWidget->addWidget(page_2);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1299, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);
        stackedWidget_2->setCurrentIndex(0);
        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        logo->setText(QString());
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "connexion", nullptr));
        id_2->setText(QCoreApplication::translate("MainWindow", "identifiant unique :", nullptr));
        id_3->setText(QCoreApplication::translate("MainWindow", "mot de passe :", nullptr));
        Bemploye_16->setText(QCoreApplication::translate("MainWindow", "se connecter", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "connexion", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "ajout", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "pr\303\251nom", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "poste", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "salaire", nullptr));
        label_->setText(QCoreApplication::translate("MainWindow", "email", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "mot de passe", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "t\303\251l\303\251phone", nullptr));
        btn_Valider->setText(QCoreApplication::translate("MainWindow", "valider", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "cin", nullptr));
        btn_Annuler->setText(QCoreApplication::translate("MainWindow", "annuler", nullptr));
        radioButtonFemme->setText(QCoreApplication::translate("MainWindow", "Femme", nullptr));
        radioButtonHomme->setText(QCoreApplication::translate("MainWindow", "Homme", nullptr));
        comboBox_Poste->setItemText(0, QCoreApplication::translate("MainWindow", "Transaction", nullptr));
        comboBox_Poste->setItemText(1, QCoreApplication::translate("MainWindow", "Conseil", nullptr));
        comboBox_Poste->setItemText(2, QCoreApplication::translate("MainWindow", "Partenaire", nullptr));
        comboBox_Poste->setItemText(3, QCoreApplication::translate("MainWindow", "RH", nullptr));

        pushButton_Recherche->setText(QCoreApplication::translate("MainWindow", "recherche ", nullptr));
        comboBoxTriEmployes->setItemText(0, QCoreApplication::translate("MainWindow", "nom", nullptr));
        comboBoxTriEmployes->setItemText(1, QCoreApplication::translate("MainWindow", "poste", nullptr));
        comboBoxTriEmployes->setItemText(2, QCoreApplication::translate("MainWindow", "genre", nullptr));

        QTableWidgetItem *___qtablewidgetitem = tableWidgetEmployes->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidgetEmployes->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "prenom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidgetEmployes->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "salaire", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidgetEmployes->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "poste", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidgetEmployes->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "cin", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidgetEmployes->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "email", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidgetEmployes->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "t\303\251l\303\251phone", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidgetEmployes->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "gender", nullptr));
        comboBoxFiltre->setItemText(0, QCoreApplication::translate("MainWindow", "nom", nullptr));
        comboBoxFiltre->setItemText(1, QCoreApplication::translate("MainWindow", "poste", nullptr));
        comboBoxFiltre->setItemText(2, QCoreApplication::translate("MainWindow", "cin", nullptr));

        deleteButton->setText(QCoreApplication::translate("MainWindow", "supprimer", nullptr));
        modifierButton->setText(QCoreApplication::translate("MainWindow", "modifier", nullptr));
        pushButton_4TriEmployes->setText(QCoreApplication::translate("MainWindow", "tri par :", nullptr));
        exportButton->setText(QCoreApplication::translate("MainWindow", "\360\237\223\244 Exporter en PDF", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "acceuil", nullptr));
        pushButton_ShowCamembert->setText(QCoreApplication::translate("MainWindow", "afficher", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "L'employe:", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "heures travailles:", nullptr));
        btnEnregistrerHeures->setText(QCoreApplication::translate("MainWindow", "enregistrer", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "date du jour :", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "statistique ", nullptr));
        Bemploye_10->setText(QCoreApplication::translate("MainWindow", " Capturer une image", nullptr));
        Bemploye_11->setText(QCoreApplication::translate("MainWindow", "Lancer la reconnaissance", nullptr));
        Bemploye_9->setText(QCoreApplication::translate("MainWindow", " Enregistrer un nouveau visage", nullptr));
        label_26->setText(QString());
        label_27->setText(QCoreApplication::translate("MainWindow", " Statut de l\342\200\231utilisateur", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", " Nom de l' employ\303\251 ", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "m\303\251tiers", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
