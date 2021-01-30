#include <stdio.h>
#include <string.h>
#include "functions.h"
#define FILE "file.txt"
//https://en.wikipedia.org/wiki/Data_Encryption_Standard
void testMainRoy();

int main(int argc, char* argv[]) 
{
	testMainRoy();
	
	return 1;
}

void testMainRoy()
{
	int blocksAmount,i;
	int charCount = findFileSize(FILE);
	char* charArr = fileToCharArray(FILE, charCount);

	//create array of all the chars blocks from the file content
	char** charsBlocksArr = CharArrToCharBlocks(charArr, charCount, &blocksAmount);
	//printf("All blocks[CHARS]:\n");
	//printCharsBlock(charsBlocksArr, blocksAmount);

	//iterate each chars block, convert it to bits block and encript 
	printf("\nStart Encription:\n----------------------------------------------\n");
	for (i = 0; i < blocksAmount; i++)
	{
		printf("Block #%d:\n",i);
		printCharBlock(charsBlocksArr[i]);
		
		//convert current char block to bits block
		int* bitsBlock = charArrToBitsArr(charsBlocksArr[i], CHARSINBLOCK);
		printf("bits block:\n", i);
		printBitsArr(bitsBlock, BITSINBLOCK);

		//perform initial permutation
		int* bitsBlockAfterInitialPer = initialPermutation(bitsBlock, CHARSINBLOCK);
		printf("InitialPer bits:\n", i);
		printBitsArr(bitsBlockAfterInitialPer, BITSINBLOCK);

		//perform block encription
		////////////////////////////////////////////////////////////

		//left half block
		HalfBlockSide left = LEFT;
		int* leftHalfBlock = divideBlock(bitsBlockAfterInitialPer, left);
		printf("LEFT side:\n");
		printBitsArr(leftHalfBlock, BITSINBLOCK / 2);

		//right half block
		//HalfBlockSide right = RIGHT;
		//int* rightHalfBlock = divideBlock(bitsBlockAfterInitialPer, right);
		//printf("RIGHT side:\n");
		//printBitsArr(rightHalfBlock, BITSINBLOCK / 2);

		//
		int subkeytest[48] =
		{
			1, 0, 1, 0, 1, 0, 1, 0,
			1, 1, 0, 1, 0, 1, 1,1,
			1, 0, 0, 1, 0, 0, 1, 1,
			0, 1, 1, 1, 1, 0, 1, 0,
			1, 0, 1, 0, 1, 0, 1, 1,
			1, 0, 0, 1, 0, 0, 0, 1

		};
		feistelFunction(leftHalfBlock, subkeytest);


		//free allocated memory
		free(bitsBlock);
		free(bitsBlockAfterInitialPer);
		free(leftHalfBlock);
		printf("\n");
	}

	//free allocated memory
	free(charArr);
	for (i = 0; i < blocksAmount; i++)
	{
		free(charsBlocksArr[i]);
	}
	free(charsBlocksArr);

}
