#include "src/block.cpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <filesystem>
#include <thread>
Utilities utilities = Utilities();
void readWitness(std::ifstream &file, Transaction &tx)
{
    Witness witness;
    std::vector<uint8_t> witness_buffer;
    uint64_t input_count = tx.getInputCount();
    uint8_t byte;
    while (input_count--)
    {
        file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
        std::vector<uint8_t> temp_v = utilities.read_compact_size(file, byte);
        int stack_items = utilities.bytes_uint_vector(temp_v);
        witness.stack_items = stack_items;
        while (stack_items--)
        {
            witness_buffer = std::vector<uint8_t>();
            Stack stack;
            file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
            std::vector<uint8_t> temp_v = utilities.read_compact_size(file, byte);
            int stack_size = utilities.bytes_uint_vector(temp_v);
            stack.size = stack_size;
            while (stack_size--)
            {
                file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
                witness_buffer.push_back(byte);
            }
            stack.item = witness_buffer;
        }
        // todo  witness
    }
}
void readOutputs(std::ifstream &file, Transaction &tx)
{
    Output output;
    std::vector<uint8_t> output_buffer;
    uint8_t byte;
    int output_index = 0;

    while (true)
    {
        file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
        output_buffer.push_back(byte);
        if (output_index == 7)
        {
            output.amount = output_buffer;
        }
        if (output_index > 7)
        {
            std::vector<uint8_t> temp_v = utilities.read_compact_size(file, byte);
            int size = utilities.bytes_uint_vector(temp_v);
            output.script_pubkey_size = size;
            output.script_pubkey_size_hex = temp_v;
            output_buffer = std::vector<uint8_t>();
            while (size--)
            {
                file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
                output_buffer.push_back(byte);
            }
            output.script_pubkey = output_buffer;
            tx.pushOutput(output);
            break;
        }
        output_index++;
    }
}
void readInputs(std::ifstream &file, Transaction &tx)
{
    std::vector<uint8_t> input_buffer;
    uint8_t byte;
    Input input;
    int input_index = 0;
    while (true)
    {
        file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
        input_buffer.push_back(byte);
        if (input_index == 31)
        {
            input.tx_id = input_buffer;
            // std::cout << utilities.bytes_hex_vector(input_buffer) << endl;
            input_buffer = std::vector<uint8_t>();
        }
        if (input_index == 35)
        {
            input.vout = input_buffer;
            input_buffer = std::vector<uint8_t>();
        }
        if (input_index == 36)
        {
            std::vector<uint8_t> temp_v = utilities.read_compact_size(file, byte);
            int size = utilities.bytes_uint_vector(temp_v);
            input.script_sig_size = size;
            input.script_sig_size_hex = temp_v;
            input_buffer = std::vector<uint8_t>();
            while (size--)
            {
                file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
                input_buffer.push_back(byte);
            }
            input.script_sig = input_buffer;
            input_buffer = std::vector<uint8_t>();
            for (int i = 0; i < 4; i++)
            {
                file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
                input_buffer.push_back(byte);
            }
            input.sequence = input_buffer;
            tx.pushInput(input);

            // cout << utilities.bytes_hex_vector(input.tx_id) << endl
            //      << utilities.bytes_hex_vector(input.vout) << endl
            //      << input.script_sig_size << endl
            //      << utilities.bytes_hex_vector(input.script_sig) << endl
            //      << utilities.bytes_hex_vector(input.sequence) << endl;
            break;
        }
        input_index++;
    }
}
void readTransaction(std::ifstream &file, Transaction &tx)
{
    uint8_t byte;
    std::vector<uint8_t> tx_buffer;
    int tx_index = 0;

    while (file.read(reinterpret_cast<char *>(&byte), sizeof(byte)))
    {
        tx_buffer.push_back(byte);
        if (tx_index == 3)
        {
            tx.setVersion(utilities.bytes_hex_vector(tx_buffer));
        }
        if (tx_index == 4)
        {
            bool witness = false;
            if (utilities.byte_uint(byte) == 0)
            {
                witness = true;
                tx.setMarker(byte);
                file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
                tx.setFlag(byte);
                // Read Next byte to get the count of input(s)
                file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
            }
            std::vector<uint8_t> temp_v = utilities.read_compact_size(file, byte);
            int inputs_count = utilities.bytes_uint_vector(temp_v);

            tx.setInputCount(inputs_count);
            // tx.setInputCountHex(temp_v);
            tx_buffer = std::vector<uint8_t>(); // Clean the buffer

            while (inputs_count--)
                readInputs(file, tx);
            // Read output
            file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
            temp_v = utilities.read_compact_size(file, byte);
            int outputs_count = utilities.bytes_uint_vector(temp_v);
            tx.setOutputCount(outputs_count);
            while (outputs_count--)
                readOutputs(file, tx);
            if (witness == true)
            {
                readWitness(file, tx);
            }
            tx_buffer = std::vector<uint8_t>(); // Clean the buffer
            for (int i = 0; i < 4; i++)
            {
                file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
                tx_buffer.push_back(byte);
            }
            std::string locktime = utilities.bytes_hex_vector(tx_buffer);
            // cout << endl
            //      << locktime << endl
            //      << endl;
            tx.setLocktime(locktime);

            break;
        }
        tx_index++;
    }
}
void readBinaryFileByBytes(const std::string &filePath)
{
    std::cout << filePath << std::endl;
    std::ifstream file(filePath, std::ios::binary); //  Open the file in binary mode

    if (!file)
    {
        throw std::runtime_error("Error opening file: " + filePath); // Check if the file was opened successfully
    }
    std::vector<uint8_t> buffer; // Create a buffer to store the bytes
    uint8_t byte;                // Read the file byte by byte
    uint8_t index = 0;           // Index of the reader in the file
    Block block = Block();
    while (file.read(reinterpret_cast<char *>(&byte), sizeof(byte)))
    {
        buffer.push_back(byte);
        if (index == 3)
        {
            block.setMagicNumber(utilities.bytes_hex_vector(buffer));
            buffer = std::vector<uint8_t>();
        }
        if (index == 7)
        {
            block.setBlockSize(utilities.bytes_hex_vector(buffer));
            buffer = std::vector<uint8_t>();
        }
        //
        if (index == 11)
        {
            std::vector<uint8_t> lbe = utilities.little_big_endian(buffer);
            block.setVersion(utilities.bytes_hex_vector(lbe));
            buffer = std::vector<uint8_t>();
        }
        if (index == 43)
        {
            std::vector<uint8_t> lbe = utilities.little_big_endian(buffer);
            block.setPreviousBlock(lbe);
            buffer = std::vector<uint8_t>();
        }
        if (index == 75)
        {
            std::vector<uint8_t> lbe = utilities.little_big_endian(buffer);
            block.setMerkleRoot(lbe);
            buffer = std::vector<uint8_t>();
        }
        if (index == 79)
        {
            uint64_t time;
            std::vector<uint8_t> lbe = utilities.little_big_endian(buffer);
            std::stringstream ss;
            ss << std::hex << utilities.bytes_hex_vector(lbe);
            ss >> time;
            block.setTime(time);
            buffer = std::vector<uint8_t>();
        }
        if (index == 83)
        {

            std::vector<uint8_t> lbe = utilities.little_big_endian(buffer);
            std::stringstream ss;
            block.setBits(lbe);
            buffer = std::vector<uint8_t>();
        }
        if (index == 87)
        {
            std::vector<uint8_t> lbe = utilities.little_big_endian(buffer);
            std::stringstream ss;
            block.setNonce(lbe);
            std::vector<uint8_t> a = block.getNonce();
            buffer = std::vector<uint8_t>();
        }

        if (index == 88)
        {
            std::vector<uint8_t> temp_v = utilities.read_compact_size(file, byte);
            uint64_t tx_count = utilities.bytes_uint_vector(temp_v);
            while (tx_count--)
            {
                Transaction tx;
                readTransaction(file, tx);
                tx.generateTransactionID();
            }

            index = -1;
        }
        index++;
    }
}
int main()
{
    std::string directory = "blocks/";   // Directory containing the files
    std::regex pattern("blk\\d+\\.dat"); // Regular expression to match files like blk00001.dat
    std::vector<std::string> files;
    // Iterate over the directory to find matching files
    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        if (std::filesystem::is_regular_file(entry.status()))
        {
            std::string filename = entry.path().filename().string();
            if (std::regex_match(filename, pattern))
            {
                files.push_back(entry.path().string());
            }
        }
    }

    try
    {
        std::string filePath = "blocks/blk03332.dat";
        std::vector<std::thread> threads;
        // Start a thread for each file
        for (const auto &file : files)
        {
            threads.emplace_back(readBinaryFileByBytes, file);
        }
        // Join all threads
        for (auto &t : threads)
        {
            if (t.joinable())
            {
                t.join();
            }
        }
        // readBinaryFileByBytes(filePath);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}