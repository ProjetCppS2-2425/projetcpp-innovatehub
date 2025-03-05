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

private:
    Ui::TransactionWidget *ui;
};

#endif // TRANSACTIONWIDGET_H
