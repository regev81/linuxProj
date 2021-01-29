#include <stdio.h>
#include <string.h>
#include "functions.h"
#define FILE "file.txt"

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
		printBitsArr(bitsBlock, BITSINBLOCK);

		//perform initial permutation
		int* bitsBlockAfterInitialPer = blockPermutation(bitsBlock, CHARSINBLOCK);
		printBitsArr(bitsBlockAfterInitialPer, BITSINBLOCK);

		//free allocated memory
		free(bitsBlock);
		free(bitsBlockAfterInitialPer);
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
