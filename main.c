#include <stdio.h>
#include <string.h>
#include "functions.h"
#define PLAINTEXTFILE "plaintextfile.txt"
#define CYPHERFILE "cypherfile.txt"
#define DECRIPTIONRES "decriptionres.txt"
//https://en.wikipedia.org/wiki/Data_Encryption_Standard
void encriptionFlow();
void decriptionFlow();

int main(int argc, char* argv[]) 
{
	//EncriptionFlow();
	decriptionFlow();

}

void encriptionFlow()
{
	int blocksAmount, i, j;
	int rounds = 16;
	int charCount = charsAmounInFile(PLAINTEXTFILE);
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


void decriptionFlow()
{
	int blocksAmount, i, j;
	int rounds = 16;
	int bitsCount = charsAmounInFile(CYPHERFILE);
	int* bitsArr = fileToBitsArray(CYPHERFILE, bitsCount);
	int** decriptionBlocksResult;
	int* decriptionResult;

	//create array of all the bits blocks from the file content
	int** bitsBlocksArr = bitsArrToBitsBlocks(bitsArr, bitsCount, &blocksAmount);

	//create decriptionBlocksResult arr
	decriptionBlocksResult = (int**)malloc(blocksAmount * sizeof(int*));
	
	//iterate each bits block and decript 
	printf("\nStart Decription:\n----------------------------------------------\n");
	for (i = 0; i < blocksAmount; i++)
	{
		printf("Block #%d:\n--------------------------------\n\n", i);
		printBitsArr(bitsBlocksArr[i], BITSINBLOCK);

		//perform initial permutation
		int* bitsBlockAfterInitialPer = initialPermutation(bitsBlocksArr[i]);
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

		//start decription rounds
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
		decriptionBlocksResult[i] = finalPermutation(joinedBlock);
		printf("Block decription output:\n");
		printBitsArr(decriptionBlocksResult[i], BITSINBLOCK);



		//free memory
		free(bitsBlockAfterInitialPer);
		free(leftHalfBlock);
		free(rightHalfBlock);
		free(joinedBlock);
		printf("\n");
	}

	//flattern the blocks to single arr of bits
	decriptionResult = (int*)malloc((blocksAmount * BITSINBLOCK) * sizeof(int));
	for (i = 0; i < (blocksAmount * BITSINBLOCK); i++)
	{
		decriptionResult[i] = decriptionBlocksResult[i / BITSINBLOCK][i % BITSINBLOCK];
	}

	//print decription result
	printf("decription result:\n");
	printBitsArr(decriptionResult, blocksAmount * BITSINBLOCK);

	//convert bits arr to char arr
	char* decriptionResultStr = bitsArrToCharArr(decriptionResult, bitsCount);
	printf("%s", decriptionResultStr);

	//save result to file
	saveDecriptionResult(decriptionResultStr, blocksAmount*CHARSINBLOCK, DECRIPTIONRES);

	//free memory
	//free(bitsArr);
	free(decriptionResult);
	for (i = 0; i < blocksAmount; i++)
	{
		free(decriptionBlocksResult[i]);
	}
	free(bitsBlocksArr);
	free(decriptionBlocksResult);
	free(decriptionResultStr);
	
	}
	