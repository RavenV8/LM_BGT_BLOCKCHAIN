#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

class User {
public:
    std::string name;
    std::string public_key;
    double balance;

    User(std::string n, std::string pk, double b) : name(n), public_key(pk), balance(b) {}
};

class Transaction {
public:
    std::string transaction_id;
    std::string sender_public_key;
    std::string receiver_public_key;
    double amount;

    Transaction(std::string id, std::string sender, std::string receiver, double amt)
        : transaction_id(id), sender_public_key(sender), receiver_public_key(receiver), amount(amt) {}
};

class Block {
public:
    std::string prev_block_hash;
    time_t timestamp;
    int version;
    std::string merkle_root_hash;
    int nonce;
    int difficulty_target;

    Block(std::string prev_hash, int ver, int target)
        : prev_block_hash(prev_hash), version(ver), difficulty_target(target) {
        timestamp = time(nullptr);
        nonce = 0;
    }
};

int main() {
    std::vector<User> users;
    for (int i = 0; i < 1000; i++) {
        std::string name = "User" + std::to_string(i);
        std::string public_key = "Public_Key" + std::to_string(i);
        double balance = 100 + (rand() % 990901);  // Random balance between 100 and 1,000,000
        users.push_back(User(name, public_key, balance));
    }

    std::vector<Transaction> transactions;
    for (int i = 0; i < 10000; i++) {
        std::string transaction_id = "TxID" + std::to_string(i);
        std::string sender = users[rand() % users.size()].public_key;
        std::string receiver = users[rand() % users.size()].public_key;
        double amount = rand() % 1000;
        transactions.push_back(Transaction(transaction_id, sender, receiver, amount));
    }

    Block new_block("Previous_Hash", 1, 4);
    while (true) {
        std::string candidate_hash = "Hash calculation result using PoW logic";
        if (candidate_hash.substr(0, new_block.difficulty_target) == std::string(new_block.difficulty_target, '0')) {
            new_block.merkle_root_hash = "Calculated_Merkle_Root_Hash";
            break;
        }
        new_block.nonce++;
    }

    return 0;
}
