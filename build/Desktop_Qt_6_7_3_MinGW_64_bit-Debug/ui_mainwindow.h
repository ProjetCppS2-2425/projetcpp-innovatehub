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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *lineEdit_Nom;
    QLineEdit *lineEdit_Prenom;
    QLineEdit *lineEdit_Salaire;
    QLineEdit *lineEdit_Poste;
    QLabel *label_;
    QLineEdit *lineEdit_Email;
    QLabel *label_10;
    QLineEdit *lineEdit_MotDePasse;
    QLineEdit *lineEdit_Telephone;
    QLabel *label_11;
    QDateEdit *dateEdit_Naissance;
    QDateEdit *dateEdit_Embauche;
    QCheckBox *checkBox_Femme;
    QCheckBox *checkBox_Homme;
    QPushButton *btn_valider;
    QLineEdit *lineEdit_CIN;
    QLabel *label_13;
    QPushButton *btn_annuler;
    QPushButton *pushButton_4;
    QComboBox *comboBox;
    QTableWidget *tableWidget;
    QLabel *label_12;
    QComboBox *comboBox_2;
    QWidget *tab_2;
    QLabel *label_2;
    QTableWidget *tableWidget_2;
    QCommandLinkButton *commandLinkButton;
    QPushButton *Bemploye_4;
    QPushButton *Bemploye_3;
    QPushButton *Bemploye_5;
    QPushButton *Bemploye_6;
    QPushButton *Bemploye_7;
    QPushButton *Bemploye_8;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *label_25;
    QWidget *tab_3;
    QPushButton *Bemploye_10;
    QPushButton *Bemploye_11;
    QPushButton *Bemploye_9;
    QWidget *widget;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *label_28;
    QWidget *tab_4;
    QTableWidget *tableWidget_3;
    QComboBox *comboBox_3;
    QPushButton *Bemploye_13;
    QPushButton *Bemploye_14;
    QPushButton *Bemploye_15;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_31;
    QLabel *label_33;
    QLabel *label_34;
    QDateEdit *dateEdit_3;
    QDateEdit *dateEdit_4;
    QDateEdit *dateEdit_5;
    QDateEdit *dateEdit_6;
    QPushButton *Bemploye_12;
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
        tabWidget->setGeometry(QRect(10, 10, 1111, 551));
        tab = new QWidget();
        tab->setObjectName("tab");
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(20, 10, 401, 511));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(30, 30, 63, 20));
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(30, 100, 63, 20));
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(30, 170, 111, 20));
        label_5->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(220, 240, 63, 20));
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(30, 310, 131, 20));
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
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
        lineEdit_Poste = new QLineEdit(groupBox_3);
        lineEdit_Poste->setObjectName("lineEdit_Poste");
        lineEdit_Poste->setGeometry(QRect(220, 270, 113, 26));
        lineEdit_Poste->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
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
        dateEdit_Naissance = new QDateEdit(groupBox_3);
        dateEdit_Naissance->setObjectName("dateEdit_Naissance");
        dateEdit_Naissance->setGeometry(QRect(30, 200, 110, 26));
        dateEdit_Naissance->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        dateEdit_Embauche = new QDateEdit(groupBox_3);
        dateEdit_Embauche->setObjectName("dateEdit_Embauche");
        dateEdit_Embauche->setGeometry(QRect(30, 340, 110, 26));
        dateEdit_Embauche->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        checkBox_Femme = new QCheckBox(groupBox_3);
        checkBox_Femme->setObjectName("checkBox_Femme");
        checkBox_Femme->setGeometry(QRect(220, 400, 91, 24));
        checkBox_Femme->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        checkBox_Homme = new QCheckBox(groupBox_3);
        checkBox_Homme->setObjectName("checkBox_Homme");
        checkBox_Homme->setGeometry(QRect(70, 400, 91, 24));
        checkBox_Homme->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        btn_valider = new QPushButton(groupBox_3);
        btn_valider->setObjectName("btn_valider");
        btn_valider->setGeometry(QRect(60, 450, 93, 29));
        QIcon icon1(QIcon::fromTheme(QString::fromUtf8("contact-new")));
        btn_valider->setIcon(icon1);
        lineEdit_CIN = new QLineEdit(groupBox_3);
        lineEdit_CIN->setObjectName("lineEdit_CIN");
        lineEdit_CIN->setGeometry(QRect(220, 340, 113, 26));
        lineEdit_CIN->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(220, 310, 63, 20));
        label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        btn_annuler = new QPushButton(groupBox_3);
        btn_annuler->setObjectName("btn_annuler");
        btn_annuler->setGeometry(QRect(180, 450, 93, 29));
        QIcon icon2(QIcon::fromTheme(QString::fromUtf8("edit-clear")));
        btn_annuler->setIcon(icon2);
        pushButton_4 = new QPushButton(tab);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(630, 120, 93, 31));
        QIcon icon3(QIcon::fromTheme(QString::fromUtf8("system-search")));
        pushButton_4->setIcon(icon3);
        comboBox = new QComboBox(tab);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(840, 120, 141, 31));
        tableWidget = new QTableWidget(tab);
        if (tableWidget->columnCount() < 6)
            tableWidget->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(430, 180, 581, 301));
        label_12 = new QLabel(tab);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(730, 120, 101, 31));
        label_12->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        comboBox_2 = new QComboBox(tab);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(480, 120, 141, 31));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        label_2 = new QLabel(tab_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 30, 481, 391));
        label_2->setMaximumSize(QSize(1600000, 10000));
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_2->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/Capture d'\303\251cran 2025-02-16 083620.png")));
        tableWidget_2 = new QTableWidget(tab_2);
        if (tableWidget_2->columnCount() < 5)
            tableWidget_2->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(4, __qtablewidgetitem10);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(500, 60, 601, 301));
        tableWidget_2->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(21, 21, 21);\n"
