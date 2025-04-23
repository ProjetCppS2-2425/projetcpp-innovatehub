#include "openai_chatbot.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QDate>

OpenAIChatbot::OpenAIChatbot(QObject *parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
{
    loadApiKey();
    connect(networkManager, &QNetworkAccessManager::finished, this, &OpenAIChatbot::onNetworkReply);
}

OpenAIChatbot::~OpenAIChatbot()
{
    delete networkManager;
}

void OpenAIChatbot::loadApiKey()
{
    QSettings settings;
    apiKey = settings.value("openai/api_key").toString();
    
    if (apiKey.isEmpty()) {
        emit error("OpenAI API key not configured. Please set it in the settings.");
    }
}

QString OpenAIChatbot::getTransactionsByPayer(const QString &payerName)
{
    QSqlQuery query;
    query.prepare("SELECT t.REFERENCE, t.MONTANT, t.MODEPAIEMENT, t.DATEEE, t.STATUTT, t.TYPEE "
                  "FROM SYRINE.TRANSACTION t "
                  "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                  "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                  "WHERE UPPER(c.NOM) LIKE UPPER(:name) OR UPPER(p.NOMENTREPRISE) LIKE UPPER(:name) "
                  "ORDER BY t.DATEEE DESC");
    query.bindValue(":name", "%" + payerName + "%");

    if (!query.exec()) {
        return "Error: " + query.lastError().text();
    }

    QString result = "Transactions for " + payerName + ":\n\n";
    bool found = false;

    while (query.next()) {
        found = true;
        result += QString("Reference: %1\n").arg(query.value("REFERENCE").toString());
        result += QString("Amount: %1\n").arg(query.value("MONTANT").toString());
        result += QString("Payment Method: %1\n").arg(query.value("MODEPAIEMENT").toString());
        result += QString("Date: %1\n").arg(query.value("DATEEE").toDate().toString("yyyy-MM-dd"));
        result += QString("Status: %1\n").arg(query.value("STATUTT").toString());
        result += QString("Type: %1\n\n").arg(query.value("TYPEE").toString());
    }

    if (!found) {
        return "No transactions found for " + payerName;
    }

    return result;
}

QString OpenAIChatbot::getPayersByAmount(double minAmount)
{
    QSqlQuery query;
    query.prepare("SELECT c.NOM as PAYER_NAME, p.NOMENTREPRISE as PARTNER_NAME, "
                  "SUM(t.MONTANT) as TOTAL_AMOUNT, COUNT(*) as TRANSACTION_COUNT "
                  "FROM SYRINE.TRANSACTION t "
                  "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                  "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                  "GROUP BY c.NOM, p.NOMENTREPRISE "
                  "HAVING SUM(t.MONTANT) >= :minAmount "
                  "ORDER BY TOTAL_AMOUNT DESC");
    query.bindValue(":minAmount", minAmount);

    if (!query.exec()) {
        return "Error: " + query.lastError().text();
    }

    QString result = QString("Payers with total amount >= %1:\n\n").arg(minAmount);
    bool found = false;

    while (query.next()) {
        found = true;
        QString payerName = query.value("PAYER_NAME").toString();
        if (payerName.isEmpty()) {
            payerName = query.value("PARTNER_NAME").toString();
        }
        result += QString("Payer: %1\n").arg(payerName);
        result += QString("Total Amount: %1\n").arg(query.value("TOTAL_AMOUNT").toString());
        result += QString("Number of Transactions: %1\n\n").arg(query.value("TRANSACTION_COUNT").toString());
    }

    if (!found) {
        return QString("No payers found with total amount >= %1").arg(minAmount);
    }

    return result;
}

// Helper methods
QString OpenAIChatbot::formatAmount(double amount) {
    return QString::number(amount, 'f', 2) + " TND";
}

QString OpenAIChatbot::formatDate(const QDate &date) {
    return date.toString("dd/MM/yyyy");
}

QString OpenAIChatbot::getTransactionStatus(const QString &status) {
    if (status == "En attente") return "Pending";
    if (status == "Validé") return "Completed";
    if (status == "Annulé") return "Failed";
    return status;
}

// Analysis methods implementation
QString OpenAIChatbot::getMostSignificantPayer() {
    QSqlQuery query;
    query.prepare("SELECT c.NOM as PAYER_NAME, p.NOMENTREPRISE as PARTNER_NAME, "
                  "SUM(t.MONTANT) as TOTAL_AMOUNT, COUNT(*) as TRANSACTION_COUNT "
                  "FROM SYRINE.TRANSACTION t "
                  "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                  "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                  "GROUP BY c.NOM, p.NOMENTREPRISE "
                  "ORDER BY TOTAL_AMOUNT DESC");
    
    if (!query.exec() || !query.next()) {
        return "Error retrieving most significant payer";
    }
    
    QString payerName = query.value("PAYER_NAME").toString();
    if (payerName.isEmpty()) {
        payerName = query.value("PARTNER_NAME").toString();
    }
    
    return QString("Most significant payer: %1\n"
                  "Total amount: %2\n"
                  "Number of transactions: %3")
            .arg(payerName)
            .arg(formatAmount(query.value("TOTAL_AMOUNT").toDouble()))
            .arg(query.value("TRANSACTION_COUNT").toString());
}

QString OpenAIChatbot::getLargestTransaction() {
    QSqlQuery query;
    query.prepare("SELECT t.REFERENCE, t.MONTANT, t.DATEEE, t.TYPEE, "
                  "COALESCE(c.NOM, p.NOMENTREPRISE) as PAYER_NAME "
                  "FROM SYRINE.TRANSACTION t "
                  "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                  "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                  "ORDER BY t.MONTANT DESC");
    
    if (!query.exec() || !query.next()) {
        return "Error retrieving largest transaction";
    }
    
    return QString("Largest transaction:\n"
                  "Reference: %1\n"
                  "Amount: %2\n"
                  "Date: %3\n"
                  "Type: %4\n"
                  "Payer: %5")
            .arg(query.value("REFERENCE").toString())
            .arg(formatAmount(query.value("MONTANT").toDouble()))
            .arg(formatDate(query.value("DATEEE").toDate()))
            .arg(query.value("TYPEE").toString())
            .arg(query.value("PAYER_NAME").toString());
}

QString OpenAIChatbot::getRecentTransactions(int count) {
    QSqlQuery query;
    query.prepare("SELECT t.REFERENCE, t.MONTANT, t.DATEEE, t.TYPEE, t.STATUTT, "
                  "COALESCE(c.NOM, p.NOMENTREPRISE) as PAYER_NAME "
                  "FROM SYRINE.TRANSACTION t "
                  "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                  "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                  "ORDER BY t.DATEEE DESC "
                  "LIMIT :count");
    query.bindValue(":count", count);
    
    if (!query.exec()) {
        return "Error retrieving recent transactions";
    }
    
    QString result = QString("Last %1 transactions:\n\n").arg(count);
    while (query.next()) {
        result += QString("Reference: %1\n"
                         "Amount: %2\n"
                         "Date: %3\n"
                         "Type: %4\n"
                         "Status: %5\n"
                         "Payer: %6\n\n")
                .arg(query.value("REFERENCE").toString())
                .arg(formatAmount(query.value("MONTANT").toDouble()))
                .arg(formatDate(query.value("DATEEE").toDate()))
                .arg(query.value("TYPEE").toString())
                .arg(getTransactionStatus(query.value("STATUTT").toString()))
                .arg(query.value("PAYER_NAME").toString());
    }
    
    return result;
}

QString OpenAIChatbot::getMonthlySpending() {
    QSqlQuery query;
    query.prepare("SELECT SUM(MONTANT) as TOTAL_SPENDING "
                  "FROM SYRINE.TRANSACTION "
                  "WHERE TYPEE = 'Achat' "
                  "AND DATEEE >= :start_date");
    query.bindValue(":start_date", QDate::currentDate().addMonths(-1));
    
    if (!query.exec() || !query.next()) {
        return "Error retrieving monthly spending";
    }
    
    return QString("Total spending this month: %1")
            .arg(formatAmount(query.value("TOTAL_SPENDING").toDouble()));
}

QString OpenAIChatbot::getFailedTransactions() {
    QSqlQuery query;
    query.prepare("SELECT t.REFERENCE, t.MONTANT, t.DATEEE, t.TYPEE, "
                  "COALESCE(c.NOM, p.NOMENTREPRISE) as PAYER_NAME "
                  "FROM SYRINE.TRANSACTION t "
                  "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                  "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                  "WHERE t.STATUTT = 'Annulé' "
                  "ORDER BY t.DATEEE DESC");
    
    if (!query.exec()) {
        return "Error retrieving failed transactions";
    }
    
    QString result = "Failed transactions:\n\n";
    bool found = false;
    
    while (query.next()) {
        found = true;
        result += QString("Reference: %1\n"
                         "Amount: %2\n"
                         "Date: %3\n"
                         "Type: %4\n"
                         "Payer: %5\n\n")
                .arg(query.value("REFERENCE").toString())
                .arg(formatAmount(query.value("MONTANT").toDouble()))
                .arg(formatDate(query.value("DATEEE").toDate()))
                .arg(query.value("TYPEE").toString())
                .arg(query.value("PAYER_NAME").toString());
    }
    
    if (!found) {
        return "No failed transactions found";
    }
    
    return result;
}

QString OpenAIChatbot::getHighValueTransactions(double threshold) {
    QSqlQuery query;
    query.prepare("SELECT t.REFERENCE, t.MONTANT, t.DATEEE, t.TYPEE, t.STATUTT, "
                  "COALESCE(c.NOM, p.NOMENTREPRISE) as PAYER_NAME "
                  "FROM SYRINE.TRANSACTION t "
                  "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                  "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                  "WHERE t.MONTANT >= :threshold "
                  "ORDER BY t.MONTANT DESC");
    query.bindValue(":threshold", threshold);
    
    if (!query.exec()) {
        return "Error retrieving high value transactions";
    }
    
    QString result = QString("Transactions above %1:\n\n").arg(formatAmount(threshold));
    bool found = false;
    
    while (query.next()) {
        found = true;
        result += QString("Reference: %1\n"
                         "Amount: %2\n"
                         "Date: %3\n"
                         "Type: %4\n"
                         "Status: %5\n"
                         "Payer: %6\n\n")
                .arg(query.value("REFERENCE").toString())
                .arg(formatAmount(query.value("MONTANT").toDouble()))
                .arg(formatDate(query.value("DATEEE").toDate()))
                .arg(query.value("TYPEE").toString())
                .arg(getTransactionStatus(query.value("STATUTT").toString()))
                .arg(query.value("PAYER_NAME").toString());
    }
    
    if (!found) {
        return QString("No transactions found above %1").arg(formatAmount(threshold));
    }
    
    return result;
}

QString OpenAIChatbot::getTransactionDayDistribution() {
    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(DATEEE, 'Day') as DAY_NAME, COUNT(*) as COUNT "
                  "FROM SYRINE.TRANSACTION "
                  "GROUP BY TO_CHAR(DATEEE, 'Day') "
                  "ORDER BY COUNT DESC");
    
    if (!query.exec()) {
        return "Error retrieving transaction day distribution";
    }
    
    QString result = "Transaction distribution by day:\n\n";
    while (query.next()) {
        result += QString("%1: %2 transactions\n")
                .arg(query.value("DAY_NAME").toString().trimmed())
                .arg(query.value("COUNT").toString());
    }
    
    return result;
}

