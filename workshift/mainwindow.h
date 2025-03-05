#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qpushbutton.h"
#include <QMainWindow>
#include <QMap>
#include <QPixmap>
#include <QIcon>
#include "transaction.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void showEmploye();
    void showClient();
    void showConseil();
    void showTransaction();
    void showPartenaires();

    void on_pushButton_2Transaction_clicked();  // Ajouter une transaction


private:
    Ui::MainWindow *ui;
    transaction T;
    QMap<QPushButton*, int> pageMap;
};
#endif // MAINWINDOW_H
