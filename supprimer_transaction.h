#ifndef TRANSACTIONWIDGET_H
#define TRANSACTIONWIDGET_H

#include <QWidget>

namespace Ui {
class TransactionWidget;
}

class TransactionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionWidget(QWidget *parent = nullptr);
    ~TransactionWidget();

private slots:
    void on_pushButton_2_supprimer_Transaction_clicked(); // Bouton "Supprimer"
    void on_pushButton_supprimer_Transaction_clicked();   // Bouton "Annuler"

private:
    Ui::TransactionWidget *ui;

    bool supprimerTransactionParID(const QString &id); // Fonction de suppression
};

#endif // TRANSACTIONWIDGET_H
