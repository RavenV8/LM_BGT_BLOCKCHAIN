#include "headers.h"
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

    friend std::ostream& operator<<(std::ostream& os, const Block& block) {
        os << "prev_block_hash: " << block.prev_block_hash << "\n"
           << "timestamp: " << block.timestamp << "\n"
           << "version: " << block.version << "\n"
           << "merkle_root_hash: " << block.merkle_root_hash << "\n"
           << "nonce: " << block.nonce << "\n"
           << "difficulty_target: " << block.difficulty_target << "\n";
        return os;
    }
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

    // Extract transaction hashes
    for (const Transaction& transaction : transactions) {
        transactionHashes.push_back(transaction.transaction_id);
    }

    // If there are no transactions, return an empty string
    if (transactionHashes.empty()) {
        return "";
    }

    // Continue building the Merkle Tree until only one hash (the Merkle Root) remains
    while (transactionHashes.size() > 1) {
        std::vector<std::string> newHashes;

        // Combine pairs of hashes and hash them together
        for (size_t i = 0; i < transactionHashes.size(); i += 2) {
            // If there's an odd number of hashes, duplicate the last one
            const std::string& hash1 = transactionHashes[i];
            const std::string& hash2 = (i + 1 < transactionHashes.size()) ? transactionHashes[i + 1] : transactionHashes[i];
            std::string combinedHash = Hashing(hash1 + hash2);

            newHashes.push_back(combinedHash);
        }

        // Replace the old hashes with the new ones
        transactionHashes = std::move(newHashes);
    }

    // The last remaining hash is the Merkle Root
    return transactionHashes[0];
}

std::string calculatePreviousBlockHash(std::vector<std::string>& prevHash) {
    if (!prevHash.empty()) {
        return prevHash.back();
    } else {
        return "0000000000000000000000000000000000000000000000000000000000000000";
    }
}
// Function to create a new block with the selected transactions
Block createBlock(std::vector<std::string>& prevHash, std::vector<Transaction>& selectedTransactions) {
    Block block;
    
    // Set block attributes
    block.prev_block_hash = calculatePreviousBlockHash(prevHash);



    auto currentTime = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count();
    std::time_t timestampAsTimeT = static_cast<std::time_t>(timestamp);
    std::tm* timeinfo = std::localtime(&timestampAsTimeT);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%d %b %Y, %H:%M:%S", timeinfo);
    block.timestamp = buffer;



    block.version = 1;  // Set the version number as needed
    block.merkle_root_hash = calculateMerkleRoot(selectedTransactions);

    block.nonce = 0;  // Initial nonces
    block.difficulty_target = 5;  // Set the difficulty target as needed

    // Set block transactions
    block.transactions = selectedTransactions;

    prevHash.push_back(block.merkle_root_hash);
    return block;
}

int main() {
    std::vector<std::string> prevHash;
    
    std::vector<Transaction> transactionPool;
    readingTransactions(transactionPool);
    std::vector<Transaction> selectedTransactions = selectTransactions(transactionPool);

    std::vector<Transaction> test = selectedTransactions;
    std::string merkleRoot = calculateMerkleRoot(test);
    std::cout << "Merkle Root: " << merkleRoot << std::endl << std::endl;

    // Create a new block with the selected transactions
    Block newBlock = createBlock(prevHash, selectedTransactions);
    std::cout << newBlock << std::endl;

    newBlock = createBlock(prevHash, selectedTransactions);
    std::cout << newBlock << std::endl;

    // Print or handle the new block as needed

    return 0;
}
