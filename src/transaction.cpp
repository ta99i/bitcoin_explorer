#include <cstdint>
#include <vector>
#include <iostream>
#include "utilities.cpp"

struct Input
{
    std::vector<uint8_t> tx_id;
    std::vector<uint8_t> vout; // Little-Endian // The index number of the output you want to spend.
    uint64_t script_sig_size;  // Compact Size // The size in bytes of the upcoming ScriptSig.
    std::vector<uint8_t> script_sig_size_hex;
    std::vector<uint8_t> script_sig; // The unlocking code for the output you want to spend.
    std::vector<uint8_t> sequence;   // Little-Endian  // Set whether the transaction can be replaced or when it can be.
};
struct Output
{
    std::vector<uint8_t> amount; // Little-Endian // The value of the output in satoshis.
    uint64_t script_pubkey_size; // Compact size // The size in bytes of the upcoming ScriptPubKey.
    std::vector<uint8_t> script_pubkey_size_hex;
    std::vector<uint8_t> script_pubkey; // The locking code for this output.
};
struct Stack
{
    uint64_t size;             // Compact size // The size of the upcoming stack item.
    std::vector<uint8_t> item; // The data to be pushed on to the stack.
};
struct Witness
{
    uint64_t stack_items; // Compact size // The number of items to be pushed on to the stack as part of the unlocking code.
    std::vector<Stack> stacks;
    // !! This structure repeats for every output.
};

class Transaction
{
private:
    std::string version;  // Little-Endian // The version number for the transaction.
    uint8_t marker;       // Used to indicate a segwit transaction. Must be 00.
    uint8_t flag;         // Used to indicate a segwit transaction. Must be 01 or greater.
    uint64_t input_count; // Compact size // Indicates the number of inputs.
    std::vector<Input> inputs;
    uint32_t output_count; // Compact size // Indicates the number of outputs.
    std::vector<Output> outputs;
    std::string locktime; // Little-Endian // Set a time or height after which the transaction can be mined.
    std::string transaction_id;

public:
    // Getters
    std::string getVersion() const { return version; }
    uint8_t getMarker() const { return marker; }
    uint8_t getFlag() const { return flag; }
    uint64_t getInputCount() const { return input_count; }
    std::vector<Input> &getInputs() { return inputs; }
    uint32_t getOutputCount() const { return output_count; }
    const std::vector<Output> &getOutputs() const { return outputs; }
    std::string getLocktime() const { return locktime; }
    std::string getTransactionId() const { return transaction_id; }
    // Setters
    void setVersion(std::string v) { version = v; }
    void setMarker(uint8_t m) { marker = m; }
    void setFlag(uint8_t f) { flag = f; }
    void setInputCount(uint64_t ic) { input_count = ic; }
    void setInputs(const std::vector<Input> &in) { inputs = in; }
    void setOutputCount(uint32_t oc) { output_count = oc; }
    void setOutputs(const std::vector<Output> &out) { outputs = out; }
    void setLocktime(std::string lt) { locktime = lt; }
    void pushInput(Input in) { inputs.push_back(in); }
    void pushOutput(Output out) { outputs.push_back(out); }

    // Generate
    void generateTransactionID()
    {
        // TXID = HASH256([version][inputs][outputs][locktime])
        Utilities u = Utilities();
        std::string to_hash = "";
        std::vector<uint8_t> input_count_hex = u.int_to_compact_size(this->input_count);
        std::vector<uint8_t> output_count_hex = u.int_to_compact_size(this->output_count);
        // std::vector<uint8_t> locktime_hex = u.int_to_compact_size(this->locktime);
        to_hash += getVersion();
        to_hash += u.bytes_hex_vector(input_count_hex);
        for (uint64_t i = 0; i < this->input_count; i++)
        {
            to_hash += u.bytes_hex_vector(inputs[i].tx_id);
            to_hash += u.bytes_hex_vector(inputs[i].vout);
            to_hash += u.bytes_hex_vector(inputs[i].script_sig_size_hex);
            to_hash += u.bytes_hex_vector(inputs[i].script_sig);
            to_hash += u.bytes_hex_vector(inputs[i].sequence);
        }
        to_hash += u.bytes_hex_vector(output_count_hex);

        for (uint64_t i = 0; i < this->output_count; i++)
        {
            to_hash += u.bytes_hex_vector(outputs[i].amount);
            to_hash += u.bytes_hex_vector(outputs[i].script_pubkey_size_hex);
            to_hash += u.bytes_hex_vector(outputs[i].script_pubkey);
        }
        to_hash += locktime;

        // std::cout << "To hash = " << u.to_upper(to_hash) << std::endl;
        transaction_id = u.double_sha256_reverse(to_hash);
    }
};