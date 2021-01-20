#pragma once
#define BITSINCHAR 8
#define CHARSINBLOCK 8
#define BITSINBLOCK 64
void readAsciiFile(char* filePath, char* fileContent);
void printCharBits(char c);
char** stringToBlocksArray(char* stream, int* blocksCount_p);
void freeBlocksArray(char** blocksArray, int blocksCount);
void printBlockStr(char* block);
void printBlockBits(char* block);
