#include <iostream>
#include <string>
#include <limits>
using namespace std;

class Account {
private:
    string accountNumber;
    double balance;

public:
    Account(string accNum, double bal) {
        accountNumber = accNum;
        balance = bal;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Deposit of " << amount << " successful." << endl;
    }

    void withdraw(double amount) {
        cout << "Withdrawing " << amount << " from account " << accountNumber << endl;
        balance -= amount;
        if (balance < 0) {
            cout << "Insufficient funds!" << endl;
            balance += amount; // revert the withdrawal
        }
    }

    double getBalance() const {
        return balance;
    }

    string getAccountNumber() const {
        return accountNumber;
    }
};
int main() {
    Account myAccount("123456789", 1000.0);

    cout << "Welcome to the simple bank app." << endl;
    cout << "Current account: " << myAccount.getAccountNumber() << " (balance: " << myAccount.getBalance() << ")" << endl;

    while (true) {
        cout << "\nChoose an option: (d)eposit, (w)ithdraw, (b)alance, (q)uit: ";
        char opt;
        if (!(cin >> opt)) {
           
            break;
        }

        if (opt == 'q' || opt == 'Q') {
            break;
        }

        if (opt == 'd' || opt == 'D' || opt == 'w' || opt == 'W') {
            double amount;
            cout << "Enter amount: ";
            if (!(cin >> amount)) {
                cout << "Invalid amount entered. Please try again." << endl;
                
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (amount < 0) {
                cout << "Amount must be positive." << endl;
                continue;
            }

            if (opt == 'd' || opt == 'D') {
                myAccount.deposit(amount);
            } else {
                myAccount.withdraw(amount);
            }

            cout << "Updated balance: " << myAccount.getBalance() << endl;
        } else if (opt == 'b' || opt == 'B') {
            cout << "Account balance: " << myAccount.getBalance() << endl;
        } else {
            cout << "Unknown option. Please choose d, w, b, or q." << endl;
        }
    }

    cout << "\nFinal balance: " << myAccount.getBalance() << endl;
    return 0;
}