QString OpenAIChatbot::getQuarterlyIncome() {
    QSqlQuery query;
    query.prepare("SELECT SUM(MONTANT) as TOTAL_INCOME "
                  "FROM SYRINE.TRANSACTION "
                  "WHERE TYPEE = 'Vente' "
                  "AND DATEEE >= :start_date");
    query.bindValue(":start_date", QDate::currentDate().addMonths(-3));
    
    if (!query.exec() || !query.next()) {
        return "Error retrieving quarterly income";
    }
    
    return QString("Total income last quarter: %1")
            .arg(formatAmount(query.value("TOTAL_INCOME").toDouble()));
}

QString OpenAIChatbot::getAverageTransactionValue() {
    QSqlQuery query;
    query.prepare("SELECT AVG(MONTANT) as AVG_VALUE, COUNT(*) as COUNT "
                  "FROM SYRINE.TRANSACTION");
    
    if (!query.exec() || !query.next()) {
        return "Error retrieving average transaction value";
    }
    
    return QString("Average transaction value: %1\n"
                  "Total number of transactions: %2")
            .arg(formatAmount(query.value("AVG_VALUE").toDouble()))
            .arg(query.value("COUNT").toString());
}

QString OpenAIChatbot::processTransactionQuery(const QString &query)
{
    QString lowerQuery = query.toLower();
    
    // Helper function to extract numbers from queries
    auto extractNumber = [](const QString &text) -> double {
        QRegularExpression rx("(\\d+(?:\\.\\d+)?)");
        QRegularExpressionMatch match = rx.match(text);
        return match.hasMatch() ? match.captured(1).toDouble() : 0.0;
    };

    // Helper function to extract ordinal numbers
    auto extractOrdinal = [](const QString &text) -> int {
        QRegularExpression rx("(first|second|third|fourth|fifth|sixth|seventh|eighth|ninth|tenth)");
        QRegularExpressionMatch match = rx.match(text);
        if (match.hasMatch()) {
            QString ordinal = match.captured(1);
            if (ordinal == "first") return 1;
            if (ordinal == "second") return 2;
            if (ordinal == "third") return 3;
            if (ordinal == "fourth") return 4;
            if (ordinal == "fifth") return 5;
            if (ordinal == "sixth") return 6;
            if (ordinal == "seventh") return 7;
            if (ordinal == "eighth") return 8;
            if (ordinal == "ninth") return 9;
            if (ordinal == "tenth") return 10;
        }
        return 0;
    };

    // Helper function to extract year from query
    auto extractYear = [](const QString &text) -> int {
        QRegularExpression rx("(\\d{4})");
        QRegularExpressionMatch match = rx.match(text);
        return match.hasMatch() ? match.captured(1).toInt() : 0;
    };

    // Helper function to check if query contains any of the given keywords
    auto containsAny = [&lowerQuery](const QStringList &keywords) -> bool {
        for (const QString &keyword : keywords) {
            if (lowerQuery.contains(keyword)) return true;
        }
        return false;
    };

    // Helper function to check if query contains all of the given keywords
    auto containsAll = [&lowerQuery](const QStringList &keywords) -> bool {
        for (const QString &keyword : keywords) {
            if (!lowerQuery.contains(keyword)) return false;
        }
        return true;
    };

    // Helper function to check for typos
    auto hasTypo = [&lowerQuery](const QString &word, const QStringList &variations) -> bool {
        for (const QString &variation : variations) {
            if (lowerQuery.contains(variation)) return true;
        }
        return false;
    };

    // Helper function to check for client name
    auto hasClientName = [&lowerQuery]() -> bool {
        QSqlQuery query;
        query.prepare("SELECT NOM FROM SYRINE.CLIENT");
        if (query.exec()) {
            while (query.next()) {
                QString name = query.value(0).toString().toLower();
                if (lowerQuery.contains(name)) return true;
            }
        }
        return false;
    };

    // Transaction analysis queries
    if (containsAny({"most significant payer", "biggest payer", "largest payer", "who pays the most", "top payer", "highest payer"})) {
        return getMostSignificantPayer();
    }
    else if (containsAny({"largest transaction", "biggest transaction", "highest transaction", "show the latest transaction", "last transaction", "most recent transaction"})) {
        int ordinal = extractOrdinal(lowerQuery);
        if (ordinal > 0) {
            return getNthLargestTransaction(ordinal);
        }
        return getLargestTransaction();
    }
    else if ((containsAny({"recent", "last", "latest", "show", "display"}) && containsAny({"transaction", "transactions"})) ||
             hasTypo("recent", {"recnt", "recent", "resent"})) {
        int count = extractNumber(query);
        if (count == 0) count = 5; // default to 5 if no number specified
        return getRecentTransactions(count);
    }
    else if (containsAny({"spend", "expense", "cost", "payment"}) || hasTypo("spending", {"spendings", "spending", "spendin"})) {
        int year = extractYear(query);
        if (year > 0) {
            return getYearlySpending(year);
        }
        QString period = extractTimePeriod(lowerQuery);
        if (period == "month") return getMonthlySpending();
        if (period == "quarter") return getQuarterlyIncome();
        return getMonthlySpending(); // default
    }
    else if (containsAny({"failed", "cancelled", "unsuccessful", "error", "problem"}) || 
             hasTypo("failed", {"ailed", "faile", "faild"})) {
        return getFailedTransactions();
    }
    else if (containsAny({"over", "above", "more than", "high value", "large transaction"})) {
        double threshold = extractNumber(query);
        if (threshold == 0) threshold = 1000.0; // default threshold
        return getHighValueTransactions(threshold);
    }
    else if (containsAny({"day", "daily", "weekday"}) && containsAny({"most", "busiest", "active", "frequent", "distribution"})) {
        return getTransactionDayDistribution();
    }
    else if (containsAny({"income", "revenue", "sales", "earnings"})) {
        QString period = extractTimePeriod(lowerQuery);
        if (period == "quarter") return getQuarterlyIncome();
        return getQuarterlyIncome(); // default
    }
    else if (containsAny({"average", "mean", "typical", "usual"})) {
        return getAverageTransactionValue();
    }
    else if (containsAny({"client", "customer", "payer", "partner"}) || hasTypo("client", {"clients", "client", "clien"})) {
        if (containsAny({"oldest", "first", "original"})) {
            return getOldestClient();
        }
        else if (containsAny({"active", "frequent", "regular", "most active"})) {
            return getMostActiveClient();
        }
        else if (containsAny({"inactive", "dormant", "not active", "no activity"}) || 
                hasTypo("inactive", {"nactive", "inactive", "inactiv"})) {
            int months = extractNumber(query);
            if (months == 0) months = 6; // default to 6 months
            return getInactiveClients(months);
        }
        else if (containsAny({"top", "best", "highest", "most valuable"}) || 
                (containsAny({"order", "sort", "rank"}) && containsAny({"value", "amount", "total"}))) {
            int count = extractNumber(query);
            if (count == 0) count = 3; // default to top 3
            return getTopClientsByValue(count);
        }
        else if (containsAny({"late", "delayed", "overdue", "pending"})) {
            return getLatePayingClients();
        }
        else if (hasClientName()) {
            // If a client name is found, show their transactions
            QSqlQuery query;
            query.prepare("SELECT NOM FROM SYRINE.CLIENT");
            if (query.exec()) {
                while (query.next()) {
                    QString name = query.value(0).toString().toLower();
                    if (lowerQuery.contains(name)) {
                        return getTransactionsByPayer(name);
                    }
                }
            }
        }
    }
    else if (containsAny({"trend", "pattern", "analysis", "summary", "report"}) || 
             hasTypo("transaction", {"ransaction", "transacton", "transactio"})) {
        if (containsAny({"daily", "day", "weekday"})) {
            return getDailySummary();
        }
        else if (containsAny({"month", "monthly", "bimonthly"})) {
            return getMonthlyComparison();
        }
        else if (containsAny({"peak", "busy", "high activity"}) || 
                hasTypo("peaks", {"peaks", "peack", "peak"})) {
            return getTransactionPeaks();
        }
        else if (containsAny({"anomaly", "unusual", "strange", "outlier"})) {
            return getSpendingAnomalies();
        }
        else if (containsAny({"year", "yearly", "annual"})) {
            return getYearlyTrends();
        }
    }
    else if (hasTypo("what can you do", {"what acn u do", "what can u do", "what can you do"})) {
        return getHelpMessage(query);
    }
    
    // If no specific query is detected and API key is configured, send to OpenAI
    if (!apiKey.isEmpty()) {
        QNetworkRequest request(QUrl("https://api.openai.com/v1/chat/completions"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());

        QJsonObject json;
        json["model"] = "gpt-3.5-turbo";
        json["messages"] = QJsonArray() << QJsonObject{{"role", "user"}, {"content", query}};
        json["max_tokens"] = 150;

        networkManager->post(request, QJsonDocument(json).toJson());
        return "Processing your request...";
    }

    return getHelpMessage(query);
}

QString OpenAIChatbot::getHelpMessage(const QString &query)
{
    if (query.contains(QRegularExpression("[éèêëàâäîïôöùûüç]"))) {
        return "Je peux vous aider à analyser les transactions et les clients. Essayez de me demander :\n\n"
               "Analyse des transactions :\n"
               "- Paiement le plus important\n"
               "- Transaction la plus importante\n"
               "- Transactions récentes (par exemple, 'Afficher les 3 dernières transactions')\n"
               "- Dépenses mensuelles/trimestrielles\n"
               "- Transactions échouées\n"
               "- Transactions à haute valeur\n"
               "- Distribution des transactions par jour\n"
               "- Valeur moyenne des transactions\n\n"
               "Analyse des clients :\n"
               "- Client le plus ancien\n"
               "- Client le plus actif\n"
               "- Clients inactifs\n"
               "- Meilleurs clients par valeur\n"
               "- Clients en retard de paiement\n\n"
               "Analyse des tendances :\n"
               "- Modèles de transactions quotidiens\n"
               "- Comparaisons mensuelles\n"
               "- Pics de transactions\n"
               "- Anomalies de dépenses\n"
               "- Tendances annuelles\n\n"
               "Pour d'autres questions, veuillez configurer la clé API OpenAI dans les paramètres.";
    } else {
        return "I can help you analyze transactions and clients. Try asking me about:\n\n"
               "Transaction Analysis:\n"
               "- Most significant payer\n"
               "- Largest transaction\n"
               "- Recent transactions (e.g., 'Show last 3 transactions')\n"
               "- Monthly/quarterly spending\n"
               "- Failed transactions\n"
               "- High value transactions\n"
               "- Transaction day distribution\n"
               "- Average transaction value\n\n"
               "Client Analysis:\n"
               "- Oldest client\n"
               "- Most active client\n"
               "- Inactive clients\n"
               "- Top clients by value\n"
               "- Late paying clients\n\n"
               "Trend Analysis:\n"
               "- Daily transaction patterns\n"
               "- Monthly comparisons\n"
               "- Transaction peaks\n"
               "- Spending anomalies\n"
               "- Yearly trends\n\n"
               "For other questions, please configure the OpenAI API key in the settings.";
    }
}

void OpenAIChatbot::sendMessage(const QString &message)
{
    QString response = processTransactionQuery(message);
    if (response != "Processing your request...") {
        emit responseReceived(response);
    }
}

void OpenAIChatbot::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
        QString assistantMessage = response["choices"][0]["message"]["content"].toString();
        emit responseReceived(assistantMessage);
    } else {
        QString errorMessage = "Error: ";
        if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
            errorMessage += "Invalid or unauthorized API key";
        } else if (reply->error() == QNetworkReply::ConnectionRefusedError) {
            errorMessage += "Connection refused. Please check your internet connection";
        } else if (reply->error() == QNetworkReply::TimeoutError) {
            errorMessage += "Request timed out. Please check your internet connection";
        } else {
            errorMessage += reply->errorString();
        }
        emit error(errorMessage);
    }

    reply->deleteLater();
}

