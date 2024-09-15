#include <iostream>
#include <vector>
#include "transaction.cpp"

class Block
{
private:
    std::string magic_number;            // HEX
    std::string block_size;              // HEX
    std::string version;                 // Little-Endian // The version number for the block.
    std::vector<uint8_t> previous_block; // The block hash of a previous block this block is building on top of.
    std::vector<uint8_t> merkle_root;    // A fingerprint for all of the transactions included in the block.
    uint64_t time;                       // Little-endian // The current time as a Unix timestamp.
    std::vector<uint8_t> bits;           // Little-endian // A compact representation of the current target.
    std::vector<uint8_t> nonce;
    uint64_t transaction_count;            // Compact size // How many upcoming transactions are included in the block.
    std::vector<Transaction> transactions; // All of the raw transactions included in the block concatenated together.

public:
    // Getter methods
    std::string getMagicNumber() const { return magic_number; }
    std::string getBlockSize() const { return magic_number; }
    std::string getVersion() const { return version; }
    std::vector<uint8_t> getPreviousBlock() const { return previous_block; }
    std::vector<uint8_t> getMerkleRoot() const { return merkle_root; }
    uint64_t getTime() const { return time; }
    std::vector<uint8_t> getBits() const { return bits; }
    std::vector<uint8_t> getNonce() const { return nonce; }
    uint64_t getTransactionCount() const { return transaction_count; }
    std::vector<Transaction> getTransactions() const { return transactions; }

    // Setter methods
    void setMagicNumber(std::string value) { magic_number = value; }
    void setBlockSize(std::string value) { magic_number = value; }
    void setVersion(std::string value) { version = value; }
    void setPreviousBlock(const std::vector<uint8_t> &value) { previous_block = value; }
    void setMerkleRoot(const std::vector<uint8_t> &value) { merkle_root = value; }
    void setTime(uint64_t value) { time = value; }
    void setBits(const std::vector<uint8_t> &value) { bits = value; }
    void setNonce(const std::vector<uint8_t> &value) { nonce = value; }
    void setTransactionCount(uint64_t value) { transaction_count = value; }
    void setTransactions(const std::vector<Transaction> &value) { transactions = value; }
};
