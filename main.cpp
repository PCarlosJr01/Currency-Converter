#include <iostream>
#include <string>

int main() {
double amount = 0.0;
std::string fromCurrency, toCurrency;

std::cout << "--- Currency Converter ---" << std::endl;

std::cout << "Enter amount: ";
std::cin >> amount;

std::cout << "Enter from currency (e.g., USD): ";
std::cin >> fromCurrency;

std::cout << "Enter to currency (e.g., EUR): ";
std::cin >> toCurrency;

std::cout << "\nConversion Request:\n";
std::cout << "Amount: " << amount << std::endl;
std::cout << "From Currency: " << fromCurrency << std::endl;
std::cout << "To Currency: " << toCurrency << std::endl;
}