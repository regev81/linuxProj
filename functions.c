#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include "functions.h"


void readAsciiFile(char* filePath, char* fileContent)
{
	FILE* fp;

	fp = fopen(filePath, "r");
	fscanf(fp, "%s", fileContent);
	//printf("File content:\n%s", fileContent);

	fclose(fp);
}

//return block (64 bit) unsigned long from chars array of 8 chars
unsigned long charsBlockToBitsBlock(char charsArray[8])
{
	
	unsigned long block = 0;
	block = charsArray[0];
	int i = 1;

	for (i;i< CHARSINBLOCK;i++)
	{
		block = block << BITSINCHAR;
		block = block + charsArray[i];
	}

	printBlockBits(block);
	return block;
}

//return array off blocks ((64 bit) unsigned long) from the ascii string
unsigned long* asciiToBlocks(char* asciiString)
{
	//char dst[8];
	//strncpy(dst, asciiString, 8);
	//to do
	unsigned long a = 5;
	return &a;
}

void printCharBits(char c)
{
	int i = 0;
	for (int i = BITSINCHAR -1; 0 <= i; i--)
	{
		printf("%c", (c & (1 << i)) ? '1' : '0');
	}
}

void printBlockBits(unsigned long block)
{
	unsigned long mask = 1;
	int i = BITSINBLOCK;
	for (i = i-1 ; i >= 0; i--)
	{
		int bit = (block & (mask << i)) >> i;
		printf("%d", bit);
	}
}