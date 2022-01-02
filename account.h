#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <vector>

using namespace std;

class Account {

public: 
explicit Account(int id = 0, string firstName = "", string lastName = "");

int id;
string firstName;
string lastName;
int fund[10] = {0};
vector<vector<string>> transHistory = {{"Money Market"},{"Prime Money Market"},{"Long-Term Bond"},
                                        {"Short-Term Bond"},{"500 Index Fund"},{"Capital Value Fund"},
                                        {"Growth Equity Fund"},{"Growth Index Fund"},{"Value Fund"}, {"Value Stock Index"}};

};
#endif // ACCOUNT_H