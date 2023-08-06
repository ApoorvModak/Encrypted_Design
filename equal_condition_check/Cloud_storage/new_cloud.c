#include <stdio.h>
#include<stdlib.h>
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include<time.h>
#define row_num 10
#define data_size 16

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
  struct ciphertext1
   {
     LweSample* ans_pregnancies;
     LweSample* ans_glucose;
     LweSample* ans_blood_p;
     LweSample* ans_skin_thik;
     LweSample* ans_insulin;
     LweSample* ans_BMI;
     LweSample* ans_diabeties_pedig;
     LweSample* ans_age;
     LweSample* ans_outcome;

   };


  struct ciphertext ciphertext[row_num];
  struct ciphertext1 ciphertext1[row_num];
 
 
    void compare_bit(LweSample* result, const LweSample* a, const LweSample* b, LweSample* lsb_carry, LweSample* tmp, const TFheGateBootstrappingCloudKeySet* bk) {
      LweSample* temp1=new_gate_bootstrapping_ciphertext_array(1, bk->params);
      LweSample* temp2=new_gate_bootstrapping_ciphertext_array(1, bk->params);
      LweSample* temp3=new_gate_bootstrapping_ciphertext_array(1,bk->params);
      LweSample* temp4=new_gate_bootstrapping_ciphertext_array(1,bk->params);
      LweSample* temp5=new_gate_bootstrapping_ciphertext_array(1,bk->params);

      bootsXOR(temp1, a, b, bk);  //a xorb
      bootsXOR(result,temp1,lsb_carry,bk);  //a xor b xor ci
      bootsNOT(temp4,a,bk);  // complement of a
      bootsAND(temp3,temp4,b,bk); // complement a and b
      bootsNOT(temp5,temp1,bk);  // complement of a XOR b
      bootsAND(temp2,temp5,lsb_carry,bk);// complement of a XOR b AND lasb_carry
  
      bootsOR(tmp,temp2,temp3,bk);       // a&b + ci*(a xor b)
      bootsCOPY(lsb_carry,tmp,bk);
    }

    void subtract(LweSample* result, LweSample* tmps, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk) {
    
    //run the elementary comparator gate n times//
      
  	     for (int i=0; i<nb_bits; i++) {
        compare_bit(&result[i], &a[i], &b[i], &tmps[0], &tmps[1], bk);
        }
    }
	
	void compare(LweSample* output,LweSample* first_input,LweSample* second_input,LweSample* select_line,const int nb_bit, const TFheGateBootstrappingCloudKeySet* bk)
  {
      int m=0;
      for(int i=0;i<nb_bit;i++)
        {
         bootsMUX(&output[i],&select_line[m],&second_input[i],&first_input[i],bk);
        }
  }

