#include "headers.h"  // Assuming headers.h is in the same directory as this source file
#include "LM_hash.h"

const double BALANCE_MINIMUM = 100.0;
const double BALANCE_MAX = 1000000.0;

void generateUsers(std::vector<Users>& users) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> uni(BALANCE_MINIMUM, BALANCE_MAX);
    users.reserve(1000);

    for (int i = 0; i < 1000; i++) {
        Users user;
        user.name = "User" + std::to_string(i);
        user.public_key = Hashing(user.name);
        user.balance = uni(rng);
        users.push_back(user);
    }

    std::ofstream rf("Users.txt");

    for (unsigned int i = 0; i < users.size(); i++) {
        rf << users.at(i).name << std::endl;
        rf << users.at(i).public_key << std::endl;
        rf << std::fixed << std::setprecision(2) << users.at(i).balance << std::endl;
        rf << std::endl;
    }
    rf.close();
}

void generateTransactions(std::vector<Transaction> &trans, std::vector<Users> &users) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> uni(10.0,100000.0);
    std::uniform_int_distribution<int> uni1(0, 999);
    Transaction transaction;
    trans.reserve(10000);
    
    double amount;
    int a, b;
    std::string sender, receiver;

    for(int i = 0; i < 10000; i++) {
        transaction.amount = round(uni(rng));
        a = uni1(rng);
        b = uni1(rng);

        while(a == b) {b = uni1(rng);}

    transaction.sender_public_key = users.at(a).public_key;
    transaction.receiver_public_key = users.at(b).public_key;
    transaction.transaction_id = Hashing(transaction.sender_public_key + transaction.receiver_public_key + std::to_string(transaction.amount));
    trans.push_back(transaction);
    }

    std::ofstream rf("transactions.txt");

    for (unsigned int i = 0; i < trans.size(); i++) {
        rf << trans.at(i).amount << std::fixed << std::setprecision(2) << std::endl;
        rf << trans.at(i).sender_public_key << std::endl;
        rf << trans.at(i).receiver_public_key << std::endl;
        rf << trans.at(i).transaction_id << std::endl;
        rf << std::endl;
    }
    rf.close();
}

// void readUsers(std::vector<Users> &con) {
//     std::ifstream df("Users.txt");
//     std::stringstream my_buffer;
//     my_buffer.clear();
//     if (df.is_open()) {
//         my_buffer << df.rdbuf();
//         df.close();
//     }
//     Users u;
//     while(!my_buffer.eof()) {
//         my_buffer >> u.name;
//         my_buffer >> u.public_key;
//         my_buffer >> u.balance;
//         my_buffer >> std::ws;
//         con.push_back(u);
//     }
// }

int main() {
    vector <Users> users;
    generateUsers(users);

    vector<Transaction>trans;
    generateTransactions(trans, users);
    return 0;
}