#ifndef OPENAI_CHATBOT_H
#define OPENAI_CHATBOT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlError>

class OpenAIChatbot : public QObject
{
    Q_OBJECT

public:
    explicit OpenAIChatbot(QObject *parent = nullptr);
    ~OpenAIChatbot();

    void sendMessage(const QString &message);

signals:
    void responseReceived(const QString &response);
    void error(const QString &errorMessage);

private:
    QNetworkAccessManager *networkManager;
    QString apiKey;
    void loadApiKey();
    
    // Transaction-related methods
    QString getTransactionsByPayer(const QString &payerName);
    QString getPayersByAmount(double minAmount);
    QString getTransactionStatistics();
    QString processTransactionQuery(const QString &query);
    
    // New analysis methods
    QString getMostSignificantPayer();
    QString getLargestTransaction();
    QString getNthLargestTransaction(int n);
    QString getRecentTransactions(int count);
    QString getMonthlySpending();
    QString getYearlySpending(int year);
    QString getFailedTransactions();
    QString getHighValueTransactions(double threshold);
    QString getTransactionDayDistribution();
    QString getQuarterlyIncome();
    QString getAverageTransactionValue();
    
    // Client analysis methods
    QString getOldestClient();
    QString getMostActiveClient();
    QString getInactiveClients(int months);
    QString getTopClientsByValue(int count);
    QString getLatePayingClients();
    QString getMostFrequentCustomers();
    
    // Trend analysis methods
    QString getDailySummary();
    QString getMonthlyComparison();
    QString getTransactionPeaks();
    QString getSpendingAnomalies();
    QString getYearlyTrends();
    
    // Helper methods
    QString formatAmount(double amount);
    QString formatDate(const QDate &date);
    QString getTransactionStatus(const QString &status);
    QString getHelpMessage(const QString &query);
    QString extractTimePeriod(const QString &text);

private slots:
    void onNetworkReply(QNetworkReply *reply);
};

#endif // OPENAI_CHATBOT_H 