""));
        commandLinkButton = new QCommandLinkButton(tab_2);
        commandLinkButton->setObjectName("commandLinkButton");
        commandLinkButton->setGeometry(QRect(910, 440, 141, 47));
        commandLinkButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(255, 255, 255);\n"
""));
        Bemploye_4 = new QPushButton(tab_2);
        Bemploye_4->setObjectName("Bemploye_4");
        Bemploye_4->setGeometry(QRect(1000, 140, 91, 29));
        Bemploye_4->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        Bemploye_3 = new QPushButton(tab_2);
        Bemploye_3->setObjectName("Bemploye_3");
        Bemploye_3->setGeometry(QRect(1000, 100, 91, 31));
        Bemploye_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        Bemploye_5 = new QPushButton(tab_2);
        Bemploye_5->setObjectName("Bemploye_5");
        Bemploye_5->setGeometry(QRect(1000, 180, 91, 31));
        Bemploye_5->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        Bemploye_6 = new QPushButton(tab_2);
        Bemploye_6->setObjectName("Bemploye_6");
        Bemploye_6->setGeometry(QRect(1000, 220, 93, 29));
        Bemploye_6->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        Bemploye_7 = new QPushButton(tab_2);
        Bemploye_7->setObjectName("Bemploye_7");
        Bemploye_7->setGeometry(QRect(1000, 260, 91, 31));
        Bemploye_7->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        Bemploye_8 = new QPushButton(tab_2);
        Bemploye_8->setObjectName("Bemploye_8");
        Bemploye_8->setGeometry(QRect(1000, 300, 93, 29));
        Bemploye_8->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_14 = new QLabel(tab_2);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(500, 120, 91, 20));
        label_14->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
        label_15 = new QLabel(tab_2);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(500, 210, 91, 20));
        label_15->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
        label_16 = new QLabel(tab_2);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(650, 120, 51, 20));
        label_16->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
        label_17 = new QLabel(tab_2);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(780, 120, 51, 20));
        label_17->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
        label_18 = new QLabel(tab_2);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(500, 290, 91, 20));
        label_18->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
        label_19 = new QLabel(tab_2);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(780, 210, 51, 20));
        label_19->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
        label_20 = new QLabel(tab_2);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(650, 210, 51, 20));
        label_20->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
        label_21 = new QLabel(tab_2);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(910, 110, 51, 20));
        label_21->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
        label_22 = new QLabel(tab_2);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(910, 210, 51, 20));
        label_22->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
        label_23 = new QLabel(tab_2);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(910, 290, 51, 20));
        label_23->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
        label_24 = new QLabel(tab_2);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(780, 290, 51, 20));
        label_24->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
        label_25 = new QLabel(tab_2);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(650, 290, 51, 20));
        label_25->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(8, 8, 8);"));
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
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        tableWidget_3 = new QTableWidget(tab_4);
        if (tableWidget_3->columnCount() < 4)
            tableWidget_3->setColumnCount(4);
        QFont font;
        font.setPointSize(7);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        __qtablewidgetitem11->setFont(font);
        tableWidget_3->setHorizontalHeaderItem(0, __qtablewidgetitem11);
        QFont font1;
        font1.setPointSize(8);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        __qtablewidgetitem12->setFont(font1);
        tableWidget_3->setHorizontalHeaderItem(1, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(2, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(3, __qtablewidgetitem14);
        tableWidget_3->setObjectName("tableWidget_3");
        tableWidget_3->setGeometry(QRect(50, 60, 501, 291));
        tableWidget_3->setStyleSheet(QString::fromUtf8("background-color: rgb(8, 8, 8);"));
        comboBox_3 = new QComboBox(tab_4);
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName("comboBox_3");
        comboBox_3->setGeometry(QRect(730, 60, 76, 21));
        Bemploye_13 = new QPushButton(tab_4);
        Bemploye_13->setObjectName("Bemploye_13");
        Bemploye_13->setGeometry(QRect(580, 160, 171, 29));
        Bemploye_13->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        Bemploye_14 = new QPushButton(tab_4);
        Bemploye_14->setObjectName("Bemploye_14");
        Bemploye_14->setGeometry(QRect(580, 210, 171, 29));
        Bemploye_14->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        Bemploye_15 = new QPushButton(tab_4);
        Bemploye_15->setObjectName("Bemploye_15");
        Bemploye_15->setGeometry(QRect(580, 250, 171, 29));
        Bemploye_15->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_29 = new QLabel(tab_4);
        label_29->setObjectName("label_29");
        label_29->setGeometry(QRect(570, 60, 141, 21));
        label_29->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_30 = new QLabel(tab_4);
        label_30->setObjectName("label_30");
        label_30->setGeometry(QRect(450, 140, 71, 20));
        label_30->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_31 = new QLabel(tab_4);
        label_31->setObjectName("label_31");
        label_31->setGeometry(QRect(450, 190, 71, 20));
        label_31->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_33 = new QLabel(tab_4);
        label_33->setObjectName("label_33");
        label_33->setGeometry(QRect(50, 140, 131, 20));
        label_33->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_34 = new QLabel(tab_4);
        label_34->setObjectName("label_34");
        label_34->setGeometry(QRect(50, 190, 131, 20));
        label_34->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        dateEdit_3 = new QDateEdit(tab_4);
        dateEdit_3->setObjectName("dateEdit_3");
        dateEdit_3->setGeometry(QRect(200, 140, 91, 21));
        dateEdit_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        dateEdit_4 = new QDateEdit(tab_4);
        dateEdit_4->setObjectName("dateEdit_4");
        dateEdit_4->setGeometry(QRect(320, 140, 91, 21));
        dateEdit_4->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        dateEdit_5 = new QDateEdit(tab_4);
        dateEdit_5->setObjectName("dateEdit_5");
        dateEdit_5->setGeometry(QRect(320, 190, 91, 21));
        dateEdit_5->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        dateEdit_6 = new QDateEdit(tab_4);
        dateEdit_6->setObjectName("dateEdit_6");
        dateEdit_6->setGeometry(QRect(200, 190, 91, 21));
        dateEdit_6->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        Bemploye_12 = new QPushButton(tab_4);
        Bemploye_12->setObjectName("Bemploye_12");
        Bemploye_12->setGeometry(QRect(580, 120, 171, 29));
        Bemploye_12->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tabWidget->addTab(tab_4, QString());
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
        tabWidget->setCurrentIndex(0);


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
        label_5->setText(QCoreApplication::translate("MainWindow", "date naissance ", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "poste", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "date d'embauche ", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "salaire", nullptr));
        label_->setText(QCoreApplication::translate("MainWindow", "email", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "mot de passe", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "t\303\251l\303\251phone", nullptr));
        checkBox_Femme->setText(QCoreApplication::translate("MainWindow", "Femme", nullptr));
        checkBox_Homme->setText(QCoreApplication::translate("MainWindow", "Homme", nullptr));
        btn_valider->setText(QCoreApplication::translate("MainWindow", "valider", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "cin", nullptr));
        btn_annuler->setText(QCoreApplication::translate("MainWindow", "annuler", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "recherche ", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "nom", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "poste", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "statut", nullptr));

        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "prenom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "identifiant", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "salaire", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "date naissance", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "date d'embauche ", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "  trier par :", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("MainWindow", "nom", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("MainWindow", "departement", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("MainWindow", "cin", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "acceuil", nullptr));
        label_2->setText(QString());
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Heures Pr\303\251vues", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Heures Travaill\303\251es", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_2->horizontalHeaderItem(3);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Performance (%)", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_2->horizontalHeaderItem(4);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Actions        ", nullptr));
        commandLinkButton->setText(QCoreApplication::translate("MainWindow", "autres m\303\251tiers", nullptr));
        Bemploye_4->setText(QCoreApplication::translate("MainWindow", "supprimer", nullptr));
        Bemploye_3->setText(QCoreApplication::translate("MainWindow", "mise \303\240 jour", nullptr));
        Bemploye_5->setText(QCoreApplication::translate("MainWindow", "mise \303\240 jour", nullptr));
        Bemploye_6->setText(QCoreApplication::translate("MainWindow", "supprimer", nullptr));
        Bemploye_7->setText(QCoreApplication::translate("MainWindow", "mise \303\240 jour", nullptr));
        Bemploye_8->setText(QCoreApplication::translate("MainWindow", "supprimer", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "employer 1", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "employer 2", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", " 160h", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "140h", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "employer 3", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", " 100h", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", " 160h", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", " 87.5%", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "   75%", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "31.25%", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "  50h", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", " 160h", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "statistique ", nullptr));
        Bemploye_10->setText(QCoreApplication::translate("MainWindow", " Capturer une image", nullptr));
        Bemploye_11->setText(QCoreApplication::translate("MainWindow", "Lancer la reconnaissance", nullptr));
        Bemploye_9->setText(QCoreApplication::translate("MainWindow", " Enregistrer un nouveau visage", nullptr));
        label_26->setText(QString());
        label_27->setText(QCoreApplication::translate("MainWindow", " Statut de l\342\200\231utilisateur", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", " Nom de l' employ\303\251 ", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "m\303\251tiers", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_3->horizontalHeaderItem(0);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "    \360\237\223\214 Nom du contrat ", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget_3->horizontalHeaderItem(1);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "\360\237\223\205 Date de d\303\251but", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget_3->horizontalHeaderItem(2);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "\360\237\223\205 Date de Fin", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget_3->horizontalHeaderItem(3);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "\342\234\205 Statut", nullptr));
        comboBox_3->setItemText(0, QCoreApplication::translate("MainWindow", "Tous", nullptr));
        comboBox_3->setItemText(1, QCoreApplication::translate("MainWindow", "Actif", nullptr));
        comboBox_3->setItemText(2, QCoreApplication::translate("MainWindow", "expir\303\251", nullptr));

        Bemploye_13->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217 Modifier un contrat ", nullptr));
        Bemploye_14->setText(QCoreApplication::translate("MainWindow", "\342\235\214 Supprimer un contrat  ", nullptr));
        Bemploye_15->setText(QCoreApplication::translate("MainWindow", "\360\237\223\244 Exporter en PDF", nullptr));
        label_29->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215 Filtrer par statut", nullptr));
        label_30->setText(QCoreApplication::translate("MainWindow", "\360\237\237\242 Actif", nullptr));
        label_31->setText(QCoreApplication::translate("MainWindow", "\360\237\224\264 Expir\303\251", nullptr));
        label_33->setText(QCoreApplication::translate("MainWindow", "Contrat Consultant", nullptr));
        label_34->setText(QCoreApplication::translate("MainWindow", "Contrat Consultant", nullptr));
        Bemploye_12->setText(QCoreApplication::translate("MainWindow", "\342\236\225 Ajouter un contrat ", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "contrats", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
