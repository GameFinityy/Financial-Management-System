// Financial Management Application
// Made by Anshuman Iyer (Apllication No. 25BAC10027)

using namespace std;

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <map>

//Structure for a transaction
struct Transaction {
    int id;
    string date;
    string type; 
    string category;
    double amount;
    string description;
};

vector<Transaction> transactions;
const string FILENAME = "transactions.txt";
int nextId = 1;

void displayMenu();
void addTransaction();
void viewTransactions();
void updateTransaction();
void deleteTransaction();
void showSummary();
void saveToFile();
void loadFromFile();
void clearInputBuffer();

int main() {
    loadFromFile(); // Load existing data when the app starts

    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        // Checking for valid input
        if (cin.fail()) {
            cout << "\nInvalid input. Please enter a number.\n" << endl;
            clearInputBuffer();
            choice = 0; // To restart the slection process
            continue;
        }

        clearInputBuffer(); 

        switch (choice) {
        case 1:
            addTransaction();
            break;
        case 2:
            viewTransactions();
            break;
        case 3:
            updateTransaction();
            break;
        case 4:
            deleteTransaction();
            break;
        case 5:
            showSummary();
            break;
        case 6:
            saveToFile();
            cout << "\nExiting application. Your data has been saved. Goodbye!\n" << endl;
            break;
        default:
            cout << "\nInvalid choice. Please try again.\n" << endl;
        }
    } while (choice != 6);

    return 0;
}

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Menu Display
void displayMenu() {
    cout << "\n--- Financial Management Menu ---\n";
    cout << "1. Add Transaction\n";
    cout << "2. View All Transactions\n";
    cout << "3. Update Transaction\n";
    cout << "4. Delete Transaction\n";
    cout << "5. Show Financial Summary\n";
    cout << "6. Save and Exit\n";
    cout << "---------------------------------\n";
}

// Creating a new transaction
void addTransaction() {
    Transaction t;
    t.id = nextId++;

    int typeChoice;
    cout << "\n--- Add New Transaction ---\n";
    cout << "Enter Type (1 for Income, 2 for Expense): ";
    cin >> typeChoice;
    if (cin.fail() || (typeChoice != 1 && typeChoice != 2)) {
        cout << "\nInvalid type. Returning to menu.\n";
        clearInputBuffer();
        nextId--; // Don't increment ID on failure
        return;
    }
    t.type = (typeChoice == 1) ? "Income" : "Expense";
    clearInputBuffer();

    cout << "Enter Date (YYYY-MM-DD): ";
    getline(cin, t.date);

    cout << "Enter Category (e.g., Food, Salary, Bills): ";
    getline(cin, t.category);

    cout << "Enter Amount: ";
    cin >> t.amount;
    if (cin.fail() || t.amount < 0) {
        cout << "\nInvalid amount. Returning to menu.\n";
        clearInputBuffer();
        nextId--; // Don't increment ID on failure
        return;
    }
    clearInputBuffer();

    cout << "Enter Description: ";
    getline(cin, t.description);

    transactions.push_back(t);
    cout << "\nTransaction added successfully!\n" << endl;
    saveToFile(); // Auto-save after adding
}

// Displays all recorded transactions in a formatted table
void viewTransactions() {
    cout << "\n--- All Transactions ---\n";
    if (transactions.empty()) {
        cout << "No transactions recorded yet.\n" << endl;
        return;
    }

    cout << left << setw(5) << "ID"
        << setw(15) << "Date"
        << setw(12) << "Type"
        << setw(20) << "Category"
        << setw(12) << "Amount"
        << "Description" << endl;
    cout << string(80, '-') << endl;

    for (const auto& t : transactions) {
        cout << left << setw(5) << t.id
            << setw(15) << t.date
            << setw(12) << t.type
            << setw(20) << t.category
            << fixed << setprecision(2) << setw(12) << t.amount
            << t.description << endl;
    }
    cout << string(80, '-') << endl;
}