// Client Analysis Methods
QString OpenAIChatbot::getOldestClient() {
    QSqlQuery query;
    query.prepare("SELECT c.NOM, MIN(t.DATEEE) as FIRST_TRANSACTION, "
                  "COUNT(*) as TRANSACTION_COUNT, SUM(t.MONTANT) as TOTAL_AMOUNT "
                  "FROM SYRINE.CLIENT c "
                  "JOIN SYRINE.TRANSACTION t ON c.IDCLIENT = t.IDCLIENT "
                  "GROUP BY c.NOM "
                  "ORDER BY FIRST_TRANSACTION ASC");
    
    if (!query.exec() || !query.next()) {
        return "Error retrieving oldest client information";
    }
    
    return QString("Oldest Client:\n"
                  "Name: %1\n"
                  "First Transaction: %2\n"
                  "Total Transactions: %3\n"
                  "Total Amount: %4")
            .arg(query.value("NOM").toString())
            .arg(formatDate(query.value("FIRST_TRANSACTION").toDate()))
            .arg(query.value("TRANSACTION_COUNT").toString())
            .arg(formatAmount(query.value("TOTAL_AMOUNT").toDouble()));
}

QString OpenAIChatbot::getMostActiveClient() {
    QSqlQuery query;
    query.prepare("SELECT c.NOM, COUNT(*) as TRANSACTION_COUNT, "
                  "SUM(t.MONTANT) as TOTAL_AMOUNT, "
                  "MAX(t.DATEEE) as LAST_TRANSACTION "
                  "FROM SYRINE.CLIENT c "
                  "JOIN SYRINE.TRANSACTION t ON c.IDCLIENT = t.IDCLIENT "
                  "GROUP BY c.NOM "
                  "ORDER BY TRANSACTION_COUNT DESC");
    
    if (!query.exec() || !query.next()) {
        return "Error retrieving most active client information";
    }
    
    return QString("Most Active Client:\n"
                  "Name: %1\n"
                  "Number of Transactions: %2\n"
                  "Total Amount: %3\n"
                  "Last Transaction: %4")
            .arg(query.value("NOM").toString())
            .arg(query.value("TRANSACTION_COUNT").toString())
            .arg(formatAmount(query.value("TOTAL_AMOUNT").toDouble()))
            .arg(formatDate(query.value("LAST_TRANSACTION").toDate()));
}

