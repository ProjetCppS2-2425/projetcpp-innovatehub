// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QPushButton>
#include "partenaire.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation slots
    void showEmploye();
    void showClient();
    void showConseil();
    void showTransaction();
    void showPartenaires();

    // Partenaire CRUD slots
    void on_pushButton_ajouter_clicked();
    void on_pushButton_annuller_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_recherche_clicked();
    void on_pushButton_trier_clicked();
    void on_pushButton_exporter_clicked();

    void refreshPartenaireTable();

private:
    Ui::MainWindow *ui;
    QMap<QPushButton*, int> pageMap;
    Partenaire tmpPartenaire;
    void clearPartenaireFields();
    bool validateId();
    bool validateMontant();
    void setupConnections();
};
#endif // MAINWINDOW_H
