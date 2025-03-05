#include "transactionwidget.h"
#include "ui_Supprimer_Transaction.h"

Ui::Supprimer_Transaction::Supprimer_Transaction(QWidget *parent)
    : QWidget(parent)
{
    ui->setupUi(this);
}
TransactionWidget::~TransactionWidget()
{
    delete ui;
}
