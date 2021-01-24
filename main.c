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
	char** charsBlocksArr = CharArrToCharBlocks(charArr, charCount, &blocksAmount);
	printf("All blocks[CHARS]:\n");
	printCharsBlock(charsBlocksArr, blocksAmount);

	printf("\nAll blocks[BITS]:\n");
	for (i = 0; i < blocksAmount; i++)
	{
		int* bitsArr = charArrToBitsArr(charsBlocksArr[i], CHARSINBLOCK);
		printBitsArr(bitsArr, BITSINBLOCK);
		free(bitsArr);
		printf("\n");
	}

	//free memory
	free(charArr);
	for (i = 0; i < blocksAmount; i++)
	{
		free(charsBlocksArr[i]);
	}
	free(charsBlocksArr);

}