QString OpenAIChatbot::getInactiveClients(int months) {
    QSqlQuery query;
    query.prepare("SELECT c.NOM, MAX(t.DATEEE) as LAST_TRANSACTION, "
                  "COUNT(*) as TRANSACTION_COUNT, SUM(t.MONTANT) as TOTAL_AMOUNT "
                  "FROM SYRINE.CLIENT c "
                  "JOIN SYRINE.TRANSACTION t ON c.IDCLIENT = t.IDCLIENT "
                  "GROUP BY c.NOM "
                  "HAVING MAX(t.DATEEE) < :cutoff_date "
                  "ORDER BY LAST_TRANSACTION DESC");
    
    QDate cutoffDate = QDate::currentDate().addMonths(-months);
    query.bindValue(":cutoff_date", cutoffDate);
    
    if (!query.exec()) {
        return "Error retrieving inactive clients";
    }
    
    QString result = QString("Clients inactive for %1 months:\n\n").arg(months);
    bool found = false;
    
    while (query.next()) {
        found = true;
        result += QString("Client: %1\n"
                         "Last Transaction: %2\n"
                         "Total Transactions: %3\n"
                         "Total Amount: %4\n\n")
                .arg(query.value("NOM").toString())
                .arg(formatDate(query.value("LAST_TRANSACTION").toDate()))
                .arg(query.value("TRANSACTION_COUNT").toString())
                .arg(formatAmount(query.value("TOTAL_AMOUNT").toDouble()));
    }
    
    if (!found) {
        return QString("No clients found inactive for %1 months").arg(months);
    }
    
    return result;
}

