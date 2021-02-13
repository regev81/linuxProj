#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "functions.h"
#include "const.h"

void calculateParameters(double* p_n, double* p_totient, double* p_e, double* p_d)
{
    double count, k;

    *p_n = p*q;
    *p_totient = (p - 1) * (q - 1);

    //for checking co-prime which satisfies e>1
    *p_e = 2;
    while (*p_e < *p_totient) 
    {
        count = gcd(*p_e, *p_totient);
        if (count == 1)
            break;
        else
            (*p_e)++;
    }

    //choosing d such that it satisfies d*e = 1 + k * totient
    k = 2;
    *p_d = (1 + (k * (*p_totient))) / (*p_e);
}

double encryptCharToDouble(char charData, double e, double n)
{
    double m = charData;
    return fmod(pow(m, e), n);
}

char decryptDoubleToChar(double doubleData, double d, double n)
{
    double c = doubleData;
    return fmod(pow(c, d), n);
}

double* encryptString(char* str, int size, double n, double e)
{
    double* encryptedStr = (double*)malloc(size * sizeof(double));
    int i;
    for (i = 0; i < size; i++)
    {
        encryptedStr[i] = encryptCharToDouble(str[i], n, e);
    }

    return encryptedStr;
}

char* decryptDoublesArr(double* arr, int size, double d, double n)
{
    char* decryptedStr = (char*)malloc(size * sizeof(char));
    int i;
    for (i = 0; i < size; i++)
    {
        decryptedStr[i] = decryptDoubleToChar(arr[i], d, n);
    }

    return decryptedStr;
}

void printEncryptedPassword(double* arr, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%f", arr[i]);
    }
}

//to find gcd
int gcd(int a, int h)
{
    int temp;
    while (1)
    {
        temp = a % h;
        if (temp == 0)
            return h;
        a = h;
        h = temp;
    }
}