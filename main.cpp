
#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>
#include <stdexcept>
#include <string>
#include "account.h"

using namespace std;

void saveTransactionReports(const SavingsAccount& savings,
                            const ChequingAccount& chequing)
{
    string report =
        "XYZ BANK TRANSACTION REPORT\n"
        "===========================\n\n"
        "SAVINGS ACCOUNT\n"
        "---------------\n" + savings.report() +
        "\nCHEQUING ACCOUNT\n"
        "----------------\n" + chequing.report();

    bool saved = false;

    try
    {
        ofstream output("C:\\transactions.txt");
        if (!output)
        {
            throw runtime_error("Unable to open C:\\transactions.txt");
        }
        output << report;
        output.close();

        cout << "\nTransaction report saved to C:\\transactions.txt\n";
        saved = true;
    }
    catch (const exception& error)
    {
        cerr << "\nUnable to save transaction report: "
             << error.what() << endl;
    }

    if (!saved)
    {
        try
        {
            ofstream fallback("transactions.txt");
            if (!fallback)
            {
                throw runtime_error("Unable to open local transactions.txt");
            }
            fallback << report;
            fallback.close();
            cout << "\nTransaction report saved to the project folder as transactions.txt\n";
        }
        catch (const exception& error)
        {
            cerr << "\nUnable to save local fallback report: "
                 << error.what() << endl;
        }
    }
}

// ==========================================
// DISPLAY ATM MENU
// ==========================================

void displayMenu()
{
    cout << "\n============================================\n";
    cout << "             XYZ BANK ATM SYSTEM            \n";
    cout << "============================================\n";
    cout << " 1. Check Account Balance\n";
    cout << " 2. Deposit Money\n";
    cout << " 3. Withdraw Money (Chequing applies fee)\n";
    cout << " 4. Calculate & Apply Interest (Savings)\n";
    cout << " 5. View & Save Transaction Reports\n";
    cout << " 6. Exit Program\n";
    cout << "============================================\n";
    cout << "Select your transaction type (1-6): ";
}

// ==========================================
// PAUSE PROGRAM
// ==========================================

