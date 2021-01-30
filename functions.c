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

//create half a block from the left or right side of the block
int* getHalfBlock(int* block, HalfBlockSide side)
{
	int startIndex, endIndex, i, j;
	int* halfBlock = (int*)malloc((BITSINBLOCK/2) * sizeof(int));
	
	if (side == LEFT)
	{
		startIndex = 0;
		endIndex = (BITSINBLOCK / 2);
	}
	else 
	{
		startIndex = (BITSINBLOCK / 2);
		endIndex = (BITSINBLOCK);
	}

	j = 0;
	for (i = startIndex; i < endIndex; i++)
	{
		halfBlock[j] = block[i];
		j++;
	}

	return halfBlock;
}

//recive half of block (32 bit) and a subkey 
int* feistelFunction(int* halfBlock,int* subkey)
{
	printf("subkey:\n");
	printBitsArr(subkey, BITSINSUBKEY);

	//Extension
	int* extendBits = expansion(halfBlock);
	printf("step 1) extend bits:\n");
	printBitsArr(extendBits, BITSINSUBKEY);

	//Key mixing
	int* keyMixingBits = keyMixing(extendBits, subkey);
	printf("step 2) key mixing (xor) bits:\n");
	printBitsArr(keyMixingBits, BITSINSUBKEY);

	//Substitution
	int* substitutionBits = substitution(keyMixingBits, BITSINSUBKEY);
	printf("step 3) substitution bits:\n");
	printBitsArr(substitutionBits, BITSINBLOCK/2);

	//Permutation
	int* permutationBits = halfBlockPermutation(substitutionBits);
	printf("step 4) halfBlockPer bits:\n");
	printBitsArr(permutationBits, BITSINBLOCK/2);

	free(extendBits);
	//free(subkey);
	return permutationBits;
}
//perform extention from 32 bits array to 48 bits array
int* expansion(int* halfBlock)
{
	int i;
	int* extendBits = (int*)malloc(BITSINSUBKEY * sizeof(int));
	for (i = 0; i < BITSINSUBKEY; i++)
	{
		extendBits[i] = halfBlock[ExtenderMatrix[i] - 1];
	}
	return extendBits;
}

//perform xor between the half block and the subkey
int* keyMixing(int* halfBlock, int* subkey)
{
	int i;
	int* result = (int*)malloc(BITSINSUBKEY * sizeof(int));
	for (i = 0; i < BITSINSUBKEY; i++)
	{
		result[i] = bitXorOperation(halfBlock[i], subkey[i]);
	}

	return result;
}

