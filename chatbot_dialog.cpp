#include "chatbot_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

ChatbotDialog::ChatbotDialog(QWidget *parent)
    : QDialog(parent)
    , chatDisplay(new QTextEdit(this))
    , userInput(new QLineEdit(this))
    , sendButton(new QPushButton("Send", this))
    , chatbot(new OpenAIChatbot(this))
{
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Set up chat display
    chatDisplay->setReadOnly(true);
    chatDisplay->setMinimumHeight(300);
    mainLayout->addWidget(chatDisplay);
    
    // Create input layout
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(userInput);
    inputLayout->addWidget(sendButton);
    mainLayout->addLayout(inputLayout);
    
    // Connect signals
    connect(sendButton, &QPushButton::clicked, this, &ChatbotDialog::onSendButtonClicked);
    connect(userInput, &QLineEdit::returnPressed, this, &ChatbotDialog::onSendButtonClicked);
    connect(chatbot, &OpenAIChatbot::responseReceived, this, &ChatbotDialog::onResponseReceived);
    
    // Set window properties
    setWindowTitle("Chatbot");
    setMinimumWidth(400);
    
    // Add welcome message
    chatDisplay->append("Chatbot: Hello! How can I help you today?");
}

ChatbotDialog::~ChatbotDialog()
{
    delete chatbot;
}

void ChatbotDialog::onSendButtonClicked()
{
    QString message = userInput->text().trimmed();
    if (message.isEmpty()) {
        return;
    }
    
    // Display user message
    chatDisplay->append("You: " + message);
    userInput->clear();
    
    // Send message to chatbot
    chatbot->sendMessage(message);
}

void ChatbotDialog::onResponseReceived(const QString &response)
{
    chatDisplay->append("Chatbot: " + response);
} 