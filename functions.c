#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "functions.h"


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

//return char array (not NULL ("/0") terminates string!!) of the file content
char* fileToCharArray(char* filePath, int charArrSize)
{
	int i = 0;
	char ch;

	char* charArr = (char*)malloc(charArrSize * sizeof(char));
	FILE* fp = fopen(filePath, "r");

	while (!feof(fp))
	{
		ch = getc(fp);
		charArr[i] = ch;
		i++;
	}

	fclose(fp);

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
}



