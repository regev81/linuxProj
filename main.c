#include <stdio.h>
#include <string.h>
#include "functions.h"
#define PLAINTEXTFILE "plaintextfile.txt"
#define CYPHERFILE "cypherfile.txt"
#define DECRYPTIONRES "decryptionres.txt"

void encryptionFlow();
void decryptionFlow();

int main(int argc, char* argv[]) 
{
	//encryptionFlow();
	decryptionFlow();

}

void encryptionFlow()
{
	int blocksAmount, i, j;
	int rounds = 16;
	int charCount = charsAmounInFile(PLAINTEXTFILE);
	char* charArr = fileToCharArray(PLAINTEXTFILE, charCount);
	int** encryptionBlocksResult;
	int* encryptionResult;

	//create array of all the chars blocks from the file content
	char** charsBlocksArr = CharArrToCharBlocks(charArr, charCount, &blocksAmount);

	//create encryptionBlocksResult arr
	encryptionBlocksResult = (int**)malloc(blocksAmount * sizeof(int*));

	//iterate each chars block, convert it to bits block and encript 
	printf("\nStart encryption:\n----------------------------------------------\n");
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

		//start encryption rounds
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
		encryptionBlocksResult[i] = finalPermutation(joinedBlock);
		printf("Block encryption output:\n");
		printBitsArr(encryptionBlocksResult[i], BITSINBLOCK);



		//free memory
		free(bitsBlock);
		free(bitsBlockAfterInitialPer);
		free(leftHalfBlock);
		free(rightHalfBlock);
		free(joinedBlock);
		printf("\n");
	}

	//flattern the blocks to single arr of bits
	encryptionResult = (int*)malloc((blocksAmount*BITSINBLOCK) * sizeof(int));
	for (i = 0; i < (blocksAmount * BITSINBLOCK);i++)
	{
		encryptionResult[i] = encryptionBlocksResult[i / BITSINBLOCK][i % BITSINBLOCK];
	}

	//print cypher result
	printf("cypher result:\n");
	printBitsArr(encryptionResult, blocksAmount * BITSINBLOCK);

	//save result to file
	saveEncryptionResult(encryptionResult, blocksAmount * BITSINBLOCK, CYPHERFILE);

	//free memory
	free(charArr);
	free(encryptionResult);
	for (i = 0; i < blocksAmount; i++)
	{
		free(charsBlocksArr[i]);
		free(encryptionBlocksResult[i]);
	}
	free(charsBlocksArr);
	free(encryptionBlocksResult);
}


void decryptionFlow()
{
	int blocksAmount, i, j;
	int rounds = 16;
	int bitsCount = charsAmounInFile(CYPHERFILE);
	int* bitsArr = fileToBitsArray(CYPHERFILE, bitsCount);
	int** decryptionBlocksResult;
	int* decryptionResult;

	//create array of all the bits blocks from the file content
	int** bitsBlocksArr = bitsArrToBitsBlocks(bitsArr, bitsCount, &blocksAmount);

	//create decryptionBlocksResult arr
	decryptionBlocksResult = (int**)malloc(blocksAmount * sizeof(int*));
	
	//iterate each bits block and decript 
	printf("\nStart decryption:\n----------------------------------------------\n");
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

		//start decryption rounds
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
		decryptionBlocksResult[i] = finalPermutation(joinedBlock);
		printf("Block decryption output:\n");
		printBitsArr(decryptionBlocksResult[i], BITSINBLOCK);



		//free memory
		free(bitsBlockAfterInitialPer);
		free(leftHalfBlock);
		free(rightHalfBlock);
		free(joinedBlock);
		printf("\n");
	}

	//flattern the blocks to single arr of bits
	decryptionResult = (int*)malloc((blocksAmount * BITSINBLOCK) * sizeof(int));
	for (i = 0; i < (blocksAmount * BITSINBLOCK); i++)
	{
		decryptionResult[i] = decryptionBlocksResult[i / BITSINBLOCK][i % BITSINBLOCK];
	}

	//print decryption result
	printf("decryption result:\n");
	printBitsArr(decryptionResult, blocksAmount * BITSINBLOCK);

	//convert bits arr to char arr
	char* decryptionResultStr = bitsArrToCharArr(decryptionResult, bitsCount);
	printf("%s", decryptionResultStr);

	//save result to file
	saveDecryptionResult(decryptionResultStr, blocksAmount*CHARSINBLOCK, DECRYPTIONRES);

	//free memory
	//free(bitsArr);
	free(decryptionResult);
	for (i = 0; i < blocksAmount; i++)
	{
		free(decryptionBlocksResult[i]);
	}
	free(bitsBlocksArr);
	free(decryptionBlocksResult);
	free(decryptionResultStr);
	
	}
	