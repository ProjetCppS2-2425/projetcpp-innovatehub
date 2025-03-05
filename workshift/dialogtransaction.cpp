#include "dialogtransaction.h"
#include "ui_DialogTransaction.h"  // This is necessary to include the UI class generated from the .ui file

DialogTransaction::DialogTransaction(QWidget *parent) :
    Qdialog(parent),
    ui(new DialogTransaction)  // Initialize the UI pointer
{
    ui->setupUi(this);  // Set up the UI for the dialog
}

DialogTransaction::~DialogTransaction()
{
    delete ui;  // Clean up the UI object
}
