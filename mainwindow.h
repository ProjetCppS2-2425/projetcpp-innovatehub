#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDate>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include "ui_mainwindow.h"
#include <QPainter>     // Pour QPainter
#include <QFont>        // Pour QFont
#include <QDebug>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QHBoxLayout>
#include <QPageSize>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>



namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void on_comboBox_TriEmployes_currentIndexChanged(int index);


private slots:
    void on_btn_Valider_clicked();   // Déclaration du slot pour le bouton "Valider"
    void on_btn_Annuler_clicked();   // Déclaration du slot pour le bouton "Annuler"

    void on_deleteButton_clicked();
    void afficherEmployes();
    void on_modifierButton_clicked();
    void on_tableWidgetEmployes_itemSelectionChanged();
    void afficherCamembert();
    void on_pushButton_Recherche_clicked();
    void on_pushButton_4TriEmployes_clicked();
    void remplirComboBoxEmployes();
    void enregistrerHeures();
    void exportSelectedContractToPDF() ;
      // Déclaration de la fonction d'exportation PDF
    void verifyFace(const QString &imagePath, const QString &faceToken);
    void startCameraAndCapture();
    void registerFace(const QString &imagePath);
    void updateFaceIdAndToken(const QString &faceId, const QString &faceToken, int currentEmployeeId);
    void getFaceId(const QString &faceToken);
    void enregistrerFaceTokenDansBDD(const QString &faceToken);
private:
    Ui::MainWindow *ui;
   int currentEmployeeId;
void searchEmployes(const QString &searchText);
    void sortEmployes(const QString &criterion);
QPushButton *exportButton;  // Déclaration du bouton
   QNetworkAccessManager *networkManager;
 QHBoxLayout *layoutCamembert;
};

#endif // MAINWINDOW_H
