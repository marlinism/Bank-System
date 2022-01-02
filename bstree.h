
#ifndef BSTREE_H
#define BSTREE_H

#include "account.h"
#include <iostream>

class BSTree { //NOLINT
public:
  
  // Create BST
  BSTree();

  explicit BSTree(Account* account);

  // Delete all nodes in BST
  ~BSTree();

  // Insert new account
  bool insert(Account *account);

  // Retrieve account
  // returns true if successful AND *Account points to account
  bool retrieve(const int &accountNumber, Account *&account) const;

  // Display information on all accounts
  void display() const;

  // delete all information in AccountTree
  void clear();

  // check if tree is empty
  bool isEmpty() const;

  void inOrder() const;

private:
  struct Node {
    Account *account;
    Node *right;
    Node *left;
    Node *prev;

    explicit Node(Account* acc) {
      account = acc;
      left = nullptr; 
      right = nullptr;
      prev = nullptr;
    }
  };
  Node* root;
  Node* current;
};
#endif // BSTREE_H
