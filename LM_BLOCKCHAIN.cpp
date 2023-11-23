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
        os << "Block hash: " << block.merkle_root_hash << "\n" 
           << "Previous block hash: " << block.prev_block_hash << "\n"
           << "Mined on: " << block.timestamp << "\n"
           << "Nonce: " << block.nonce << "\n"
           << "Difficulty target: " << block.difficulty_target << "\n"
           << "-------------------------------------------------------" << "\n";
        return os;
    }
};

std::vector<Transaction> selectTransactions(std::vector<Transaction>& transactionPool, std::vector<Users>& users) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(transactionPool.begin(), transactionPool.end(), rng);

    std::vector<Transaction> selectedTransactions;
    selectedTransactions.reserve(100);

    for (int i = 0; i < 100 && i < transactionPool.size(); ++i) {
        const Transaction& currentTransaction = transactionPool[i];

        // Find the sender in the users vector
        bool senderFound = false;
        for (auto& user : users) {
            if (user.public_key == currentTransaction.sender_public_key) {
                if (user.balance >= currentTransaction.amount){
                    selectedTransactions.push_back(currentTransaction);
                    // Update the balance (assuming balances are being deducted for transactions)
                    user.balance -= currentTransaction.amount;
                    senderFound = true;
                    break;  // No need to continue searching
                }
                else {
                    std::cout<<transactionPool[i].amount<<std::endl;
                }
            }
        }
    }

    transactionPool.erase(transactionPool.begin(), transactionPool.begin() + 100);

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
}

void readingUsers(std::vector<Users>& userPool){
    double balance;
    std::string name, public_key;

    std::ifstream userFile("transactions.txt");
    while (userFile >> name >> public_key >> balance) {
        Users user;
        user.name = name;
        user.public_key = public_key;
        user.balance = balance;

        userPool.push_back(user);
    }
    userFile.close();
}

std::string calculateMerkleRoot(const std::vector<Transaction>& transactions) {
    std::vector<std::string> transactionHashes;

    for (const Transaction& transaction : transactions) {
        transactionHashes.push_back(transaction.transaction_id);
    }

    // Jei nera tranzakciju grazina tuscia
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

std::string calculateBlockHash(const Block& block) {
    std::string blockData = block.prev_block_hash + block.timestamp + std::to_string(block.version) +
                            block.merkle_root_hash + std::to_string(block.nonce) + std::to_string(block.difficulty_target);

    return Hashing(blockData);
}

bool isProofOfWorkValid(const std::string& blockHash, int difficultyTarget) {
    for (int i = 0; i < difficultyTarget; ++i) {
        if (blockHash[i] != '0') {
            return false;
        }
    }
    return true;
}

std::string calculatePreviousBlockHash(std::vector<std::string>& prevHash) {
    if (!prevHash.empty()) {
        return prevHash.back();
    } else {
        return "0000000000000000000000000000000000000000000000000000000000000000";
    }
}

Block createBlock(std::vector<std::string>& prevHash, std::vector<Transaction>& selectedTransactions, int difficultyTarget) {
    Block block;
    
    block.prev_block_hash = calculatePreviousBlockHash(prevHash);

    block.difficulty_target = std::abs(difficultyTarget); 
    block.nonce = 0;
    // Mine the block by finding a suitable nonce
    while (true) {
        block.nonce++;  // Increment the nonce
        
        block.merkle_root_hash = calculateMerkleRoot(selectedTransactions);
        std::string blockHash = calculateBlockHash(block);

        // Check if the mined hash meets the difficulty target
        if (isProofOfWorkValid(blockHash, difficultyTarget)) {

            auto currentTime = std::chrono::system_clock::now();
            auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count();
            std::time_t timestampAsTimeT = static_cast<std::time_t>(timestamp);
            std::tm* timeinfo = std::localtime(&timestampAsTimeT);
            char buffer[80];
            std::strftime(buffer, sizeof(buffer), "%d %b %Y, %H:%M:%S", timeinfo);
            block.timestamp = buffer;

            // Set the difficulty target for the block
            block.difficulty_target = std::abs(difficultyTarget);  // Ensure difficulty target is positive
            // Set block transactions
            block.transactions = selectedTransactions;
            // Add the Merkle Root Hash to the previous hashes
            prevHash.push_back(blockHash);
            block.merkle_root_hash = blockHash;
            return block;
        }
        //std::cout << std::endl << std::endl << blockHash << std::endl << std::endl;
    }
}

int main() {
    std::vector<std::string> prevHash;
    
    std::vector<Transaction> transactionPool;
    readingTransactions(transactionPool);
    std::vector<Users> userPool;
    readingUsers(userPool);

    std::vector<Transaction> selectedTransactions;
    std::vector<Block> newBlock;
    // Create a new block with the selected transactions
    int difficultyTarget = 1;
    int numberOfBlocks = 0;
    while(transactionPool.size() > 0){
        selectedTransactions = selectTransactions(transactionPool, userPool);
        newBlock.push_back(createBlock(prevHash, selectedTransactions, difficultyTarget)); 
        std::cout << "----------------------BLOCK-" << numberOfBlocks+1 << "------------------------" << "\n";
        std::cout << newBlock[numberOfBlocks] << std::endl;
        numberOfBlocks++;
    }

    

    // newBlock = createBlock(prevHash, selectedTransactions, difficultyTarget);
    // std::cout << newBlock << std::endl;

    // Print or handle the new block as needed

    return 0;
}