int main()
 {
   clock_t begin = clock();



    FILE* cloud_key = fopen("cloud.key","rb");
    TFheGateBootstrappingCloudKeySet* bk = new_tfheGateBootstrappingCloudKeySet_fromFile(cloud_key);
    fclose(cloud_key);
    //if necessary, the params are inside the key
    const TFheGateBootstrappingParameterSet* params = bk->params;
    // variable declarartion
    for(int i=0;i<row_num;i++)
     {

      ciphertext[i].ci_pregnancies = new_gate_bootstrapping_ciphertext_array(data_size, params);
      ciphertext[i].ci_glucose = new_gate_bootstrapping_ciphertext_array(data_size, params);
      ciphertext[i].ci_blood_p = new_gate_bootstrapping_ciphertext_array(data_size, params);
      ciphertext[i].ci_skin_thik = new_gate_bootstrapping_ciphertext_array(data_size, params);
      ciphertext[i].ci_insulin = new_gate_bootstrapping_ciphertext_array(data_size, params);
      ciphertext[i].ci_BMI = new_gate_bootstrapping_ciphertext_array(data_size, params);
      ciphertext[i].ci_diabeties_pedig = new_gate_bootstrapping_ciphertext_array(data_size, params);
      ciphertext[i].ci_age = new_gate_bootstrapping_ciphertext_array(data_size, params);
      ciphertext[i].ci_outcome = new_gate_bootstrapping_ciphertext_array(data_size, params);

     }

   for(int i=0;i<row_num;i++)
     {
       ciphertext1[i].ans_pregnancies = new_gate_bootstrapping_ciphertext_array(data_size, params);
       ciphertext1[i].ans_glucose = new_gate_bootstrapping_ciphertext_array(data_size, params);
       ciphertext1[i].ans_blood_p = new_gate_bootstrapping_ciphertext_array(data_size, params);
       ciphertext1[i].ans_skin_thik = new_gate_bootstrapping_ciphertext_array(data_size, params);
       ciphertext1[i].ans_insulin = new_gate_bootstrapping_ciphertext_array(data_size, params);
       ciphertext1[i].ans_BMI = new_gate_bootstrapping_ciphertext_array(data_size, params);
       ciphertext1[i].ans_diabeties_pedig = new_gate_bootstrapping_ciphertext_array(data_size, params);
       ciphertext1[i].ans_age = new_gate_bootstrapping_ciphertext_array(data_size, params);
       ciphertext1[i].ans_outcome = new_gate_bootstrapping_ciphertext_array(data_size, params);
     }

     LweSample* enc_threshold = new_gate_bootstrapping_ciphertext_array(data_size, params);
     for(int i=0;i<data_size;i++){
       bootsCONSTANT(&enc_threshold[i], 0, bk);
     }  
     LweSample* result = new_gate_bootstrapping_ciphertext_array(data_size, params);
     LweSample* result1 = new_gate_bootstrapping_ciphertext_array(data_size, params);
     LweSample* ans= new_gate_bootstrapping_ciphertext_array(1,params);
     //LweSample* ans3= new_gate_bootstrapping_ciphertext_array(1,params);
     LweSample* complement = new_gate_bootstrapping_ciphertext_array(1,params);
     LweSample* complement3 = new_gate_bootstrapping_ciphertext_array(1,params);
     LweSample* complement1 = new_gate_bootstrapping_ciphertext_array(1,params);
     
     LweSample* query = new_gate_bootstrapping_ciphertext_array(data_size, params);
 
    LweSample* signbit = new_gate_bootstrapping_ciphertext_array(2, params);
    LweSample* signbit1= new_gate_bootstrapping_ciphertext_array(2, params);
    LweSample* signbit6 = new_gate_bootstrapping_ciphertext_array(2, params);
    LweSample* signbit7 = new_gate_bootstrapping_ciphertext_array(2, params);
    LweSample* result6 = new_gate_bootstrapping_ciphertext_array(16, params);
    LweSample* result7 = new_gate_bootstrapping_ciphertext_array(16, params);
    LweSample* ans3= new_gate_bootstrapping_ciphertext_array(1,params);
    printf("reading the query...\n");
    int num;
    FILE* query_data = fopen("Query.data","rb");
    for(int i=0;i<data_size;i++)
      {
        import_gate_bootstrapping_ciphertext_fromFile(query_data, &query[i], params); 
       }
    fclose(query_data);
   /* FILE* secret_key=fopen("/home/user/new_select/select_single_condition/equal_condition/check/client_side/secret.key","rb");
    TFheGateBootstrappingSecretKeySet* key = new_tfheGateBootstrappingSecretKeySet_fromFile(secret_key);
    fclose(secret_key);
    int32_t answer=0;
    for(int i=0;i<data_size;i++){
      int abc=bootsSymDecrypt(&query[i],key)>0;
      answer |=(abc<<i);
     }
     printf("\nquery=%d", answer);
    */
   
    FILE* query_data1=fopen("Query1.data","rb");
    fscanf(query_data1,"%d", &num);
  //  printf("Value of n=%d", num);
    printf("reading the cloud data...\n");
    FILE* cloud_data = fopen("cloud.data","rb");
    for(int j=0;j<row_num;j++)
      {
         for(int i=0; i<data_size; i++)
           {

             import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext[j].ci_pregnancies[i], params);
             import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext[j].ci_glucose[i], params);
             import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext[j].ci_blood_p[i], params);
             import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext[j].ci_skin_thik[i], params);
             import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext[j].ci_insulin[i], params);
             import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext[j].ci_BMI[i], params);
             import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext[j].ci_diabeties_pedig[i], params);
             import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext[j].ci_age[i], params);
             import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &ciphertext[j].ci_outcome[i], params);

           }
       }
    fclose(cloud_data); 

     
      printf("doing the homomorphic computation...\n");
    switch(num){
    case 1: 
          //printf("num:%d",num); 
          for(int j=0;j<row_num;j++){
           bootsCONSTANT(&signbit[0], 0, bk); //initialize the carry to 0
	   bootsCONSTANT(&signbit1[0], 0, bk); //initialize the carry to 0		
           subtract(result,signbit, query, ciphertext[j].ci_pregnancies,data_size,bk);   //
	   subtract(result1,signbit1,ciphertext[j].ci_pregnancies,query,data_size,bk); 
           bootsOR(ans,signbit,signbit1,bk);
           bootsNOT(complement,ans,bk); 
		 
		 
   compare(ciphertext1[j].ans_pregnancies,enc_threshold,ciphertext[j].ci_pregnancies,complement,data_size,bk);
		 
       compare(ciphertext1[j].ans_glucose,enc_threshold,ciphertext[j].ci_glucose,complement,data_size,bk);
       compare(ciphertext1[j].ans_blood_p,enc_threshold,ciphertext[j].ci_blood_p,complement,data_size,bk);
       compare(ciphertext1[j].ans_skin_thik,enc_threshold,ciphertext[j].ci_skin_thik,complement,data_size,bk);
       compare(ciphertext1[j].ans_insulin,enc_threshold,ciphertext[j].ci_insulin,complement,data_size,bk);
       compare(ciphertext1[j].ans_BMI,enc_threshold,ciphertext[j].ci_BMI,complement,data_size,bk);
       compare(ciphertext1[j].ans_diabeties_pedig,enc_threshold,ciphertext[j].ci_diabeties_pedig,complement,data_size,bk);
       compare(ciphertext1[j].ans_age,enc_threshold,ciphertext[j].ci_age,complement,data_size,bk);
       compare(ciphertext1[j].ans_outcome,enc_threshold,ciphertext[j].ci_outcome,complement,data_size,bk);

/*int32_t answer=0;	    
      for(int i=0;i<data_size;i++){
      int abc=bootsSymDecrypt(&ciphertext1[j].ans_pregnancies[i],key)>0;
      answer |=(abc<<i);
     }
     printf("\nquery=%d", answer);*/
      }
     break;
    case 2: 
           //printf("num:%d",num); 
          for(int j=0;j<row_num;j++){
           bootsCONSTANT(&signbit[0], 0, bk); //initialize the carry to 0
	   bootsCONSTANT(&signbit1[0], 0, bk); //initialize the carry to 0		
           subtract(result,signbit, query, ciphertext[j].ci_glucose,data_size,bk);   //
	   subtract(result1,signbit1,ciphertext[j].ci_glucose,query,data_size,bk); 
           bootsOR(ans,signbit,signbit1,bk);
           bootsNOT(complement,ans,bk); 
		 
		 
   compare(ciphertext1[j].ans_pregnancies,enc_threshold,ciphertext[j].ci_pregnancies,complement,data_size,bk);
		 
       compare(ciphertext1[j].ans_glucose,enc_threshold,ciphertext[j].ci_glucose,complement,data_size,bk);
       compare(ciphertext1[j].ans_blood_p,enc_threshold,ciphertext[j].ci_blood_p,complement,data_size,bk);
       compare(ciphertext1[j].ans_skin_thik,enc_threshold,ciphertext[j].ci_skin_thik,complement,data_size,bk);
       compare(ciphertext1[j].ans_insulin,enc_threshold,ciphertext[j].ci_insulin,complement,data_size,bk);
       compare(ciphertext1[j].ans_BMI,enc_threshold,ciphertext[j].ci_BMI,complement,data_size,bk);
       compare(ciphertext1[j].ans_diabeties_pedig,enc_threshold,ciphertext[j].ci_diabeties_pedig,complement,data_size,bk);
       compare(ciphertext1[j].ans_age,enc_threshold,ciphertext[j].ci_age,complement,data_size,bk);
       compare(ciphertext1[j].ans_outcome,enc_threshold,ciphertext[j].ci_outcome,complement,data_size,bk);

/*int32_t answer=0;	    
      for(int i=0;i<data_size;i++){
      int abc=bootsSymDecrypt(&ciphertext1[j].ans_pregnancies[i],key)>0;
      answer |=(abc<<i);
     }
     printf("\nquery=%d", answer);*/
      }
     break;
    case 3: 
           //printf("num:%d",num); 
          for(int j=0;j<row_num;j++){
           bootsCONSTANT(&signbit[0], 0, bk); //initialize the carry to 0
	   bootsCONSTANT(&signbit1[0], 0, bk); //initialize the carry to 0		
           subtract(result,signbit, query, ciphertext[j].ci_blood_p,data_size,bk);   //
	   subtract(result1,signbit1,ciphertext[j].ci_blood_p,query,data_size,bk); 
           bootsOR(ans,signbit,signbit1,bk);
           bootsNOT(complement,ans,bk); 
		 
		 
   compare(ciphertext1[j].ans_pregnancies,enc_threshold,ciphertext[j].ci_pregnancies,complement,data_size,bk);
		 
       compare(ciphertext1[j].ans_glucose,enc_threshold,ciphertext[j].ci_glucose,complement,data_size,bk);
       compare(ciphertext1[j].ans_blood_p,enc_threshold,ciphertext[j].ci_blood_p,complement,data_size,bk);
       compare(ciphertext1[j].ans_skin_thik,enc_threshold,ciphertext[j].ci_skin_thik,complement,data_size,bk);
       compare(ciphertext1[j].ans_insulin,enc_threshold,ciphertext[j].ci_insulin,complement,data_size,bk);
       compare(ciphertext1[j].ans_BMI,enc_threshold,ciphertext[j].ci_BMI,complement,data_size,bk);
       compare(ciphertext1[j].ans_diabeties_pedig,enc_threshold,ciphertext[j].ci_diabeties_pedig,complement,data_size,bk);
       compare(ciphertext1[j].ans_age,enc_threshold,ciphertext[j].ci_age,complement,data_size,bk);
       compare(ciphertext1[j].ans_outcome,enc_threshold,ciphertext[j].ci_outcome,complement,data_size,bk);

/*int32_t answer=0;	    
      for(int i=0;i<data_size;i++){
      int abc=bootsSymDecrypt(&ciphertext1[j].ans_pregnancies[i],key)>0;
      answer |=(abc<<i);
     }
     printf("\nquery=%d", answer);*/
      }
     break;
    case 4: 
          // printf("num:%d",num); 
          for(int j=0;j<row_num;j++){
           bootsCONSTANT(&signbit[0], 0, bk); //initialize the carry to 0
	   bootsCONSTANT(&signbit1[0], 0, bk); //initialize the carry to 0		
           subtract(result,signbit, query, ciphertext[j].ci_skin_thik,data_size,bk);   //
	   subtract(result1,signbit1,ciphertext[j].ci_skin_thik,query,data_size,bk); 
           bootsOR(ans,signbit,signbit1,bk);
           bootsNOT(complement,ans,bk); 
		 
		 
   compare(ciphertext1[j].ans_pregnancies,enc_threshold,ciphertext[j].ci_pregnancies,complement,data_size,bk);
		 
       compare(ciphertext1[j].ans_glucose,enc_threshold,ciphertext[j].ci_glucose,complement,data_size,bk);
       compare(ciphertext1[j].ans_blood_p,enc_threshold,ciphertext[j].ci_blood_p,complement,data_size,bk);
       compare(ciphertext1[j].ans_skin_thik,enc_threshold,ciphertext[j].ci_skin_thik,complement,data_size,bk);
       compare(ciphertext1[j].ans_insulin,enc_threshold,ciphertext[j].ci_insulin,complement,data_size,bk);
       compare(ciphertext1[j].ans_BMI,enc_threshold,ciphertext[j].ci_BMI,complement,data_size,bk);
       compare(ciphertext1[j].ans_diabeties_pedig,enc_threshold,ciphertext[j].ci_diabeties_pedig,complement,data_size,bk);
       compare(ciphertext1[j].ans_age,enc_threshold,ciphertext[j].ci_age,complement,data_size,bk);
       compare(ciphertext1[j].ans_outcome,enc_threshold,ciphertext[j].ci_outcome,complement,data_size,bk);

/*int32_t answer=0;	    
      for(int i=0;i<data_size;i++){
      int abc=bootsSymDecrypt(&ciphertext1[j].ans_pregnancies[i],key)>0;
      answer |=(abc<<i);
     }
     printf("\nquery=%d", answer);*/
      }
     break;
    case 5: 
           //printf("num:%d",num); 
          for(int j=0;j<row_num;j++){
           bootsCONSTANT(&signbit[0], 0, bk); //initialize the carry to 0
	   bootsCONSTANT(&signbit1[0], 0, bk); //initialize the carry to 0		
           subtract(result,signbit, query, ciphertext[j].ci_insulin,data_size,bk);   //
	   subtract(result1,signbit1,ciphertext[j].ci_insulin,query,data_size,bk); 
           bootsOR(ans,signbit,signbit1,bk);
           bootsNOT(complement,ans,bk); 
		 
		 
   compare(ciphertext1[j].ans_pregnancies,enc_threshold,ciphertext[j].ci_pregnancies,complement,data_size,bk);
		 
       compare(ciphertext1[j].ans_glucose,enc_threshold,ciphertext[j].ci_glucose,complement,data_size,bk);
       compare(ciphertext1[j].ans_blood_p,enc_threshold,ciphertext[j].ci_blood_p,complement,data_size,bk);
       compare(ciphertext1[j].ans_skin_thik,enc_threshold,ciphertext[j].ci_skin_thik,complement,data_size,bk);
       compare(ciphertext1[j].ans_insulin,enc_threshold,ciphertext[j].ci_insulin,complement,data_size,bk);
       compare(ciphertext1[j].ans_BMI,enc_threshold,ciphertext[j].ci_BMI,complement,data_size,bk);
       compare(ciphertext1[j].ans_diabeties_pedig,enc_threshold,ciphertext[j].ci_diabeties_pedig,complement,data_size,bk);
       compare(ciphertext1[j].ans_age,enc_threshold,ciphertext[j].ci_age,complement,data_size,bk);
       compare(ciphertext1[j].ans_outcome,enc_threshold,ciphertext[j].ci_outcome,complement,data_size,bk);

/*int32_t answer=0;	    
      for(int i=0;i<data_size;i++){
      int abc=bootsSymDecrypt(&ciphertext1[j].ans_pregnancies[i],key)>0;
      answer |=(abc<<i);
     }
     printf("\nquery=%d", answer);*/
      }
     break;
    case 6: 
           //printf("num:%d",num); 
          for(int j=0;j<row_num;j++){
           bootsCONSTANT(&signbit[0], 0, bk); //initialize the carry to 0
	   bootsCONSTANT(&signbit1[0], 0, bk); //initialize the carry to 0		
           subtract(result,signbit, query, ciphertext[j].ci_BMI,data_size,bk);   //
	   subtract(result1,signbit1,ciphertext[j].ci_BMI,query,data_size,bk); 
           bootsOR(ans,signbit,signbit1,bk);
           bootsNOT(complement,ans,bk); 
		 
		 
   compare(ciphertext1[j].ans_pregnancies,enc_threshold,ciphertext[j].ci_pregnancies,complement,data_size,bk);
		 
       compare(ciphertext1[j].ans_glucose,enc_threshold,ciphertext[j].ci_glucose,complement,data_size,bk);
       compare(ciphertext1[j].ans_blood_p,enc_threshold,ciphertext[j].ci_blood_p,complement,data_size,bk);
       compare(ciphertext1[j].ans_skin_thik,enc_threshold,ciphertext[j].ci_skin_thik,complement,data_size,bk);
       compare(ciphertext1[j].ans_insulin,enc_threshold,ciphertext[j].ci_insulin,complement,data_size,bk);
       compare(ciphertext1[j].ans_BMI,enc_threshold,ciphertext[j].ci_BMI,complement,data_size,bk);
       compare(ciphertext1[j].ans_diabeties_pedig,enc_threshold,ciphertext[j].ci_diabeties_pedig,complement,data_size,bk);
       compare(ciphertext1[j].ans_age,enc_threshold,ciphertext[j].ci_age,complement,data_size,bk);
       compare(ciphertext1[j].ans_outcome,enc_threshold,ciphertext[j].ci_outcome,complement,data_size,bk);

/*int32_t answer=0;	    
      for(int i=0;i<data_size;i++){
      int abc=bootsSymDecrypt(&ciphertext1[j].ans_pregnancies[i],key)>0;
      answer |=(abc<<i);
     }
     printf("\nquery=%d", answer);*/
      }
     break;
    case 7: 
           //printf("num:%d",num); 
          for(int j=0;j<row_num;j++){
           bootsCONSTANT(&signbit[0], 0, bk); //initialize the carry to 0
	   bootsCONSTANT(&signbit1[0], 0, bk); //initialize the carry to 0		
           subtract(result,signbit, query, ciphertext[j].ci_diabeties_pedig,data_size,bk);   //
	   subtract(result1,signbit1,ciphertext[j].ci_diabeties_pedig,query,data_size,bk); 
           bootsOR(ans,signbit,signbit1,bk);
           bootsNOT(complement,ans,bk); 
		 
		 
   compare(ciphertext1[j].ans_pregnancies,enc_threshold,ciphertext[j].ci_pregnancies,complement,data_size,bk);
		 
       compare(ciphertext1[j].ans_glucose,enc_threshold,ciphertext[j].ci_glucose,complement,data_size,bk);
       compare(ciphertext1[j].ans_blood_p,enc_threshold,ciphertext[j].ci_blood_p,complement,data_size,bk);
       compare(ciphertext1[j].ans_skin_thik,enc_threshold,ciphertext[j].ci_skin_thik,complement,data_size,bk);
       compare(ciphertext1[j].ans_insulin,enc_threshold,ciphertext[j].ci_insulin,complement,data_size,bk);
       compare(ciphertext1[j].ans_BMI,enc_threshold,ciphertext[j].ci_BMI,complement,data_size,bk);
       compare(ciphertext1[j].ans_diabeties_pedig,enc_threshold,ciphertext[j].ci_diabeties_pedig,complement,data_size,bk);
       compare(ciphertext1[j].ans_age,enc_threshold,ciphertext[j].ci_age,complement,data_size,bk);
       compare(ciphertext1[j].ans_outcome,enc_threshold,ciphertext[j].ci_outcome,complement,data_size,bk);

/*int32_t answer=0;	    
      for(int i=0;i<data_size;i++){
      int abc=bootsSymDecrypt(&ciphertext1[j].ans_pregnancies[i],key)>0;
      answer |=(abc<<i);
     }
     printf("\nquery=%d", answer);*/
      }
     break;
    case 8: 
           //printf("num:%d",num); 
          for(int j=0;j<row_num;j++){
           bootsCONSTANT(&signbit[0], 0, bk); //initialize the carry to 0
	   bootsCONSTANT(&signbit1[0], 0, bk); //initialize the carry to 0		
           subtract(result,signbit, query, ciphertext[j].ci_age,data_size,bk);   //
	   subtract(result1,signbit1,ciphertext[j].ci_age,query,data_size,bk); 
           bootsOR(ans,signbit,signbit1,bk);
           bootsNOT(complement,ans,bk); 
		 
		 
   compare(ciphertext1[j].ans_pregnancies,enc_threshold,ciphertext[j].ci_pregnancies,complement,data_size,bk);
		 
       compare(ciphertext1[j].ans_glucose,enc_threshold,ciphertext[j].ci_glucose,complement,data_size,bk);
       compare(ciphertext1[j].ans_blood_p,enc_threshold,ciphertext[j].ci_blood_p,complement,data_size,bk);
       compare(ciphertext1[j].ans_skin_thik,enc_threshold,ciphertext[j].ci_skin_thik,complement,data_size,bk);
       compare(ciphertext1[j].ans_insulin,enc_threshold,ciphertext[j].ci_insulin,complement,data_size,bk);
       compare(ciphertext1[j].ans_BMI,enc_threshold,ciphertext[j].ci_BMI,complement,data_size,bk);
       compare(ciphertext1[j].ans_diabeties_pedig,enc_threshold,ciphertext[j].ci_diabeties_pedig,complement,data_size,bk);
       compare(ciphertext1[j].ans_age,enc_threshold,ciphertext[j].ci_age,complement,data_size,bk);
       compare(ciphertext1[j].ans_outcome,enc_threshold,ciphertext[j].ci_outcome,complement,data_size,bk);

/*int32_t answer=0;	    
      for(int i=0;i<data_size;i++){
      int abc=bootsSymDecrypt(&ciphertext1[j].ans_age[i],key)>0;
      answer |=(abc<<i);
     }
     printf("\nquery=%d", answer);*/
      }
     break;
    case 9: 
          // printf("num:%d",num); 
          for(int j=0;j<row_num;j++){
           bootsCONSTANT(&signbit[0], 0, bk); //initialize the carry to 0
	   bootsCONSTANT(&signbit1[0], 0, bk); //initialize the carry to 0		
           subtract(result,signbit, query, ciphertext[j].ci_outcome,data_size,bk);   //
	   subtract(result1,signbit1,ciphertext[j].ci_outcome,query,data_size,bk); 
           bootsOR(ans,signbit,signbit1,bk);
           bootsNOT(complement,ans,bk); 
		 
		 
   compare(ciphertext1[j].ans_pregnancies,enc_threshold,ciphertext[j].ci_pregnancies,complement,data_size,bk);
		 
       compare(ciphertext1[j].ans_glucose,enc_threshold,ciphertext[j].ci_glucose,complement,data_size,bk);
       compare(ciphertext1[j].ans_blood_p,enc_threshold,ciphertext[j].ci_blood_p,complement,data_size,bk);
       compare(ciphertext1[j].ans_skin_thik,enc_threshold,ciphertext[j].ci_skin_thik,complement,data_size,bk);
       compare(ciphertext1[j].ans_insulin,enc_threshold,ciphertext[j].ci_insulin,complement,data_size,bk);
       compare(ciphertext1[j].ans_BMI,enc_threshold,ciphertext[j].ci_BMI,complement,data_size,bk);
       compare(ciphertext1[j].ans_diabeties_pedig,enc_threshold,ciphertext[j].ci_diabeties_pedig,complement,data_size,bk);
       compare(ciphertext1[j].ans_age,enc_threshold,ciphertext[j].ci_age,complement,data_size,bk);
       compare(ciphertext1[j].ans_outcome,enc_threshold,ciphertext[j].ci_outcome,complement,data_size,bk);

      /*int32_t answer=0;	    
      for(int i=0;i<data_size;i++){
      int abc=bootsSymDecrypt(&ciphertext1[j].ans_pregnancies[i],key)>0;
      answer |=(abc<<i);
     }
     printf("\nquery=%d", answer);*/
      }
     break;
 default:
       printf("Wrong input");
   }
	
     FILE* answer_data=fopen("/home/user/new_select/select_single_condition/equal_condition_check/client_side/answer.data","wb");

     for(int j=0;j<row_num;j++){
     for(int n=0;n<data_size;n++)
      {

      export_gate_bootstrapping_ciphertext_toFile(answer_data, &ciphertext1[j].ans_pregnancies[n],params);
      export_gate_bootstrapping_ciphertext_toFile(answer_data, &ciphertext1[j].ans_glucose[n],params);
      export_gate_bootstrapping_ciphertext_toFile(answer_data, &ciphertext1[j].ans_blood_p[n],params);
      export_gate_bootstrapping_ciphertext_toFile(answer_data, &ciphertext1[j].ans_skin_thik[n],params);
      export_gate_bootstrapping_ciphertext_toFile(answer_data, &ciphertext1[j].ans_insulin[n],params);
      export_gate_bootstrapping_ciphertext_toFile(answer_data, &ciphertext1[j].ans_BMI[n],params);
      export_gate_bootstrapping_ciphertext_toFile(answer_data, &ciphertext1[j].ans_diabeties_pedig[n],params);
      export_gate_bootstrapping_ciphertext_toFile(answer_data, &ciphertext1[j].ans_age[n],params);
      export_gate_bootstrapping_ciphertext_toFile(answer_data, &ciphertext1[j].ans_outcome[n],params);

      }
  }
  
 
      //fclose(answer_data);
             clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("Processing operation for %d bits took %lf seconds", 16, time_spent);

        //fclose(secret_key);
     for(int i=0;i<row_num;i++)
     {

    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext[i].ci_pregnancies);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext[i].ci_glucose);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext[i].ci_blood_p);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext[i].ci_skin_thik);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext[i].ci_insulin);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext[i].ci_BMI);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext[i].ci_diabeties_pedig);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext[i].ci_age);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext[i].ci_outcome);

  }
 // printf("deletion");
  
   for(int i=0;i<row_num;i++)
  {

    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext1[i].ans_pregnancies);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext1[i].ans_glucose);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext1[i].ans_blood_p);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext1[i].ans_skin_thik);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext1[i].ans_insulin);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext1[i].ans_BMI);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext1[i].ans_diabeties_pedig);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext1[i].ans_age);
    delete_gate_bootstrapping_ciphertext_array(data_size, ciphertext1[i].ans_outcome);

  }
     
    delete_gate_bootstrapping_cloud_keyset(bk);
	delete_gate_bootstrapping_ciphertext_array(1, complement);
	delete_gate_bootstrapping_ciphertext_array(1, ans);
    //delete_gate_bootstrapping_ciphertext_array(data_size, query);
    delete_gate_bootstrapping_ciphertext_array(data_size, result);
    delete_gate_bootstrapping_ciphertext_array(data_size, result1);

 // double end_time = omp_get_wtime();
 //printf("\n......computation of the 16 binary + 32 mux gates took: %.0f second\n",end_time-start_time);
 printf("\ncomplete\n");
 
 }





 


