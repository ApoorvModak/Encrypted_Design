#include <stdio.h>
#include<stdlib.h>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#define row_num 767
#define data_size 16
struct plaintext
{

 int32_t pregnancies;
 int32_t glucose;
 int32_t blood_p;
 int32_t skin_thik;
 int32_t insulin;
 int32_t BMI;
 int32_t diabeties_pedig;
 int32_t age;
 int32_t outcome;
  };




struct ciphertext
 {

  LweSample* ci_pregnancies;
  LweSample* ci_glucose;
  LweSample* ci_blood_p;
  LweSample* ci_skin_thik;
  LweSample* ci_insulin;
  LweSample* ci_BMI;
  LweSample* ci_diabeties_pedig;
  LweSample* ci_age;
  LweSample* ci_outcome;

 };
struct ciphertext ciphertext[row_num];
struct plaintext plaintext[row_num];


//typedef struct plaintext plaintext;
int main()
 {

//generate a keyset
   const int minimum_lambda=110;
   TFheGateBootstrappingParameterSet* params=new_default_gate_bootstrapping_parameters(minimum_lambda);
  //generate  a random key

   uint32_t seed[]={314,1592,657};
   tfhe_random_generator_setSeed(seed,3);

   TFheGateBootstrappingSecretKeySet* key = new_random_gate_bootstrapping_secret_keyset(params);


/* create a file to store data */
char const* const filename="dataset.txt";
   FILE *fp=fopen(filename,"r");

int p=0;
/* take input from text file and store in a structure array */
while(fscanf(fp,"%d%d%d%d%d%d%d%d%d",&plaintext[p].pregnancies,&plaintext[p].glucose,&plaintext[p].blood_p,&plaintext[p].skin_thik,&plaintext[p].insulin,&plaintext[p].BMI,&plaintext[p].diabeties_pedig,&plaintext[p].age,&plaintext[p].outcome)!=EOF)
   {

     p++;
   }
for(int i=0;i<p;i++)
{
 printf("\n%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t",plaintext[i].pregnancies,plaintext[i].glucose,plaintext[i].blood_p,plaintext[i].skin_thik,plaintext[i].insulin,plaintext[i].BMI,plaintext[i].diabeties_pedig,plaintext[i].age,plaintext[i].outcome);

}

  

/* initialize the ciphertext structure array*/
for(int i=0;i<row_num;i++)
{

ciphertext[i].ci_pregnancies=new_gate_bootstrapping_ciphertext_array(data_size,params);
ciphertext[i].ci_glucose=new_gate_bootstrapping_ciphertext_array(data_size,params);
ciphertext[i].ci_blood_p=new_gate_bootstrapping_ciphertext_array(data_size,params);
ciphertext[i].ci_skin_thik=new_gate_bootstrapping_ciphertext_array(data_size,params);
ciphertext[i].ci_insulin=new_gate_bootstrapping_ciphertext_array(data_size,params);
ciphertext[i].ci_BMI=new_gate_bootstrapping_ciphertext_array(data_size,params);
ciphertext[i].ci_diabeties_pedig=new_gate_bootstrapping_ciphertext_array(data_size,params);
ciphertext[i].ci_age=new_gate_bootstrapping_ciphertext_array(data_size,params);
ciphertext[i].ci_outcome=new_gate_bootstrapping_ciphertext_array(data_size,params);
}


/* Encrypt the plaintexts and store in ciphertexts array */
  for (int j=0;j<row_num;j++)
  {
    for(int n=0;n<data_size;n++)
     {

      bootsSymEncrypt(&ciphertext[j].ci_pregnancies[n],(plaintext[j].pregnancies>>n)&1,key);
      bootsSymEncrypt(&ciphertext[j].ci_glucose[n],(plaintext[j].glucose>>n)&1,key);
      bootsSymEncrypt(&ciphertext[j].ci_blood_p[n],(plaintext[j].blood_p>>n)&1,key);
      bootsSymEncrypt(&ciphertext[j].ci_skin_thik[n],(plaintext[j].skin_thik>>n)&1,key);
      bootsSymEncrypt(&ciphertext[j].ci_insulin[n],(plaintext[j].insulin>>n)&1,key);
      bootsSymEncrypt(&ciphertext[j].ci_BMI[n],(plaintext[j].BMI>>n)&1,key);
      bootsSymEncrypt(&ciphertext[j].ci_diabeties_pedig[n],(plaintext[j].diabeties_pedig>>n)&1,key);
      bootsSymEncrypt(&ciphertext[j].ci_age[n],(plaintext[j].age>>n)&1,key);
      bootsSymEncrypt(&ciphertext[j].ci_outcome[n],(plaintext[j].outcome>>n)&1,key);
     }

  }

  


 //export the secret key to file for later use

     FILE* Secret_key=fopen("secret.key","wb");
     export_tfheGateBootstrappingSecretKeySet_toFile(Secret_key, key);

     fclose(Secret_key);
 
    //export the cloud key to file for later use 

     FILE* cloud_key=fopen("/home/user/new_select/select_single_condition/equal_condition_check/Cloud_storage/cloud.key","wb");
     export_tfheGateBootstrappingCloudKeySet_toFile(cloud_key, &key->cloud);
     fclose(cloud_key);
     //export the encrypted data to cloud

     FILE* cloud_data=fopen("/home/user/new_select/select_single_condition/equal_condition_check/Cloud_storage/cloud.data","wb");

     for(int j=0;j<row_num;j++){
     for(int n=0;n<data_size;n++)
      {

      export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext[j].ci_pregnancies[n],params);
      export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext[j].ci_glucose[n],params);
      export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext[j].ci_blood_p[n],params);
      export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext[j].ci_skin_thik[n],params);
      export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext[j].ci_insulin[n],params);
      export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext[j].ci_BMI[n],params);
      export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext[j].ci_diabeties_pedig[n],params);
      export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext[j].ci_age[n],params);
      export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext[j].ci_outcome[n],params);

      }
  }
      fclose(cloud_data);

     
    //clean up all pointer
for(int i=0;i<row_num;i++)
{

     delete_gate_bootstrapping_ciphertext_array(data_size,ciphertext[i].ci_pregnancies);
     delete_gate_bootstrapping_ciphertext_array(data_size,ciphertext[i].ci_glucose);
     delete_gate_bootstrapping_ciphertext_array(data_size,ciphertext[i].ci_blood_p);
     delete_gate_bootstrapping_ciphertext_array(data_size,ciphertext[i].ci_skin_thik);
     delete_gate_bootstrapping_ciphertext_array(data_size,ciphertext[i].ci_insulin);
     delete_gate_bootstrapping_ciphertext_array(data_size,ciphertext[i].ci_BMI);
     delete_gate_bootstrapping_ciphertext_array(data_size,ciphertext[i].ci_diabeties_pedig);
     delete_gate_bootstrapping_ciphertext_array(data_size,ciphertext[i].ci_age);
     delete_gate_bootstrapping_ciphertext_array(data_size,ciphertext[i].ci_outcome);

}

   
    delete_gate_bootstrapping_secret_keyset(key);
    delete_gate_bootstrapping_parameters(params);
 return 0;
 }
