
#include <iostream>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <iomanip> // Include this header for std::hex manipulator#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <chrono>
#include <bits/stdc++.h>


using namespace std;

vector<pair<unsigned char*, int>> loadDataFromCSV(const string& filename) {
    vector<pair<unsigned char*, int>> data;

    ifstream csvFile(filename);
    if (!csvFile) {
        cerr << "Error opening file: " << filename << endl;
        return data;
    }

    string line;
    while (getline(csvFile, line)) {
        istringstream iss(line);
        string hexString;
        int intValue;

        if (getline(iss, hexString, ',')) {
            if (iss >> intValue) {
                unsigned char buffer[16] = {};
                for (int i = 0; i < 16; ++i) {
                    string byteHex = hexString.substr(i * 2, 2);
                    buffer[i] = static_cast<unsigned char>(stoi(byteHex, nullptr, 16));
                }
                unsigned char* charPtr = new unsigned char[16];
                memcpy(charPtr, buffer, sizeof(buffer));
                data.push_back(make_pair(charPtr, intValue));
            } else {
                cerr << "Error parsing integer value in line: " << line << endl;
            }
        } else {
            cerr << "Error parsing hexadecimal string in line: " << line << endl;
        }
    }

    csvFile.close();

    return data;
}

// Function to initialize OpenSSL library
void init_openssl() {
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
}

// Function to clean up OpenSSL library
void cleanup_openssl() {
    EVP_cleanup();
}

// Function to read the AES key from a binary file
bool read_aes_key_from_file(const string& filename, unsigned char* key, int key_length) {
    ifstream file(filename, std::ios::binary);
    if (!file) {
        cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    file.read(reinterpret_cast<char*>(key), key_length);
    if (!file) {
        cerr << "Error reading from file: " << filename << std::endl;
        return false;
    }

    file.close();
    return true;
}

// Function to encrypt an integer into 16 byte ciphertext with AES
int aes_encrypt(const unsigned char* key, const unsigned char* iv, int value, int plaintext_len, unsigned char* ciphertext) {
    
    // Convert the integer to a byte array
    unsigned char plaintext[sizeof(unsigned int)];
    for (size_t i = 0; i < sizeof(unsigned int); ++i) {
        plaintext[i] = (value >> (i * 8)) & 0xFF;
    }

    EVP_CIPHER_CTX* ctx;
    int len, ciphertext_len;

    // Create and initialize the context
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

    // Encrypt the plaintext
    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;

    // Finalize the encryption    (-------)(-------)
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    // Clean up the context
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

// Function to decrypt 16 byte ciphertext with AES to an integer
int aes_decrypt(const unsigned char* key, const unsigned char* iv, const unsigned char* ciphertext, int ciphertext_len, unsigned int &decrypted_integer) {
    EVP_CIPHER_CTX* ctx;
    int len, plaintext_len;

    unsigned char plaintext[sizeof(unsigned int)];

    // Create and initialize the context
    ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

    // Decrypt the ciphertext
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;

    // Finalize the decryption
    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;

    // Convert the decrypted byte array back to an integer
    decrypted_integer = 0;
    for (size_t i = 0; i < plaintext_len; ++i) {
        decrypted_integer |= static_cast<unsigned int>(plaintext[i]) << (i * 8);
    }


    // Clean up the context
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

// Function to create Hash
void createHash(const unsigned char* encryptedData, int input, unsigned char* hashOutput) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, encryptedData, 16);
    EVP_DigestUpdate(mdctx, reinterpret_cast<const unsigned char*>(&input), sizeof(input));
    EVP_DigestFinal_ex(mdctx, hashOutput, NULL);
    EVP_MD_CTX_free(mdctx);
}

// Function to produce PRN
int mapHashToOutput(const unsigned char* hashValue) {
    // Convert the hash value to an integer
    seed_seq seed(hashValue, hashValue + 32);
    // Initialize the pseudo-random number generator with the hash value as the seed
    std::mt19937_64 prng(seed);

    // Generate a pseudo-random number
    uint64_t randomNumber = prng();

    return randomNumber;
}

int main() {
    string filename = "inverted_index.csv";
    vector<pair<unsigned char*, int>> inverted_index_table = loadDataFromCSV(filename);


    // printind loaded data from inverted index tree
    for(const auto& row : inverted_index_table){
        for(int i=0; i < 16; i++){
            cout << hex << setw(2) << setfill('0') << static_cast<int>(row.first[i]);
            cout << dec << " ";
        }
        cout << "       ";
        cout << row.second;
        cout << endl;

    }

    // AES key setup
    string keyFilename = "aes_key.bin";
    const int keyLength = 16; // AES-128 uses 16 bytes (128 bits) key
    unsigned char key[keyLength];

    // Read key from existing file
    if (!read_aes_key_from_file(keyFilename, key, keyLength)) {
        cerr << "Failed to read AES key from file." << endl;
        return 1;
    }

    for(const auto& row : inverted_index_table){
        for(int i=0; i <=3; i++){
            // Create a hash value from the input
            unsigned char hashValue[32];
            createHash(row.first, i, hashValue);
            int check_id = mapHashToOutput(hashValue);
            if(check_id == row.second) cout << i << "  ";
        }
        cout << endl;
    }

    cout << endl;
    cout << endl;
    cout << endl;

    int querry_user_id = 1;
    
    unsigned char* ciphertext = new unsigned char[16];

    const int ivLength = 16;
    unsigned char iv[ivLength] = { 'I', 'n', 'i', 't', 'V', 'e', 'c', 't', 'o', 'r', 'F', 'o', 'r', 'U', 's', 'e' };

    
    int ciphertext_len = aes_encrypt(key, iv, querry_user_id, sizeof(unsigned int), ciphertext);
    auto startTime = std::chrono::high_resolution_clock::now();
    for(int i =0; i<=3; i++){
        unsigned char hashValue[16];
        createHash(ciphertext, i, hashValue);
        int check_id = mapHashToOutput(hashValue);
        for(int j=0; j < inverted_index_table.size(); j++){
            if(check_id == inverted_index_table[j].second) cout << i;
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    cout << endl;
    std::cout << "Time taken by the loop: " << duration << " microseconds" << std::endl;

    // deallocate memory for the dynamically allocated char arrays:
    for (const auto& row : inverted_index_table) {
        delete[] row.first;
    }

    return 0;
}



