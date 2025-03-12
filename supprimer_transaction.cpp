#include "ui_supprimer_transaction.h"

Ui::Supprimer_Transaction::Supprimer_Transaction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Supprimer_Transaction)
{
    ui->setupUi(this);
}

Ui::Supprimer_Transaction::~Supprimer_Transaction()
{
    delete ui;
}