QString OpenAIChatbot::getTopClientsByValue(int count) {
    QSqlQuery query;
    query.prepare("SELECT c.NOM, SUM(t.MONTANT) as TOTAL_AMOUNT, "
                  "COUNT(*) as TRANSACTION_COUNT, "
                  "MAX(t.DATEEE) as LAST_TRANSACTION "
                  "FROM SYRINE.CLIENT c "
                  "JOIN SYRINE.TRANSACTION t ON c.IDCLIENT = t.IDCLIENT "
                  "GROUP BY c.NOM "
                  "ORDER BY TOTAL_AMOUNT DESC "
                  "LIMIT :count");
    query.bindValue(":count", count);
    
    if (!query.exec()) {
        return "Error retrieving top clients";
    }
    
    QString result = QString("Top %1 Clients by Value:\n\n").arg(count);
    int rank = 1;
    
    while (query.next()) {
        result += QString("%1. %2\n"
                         "Total Amount: %3\n"
                         "Number of Transactions: %4\n"
                         "Last Transaction: %5\n\n")
                .arg(rank++)
                .arg(query.value("NOM").toString())
                .arg(formatAmount(query.value("TOTAL_AMOUNT").toDouble()))
                .arg(query.value("TRANSACTION_COUNT").toString())
                .arg(formatDate(query.value("LAST_TRANSACTION").toDate()));
    }
    
    return result;
}