// Allows the user to find a transaction by ID and update it
void updateTransaction() {
    int id;
    cout << "\nEnter the ID of the transaction to update: ";
    cin >> id;

    if (cin.fail()) {
        cout << "\nInvalid ID format.\n";
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    for (auto& t : transactions) {
        if (t.id == id) {
            cout << "\n--- Updating Transaction ID: " << id << " ---\n";

            int typeChoice;
            cout << "Enter new Type (1 for Income, 2 for Expense): ";
            cin >> typeChoice;
            if (cin.fail() || (typeChoice != 1 && typeChoice != 2)) {
                cout << "\nInvalid type. Update cancelled.\n";
                clearInputBuffer();
                return;
            }
            t.type = (typeChoice == 1) ? "Income" : "Expense";
            clearInputBuffer();

            cout << "Enter new Date (YYYY-MM-DD): ";
            getline(cin, t.date);

            cout << "Enter new Category: ";
            getline(cin, t.category);

            cout << "Enter new Amount: ";
            cin >> t.amount;
            if (cin.fail() || t.amount < 0) {
                cout << "\nInvalid amount. Update cancelled.\n";
                clearInputBuffer();
                return;
            }
            clearInputBuffer();

            cout << "Enter new Description: ";
            getline(cin, t.description);

            cout << "\nTransaction updated successfully!\n" << endl;
            saveToFile(); // Auto-save after update
            return;
        }
    }
    cout << "\nTransaction with ID " << id << " not found.\n" << endl;
}

// Allows the user to find a transaction by ID and delete it
void deleteTransaction() {
    int id;
    cout << "\nEnter the ID of the transaction to delete: ";
    cin >> id;

    if (cin.fail()) {
        cout << "\nInvalid ID format.\n";
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    for (auto it = transactions.begin(); it != transactions.end(); ++it) {
        if (it->id == id) {
            transactions.erase(it);
            cout << "\nTransaction deleted successfully!\n" << endl;
            saveToFile(); // Auto-save after delete
            return;
        }
    }
    cout << "\nTransaction with ID " << id << " not found.\n" << endl;
}

// Calculates and displays a summary of finances
void showSummary() {
    if (transactions.empty()) {
        cout << "\nNo transactions to summarize.\n" << endl;
        return;
    }

    double totalIncome = 0.0;
    double totalExpense = 0.0;
    map<string, double> expenseByCategory;

    for (const auto& t : transactions) {
        if (t.type == "Income") {
            totalIncome += t.amount;
        }
        else {
            totalExpense += t.amount;
            expenseByCategory[t.category] += t.amount;
        }
    }

    double balance = totalIncome - totalExpense;

    cout << "\n--- Financial Summary ---\n";
    cout << fixed << setprecision(2);
    cout << "Total Income:   $" << totalIncome << endl;
    cout << "Total Expenses: $" << totalExpense << endl;
    cout << "-------------------------\n";
    cout << "Remaining Balance: $" << balance << endl;
    cout << "\n--- Expenses by Category ---\n";

    if (expenseByCategory.empty()) {
        cout << "No expenses recorded.\n";
    }
    else {
        for (const auto& pair : expenseByCategory) {
            cout << left << setw(20) << pair.first << ": $" << pair.second << endl;
        }
    }
    cout << "---------------------------\n" << endl;
}

// Saves all transactions from the vector to the data file
void saveToFile() {
    ofstream outFile(FILENAME);
    if (!outFile) {
        cerr << "Error: Could not open file for writing." << endl;
        return;
    }

    // Save the next ID on the first line
    outFile << nextId << endl;

    // Save each transaction
    for (const auto& t : transactions) {
        outFile << t.id << ',' << t.date << ',' << t.type << ','
            << t.category << ',' << t.amount << ',' << t.description << endl;
    }
    outFile.close();
}

// Loads transactions from the data file into the vector
void loadFromFile() {
    ifstream inFile(FILENAME);
    if (!inFile) {
        // If the file doesn't exist, it's not an error.
        // It just means no data has been saved yet.
        return;
    }

    transactions.clear();

    // Load the next ID from the first line
    inFile >> nextId;
    inFile.ignore(); // Consume the newline character

    string line;
    while (getline(inFile, line)) {
        Transaction t;
        size_t pos = 0;
        string token;
        vector<string> parts;

        // Split the line by commas
        while ((pos = line.find(',')) != string::npos) {
            parts.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        parts.push_back(line); // Add the last part

        if (parts.size() == 6) {
            t.id = stoi(parts[0]);
            t.date = parts[1];
            t.type = parts[2];
            t.category = parts[3];
            t.amount = stod(parts[4]);
            t.description = parts[5];
            transactions.push_back(t);
        }
    }
    inFile.close();
}
