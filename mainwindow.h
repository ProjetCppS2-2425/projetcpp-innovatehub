#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include "conseil.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void viderChamps();

private slots:
    void on_affconseil_clicked();
    void on_tableViewConseil_clicked(const QModelIndex &index);
    void on_suppconseil_clicked();
    void on_editconseil_clicked();
    void on_ajoutconseil_clicked();




    void on_pdfconseil_clicked();

    void on_recherconseil_clicked();

    void on_trieconseil_clicked();

    void on_statconseil_clicked();

    void on_historique_clicked();

    void on_mailconseil_clicked();

private:
    Ui::MainWindow *ui; // Pointeur vers l'interface utilisateur
        QSqlTableModel *model; // Pointeur vers le modèle de table
    int currentConseilId = -1; // ID du conseil courant
        Conseil c;
    void peuplerComboBoxRecherche(); // Déclaration de la fonction ici

    int selectedId = -1;

};

#endif // MAINWINDOW_H
