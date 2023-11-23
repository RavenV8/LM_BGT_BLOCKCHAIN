#include "headers.h"  // Assuming headers.h is in the same directory as this source file
#include "LM_hash.h"

class Block {
public:
    std::string prev_block_hash;
    std::string timestamp;
    int version;
    std::string merkle_root_hash;
    int nonce;
    int difficulty_target;
    std::vector<Transaction> transactions;
};

std::vector<Transaction> selectTransactions(std::vector<Transaction>& transactionPool) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(transactionPool.begin(), transactionPool.end(), rng);

    for (int i =0; i<4; i++){
        std::cout << transactionPool[i] << std::endl;
    }

    std::vector<Transaction> selectedTransactions;
    selectedTransactions.reserve(100);

    for (int i = 0; i < 100; ++i) {
        selectedTransactions.push_back(transactionPool[i]);
    }

    return selectedTransactions;
}



void readingTransactions(std::vector<Transaction>& transactionPool){
    double amount;
    std::string sender, receiver, transactionId;

    std::ifstream transactionFile("transactions.txt");
    while (transactionFile >> transactionId >> sender >> receiver >> amount) {
        Transaction transaction;
        transaction.transaction_id = transactionId;
        transaction.sender_public_key = sender;
        transaction.receiver_public_key = receiver;
        transaction.amount = amount;

        transactionPool.push_back(transaction);
    }
    transactionFile.close();

    if (transactionPool.size() < 100) {
        std::cout << "Nepakanka transakciju. Turiu " << transactionPool.size() << " transakciju." << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::string calculateMerkleRoot(const std::vector<Transaction>& transactions) {
    std::vector<std::string> transactionHashes;

    
    for (const Transaction& transaction : transactions) {
        transactionHashes.push_back(transaction.transaction_id);
    }

    
    if (transactionHashes.empty()) {
        return "";
    }

    
    while (transactionHashes.size() > 1) {
        std::vector<std::string> newHashes;


        for (size_t i = 0; i < transactionHashes.size(); i += 2) {

            const std::string& hash1 = transactionHashes[i];
            const std::string& hash2 = (i + 1 < transactionHashes.size()) ? transactionHashes[i + 1] : transactionHashes[i];
            std::string combinedHash = Hashing(hash1 + hash2);

            newHashes.push_back(combinedHash);
        }


        transactionHashes = std::move(newHashes);
    }

    return transactionHashes[0];
}

int main() {
    // Perskaitomos transakcijos ir isrenkama 100 atsitiktinai
    std::vector<Transaction> transactionPool;
    readingTransactions(transactionPool);
    std::vector<Transaction> selectedTransactions = selectTransactions(transactionPool);

    std::vector<Transaction> transactions;
    std::string merkleRoot = calculateMerkleRoot(transactionPool);
    std::cout << "Merkle Root: " << merkleRoot << std::endl;


    return 0;
}
