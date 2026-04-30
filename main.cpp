#include <iostream>
#include <string>
#include <algorithm>

bool isValidCurrency(const std::string& currency) 
{
    return currency.length() == 3 &&
              std::all_of(currency.begin(), currency.end(), 
              [](unsigned char c) {return std::isalpha(c); });
}

double getMockExchangeRate(const std::string& fromCurrency, const std::string& toCurrency) {
    if (fromCurrency == toCurrency) return 1.0;

    if (fromCurrency == "USD" && toCurrency == "EUR") return 0.92;
    if (fromCurrency == "EUR" && toCurrency == "USD") return 1.09;

    if (fromCurrency == "USD" && toCurrency == "MXN") return 17.10;
    if (fromCurrency == "MXN" && toCurrency == "USD") return 0.058;

    if (fromCurrency == "USD" && toCurrency == "GBP") return 0.79;
    if (fromCurrency == "GBP" && toCurrency == "USD") return 1.27;

    return -1.0; // unsupported
}


int main() 
{
double amount = 0.0;
std::string fromCurrency, toCurrency;

std::cout << "--- Currency Converter ---" << std::endl;

std::cout << "Enter amount: ";
std::cin >> amount;

if (amount <= 0 )
{
    std::cout << "Invalid amount. Please enter a positive number that is greater than 0." << std::endl;
    return 1; 
}

std::cout << "Enter from currency (e.g., USD): ";
std::cin >> fromCurrency;

std::cout << "Enter to currency (e.g., EUR): ";
std::cin >> toCurrency;

//Normalize uppercase
std::transform(fromCurrency.begin(), fromCurrency.end(), fromCurrency.begin(), ::toupper);
std::transform(toCurrency.begin(), toCurrency.end(), toCurrency.begin(), ::toupper);

//Validate currencies
if (!isValidCurrency(fromCurrency) || !isValidCurrency(toCurrency)) 
{
    std::cout << "Invalid currency code. Please enter a valid 3-letter currency code." << std::endl;
    return 1; 
}

double rate = getMockExchangeRate(fromCurrency, toCurrency);

if (rate < 0) {
    std::cout << "Error: Unsupported currency conversion." << std::endl;
    return 1;
}

double result = amount * rate;

std::cout << "\nConversion Result:\n";
std::cout << amount << " " << fromCurrency << " = "
          << result << " " << toCurrency << std::endl;

return 0;

return 0;
}