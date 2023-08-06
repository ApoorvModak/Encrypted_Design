#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#define data_size 16
#define row_num 767

struct plaintext
{

    int32_t rice;
    int32_t barley;
    int32_t wheat;
    int32_t maize;
    int32_t jowar;
    int32_t potato;

};

struct ciphertext
 {

  LweSample* rice;
  LweSample* barley;
  LweSample* wheat;
  LweSample* maize;
  LweSample* jowar;
  LweSample* potato;

 };

struct ciphertext ciphertext[row_num];
struct plaintext plaintext[row_num];

int main() {
    //generate a keyset
    const int minimum_lambda = 110;
    TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(minimum_lambda);

    //generate a random key
    uint32_t seed[] = { 314, 1592, 657 };
    tfhe_random_generator_setSeed(seed,3);
    TFheGateBootstrappingSecretKeySet* key = new_random_gate_bootstrapping_secret_keyset(params);

    //export the secret key to file for later use
    FILE* secret_key = fopen("secret.key","wb");
    export_tfheGateBootstrappingSecretKeySet_toFile(secret_key, key);
    fclose(secret_key);

    //export the cloud key to a file (for the cloud)
    FILE* cloud_key = fopen("cloud.key","wb");
    export_tfheGateBootstrappingCloudKeySet_toFile(cloud_key, &key->cloud);
    fclose(cloud_key);


}