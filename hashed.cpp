#include <iostream>
#include <random>
#include <openssl/aes.h> // For AES encryption (you need to link with OpenSSL library)
#include <openssl/evp.h> // For EVP interface (you need to link with OpenSSL library)

// Function to create a hash value from the combination of the encrypted value and the integer input
void createHash(const unsigned char* encryptedData, int input, unsigned char* hashOutput) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, encryptedData, 16);
    EVP_DigestUpdate(mdctx, reinterpret_cast<const unsigned char*>(&input), sizeof(input));
    EVP_DigestFinal_ex(mdctx, hashOutput, NULL);
    EVP_MD_CTX_free(mdctx);
}

// Function to map the hash value to the output range (1 to 50)
int mapHashToOutput(const unsigned char* hashValue) {
    // Convert the hash value to an integer
    uint64_t hashIntValue = *reinterpret_cast<const uint64_t*>(hashValue);

    // Initialize the pseudo-random number generator with the hash value as the seed
    std::mt19937_64 prng(hashIntValue);

    // Generate a pseudo-random number
    uint64_t randomNumber = prng();

    // Map the pseudo-random number to the output range (1 to 50)
    int mappedOutput = (randomNumber % 50) + 1;

    return mappedOutput;
}

int main() {
    // Example input
    unsigned char encryptedData[16] = {'f', 'N', 'i', 't', '4', 'e', 'c', 't', 'o', 'w', 'F', 'o', 'r', 'U', 'L', '-'}; // Replace with actual encrypted data
    int inputInteger = 0; // Replace with your integer input

    // Create a hash value from the input
    unsigned char hashValue[EVP_MAX_MD_SIZE];
    createHash(encryptedData, inputInteger, hashValue);

    // Map the hash value to the output range using the pseudo-random function
    int outputInteger = mapHashToOutput(hashValue);

    std::cout << "Mapped output: " << outputInteger << std::endl;

    return 0;
}