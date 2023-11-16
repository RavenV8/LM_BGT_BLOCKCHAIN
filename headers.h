#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>

using std::string;
using std::vector;

class Users {
public:
    string name;
    string public_key;
    double balance;
};

class Transaction {
public:
    string transaction_id = "";
    string sender_public_key;
    string receiver_public_key;
    double amount;
};