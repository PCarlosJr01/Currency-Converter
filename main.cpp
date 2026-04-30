#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <filesystem>

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

    std::string cachedFromCurrency;
    std::string cachedToCurrency;
    double cachedRate = 0.0;

    while (cacheFile >> cachedFromCurrency >> cachedToCurrency >> cachedRate)
    {
        if (cachedFromCurrency == fromCurrency && cachedToCurrency == toCurrency)
        {
            return cachedRate;
        }
    }

    return -1.0;
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
        std::cout << "Exchange rate cache missing or expired.\n";
        std::cout << "Live API fetching will be added in a future version.\n";

        // Create mock cache file 
        std::ofstream cacheFile(cacheFilePath);

        if (cacheFile)
        {
            cacheFile << "USD EUR 0.92\n";
            cacheFile << "EUR USD 1.09\n";
            cacheFile << "USD MXN 17.10\n";
            cacheFile << "MXN USD 0.058\n";
            cacheFile << "USD GBP 0.79\n";
            cacheFile << "GBP USD 1.27\n";
        }
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

    // Normalize to uppercase
    std::transform(fromCurrency.begin(), fromCurrency.end(), fromCurrency.begin(), ::toupper);
    std::transform(toCurrency.begin(), toCurrency.end(), toCurrency.begin(), ::toupper);

    // Validate currencies
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