#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include <QString>
#include <sstream>
#include <QLocale>

bool isValidCurrency(const std::string& currency) 
{
    return currency.length() == 3 &&
              std::all_of(currency.begin(), currency.end(), 
              [](unsigned char c) {return std::isalpha(c); });
}

bool isCacheValid(const std::string& filePath)
{
    namespace fs = std::filesystem;

    if (!fs::exists(filePath))
    {
        return false;
    }

    auto lastWriteTime = fs::last_write_time(filePath);
    auto now = fs::file_time_type::clock::now();

    auto cacheAge = now - lastWriteTime;
    auto oneHour = std::chrono::hours(1);

    return cacheAge <= oneHour;
}

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

bool fetchAndCacheRates(const std::string& filePath)
{
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        return false;
    }

    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, "https://open.er-api.com/v6/latest/USD");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
    {
        return false;
    }

    try
    {
        nlohmann::json exchangeData = nlohmann::json::parse(response);

        if (!exchangeData.contains("result") || exchangeData["result"] != "success")
        {
            return false;
        }

        if (!exchangeData.contains("rates"))
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }

    std::ofstream cacheFile(filePath);

    if (!cacheFile)
    {
        return false;
    }

    cacheFile << response;

    return true;
}

double getCachedExchangeRate(
    const std::string& filePath,
    const std::string& fromCurrency,
    const std::string& toCurrency
)
{
    if (fromCurrency == toCurrency)
    {
        return 1.0;
    }

    std::ifstream cacheFile(filePath);

    if (!cacheFile)
    {
        return -1.0;
    }

    nlohmann::json exchangeData;

    try
    {
        cacheFile >> exchangeData;

        if (!exchangeData.contains("rates"))
        {
            return -1.0;
        }

        const auto& rates = exchangeData["rates"];

        if (!rates.contains(fromCurrency) || !rates.contains(toCurrency))
        {
            return -1.0;
        }

        double fromRate = rates[fromCurrency];
        double toRate = rates[toCurrency];

        return toRate / fromRate;
    }
    catch (...)
    {
        return -1.0;
    }
}

class CurrencyConverterBackend : public QObject
{
    Q_OBJECT

public:
    explicit CurrencyConverterBackend(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    Q_INVOKABLE QString convert(double amount, const QString& fromCurrency, const QString& toCurrency)
    {
        const std::string cacheFilePath = "exchange_rates_cache.txt";

        if (amount <= 0)
        {
            return "Invalid amount. Please enter a positive number.";
        }

        std::string from = fromCurrency.toStdString();
        std::string to = toCurrency.toStdString();

        std::transform(from.begin(), from.end(), from.begin(), ::toupper);
        std::transform(to.begin(), to.end(), to.begin(), ::toupper);

        if (!isValidCurrency(from) || !isValidCurrency(to))
        {
            return "Invalid currency code. Please enter a valid 3-letter currency code.";
        }

        if (!isCacheValid(cacheFilePath))
        {
            if (!fetchAndCacheRates(cacheFilePath))
            {
                if (!std::filesystem::exists(cacheFilePath))
                {
                    return "Error: No cached exchange rate data available.";
                }
            }
        }

        double rate = getCachedExchangeRate(cacheFilePath, from, to);

        if (rate < 0)
        {
            return QString::fromStdString("Error: Conversion rate not found for " + from + " -> " + to);
        }

        double result = amount * rate;

        QLocale locale;

        QString formattedAmount = locale.toString(amount, 'f', 2);
        QString formattedResult = locale.toString(result, 'f', 2);
        QString formattedRate = locale.toString(rate, 'f', 4);

        return formattedAmount + " " + QString::fromStdString(from) + " = " +
            formattedResult + " " + QString::fromStdString(to) +
            "\nRate: 1 " + QString::fromStdString(from) + " = " +
            formattedRate + " " + QString::fromStdString(to);
    }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    CurrencyConverterBackend backend;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("converterBackend", &backend);
    engine.loadFromModule("CurrencyConverter", "Main");

    if (engine.rootObjects().isEmpty())
    {
        return 1;
    }

    return app.exec();
}

#include "main.moc"