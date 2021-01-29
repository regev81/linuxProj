#pragma once
#define BITSINCHAR 8
#define CHARSINBLOCK 8
#define BITSINBLOCK 64

int findFileSize(char* filePath);
char* fileToCharArray(char* filePath, int charArrSize);
int* charArrToBitsArr(char* charArr, int charArrSize);
int* charToBitsArr(char c);
void printBitsArr(int* bitsArr, int size);
char** CharArrToCharBlocks(char* charArr, int charCount, int* blockCount_p);
void printCharsBlock(char** charsBlocksArr, int blocksAmount);
void printCharBlock(char* charBlock);
int* blockPermutation(int* bitsBlock);