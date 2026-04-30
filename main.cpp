#include <iostream>
#include <string>
#include <algorithm>

bool isValidCurrency(const std::string& currency) 
{
    return currency.length() == 3 &&
              std::all_of(currency.begin(), currency.end(), ::isalpha;
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

std::cout << "\nConversion Request:\n";
std::cout << "Amount: " << amount << std::endl;
std::cout << "From Currency: " << fromCurrency << std::endl;
std::cout << "To Currency: " << toCurrency << std::endl;
}