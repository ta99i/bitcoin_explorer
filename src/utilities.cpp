#include <vector>
#include <cstdint>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <cctype>
#include <openssl/sha.h>
#include <algorithm> // For std::reverse

class Utilities
{
public:
    std::vector<uint8_t> little_big_endian(std::vector<uint8_t> bytes)
    {
        std::vector<uint8_t> rbytes;
        for (auto it = bytes.rbegin(); it != bytes.rend(); it++)
            rbytes.push_back(*it);
        return rbytes;
    }
    uint64_t byte_uint(uint8_t byte)
    {
        return static_cast<int>(byte);
    }
    // not good To Check !!!!!
    // check index 79
    uint64_t bytes_uint_vector(std::vector<uint8_t> &bytes)
    {
        uint64_t result = 0;

        for (size_t i = 0; i < bytes.size(); ++i)
        {
            result |= static_cast<uint64_t>(bytes[i]) << (i * 8);
        }
        return result;
    }
    std::string bytes_hex_vector(std::vector<uint8_t> &bytes)
    {
        std::ostringstream oss;
        for (const auto &byte : bytes)
        {
            oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
        }
        return oss.str();
    }

    uint8_t get_size(uint8_t byte)
    {
        uint8_t size = byte_uint(byte);
        if (size < 253)
            return size;
        if (size == 253)
            return 3;
        if (size == 254)
            return 5;
        return 9;
    }
    std::vector<uint8_t> read_compact_size(std::ifstream &file, uint8_t b)
    {
        uint8_t size = byte_uint(b);
        if (size < 253)
            return std::vector<uint8_t>({b});
        else
        {
            uint8_t byte;
            uint8_t bytes_to_read = 9;
            std::vector<uint8_t> buffer;
            if (size == 253)
                bytes_to_read = 3;
            if (size == 254)
                bytes_to_read = 5;
            for (int i = 1; i < bytes_to_read; i++)
            {
                file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
                buffer.push_back(byte);
            }
            return buffer;
        }
    }
    std::vector<uint8_t> int_to_compact_size(uint64_t value)
    {
        std::vector<uint8_t> result;
        if (value <= 0xfc)
        {
            result.push_back(static_cast<uint8_t>(value));
        }
        else if (value <= 0xffff)
        {
            result.push_back(0xfd);
            result.push_back(static_cast<uint8_t>(value & 0xff));
            result.push_back(static_cast<uint8_t>((value >> 8) & 0xff));
        }
        else if (value <= 0xffffffff)
        {
            result.push_back(0xfe);
            result.push_back(static_cast<uint8_t>(value & 0xff));
            result.push_back(static_cast<uint8_t>((value >> 8) & 0xff));
            result.push_back(static_cast<uint8_t>((value >> 16) & 0xff));
            result.push_back(static_cast<uint8_t>((value >> 24) & 0xff));
        }
        else
        {
            result.push_back(0xff);
            result.push_back(static_cast<uint8_t>(value & 0xff));
            result.push_back(static_cast<uint8_t>((value >> 8) & 0xff));
            result.push_back(static_cast<uint8_t>((value >> 16) & 0xff));
            result.push_back(static_cast<uint8_t>((value >> 24) & 0xff));
            result.push_back(static_cast<uint8_t>((value >> 32) & 0xff));
            result.push_back(static_cast<uint8_t>((value >> 40) & 0xff));
            result.push_back(static_cast<uint8_t>((value >> 48) & 0xff));
            result.push_back(static_cast<uint8_t>((value >> 56) & 0xff));
        }
        return result;
    }
    std::string to_upper(std::string s)
    {
        for (long unsigned int i = 0; i < s.size(); i++)
            s[i] = toupper(s[i]);
        return s;
    }
    // CRYPTO PARTE
    // Function to convert a hexadecimal string to a binary string

    std::string hex_to_binary(const std::string &hex)
    {
        std::string binary;
        binary.reserve(hex.size() / 2);
        for (std::string::size_type i = 0; i < hex.size(); i += 2)
        {
            std::istringstream iss(hex.substr(i, 2));
            unsigned int byte;
            iss >> std::hex >> byte;
            binary.push_back(static_cast<char>(byte));
        }
        return binary;
    }

    // Function to compute SHA-256 hash
    std::string sha256(const std::string &input)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, input.c_str(), input.size());
        SHA256_Final(hash, &sha256);

        return std::string(reinterpret_cast<char *>(hash), SHA256_DIGEST_LENGTH);
    }

    // Function to convert binary string to hexadecimal string
    std::string binary_to_hex(const std::string &binary)
    {
        std::stringstream ss;
        for (unsigned char c : binary)
        {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
        }
        return ss.str();
    }
    // Function to perform the complete hashing process
    std::string double_sha256_reverse(const std::string &hex_input)
    {
        // Step 1: Convert hex to binary
        std::string binary_input = hex_to_binary(hex_input);

        // Step 2: Compute SHA-256 hash twice
        std::string hash1 = sha256(binary_input);
        std::string hash2 = sha256(hash1);

        // Step 3: Reverse the final hash
        std::reverse(hash2.begin(), hash2.end());

        // Convert the result to hexadecimal for output
        return binary_to_hex(hash2);
    }
};