#include "bank.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

using namespace std;

// Process Transaction File
void Bank::processTransactionFile(const string &fileName) {
  string line;
  ifstream inputFile(fileName);
  if (!inputFile) {
    cout << "Could not open the file - '" << fileName << "'" << endl;
    return;
  }

  // Push each line to the queue
  while (getline(inputFile, line)) {
    storeInformation.push(line);
  }

  // Push each string into a new queue
  while (!storeInformation.empty()) {
    queue<string> lineQueue;
    string eachLine = storeInformation.front();
    string temp = eachLine;
    storeInformation.pop();

    while (!eachLine.empty()) {
      int index = eachLine.find(' ');
      if (index != string::npos) {
        lineQueue.push(eachLine.substr(0, index));
        eachLine = eachLine.substr(index + 1);
        if (eachLine.empty()) {
          lineQueue.push(eachLine);
        }
      } else {
        lineQueue.push(eachLine);
        eachLine = "";
      }
    }

    // Define what transaction
    string firstElement = lineQueue.front();
    lineQueue.pop();
    if (firstElement == "O") {
      string firstName = lineQueue.front();
      lineQueue.pop();
      string lastName = lineQueue.front();
      lineQueue.pop();
      string idInStr = lineQueue.front();
      lineQueue.pop();
      int id = stoi(idInStr);
      openAccount(firstName, lastName, id);
    } else if (firstElement == "D") {
      string idInStr = lineQueue.front();
      lineQueue.pop();
      char digitInChar = idInStr[idInStr.size() - 1];
      int digit = digitInChar - '0';
      int id = (stoi(idInStr) - digit) / 10;
      if (id <= 999 || id > 10000) {
        cout << "Invalid ID!" << endl;
        return;
      }
      string moneyInStr = lineQueue.front();
      lineQueue.pop();
      int money = stoi(moneyInStr);
      deposit(money, id, digit, temp);
    } else if (firstElement == "W") {
      string idInStr = lineQueue.front();
      lineQueue.pop();
      char digitInChar = idInStr[idInStr.size() - 1];
      int digit = digitInChar - '0';
      int id = (stoi(idInStr) - digit) / 10;
      if (id <= 999 || id > 10000) {
        cout << "Invalid ID!" << endl;
        return;
      }
      string moneyInStr = lineQueue.front();
      lineQueue.pop();
      int money = stoi(moneyInStr);
      withdraw(money, id, digit, temp);
    } else if (firstElement == "T") {
      string idInStr = lineQueue.front();
      lineQueue.pop();
      char digitInChar = idInStr[idInStr.size() - 1];
      int digit = digitInChar - '0';
      int id = (stoi(idInStr) - digit) / 10;
      if (id <= 999 || id > 10000) {
        cout << "Invalid ID!" << endl;
        return;
      }
      string moneyInStr = lineQueue.front();
      lineQueue.pop();
      int money = stoi(moneyInStr);

      string id2InStr = lineQueue.front();
      lineQueue.pop();
      char digit2InChar = id2InStr[id2InStr.size() - 1];
      int digit2 = digit2InChar - '0';
      int id2 = (stoi(id2InStr) - digit2) / 10;
      if (id2 <= 999 || id2 > 10000) {
        cout << "Invalid ID!" << endl;
        return;
      }
      transfer(money, id, digit, id2, digit2, temp);
    } else if (firstElement == "H") {
      string idInStr = lineQueue.front();
      lineQueue.pop();
      int id = stoi(idInStr);
      if (id >= 10000) {
        char digitInChar = idInStr[idInStr.size() - 1];
        int digit = digitInChar - '0';
        int id = (stoi(idInStr) - digit) / 10;
        if (id <= 999 || id > 10000) {
          cout << "Invalid ID!" << endl;
          return;
        }
        history(id, digit);
      } else {
        if (id <= 999 || id > 10000) {
          cout << "Invalid ID!" << endl;
          return;
        }
        history(id);
      }
    } else {
      cout << "Invalid transaction type!" << endl;
      return;
    }
  }
  finalBalances();
  inputFile.close();
}

