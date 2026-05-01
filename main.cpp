#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

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

int main() 
{
    double amount = 0.0;
    std::string fromCurrency, toCurrency;

    const std::string cacheFilePath = "exchange_rates_cache.txt";

    if (isCacheValid(cacheFilePath))
    {
        std::cout << "Using cached exchange rate data.\n";
    }
    else
    {
        std::cout << "Cache is invalid or expired.\n";

        if(!fetchAndCacheRates(cacheFilePath))
        {
            std::cout <<"Error: Failed to fetch exchange rates." << std::endl;
            return 1;
        }
        std::cout << "Exchange rates updated and cached successfully.\n";
   
    }

    std::cout << "--- Currency Converter ---" << std::endl;

    std::cout << "Enter amount: ";
    std::cin >> amount;

    if (amount <= 0)
    {
        std::cout << "Invalid amount. Please enter a positive number greater than 0." << std::endl;
        return 1;
    }

    std::cout << "Enter from currency (e.g., USD): ";
    std::cin >> fromCurrency;

    std::cout << "Enter to currency (e.g., EUR): ";
    std::cin >> toCurrency;

    std::transform(fromCurrency.begin(), fromCurrency.end(), fromCurrency.begin(), ::toupper);
    std::transform(toCurrency.begin(), toCurrency.end(), toCurrency.begin(), ::toupper);

    if (!isValidCurrency(fromCurrency) || !isValidCurrency(toCurrency))
    {
        std::cout << "Invalid currency code. Please enter a valid 3-letter currency code." << std::endl;
        return 1;
    }

    double rate = getCachedExchangeRate(cacheFilePath, fromCurrency, toCurrency);

    if (rate < 0)
    {
        std::cout << "Error: Currency conversion not available." << std::endl;
        return 1;
    }

    double result = amount * rate;

    std::cout << "\nConversion Result:\n";
    std::cout << amount << " " << fromCurrency << " = "
              << result << " " << toCurrency << std::endl;

    return 0;
}