
#ifndef BANK_H
#define BANK_H

#include "bstree.h"
#include <fstream>
#include <queue>
#include <queue>

using namespace std;

class Bank {
public:
  queue<string> storeInformation;
  void processTransactionFile(const string &fileName);
  void openAccount(string firstName, string lastName, int id);
  void deposit(int money, int id, int lastdigit, string const &line);
  void withdraw(int money, int id, int lastdigit, string line);
  void transfer (int money, int id, int lastdigit, int id2, int lastdigit2, string line);
  void history(int id);
  void history(int id, int lastdigit);
  void finalBalances();
private:
  BSTree accounts;
  queue<string> storeinformation;
  
};
#endif // BANK_H