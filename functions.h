#pragma once
#define BITSINCHAR 8
#define CHARSINBLOCK 8
#define BITSINBLOCK 64
void readAsciiFile(char* filePath, char* fileContent);
void printCharBits(char c);
void printBlockBits(unsigned long block);
