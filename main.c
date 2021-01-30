#include <stdio.h>
#include <string.h>
#include "functions.h"
#define PLAINTEXTFILE "plaintextfile.txt"
#define CYPHERFILE "cypherfile.txt"
//https://en.wikipedia.org/wiki/Data_Encryption_Standard
void testMainRoy();

int main(int argc, char* argv[]) 
{
	testMainRoy();
	return 1;
}

void testMainRoy()
{
	int blocksAmount, i, j;
	int rounds = 16;
	int charCount = findFileSize(PLAINTEXTFILE);
	char* charArr = fileToCharArray(PLAINTEXTFILE, charCount);
	int** encriptionBlocksResult;
	int* encriptionResult;

	//create array of all the chars blocks from the file content
	char** charsBlocksArr = CharArrToCharBlocks(charArr, charCount, &blocksAmount);

	//create encriptionBlocksResult arr
	encriptionBlocksResult = (int**)malloc(blocksAmount * sizeof(int*));

	//iterate each chars block, convert it to bits block and encript 
	printf("\nStart Encription:\n----------------------------------------------\n");
	for (i = 0; i < blocksAmount; i++)
	{
		printf("Block #%d:\n--------------------------------\n\n",i);
		printCharBlock(charsBlocksArr[i]);
		
		//convert current char block to bits block
		int* bitsBlock = charArrToBitsArr(charsBlocksArr[i], CHARSINBLOCK);
		printf("bits block:\n");
		printBitsArr(bitsBlock, BITSINBLOCK);

		//perform initial permutation
		int* bitsBlockAfterInitialPer = initialPermutation(bitsBlock);
		printf("InitialPer bits:\n");
		printBitsArr(bitsBlockAfterInitialPer, BITSINBLOCK);
		
		/////////////////////////
		int subkeytest[48] =
		{
			1, 0, 1, 0, 1, 0, 1, 0,
			1, 1, 0, 1, 0, 1, 1,1,
			1, 0, 0, 1, 0, 0, 1, 1,
			0, 1, 1, 1, 1, 0, 1, 0,
			1, 0, 1, 0, 1, 0, 1, 1,
			1, 0, 0, 1, 0, 0, 0, 1

		};
		/////////////////////////

		//right half block
		HalfBlockSide right = RIGHT;
		int* rightHalfBlock = getHalfBlock(bitsBlockAfterInitialPer, right);
		printf("RIGHT side:\n");
		printBitsArr(rightHalfBlock, BITSINBLOCK / 2);

		//left half block
		HalfBlockSide left = LEFT;
		int* leftHalfBlock = getHalfBlock(bitsBlockAfterInitialPer, left);
		printf("LEFT side:\n");
		printBitsArr(leftHalfBlock, BITSINBLOCK / 2);

		//start encription rounds
		for (j = 0; j < rounds; j++)
		{
			printf("\nRound #%d:\n---------------\n", j);
			int* leftHalfBlockOutput;
			int* rightHalfBlockOutput;
			prformRound(leftHalfBlock, rightHalfBlock, &leftHalfBlockOutput, &rightHalfBlockOutput, subkeytest);
			leftHalfBlock = leftHalfBlockOutput;
			rightHalfBlock = rightHalfBlockOutput;

			printf("Round #%d LEFT output:\n", j);
			printBitsArr(leftHalfBlockOutput, BITSINBLOCK / 2);
			printf("Round #%d RIGHT output:\n", j);
			printBitsArr(rightHalfBlockOutput, BITSINBLOCK / 2);
		}
		
		//join the 2 half blocks (left and right)
		int* joinedBlock = joinHalfblocks(rightHalfBlock, leftHalfBlock);

		//perform the final permutation
		encriptionBlocksResult[i] = finalPermutation(joinedBlock);
		printf("Block encription output:\n");
		printBitsArr(encriptionBlocksResult[i], BITSINBLOCK);



		//free memory
		free(bitsBlock);
		free(bitsBlockAfterInitialPer);
		free(leftHalfBlock);
		free(rightHalfBlock);
		free(joinedBlock);
		printf("\n");
	}

	//flattern the blocks to single arr of bits
	encriptionResult = (int*)malloc((blocksAmount*BITSINBLOCK) * sizeof(int));
	for (i = 0; i < (blocksAmount * BITSINBLOCK);i++)
	{
		encriptionResult[i] = encriptionBlocksResult[i / BITSINBLOCK][i % BITSINBLOCK];
	}

	//print cypher result
	printf("cypher result:\n");
	printBitsArr(encriptionResult, blocksAmount * BITSINBLOCK);

	//save result to file
	saveEncriptionResult(encriptionResult, blocksAmount * BITSINBLOCK, CYPHERFILE);

	//free memory
	free(charArr);
	free(encriptionResult);
	for (i = 0; i < blocksAmount; i++)
	{
		free(charsBlocksArr[i]);
		free(encriptionBlocksResult[i]);
	}
	free(charsBlocksArr);
	free(encriptionBlocksResult);
}
