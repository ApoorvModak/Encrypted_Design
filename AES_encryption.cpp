#include <iostream>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <iomanip> // Include this header for std::hex manipulator

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

int aes_encrypt(const unsigned char* key, const unsigned char* iv, const unsigned char* plaintext, int plaintext_len, unsigned char* ciphertext) {
    EVP_CIPHER_CTX* ctx;
    int len, ciphertext_len;

    // Create and initialize the context
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

    // Encrypt the plaintext
    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;

    // Finalize the encryption
    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    // Clean up the context
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int aes_decrypt(const unsigned char* key, const unsigned char* iv, const unsigned char* ciphertext, int ciphertext_len, unsigned char* plaintext) {
    EVP_CIPHER_CTX* ctx;
    int len, plaintext_len;

    // Create and initialize the context
    ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

    // Decrypt the ciphertext
    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;

    // Finalize the decryption
    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;

    // Clean up the context
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int main() {
    // Initialize OpenSSL library
    init_openssl();

    // AES key setup - Generate a random AES key
    const int keyLength = 16; // AES-128 uses 16 bytes (128 bits) key
    unsigned char key[keyLength];
    generate_random_aes_key(key, keyLength);

    // Input integer to be encrypted
    unsigned int inputInteger = 90; // Replace with your desired integer

    // Convert the integer to a byte array
    unsigned char plaintext[sizeof(unsigned int)];
    for (size_t i = 0; i < sizeof(unsigned int); ++i) {
        plaintext[i] = (inputInteger >> (i * 8)) & 0xFF;
    }

    // Constant Initialization Vector (IV) - Defined using characters
    const int ivLength = 16;
    unsigned char iv[ivLength] = { 'I', 'n', 'i', 't', 'V', 'e', 'c', 't', 'o', 'r', 'F', 'o', 'r', 'U', 's', 'e' };

    // Buffer to store the encrypted data
    unsigned char ciphertext[EVP_MAX_BLOCK_LENGTH];
    std::cout << " max block length :-" << EVP_MAX_BLOCK_LENGTH << std::endl;

    // Perform AES encryption
    int ciphertext_len = aes_encrypt(key, iv, plaintext, sizeof(unsigned int), ciphertext);

    std::cout << "cipher text length: " << ciphertext_len << std::endl;
    std::cout << "Encrypted Integer (in hexadecimal): ";
    // Output the encrypted data (in hexadecimal format)
    for (int i = 0; i < ciphertext_len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ciphertext[i]);
    }
    std::cout << std::dec << std::endl; // Restore decimal output

    // Buffer to store the decrypted data
    unsigned char decrypted[sizeof(unsigned int)];

    // Perform AES decryption
    int decrypted_len = aes_decrypt(key, iv, ciphertext, ciphertext_len, decrypted);

    std::cout << "decrypted text length: " << decrypted_len << std::endl;

    // Convert the decrypted byte array back to an integer
    unsigned int decrypted_integer = 0;
    for (size_t i = 0; i < decrypted_len; ++i) {
        decrypted_integer |= static_cast<unsigned int>(decrypted[i]) << (i * 8);
    }

    // Output the decrypted integer
    std::cout << "Decrypted Integer: " << decrypted_integer << std::endl;

    // Clean up OpenSSL library
    cleanup_openssl();

    return 0;
}