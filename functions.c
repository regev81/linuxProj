#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include "functions.h"

//read the ascii string from the file
void readAsciiFile(char* filePath, char* fileContent)
{
	FILE* fp;

	fp = fopen(filePath, "r");
	fscanf(fp, "%s", fileContent);
	//printf("File content:\n%s", fileContent);

	fclose(fp);
}

//return array of block (64 bit/8 chars) from string stream
char** stringToBlocksArray(char* stream, int* blocksCount_p)
{
	int i;
	int blocksCount;
	int streamLen = strlen(stream);
	int remainder = streamLen % CHARSINBLOCK;
	int div = streamLen / CHARSINBLOCK;
	
	if (remainder == 0)
	{
		blocksCount = div;
	}
	else
	{
		blocksCount = div+1;
	}
	
	//create the blocksArray
	char** blocksArray = (char**)malloc(blocksCount * sizeof(char*));
	char* currentBlock = stream;
	for (i=0; i < blocksCount; i++)
	{
		blocksArray[i] = (char*)malloc(CHARSINBLOCK+1 * sizeof(char));
		memset(blocksArray[i]+ CHARSINBLOCK, '\0', 1);
		strncpy(blocksArray[i], currentBlock, CHARSINBLOCK);
		currentBlock = currentBlock + CHARSINBLOCK;
		
		//for debug
		//puts(blocksArray[i]);
	}

	*blocksCount_p = blocksCount;
	return blocksArray;
}

//print the block as string
void printBlockStr(char* block)
{
	puts(block);
}

//print the block as bits
void printBlockBits(char* block)
{
	char* c_p = block;
	for (c_p = block;*c_p != '\0'; c_p++)
	{
		printCharBits(*c_p);
	}
}

//print char as bits
void printCharBits(char c)
{
	int i = 0;
	for (int i = BITSINCHAR - 1; 0 <= i; i--)
	{
		printf("%c", (c & (1 << i)) ? '1' : '0');
	}
}

//free the blocks array (from malloc)
void freeBlocksArray(char** blocksArray, int blocksCount)
{
	int i;
	for (i = 0; i < blocksCount; i++)
	{
		free(blocksArray[i]);
	}
}


