#include <stdio.h>
#include<stdlib.h>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>

int main()
{
   //get secrete key 
   FILE* secret_key=fopen("secret.key","rb");
   TFheGateBootstrappingSecretKeySet* key = new_tfheGateBootstrappingSecretKeySet_fromFile(secret_key);
    fclose(secret_key);

     const TFheGateBootstrappingParameterSet* params=key->params;
     LweSample* ci_value=new_gate_bootstrapping_ciphertext_array(16,params);
    
	
    int value;
    int number;
    printf("Enter your choice:\n 1. Pregnancy\n 2. Glucose\n 3.blood_pressure\n 4.skin_thick\n 5.Insulin\n 6.BMI\n 7.Diabeties_pedig\n 8.age\n 9.outcome\n");
    scanf("%d",&number);
    switch(number){
    case 1:
           printf("\nEnter pregnancy value range from (0-13): \t");
           scanf("%d",&value);
           printf("\nSelect * from EDB where pregnancy=%d\n",value);
  
           for(int i=0;i<16;i++){
           bootsSymEncrypt(&ci_value[i],(value>>i)&1,key);
           }
   
          break;
     case 2:
           printf("\nEnter glucose label range from (0-200):\t");
           scanf("%d",&value);
           printf("\nSelect * from EDB where gluclose=%d\n",value);
  
           for(int i=0;i<16;i++){
           bootsSymEncrypt(&ci_value[i],(value>>i)&1,key);
           }
   
          break;
    case 3:
           printf("\nEnter blood pressure value range from (0-130):\t");
           scanf("%d",&value);
           printf("\nSelect * from EDB where blood pressure=%d\n",value);
  
           for(int i=0;i<16;i++){
           bootsSymEncrypt(&ci_value[i],(value>>i)&1,key);
           }
   
          break;
    case 4:
           printf("\nEnter skin thick  range from (0-60):\t");
           scanf("%d",&value);
           printf("\nSelect * from EDB where skin thick=%d\n",value);
  
           for(int i=0;i<16;i++){
           bootsSymEncrypt(&ci_value[i],(value>>i)&1,key);
           }
   
          break;
    case 5:
           printf("\nEnter Insulin range from (0-200):\t");
           scanf("%d",&value);
           printf("\nSelect * from EDB where Insulin=%d\n",value);
  
           for(int i=0;i<16;i++){
           bootsSymEncrypt(&ci_value[i],(value>>i)&1,key);
           }
   
          break;
   case 6:
           printf("\nEnter BMI range from (0-57):\t");
           scanf("%d",&value);
           printf("\nSelect * from EDB where BMI=%d\n",value);
  
           for(int i=0;i<16;i++){
           bootsSymEncrypt(&ci_value[i],(value>>i)&1,key);
           }
   
          break;
   case 7:
           printf("\nEnter diabeties pedigree range from (0-2):\t");
           scanf("%d",&value);
           printf("\nSelect * from EDB where diabeties pedigree=%d\n",value);
  
           for(int i=0;i<16;i++){
           bootsSymEncrypt(&ci_value[i],(value>>i)&1,key);
           }
   
          break;
  case 8:
           printf("\nEnter age range from (21-70):\t");
           scanf("%d",&value);
           printf("\nSelect * from EDB where age=%d\n",value);
  
           for(int i=0;i<16;i++){
           bootsSymEncrypt(&ci_value[i],(value>>i)&1,key);
           }
   
          break;
  case 9:
           printf("\nEnter outcome range from (0-1):\t");
           scanf("%d",&value);
           printf("\nSelect * from EDB where outcome=%d\n",value);
  
           for(int i=0;i<16;i++){
           bootsSymEncrypt(&ci_value[i],(value>>i)&1,key);
           }
   
          break;
 
  default: 
         printf("\nWrong input");
	/* store the encrypted value */
 }
   FILE* query_data=fopen("/home/user/new_select/select_single_condition/equal_condition_check/Cloud_storage/Query.data","wb");
   FILE* query_data1=fopen("/home/user/new_select/select_single_condition/equal_condition_check/Cloud_storage/Query1.data","wb");
     for(int i=0;i<16;i++)
     {
      export_gate_bootstrapping_ciphertext_toFile(query_data, &ci_value[i],params);
      
     }
    fclose(query_data);
    fprintf(query_data1,"%d",number);
    fclose(query_data1);

   //delete_gate_bootstrapping_ciphertext_array(16, ciphertext);
   delete_gate_bootstrapping_secret_keyset(key);
    //delete_gate_bootstrapping_parameters(params);
}

