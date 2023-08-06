#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

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

    string s = "something like this";

    vector<LweSample*> ciphertext;
    printf("vector defined...\n");
    for(int i=0; i < s.size(); i++){
        int8_t a = s[i];

        LweSample* temp = new_gate_bootstrapping_ciphertext_array(8, params);
        for (int j=0; j<8; j++) {
            bootsSymEncrypt(&temp[j], (a>>j)&1, key);
        }
        ciphertext.push_back(temp);
    }
    printf("encryption done...\n");

    FILE* encrypted_string = fopen("encrypted_string.data","wb");

    for(int i=0; i<s.size(); i++){
        for(int j=0; j<8; j++){
            export_gate_bootstrapping_ciphertext_toFile(encrypted_string, &ciphertext[i][j], params);
        }
    }

    fclose(encrypted_string);

    printf("done creating encrypted string file");

    for(int i=0; i < s.size(); i++) delete_gate_bootstrapping_ciphertext_array(8, ciphertext[i]);
    delete_gate_bootstrapping_secret_keyset(key);
    delete_gate_bootstrapping_parameters(params);

    return 0;
}

