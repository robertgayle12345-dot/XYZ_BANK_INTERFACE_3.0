#include <iomanip>
#include <sstream>
#include "account.h"

Transaction::Transaction(double transactionAmount,
                         const std::string& transactionType)
    : amount(transactionAmount), type(transactionType)
{
}

double Transaction::getAmount() const
{
    return amount;
}

std::string Transaction::getType() const
{
    return type;
}

std::string Transaction::report() const
{
    std::ostringstream output;
    output << std::fixed << std::setprecision(2);
    output << type << ": $" << amount;
    return output.str();
}

Account::Account(double initialBalance)
{
    if (initialBalance < 0.0)
    {
        throw std::invalid_argument("Initial balance cannot be negative.");
    }
    balance = initialBalance;
}

void Account::setBalance(double b)
{
    if (b < 0.0)
    {
        throw std::invalid_argument("Balance cannot be negative.");
    }
    balance = b;
}

double Account::getBalance() const
{
    return balance;
}

void Account::addTransaction(const Transaction& transaction)
{
    if (transaction.getType().empty())
    {
        throw std::invalid_argument("Transaction type cannot be empty.");
    }

    try
    {
        log.push_back(transaction);
    }
    catch (const std::bad_alloc&)
    {
        throw std::runtime_error("Unable to allocate memory for transaction log.");
    }
}

std::string Account::report() const
{
    std::ostringstream output;
    output << "Transaction History\n";

    if (log.empty())
    {
        output << "No transactions recorded.\n";
        return output.str();
    }

    for (std::size_t i = 0; i < log.size(); ++i)
    {
        output << (i + 1) << ". " << log[i].report() << "\n";
    }

    output << "Current Balance: $" << std::fixed
           << std::setprecision(2) << balance << "\n";
    return output.str();
}

bool Account::deposit(double amount)
{
    if (amount <= 0.0)
    {
        throw std::invalid_argument("Deposit amount must be greater than zero.");
    }

    balance += amount;
    addTransaction(Transaction(amount, "Deposit"));
    return true;
}

bool Account::withdraw(double amount)
{
    if (amount <= 0.0)
    {
        throw std::invalid_argument("Withdrawal amount must be greater than zero.");
    }
    if (amount > balance)
    {
        return false;
    }

    balance -= amount;
    addTransaction(Transaction(amount, "Withdrawal"));
    return true;
}

SavingsAccount::SavingsAccount(double init_balance, float rate)
    : Account(init_balance), interestRate(rate)
{
    if (rate < 0.0f)
    {
        throw std::invalid_argument("Interest rate cannot be negative.");
    }
}

double SavingsAccount::calcInterest() const
{
    return getBalance() * static_cast<double>(interestRate);
}

ChequingAccount::ChequingAccount(double init_balance, double fee)
    : Account(init_balance), transactionFee(fee)
{
    if (fee < 0.0)
    {
        throw std::invalid_argument("Transaction fee cannot be negative.");
    }
}

bool ChequingAccount::withdraw(double amount)
{
    if (amount <= 0.0)
    {
        throw std::invalid_argument("Withdrawal amount must be greater than zero.");
    }

    double totalRequired = amount + transactionFee;
    if (totalRequired > getBalance())
    {
        return false;
    }

    setBalance(getBalance() - totalRequired);
    addTransaction(Transaction(amount, "Withdrawal"));
    addTransaction(Transaction(transactionFee, "Chequing Transaction Fee"));
    return true;
}
