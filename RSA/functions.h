#pragma once
void calculateParameters(double* p_n, double* p_totient, double* p_e, double* p_d);
double encryptCharToDouble(char charData, double e, double n);
char decryptDoubleToChar(double doubleData, double d, double n);
double* encryptString(char* str, int size, double n, double e);
char* decryptDoublesArr(double* arr, int size, double d, double n);
void printEncryptedPassword(double* arr, int size);
int gcd(int a, int h);