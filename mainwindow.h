#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qpushbutton.h"
#include <QMainWindow>
#include <QMap>
#include <QPixmap>
#include <QIcon>
#include "transaction.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QRegularExpression>
#include <QTimer>
#include "chatbot_dialog.h"

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
    void on_pushButton_8_Transaction_clicked(); // Modifier une transaction à partir de la liste
    void on_pushButton_8_clicked();            // Supprimer une transaction
    void showTransactionContextMenu(const QPoint &pos);
    void deleteTransaction();
    void setupTableContextMenu();
    
    // Fonction pour rechercher une transaction par référence
    void on_pushButtonRechercherTransaction_clicked();

    void on_pushButton_5_clicked();
    
    // Nouvelles fonctions pour la recherche et le tri
    void on_pushButton_3Transaction_clicked();  // Rechercher par référence/nom payeur/statut
    void on_pushButton_4Transaction_clicked();  // Trier la liste des transactions
    void on_comboBox_3Transaction_currentIndexChanged(int index); // Gérer le changement de critère de tri
    
    // Nouvelles fonctions pour l'optimisation de l'interface
    void on_pushButtonRechercherParReference_clicked();
    void on_pushButtonRechercherAvance_clicked();
    void on_pushButtonSupprimer_clicked();
    void on_pushButtonAjouter_clicked();
    void on_pushButtonModifier_clicked();
    void on_pushButtonReinitialiser_clicked();
    void on_pushButtonTrier_clicked();
    
    // Slot pour gérer le double-clic sur une ligne du tableau
    void on_tableViewAFFICHELIST_3_doubleClicked(const QModelIndex &index);
    
    // Nouvelles méthodes pour la gestion des transactions
    void loadClientsAndPartenaires();
    void onTransactionTypeChanged(int index);

    void on_pushButton_9_Transaction_clicked();
    void showTransactionCharts();

    // UI Button handlers
    void on_pushButton_listePartenaires_clicked();
    void on_pushButton_transaction_editClient_clicked();

    void on_pushButton_6_transaction_clicked();
    void setOpenAIApiKey(const QString &apiKey);

private:
    Ui::MainWindow *ui;
    QMap<QPushButton*, int> pageMap;
    void setupTransactionValidators();
    void refreshTransactionTable();
    int currentTransactionReference = -1;
    
    bool validateTransactionAmount(double montant, const QString &type);
    bool validateTransaction();
    void searchTransactions(const QString &searchText);
    void sortTransactions(const QString &criterion);
    void connectTransactionButtons();

    QNetworkAccessManager *networkManager;
    ChatbotDialog *chatbotDialog;
};
#endif // MAINWINDOW_H
