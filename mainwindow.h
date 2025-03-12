#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDate>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_Valider_clicked();   // Déclaration du slot pour le bouton "Valider"
    void on_btn_Annuler_clicked();   // Déclaration du slot pour le bouton "Annuler"

void on_deleteButton_clicked();
    void on_pushButtonAfficher_clicked();
void on_modifierButton_clicked();
    void on_tableWidgetEmployes_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
