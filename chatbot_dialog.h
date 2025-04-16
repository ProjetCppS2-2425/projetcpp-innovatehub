#ifndef CHATBOT_DIALOG_H
#define CHATBOT_DIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include "openai_chatbot.h"

class ChatbotDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatbotDialog(QWidget *parent = nullptr);
    ~ChatbotDialog();

private slots:
    void onSendButtonClicked();
    void onResponseReceived(const QString &response);

private:
    QTextEdit *chatDisplay;
    QLineEdit *userInput;
    QPushButton *sendButton;
    OpenAIChatbot *chatbot;
};

#endif // CHATBOT_DIALOG_H 