QString OpenAIChatbot::getLatePayingClients() {
    QSqlQuery query;
    query.prepare("SELECT c.NOM, t.REFERENCE, t.MONTANT, t.DATEEE, "
                  "t.STATUTT, t.TYPEE "
                  "FROM SYRINE.CLIENT c "
                  "JOIN SYRINE.TRANSACTION t ON c.IDCLIENT = t.IDCLIENT "
                  "WHERE t.STATUTT = 'En attente' "
                  "AND t.DATEEE < :cutoff_date "
                  "ORDER BY t.DATEEE ASC");
    
    QDate cutoffDate = QDate::currentDate().addDays(-30); // 30 days overdue
    query.bindValue(":cutoff_date", cutoffDate);
    
    if (!query.exec()) {
        return "Error retrieving late paying clients";
    }
    
    QString result = "Late Paying Clients:\n\n";
    bool found = false;
    
    while (query.next()) {
        found = true;
        result += QString("Client: %1\n"
                         "Transaction Reference: %2\n"
                         "Amount: %3\n"
                         "Due Date: %4\n"
                         "Type: %5\n\n")
                .arg(query.value("NOM").toString())
                .arg(query.value("REFERENCE").toString())
                .arg(formatAmount(query.value("MONTANT").toDouble()))
                .arg(formatDate(query.value("DATEEE").toDate()))
                .arg(query.value("TYPEE").toString());
    }
    
    if (!found) {
        return "No late paying clients found";
    }
    
    return result;
}