// Open account
void Bank::openAccount(string firstName, string lastName, int id) {
  Account *account = new Account();
  if (accounts.retrieve(id, account)) {
    cout << "ERROR: Account " << id << " is already open. Transaction refused."
         << endl;
    return;
  }
  account->id = id;
  account->firstName = firstName; // NOLINT
  account->lastName = lastName;   // NOLINT
  if (!accounts.insert(account)) {
    cout << "ERROR: Cannot create account!" << endl;
    return;
  }
}

// Deposit money
void Bank::deposit(int money, int id, int lastdigit, string const &line) {
  Account *account = new Account();
  if (!accounts.retrieve(id, account)) {
    cout << "ERROR: Could not find Account " << id << " Deposit cancelled."
         << endl;
    return;
  }

  account->fund[lastdigit] += money;
  account->transHistory[lastdigit].push_back(line);
}

// Withdraw money
void Bank::withdraw(int money, int id, int lastdigit, string line) {
  Account *account = new Account();
  if (!accounts.retrieve(id, account)) {
    cout << "ERROR: Could not find Account " << id << " Withdraw cancelled."
         << endl;
    return;
  }

  if (account->fund[lastdigit] < money) {
    line = line + " (Failed)";
    account->transHistory[lastdigit].push_back(line);
    return;
  }
  account->fund[lastdigit] -= money;
  account->transHistory[lastdigit].push_back(line);
}

// Transfer money
void Bank::transfer(int money, int id, int lastdigit, int id2, int lastdigit2,
                    string line) {
  Account *account = new Account();
  if (!accounts.retrieve(id, account)) {
    cout << "ERROR: Could not find Account " << id << " Transfer cancelled."
         << endl;
    return;
  }

  Account *account2 = new Account();
  if (!accounts.retrieve(id2, account2)) {
    cout << "ERROR: Could not find Account " << id2 << " Transfer cancelled."
         << endl;
    return;
  }

  if (account->fund[lastdigit] < money) {
    line = line + " (Failed)";
    account->transHistory[lastdigit].push_back(line);
    return;
  }
  account->fund[lastdigit] -= money;
  account->transHistory[lastdigit].push_back(line);

  line[0] = 'D';
  for (int i = 2; i < 7; i++) {
    line[i] = line[i + 9];
  }
  line.erase(11, 5);

  account2->fund[lastdigit2] += money;
  account2->transHistory[lastdigit2].push_back(line);
}

// Print out all funds
void Bank::history(int id) {
  Account *account = new Account();
  if (!accounts.retrieve(id, account)) {
    cout << "ERROR: Could not find Account " << id << " Transaction cancelled."
         << endl;
    return;
  }
  cout << "Displaying Transaction History for " << account->firstName << " "
       << account->lastName << " by fund." << endl;
  for (int i = 0; i < account->transHistory.size(); i++) {
    cout << account->transHistory[i][0] << ": $" << account->fund[i] << endl;

    for (int j = 1; j < account->transHistory[i].size(); j++) {
      cout << "\t" << account->transHistory[i][j] << endl;
    }
  }
}

// Print out 1 fund
void Bank::history(int id, int lastdigit) {
  Account *account = new Account();
  if (!accounts.retrieve(id, account)) {
    cout << "ERROR: Could not find Account " << id << " Transaction cancelled."
         << endl;
    return;
  }
  cout << "Displaying Transaction History for " << account->firstName << " "
       << account->lastName << " by " << account->transHistory[lastdigit][0]
       << ": $" << account->fund[lastdigit] << endl;
  for (int i = 1; i < account->transHistory[lastdigit].size(); i++) {
    cout << "\t" << account->transHistory[lastdigit][i] << endl;
  }
}

// Print out final balances
void Bank::finalBalances() {
  cout << "\nProcessing Done. Final Balances." << endl;
  accounts.display();
}