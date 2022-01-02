#include "account.h"
#include <cassert>
#include <iostream>
#include <vector>

// Constructor
Account::Account(int id, string firstName, string lastName) // NOLINT
    : id{id}, firstName{firstName}, lastName{lastName} {}   // NOLINT