// Trend Analysis Methods
QString OpenAIChatbot::getDailySummary() {
    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(DATEEE, 'Day') as DAY_NAME, "
                  "COUNT(*) as TRANSACTION_COUNT, "
                  "SUM(MONTANT) as TOTAL_AMOUNT, "
                  "AVG(MONTANT) as AVG_AMOUNT "
                  "FROM SYRINE.TRANSACTION "
                  "GROUP BY TO_CHAR(DATEEE, 'Day') "
                  "ORDER BY TRANSACTION_COUNT DESC");
    
    if (!query.exec()) {
        return "Error retrieving daily summary";
    }
    
    QString result = "Daily Transaction Summary:\n\n";
    
    while (query.next()) {
        result += QString("%1:\n"
                         "Number of Transactions: %2\n"
                         "Total Amount: %3\n"
                         "Average Amount: %4\n\n")
                .arg(query.value("DAY_NAME").toString().trimmed())
                .arg(query.value("TRANSACTION_COUNT").toString())
                .arg(formatAmount(query.value("TOTAL_AMOUNT").toDouble()))
                .arg(formatAmount(query.value("AVG_AMOUNT").toDouble()));
    }
    
    return result;
}

QString OpenAIChatbot::getMonthlyComparison() {
    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(DATEEE, 'YYYY-MM') as MONTH, "
                  "COUNT(*) as TRANSACTION_COUNT, "
                  "SUM(CASE WHEN TYPEE = 'Vente' THEN MONTANT ELSE 0 END) as SALES, "
                  "SUM(CASE WHEN TYPEE = 'Achat' THEN MONTANT ELSE 0 END) as PURCHASES "
                  "FROM SYRINE.TRANSACTION "
                  "GROUP BY TO_CHAR(DATEEE, 'YYYY-MM') "
                  "ORDER BY MONTH DESC");
    
    if (!query.exec()) {
        return "Error retrieving monthly comparison";
    }
    
    QString result = "Monthly Transaction Comparison:\n\n";
    
    while (query.next()) {
        result += QString("%1:\n"
                         "Total Transactions: %2\n"
                         "Total Sales: %3\n"
                         "Total Purchases: %4\n\n")
                .arg(query.value("MONTH").toString())
                .arg(query.value("TRANSACTION_COUNT").toString())
                .arg(formatAmount(query.value("SALES").toDouble()))
                .arg(formatAmount(query.value("PURCHASES").toDouble()));
    }
    
    return result;
}

QString OpenAIChatbot::getTransactionPeaks() {
    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(DATEEE, 'YYYY-MM-DD') as DATE, "
                  "COUNT(*) as TRANSACTION_COUNT, "
                  "SUM(MONTANT) as TOTAL_AMOUNT "
                  "FROM SYRINE.TRANSACTION "
                  "GROUP BY TO_CHAR(DATEEE, 'YYYY-MM-DD') "
                  "ORDER BY TRANSACTION_COUNT DESC "
                  "LIMIT 5");
    
    if (!query.exec()) {
        return "Error retrieving transaction peaks";
    }
    
    QString result = "Top 5 Busiest Days:\n\n";
    int rank = 1;
    
    while (query.next()) {
        result += QString("%1. %2\n"
                         "Number of Transactions: %3\n"
                         "Total Amount: %4\n\n")
                .arg(rank++)
                .arg(query.value("DATE").toString())
                .arg(query.value("TRANSACTION_COUNT").toString())
                .arg(formatAmount(query.value("TOTAL_AMOUNT").toDouble()));
    }
    
    return result;
}

