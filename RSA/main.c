#include<stdio.h>
#include<math.h>
void encrypt(char* input);
void decrypt(double* input);

int main(int argc, char* argv[])
{
    encrypt(argv[1]);
    return 0;
}   


void encrypt(char* input)
{
    double n, totient;
    double e; //public key
    double d; //private key

    calculateParameters(&n, &totient, &e, &d);

    int inputLen = strlen(input);
    double* encryptedStr = encryptString(input, inputLen, n, e);

    printf("orginal password = %s\n", input);
    printf("encrypted password = ");
    printEncryptedPassword(encryptedStr, inputLen);

    free(encryptedStr);
}

void decrypt(double* input)
{
    double n, totient;
    double e; //public key
    double d; //private key

    calculateParameters(&n, &totient, &e, &d);

    int inputLen = 2;
    char* decryptedStr = decryptDoublesArr(input, inputLen, d, n);

    printf("cypher password = %s\n", input);
    printf("decrypted password = ");
    printf(decryptedStr);

    free(decryptedStr);
}