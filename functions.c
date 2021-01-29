#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "functions.h"
#include "Helpers.h"


//get amount of chars in file
int findFileSize(char* filePath)
{
	int size;
	FILE* fp = fopen(filePath, "r");
	if (fseek(fp, 0L, SEEK_END))
		perror("fseek() failed");
	else
		size = ftell(fp);
	fclose(fp);

	return size;
}

//return char array of the file content
char* fileToCharArray(char* filePath, int charArrSize)
{
	int i = 0;
	char ch;

	char* charArr = (char*)malloc((charArrSize+1) * sizeof(char));
	FILE* fp = fopen(filePath, "r");

	while (!feof(fp))
	{
		ch = getc(fp);
		charArr[i] = ch;
		i++;
	}

	fclose(fp);
	charArr[charArrSize] = 0;
	return charArr;

}

//return bits array of the char array
int* charArrToBitsArr(char* charArr, int charArrSize)
{
	int i, j;
	int bitIndex = 0;
	int bitsCount = charArrSize * BITSINCHAR;
	int* bitsArr = (int*)malloc(bitsCount * sizeof(int));
	for (i = 0; i < charArrSize; i++)
	{
		int* charBitsArr = charToBitsArr(charArr[i]);
		for (j = 0; j < BITSINCHAR; j++)
		{
			bitsArr[bitIndex] = charBitsArr[j];
			bitIndex++;
		}
		// free the charBitsArr
		free(charBitsArr);
	}

	return bitsArr;
}

// return bits array of the char
int* charToBitsArr(char c)
{
	int k, m, j;
	int* bitsArr = (int*)malloc(BITSINCHAR * sizeof(int));
	j = 0;
	for (int i = BITSINCHAR-1; i >= 0; i--)
	{
		m = 1 << i;
		k = c & m;
		k = k >> i;
		if (k == 0)
			bitsArr[j] = 0;
		else
			bitsArr[j] = 1;
		j++;
	}

	return bitsArr;
}

void printBitsArr(int* bitsArr, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%d",bitsArr[i]);

	}
	printf("\n");
}

//devide the charArr to blocks (8 chars in block)
char** CharArrToCharBlocks(char* charArr, int charCount,int* blockCount_p)
{
	
	int reminder = charCount % CHARSINBLOCK;
	*blockCount_p = charCount / CHARSINBLOCK;

	if (reminder != 0)
	{
		*blockCount_p = *blockCount_p + 1;
	}

	char** blocksArr = (char**)malloc(*blockCount_p * sizeof(char*));
	int i,j;
	for (i = 0; i < *blockCount_p; i++)
	{
		blocksArr[i] = (char*)malloc((CHARSINBLOCK+1) * sizeof(char));

		for (j = 0; j < CHARSINBLOCK; j++)
		{
			int indexInCharArr = j + (i * CHARSINBLOCK);
			if (indexInCharArr > charCount - 1)
			{
				blocksArr[i][j] = 0;
			}
			else
			{
				blocksArr[i][j] = charArr[j + (i * CHARSINBLOCK)];
			}
		}
		blocksArr[i][CHARSINBLOCK] = 0;
	}
	return blocksArr;
}

void printCharsBlock(char** charsBlocksArr, int blocksAmount)
{
	int i;
	for (i = 0; i < blocksAmount;i++)
	{
		printCharBlock(charsBlocksArr[i]);
		printf("\n");
		//fflush(stdout);
	}
}

void printCharBlock(char* charBlock)
{
	int i;
	for (i = 0; i < CHARSINBLOCK; i++)
	{
		printf("%c", charBlock[i]);
	}
	printf("\n");
}

//perform initial permutation for bits block
int* initialPermutation(int* bitsBlock)
{
	int i;
	int* bitsBlockAfterPermutation = (int*)malloc(BITSINBLOCK * sizeof(int));
	for (i = 0; i < BITSINBLOCK; i++)
	{
		bitsBlockAfterPermutation[i] = bitsBlock[InitialPermutationMatrix[i]-1];
	}
	return bitsBlockAfterPermutation;
}

//recive half of block (32 bit) and a key 
int* feistelFunction(int* rightInput,int* key)
{
	//Extension
	int* extendBits = expansion(rightInput);

	//Key mixing
	//Substitution
	//Permutation

}
//perform extention from 32 bits array to 48 bits array
int* expansion(int* rightInput)
{
	int i;
	int bitsInOutput = 48;
	int* extendBits = (int*)malloc(bitsInOutput * sizeof(int));
	for (i = 0; i < bitsInOutput; i++)
	{
		extendBits[i] = rightInput[ExtenderMatrix[i] - 1];
	}
	return extendBits;
}