QString OpenAIChatbot::getSpendingAnomalies() {
    QSqlQuery query;
    query.prepare("SELECT t.REFERENCE, t.MONTANT, t.DATEEE, t.TYPEE, "
                  "COALESCE(c.NOM, p.NOMENTREPRISE) as PAYER_NAME "
                  "FROM SYRINE.TRANSACTION t "
                  "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                  "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                  "WHERE t.MONTANT > (SELECT AVG(MONTANT) * 3 FROM SYRINE.TRANSACTION) "
                  "ORDER BY t.MONTANT DESC");
    
    if (!query.exec()) {
        return "Error retrieving spending anomalies";
    }
    
    QString result = "Unusual Spending Patterns:\n\n";
    bool found = false;
    
    while (query.next()) {
        found = true;
        result += QString("Reference: %1\n"
                         "Amount: %2\n"
                         "Date: %3\n"
                         "Type: %4\n"
                         "Payer: %5\n\n")
                .arg(query.value("REFERENCE").toString())
                .arg(formatAmount(query.value("MONTANT").toDouble()))
                .arg(formatDate(query.value("DATEEE").toDate()))
                .arg(query.value("TYPEE").toString())
                .arg(query.value("PAYER_NAME").toString());
    }
    
    if (!found) {
        return "No unusual spending patterns detected";
    }
    
    return result;
}

QString OpenAIChatbot::getYearlyTrends() {
    QSqlQuery query;
    query.prepare("SELECT TO_CHAR(DATEEE, 'YYYY') as YEAR, "
                  "COUNT(*) as TRANSACTION_COUNT, "
                  "SUM(MONTANT) as TOTAL_AMOUNT, "
                  "AVG(MONTANT) as AVG_AMOUNT, "
                  "SUM(CASE WHEN TYPEE = 'Vente' THEN MONTANT ELSE 0 END) as SALES, "
                  "SUM(CASE WHEN TYPEE = 'Achat' THEN MONTANT ELSE 0 END) as PURCHASES "
                  "FROM SYRINE.TRANSACTION "
                  "GROUP BY TO_CHAR(DATEEE, 'YYYY') "
                  "ORDER BY YEAR DESC");
    
    if (!query.exec()) {
        return "Error retrieving yearly trends";
    }
    
    QString result = "Yearly Transaction Trends:\n\n";
    
    while (query.next()) {
        result += QString("%1:\n"
                         "Total Transactions: %2\n"
                         "Total Amount: %3\n"
                         "Average Amount: %4\n"
                         "Total Sales: %5\n"
                         "Total Purchases: %6\n\n")
                .arg(query.value("YEAR").toString())
                .arg(query.value("TRANSACTION_COUNT").toString())
                .arg(formatAmount(query.value("TOTAL_AMOUNT").toDouble()))
                .arg(formatAmount(query.value("AVG_AMOUNT").toDouble()))
                .arg(formatAmount(query.value("SALES").toDouble()))
                .arg(formatAmount(query.value("PURCHASES").toDouble()));
    }
    
    return result;
}

QString OpenAIChatbot::getNthLargestTransaction(int n)
{
    QSqlQuery query;
    query.prepare("SELECT t.REFERENCE, t.MONTANT, t.DATEEE, t.TYPEE, "
                  "COALESCE(c.NOM, p.NOMENTREPRISE) as PAYER_NAME "
                  "FROM SYRINE.TRANSACTION t "
                  "LEFT JOIN SYRINE.CLIENT c ON t.IDCLIENT = c.IDCLIENT "
                  "LEFT JOIN SYRINE.PARTENAIRE p ON t.IDPARTENAIRE = p.IDPARTENAIRE "
                  "ORDER BY t.MONTANT DESC "
                  "OFFSET :offset ROWS FETCH NEXT 1 ROWS ONLY");
    query.bindValue(":offset", n - 1);
    
    if (!query.exec() || !query.next()) {
        return QString("Error retrieving %1th largest transaction").arg(n);
    }
    
    return QString("%1th Largest transaction:\n"
                  "Reference: %2\n"
                  "Amount: %3\n"
                  "Date: %4\n"
                  "Type: %5\n"
                  "Payer: %6")
            .arg(n)
            .arg(query.value("REFERENCE").toString())
            .arg(formatAmount(query.value("MONTANT").toDouble()))
            .arg(formatDate(query.value("DATEEE").toDate()))
            .arg(query.value("TYPEE").toString())
            .arg(query.value("PAYER_NAME").toString());
}

QString OpenAIChatbot::getYearlySpending(int year)
{
    QSqlQuery query;
    query.prepare("SELECT SUM(MONTANT) as TOTAL_SPENDING "
                  "FROM SYRINE.TRANSACTION "
                  "WHERE TYPEE = 'Achat' "
                  "AND EXTRACT(YEAR FROM DATEEE) = :year");
    query.bindValue(":year", year);
    
    if (!query.exec() || !query.next()) {
        return QString("Error retrieving spending for year %1").arg(year);
    }
    
    return QString("Total spending in %1: %2")
            .arg(year)
            .arg(formatAmount(query.value("TOTAL_SPENDING").toDouble()));
}

QString OpenAIChatbot::extractTimePeriod(const QString &text)
{
    if (text.contains("last month") || text.contains("past month") || text.contains("this month")) return "month";
    if (text.contains("last quarter") || text.contains("past quarter") || text.contains("this quarter")) return "quarter";
    if (text.contains("last year") || text.contains("past year") || text.contains("this year")) return "year";
    if (text.contains("last week") || text.contains("past week") || text.contains("this week")) return "week";
    return "month"; // default
} 