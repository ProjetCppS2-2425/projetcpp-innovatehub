#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDate>
#include "client.h"
#include "ui_mainwindow.h"

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
    void on_pushButtonAJOUTERCLIENT_clicked();


private slots:
    void on_pushButton_afficher_clicked();


    void on_pushButtonSPPRIMERCLIENT_clicked();

private:
    Ui::MainWindow *ui;
    Client Etmp; // Objet pour appeler afficher()
};
#endif // MAINWINDOW_H
