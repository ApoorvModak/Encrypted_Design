#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
#define row_num 767

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

struct plaintext plaintext[row_num];

int main(){
    printf("JSON data entry:- \n 1. Pregnancy\n 2. Glucose\n 3.blood_pressure\n 4.skin_thick\n 5.Insulin\n 6.BMI\n 7.Diabeties_pedig\n 8.age\n 9.outcome\n");
    int number;

}