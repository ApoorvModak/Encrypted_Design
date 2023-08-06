#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

int main() {

    //reads the cloud key from file
    FILE* secret_key = fopen("secret.key","rb");
    TFheGateBootstrappingSecretKeySet* key = new_tfheGateBootstrappingSecretKeySet_fromFile(secret_key);
    fclose(secret_key);
 
    //if necessary, the params are inside the key
    const TFheGateBootstrappingParameterSet* params = key->params;

    vector<LweSample*> answer;

    FILE* encrypted_string = fopen("encrypted_string.data","rb");

    printf("string decreption started ...\n");

    for(int x=0; x < 19; x++){

        LweSample* temp = new_gate_bootstrapping_ciphertext_array(8, params);
            for (int i=0; i<8; i++) import_gate_bootstrapping_ciphertext_fromFile(encrypted_string, &temp[i], params);
            int8_t a =0;
            for(int j=0; j<8; j++){
                int ai = bootsSymDecrypt(&temp[j], key);
                a |=(ai<<j);
            }

            char A = a;
            cout << A;
    }
    cout << endl;
    printf("decreption ended...\n");

    return 0;
}
