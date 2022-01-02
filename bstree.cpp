
#include "bstree.h"
#include <queue>
#include <stack>
#include <string>

using namespace std;

// Create BST
BSTree::BSTree() : root{nullptr}, current{nullptr} {}

// Delete all nodes in BST
BSTree::~BSTree() { clear(); }

// Insert new account
bool BSTree::insert(Account *account) {
  if (root == nullptr) {
    root = new Node(account);
    return true;
  }
  Node *node = root;
  Node *previous;

  while (true) {
    if (node->account->id == account->id) {
      return false;
    }
    previous = node;

    bool lessThan = node->account->id > account->id;
    if (lessThan) {
      node = node->left;
    } else {
      node = node->right;
    }

    if (node == nullptr) {
      if (lessThan) {
        previous->left = new Node(account);
        return true;
      }
      previous->right = new Node(account);
      return true;
    }
  }
  return false;
}

// Retrieve account
// returns true if successful AND *Account points to account
bool BSTree::retrieve(const int &accountNumber, Account *&account) const {
  if (root == nullptr) {
    return false;
  }

  Node *cur = root;

  while (cur != nullptr) {
    if (accountNumber == cur->account->id) {
      account = cur->account;
      return true;
    }

    if (accountNumber < cur->account->id) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }
  return false;
}

void BSTree::inOrder() const {
  stack<Node *> s;
  Node *curr = root;

  while (curr != nullptr || !s.empty()) {
    while (curr != nullptr) {
      s.push(curr);
      curr = curr->left;
    }
    curr = s.top();
    s.pop();
    cout << curr->account->firstName << " " << curr->account->lastName
         << " Account ID: " << curr->account->id << endl;
    for (int i = 0; i < curr->account->transHistory.size(); i++) {
      cout << "\t" << curr->account->transHistory[i][0] << ": $"
           << curr->account->fund[i] << endl;
    }
    cout << "\n";
    curr = curr->right;
  }
}

// Display information on all accounts
void BSTree::display() const {
  if (isEmpty()) {
    return;
  }
  inOrder();
}

// delete all information in AccountTree
void BSTree::clear() {
  if (root == nullptr) {
    return;
  }

  queue<Node *> queue;
  queue.push(root);

  Node *front = nullptr;

  while (!queue.empty()) {
    front = queue.front();
    queue.pop();
    if (front->left != nullptr) {
      queue.push(front->left);
    }

    if (front->right != nullptr) {
      queue.push(front->right);
    }

    delete front;
  }
  root = nullptr;
}

// check if tree is empty
bool BSTree::isEmpty() const { return root == nullptr; }
