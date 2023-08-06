#include <iostream>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <iomanip> // Include this header for std::hex manipulator
#include <bits/stdc++.h>
using namespace std;

typedef std::vector<int> Row;
typedef std::vector<Row> CSVData;

typedef std::pair<unsigned char * , int> Inverted_Row;
typedef std::vector<Inverted_Row> CSVData2;

// Function to initialize OpenSSL library
void init_openssl() {
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
}

// Function to clean up OpenSSL library
void cleanup_openssl() {
    EVP_cleanup();
}

// Function to generate a random AES key
void generate_random_aes_key(unsigned char* key, int key_length) {
    if (RAND_bytes(key, key_length) != 1) {
        std::cerr << "Error generating random bytes for AES key" << std::endl;
        // Handle error, exit, or use a secure fallback key
    }
}

// Function to save the AES key to a binary file
bool save_aes_key_to_file(const string& filename, const unsigned char* key, int key_length) {
    ofstream file(filename, std::ios::binary);
    if (!file) {
        cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    file.write(reinterpret_cast<const char*>(key), key_length);
    file.close();
    return true;
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

// Function to compare two encrypted integers
bool compare_AES_ciphertext(unsigned char * ciphertext1, unsigned char * ciphertext2){
    for(int i =0; i< 16; i++){
        if(ciphertext1[i] != ciphertext2[i]) return false;
    }
    return true;
}

// Function to read CSV and get data in vector<vector<int>> format
bool readCSV(const std::string& filename, CSVData& index_table) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Row row;
        int value;
        char comma;
        while (iss >> value) {
            row.push_back(value);
            if (iss.peek() == ',')
                iss >> comma; // Read and discard the comma
        }
        index_table.push_back(row);
    }

    file.close();
    return true;

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

// Function to write inverted_index file
void writeDataToCSV(const std::vector<std::pair<unsigned char*, int>>& data, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto& entry : data) {
        for (int i = 0; i < 16; ++i) {
            file << hex << setw(2) << setfill('0') << static_cast<int>(entry.first[i]); // Writing the character array as integer values
            if(i<15){
            }
        }
        file << dec << "," << entry.second << "\n";
    }

    file.close();
}

int main(){

    CSVData index_table;
    CSVData2 inverted_index_table;
    std::string filename = "sample_index_data.csv";

    if (readCSV(filename, index_table)) {
        // Print the CSV data
        for (const auto& row : index_table) {
            for (const auto& value : row) {
                cout << value << "\t";
            }
            cout << endl;
        }
    }

    cout <<  endl;

    // Initialize OpenSSL library
    init_openssl();

    // AES key setup
    string keyFilename = "aes_key.bin";
    const int keyLength = 16; // AES-128 uses 16 bytes (128 bits) key
    unsigned char key[keyLength];

    // Either generate a new key
    generate_random_aes_key(key, keyLength);

    // save it to a file
    if (!save_aes_key_to_file(keyFilename, key, keyLength)) {
        cerr << "Failed to save AES key to file." << endl;
        return 1;
    }

    // Or read key from existing file
    if (!read_aes_key_from_file(keyFilename, key, keyLength)) {
        cerr << "Failed to read AES key from file." << endl;
        return 1;
    }

    // Constant Initialization Vector (IV) - Defined using characters
    const int ivLength = 16;
    unsigned char iv[ivLength] = { 'I', 'n', 'i', 't', 'V', 'e', 'c', 't', 'o', 'r', 'F', 'o', 'r', 'U', 's', 'e' };


    for(auto& row : index_table){
        // Buffer to store the encrypted data
        unsigned char* ciphertext = new unsigned char[16];
        // unsigned char ciphertext[16];
        
        int ciphertext_len = aes_encrypt(key, iv, *(row.begin()), sizeof(unsigned int), ciphertext);

        // Create a hash value from the input
        unsigned char hashValue[32];
        createHash(ciphertext, *(row.begin() + 1), hashValue);

        // Map the hash value to the output range using the pseudo-random function
        int s_id = mapHashToOutput(hashValue);

        // i am placing pointer to a modifyable value, which will obviously give me lastest value, because i am an IDIOT!!!
        // i need to change the datatype to make it store entire array
        Inverted_Row row2 = make_pair(ciphertext, s_id);

        // printing ciphertext for validation
        // for(int i=0; i<16;i++){
        //     cout << static_cast<int>(ciphertext[i]) << "  ";
        // }
        // cout << endl;

        inverted_index_table.push_back(row2);

        row.push_back(s_id);
    }

    cout << endl;
    cout << endl;

    // Display the vector<pair<cipher text , int>>

    for(const auto& row : inverted_index_table){
        for(int i=0; i<16; i++){
            cout << static_cast<int>(row.first[i]) << "  ";
        }
        cout<< ",";
        cout << row.second;
        cout << endl;
    }

    // // Print the CSV data
    // for (const auto& row : index_table) {
    //     for (const auto& value : row) {
    //         std::cout << value << "\t";
    //     }
    //     std::cout << std::endl;
    // }

    // print the inverted index to a file named inverted_index.csv

    // Open the file in truncation mode to erase the existing content
    std::ofstream existingFile("inverted_index.csv", std::ofstream::trunc);
    existingFile.close();

    writeDataToCSV(inverted_index_table, "inverted_index.csv");

    
    return 0;
}