//perform xor operation between 2 bits represented as int
int bitXorOperation(int bit, int bit2)
{
	if (bit == bit2)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

//perform permutation for half block
int* halfBlockPermutation(int* halfBlock)
{
	int i;
	int* halfBlockBlockAfterPermutation = (int*)malloc((BITSINBLOCK/2) * sizeof(int));
	for (i = 0; i < (BITSINBLOCK / 2); i++)
	{
		halfBlockBlockAfterPermutation[i] = halfBlock[HalfBlockPermutationMatrix[i] - 1];
	}
	return halfBlockBlockAfterPermutation;
}

//perform substitution BY THE S-boxes
int* substitution(int* bitsArr,int size)
{
	int i;
	//crerate arr of 8 arrays, each array is 6 bits array
	int grousCount = 8;
	int** arrOf6bitsArrays = divideBitsArr(bitsArr, size, grousCount);

	//perform the s_box substitution
	int** arrOf4bitsArrays = performSboxSubstitution(arrOf6bitsArrays, grousCount);

	//join the array of arrays to one array of bits
	int* bitsAfterSubstitution = (int*)malloc((BITSINBLOCK / 2) * sizeof(int));
	for (i = 0; i < (BITSINBLOCK / 2); i++)
	{
		bitsAfterSubstitution[i] = arrOf6bitsArrays[i / 4][i % 4];
	}
	
	// free memory
	for (i = 0; i < grousCount; i++)
	{
		free(arrOf6bitsArrays[i]);
		free(arrOf4bitsArrays[i]);
	}
	free(arrOf6bitsArrays);
	free(arrOf4bitsArrays);

	return bitsAfterSubstitution;
} 

int getRowInBox(int bit1, int bit6)
{
	return (2 * bit1) + bit6;
}

int getColInBox(int bit2, int bit3, int bit4, int bit5)
{
	return (bit5 * pow(2, 0))
		+ (bit4 * pow(2, 2))
		+ (bit3 * pow(2, 4))
		+ (bit2 * pow(2, 8));
}

//return int which represent 4 bits, from the s_box
int getBoxResult(int* sixBitsInput, int boxIndex)
{
	int row = getRowInBox(sixBitsInput[0], sixBitsInput[5]);
	int col = getColInBox(sixBitsInput[1], sixBitsInput[2], sixBitsInput[3], sixBitsInput[4]);

	switch (boxIndex)
	{
	case 1:
		return Sbox5[row, col];
	case 2:
		return Sbox2[row, col];
	case 3:
		return Sbox3[row, col];
	case 4:
		return Sbox4[row, col];
	case 5:
		return Sbox5[row, col];
	case 6:
		return Sbox6[row, col];
	case 7:
		return Sbox7[row, col];
	case 8:
		return Sbox8[row, col];
	default:
		printf("wrong index not in range");
	}
	return 0;
}

// return bits array of the int
int* intTo4bits(int num)
{
	int k, m, j;
	int* bitsArr = (int*)malloc(4 * sizeof(int));
	j = 0;
	for (int i = 4 - 1; i >= 0; i--)
	{
		m = 1 << i;
		k = num & m;
		k = k >> i;
		if (k == 0)
			bitsArr[j] = 0;
		else
			bitsArr[j] = 1;
		j++;
	}

	return bitsArr;

}

//devide the bits arr to multiple bits arr
int** divideBitsArr(int* bitsArr, int bitsInArr, int grousCount)
{
	int i, j;
	int bitsInGroup = bitsInArr / grousCount;
	int** multipleBitsArr = (int**)malloc(grousCount * sizeof(int*));
	for (i = 0; i < grousCount; i++)
	{
		multipleBitsArr[i] = (int*)malloc(bitsInGroup * sizeof(int));
		for (j = 0; j < bitsInGroup;j++)
		{
			multipleBitsArr[i][j] = bitsArr[j + (i * bitsInGroup)];
		}
	}

	return multipleBitsArr;
}

int** performSboxSubstitution(int** arrOf6bitsArrays,int arrSize)
{
	int i;
	int** arrOf4bitsArrays = (int**)malloc(arrSize * sizeof(int*));
	for (i = 0; i < arrSize; i++)
	{
		int boxResult = getBoxResult(arrOf6bitsArrays[i],i+1);
		arrOf4bitsArrays[i] = intTo4bits(boxResult);
	}

	return arrOf4bitsArrays;
}

int* halfBlockXorOperation(int* halfBlock1, int* halfBlock2)
{
	int i;
	int* xorBitsArr = (int*)malloc((BITSINBLOCK / 2) * sizeof(int));
	for (i = 0; i < BITSINBLOCK / 2; i++)
	{
		xorBitsArr[i] = bitXorOperation(halfBlock1[i], halfBlock2[i]);
	}

	return xorBitsArr;
}

void prformRound(int* leftInput, int* rightInput, int** leftOutput_p, int** rightOutput_p,int* subkey)
{
		*rightOutput_p = halfBlockXorOperation(feistelFunction(rightInput, subkey), leftInput);
		*leftOutput_p = rightInput;
}

int* joinHalfblocks(int* firstHalfBlock, int* lastHalfBlock)
{
	int i;
	int* joinedBlock = (int*)malloc(BITSINBLOCK * sizeof(int));
	for (i = 0; i < BITSINBLOCK/2; i++)
	{
		joinedBlock[i] = firstHalfBlock[i];
	}
	for (i ; i < BITSINBLOCK; i++)
	{
		joinedBlock[i] = lastHalfBlock[i- (BITSINBLOCK/2)];
	}

	return joinedBlock;
}

//perform final permutation for bits block
int* finalPermutation(int* bitsBlock)
{
	int i;
	int* bitsBlockAfterPermutation = (int*)malloc(BITSINBLOCK * sizeof(int));
	for (i = 0; i < BITSINBLOCK; i++)
	{
		bitsBlockAfterPermutation[i] = bitsBlock[FinalPermutationMatrix[i] - 1];
	}
	return bitsBlockAfterPermutation;
}

void saveEncriptionResult(int* encriptionResult, int size, char* filePath)
{
	int i;
	FILE* f = fopen(filePath, "wb");
	for (i = 0; i < size; i++)
	{
		fprintf(f, "%d", encriptionResult[i]);
	}
	
	fclose(f);
	
}