void pauseAndContinue()
{
    cout << "\nPress Enter to return to main menu...";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ==========================================
// MAIN PROGRAM
// ==========================================

int main()
{
    cout << fixed << setprecision(2);

    // Welcome Banner
    cout << "============================================\n";
    cout << "          WELCOME TO XYZ BANK SYSTEM        \n";
    cout << "       Object-Oriented Inheritance ATM      \n";
    cout << "============================================\n\n";

    // Create Account Objects
    SavingsAccount mySavings(2500.00, 0.05);

    ChequingAccount myChequing(1500.00, 2.50);

    int choice;
    int accountType;
    double amount;

    // ATM Menu Loop
    while (true)
    {
        displayMenu();

        if (!(cin >> choice))
        {
            cout << "Invalid input. Please enter a number.\n";

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            continue;
        }

        if (choice == 6)
        {
            cout << "\nThank you for banking with XYZ Bank.\n";
            cout << "Goodbye!\n";
            break;
        }

        if (choice < 1 || choice > 6)
        {
            cout << "\nInvalid menu selection.\n";
            pauseAndContinue();
            continue;
        }

        // Select Account Type
        if (choice >= 1 && choice <= 3)
        {
            cout << "\nSelect Account Type:\n";
            cout << "1. Savings Account\n";
            cout << "2. Chequing Account\n";
            cout << "Enter choice (1-2): ";

            if (!(cin >> accountType))
            {
                cout << "Invalid account selection.\n";

                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                continue;
            }

            if (accountType != 1 && accountType != 2)
            {
                cout << "Invalid account type selected.\n";
                pauseAndContinue();
                continue;
            }
        }

        switch (choice)
        {
            // ==================================
            // OPTION 1: CHECK BALANCE
            // ==================================

            case 1:
                try
                {
                    if (accountType == 1)
                    {
                        double balance = mySavings.getBalance();
                        mySavings.addTransaction(Transaction(balance, "Balance Query"));
                        cout << "\nSavings Account Balance: $"
                             << balance << endl;
                    }
                    else
                    {
                        double balance = myChequing.getBalance();
                        myChequing.addTransaction(Transaction(balance, "Balance Query"));
                        cout << "\nChequing Account Balance: $"
                             << balance << endl;
                    }
                }
                catch (const exception& error)
                {
                    cerr << "\nUnable to check balance: "
                         << error.what() << endl;
                }

                pauseAndContinue();
                break;

            // ==================================
            // OPTION 2: DEPOSIT MONEY
            // ==================================

            case 2:
                try
                {
                    cout << "\nEnter amount to deposit: $";
                    if (!(cin >> amount))
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw invalid_argument("Invalid amount entered.");
                    }
                    if (amount <= 0.0)
                    {
                        throw invalid_argument("Deposit amount must be greater than zero.");
                    }

                    if (accountType == 1)
                    {
                        if (!mySavings.deposit(amount))
                        {
                            throw runtime_error("Deposit could not be completed.");
                        }
                        cout << "Updated Savings Balance: $"
                             << mySavings.getBalance() << endl;
                    }
                    else
                    {
                        if (!myChequing.deposit(amount))
                        {
                            throw runtime_error("Deposit could not be completed.");
                        }
                        cout << "Updated Chequing Balance: $"
                             << myChequing.getBalance() << endl;
                    }
                }
                catch (const exception& error)
                {
                    cerr << "\nTransaction error: " << error.what() << endl;
                }

                pauseAndContinue();
                break;

            // ==================================
            // OPTION 3: WITHDRAW MONEY
            // ==================================

            case 3:
                try
                {
                    cout << "\nEnter amount to withdraw: $";
                    if (!(cin >> amount))
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw invalid_argument("Invalid amount entered.");
                    }
                    if (amount <= 0.0)
                    {
                        throw invalid_argument("Withdrawal amount must be greater than zero.");
                    }

                    if (accountType == 1)
                    {
                        if (!mySavings.withdraw(amount))
                        {
                            throw runtime_error("Insufficient savings funds.");
                        }
                        cout << "Updated Savings Balance: $"
                             << mySavings.getBalance() << endl;
                    }
                    else
                    {
                        if (!myChequing.withdraw(amount))
                        {
                            throw runtime_error("Insufficient chequing funds or invalid withdrawal.");
                        }
                        cout << "Updated Chequing Balance: $"
                             << myChequing.getBalance() << endl;
                    }
                }
                catch (const exception& error)
                {
                    cerr << "\nTransaction error: " << error.what() << endl;
                }

                pauseAndContinue();
                break;

            // ==================================
            // OPTION 4: CALCULATE INTEREST
            // ==================================

            case 4:
            {
                try
                {
                    cout << "\n--- SAVINGS ACCOUNT INTEREST ---\n";

                    double interest = mySavings.calcInterest();
                    cout << "Interest Earned: $" << interest << endl;

                    if (!mySavings.deposit(interest))
                    {
                        throw runtime_error("Interest could not be deposited.");
                    }
                    mySavings.addTransaction(
                        Transaction(interest, "Interest Earned"));

                    cout << "New Savings Balance: $"
                         << mySavings.getBalance() << endl;
                }
                catch (const exception& error)
                {
                    cerr << "\nInterest error: " << error.what() << endl;
                }

                pauseAndContinue();
                break;
            }

            case 5:
            {
                cout << "\n--- SAVINGS ACCOUNT REPORT ---\n";
                cout << mySavings.report();
                cout << "\n--- CHEQUING ACCOUNT REPORT ---\n";
                cout << myChequing.report();
                saveTransactionReports(mySavings, myChequing);
                pauseAndContinue();
                break;
            }

            default:

                cout << "Invalid selection.\n";
                pauseAndContinue();
        }
    }

    return 0;
}
