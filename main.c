#include <stdio.h>
#include <string.h>
#include "functions.h"
#define PLAINTEXTFILE "plaintextfile.txt"
#define CYPHERFILE "cypherfile.txt"
#define DECRYPTIONRES "decryptionres.txt"

void threeDesEncrypt();
void threeDesDecrypt();

int main(int argc, char* argv[]) 
{
	//threeDesEncrypt();
	threeDesDecrypt();

}


void threeDesDecrypt()
{
	int blocksAmount, i;
	int bitsCount = charsAmounInFile(CYPHERFILE);
	int* bitsArr = fileToBitsArray(CYPHERFILE, bitsCount);
	int** decryptionBlocksResult;
	int* allBlocksResult;

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

		//perform three des encription
		int key[48] =
		{
			1, 0, 1, 0, 1, 0, 1, 0,
			1, 1, 0, 1, 0, 1, 1,1,
			1, 0, 0, 1, 0, 0, 1, 1,
			0, 1, 1, 1, 1, 0, 1, 0,
			1, 0, 1, 0, 1, 0, 1, 1,
			1, 0, 0, 1, 0, 0, 0, 1

		};

		//first des
		int* desDecryptionResult_a = desDecrypt(bitsBlocksArr[i], key);

		//second des
		int* desDecryptionResult_b = desDecrypt(desDecryptionResult_a, key);

		//third des
		decryptionBlocksResult[i] = desDecrypt(desDecryptionResult_b, key);

		free(desDecryptionResult_a);
		free(desDecryptionResult_b);
	}

	//flattern the blocks to single arr of bits
	allBlocksResult = (int*)malloc((blocksAmount * BITSINBLOCK) * sizeof(int));
	for (i = 0; i < (blocksAmount * BITSINBLOCK); i++)
	{
		allBlocksResult[i] = decryptionBlocksResult[i / BITSINBLOCK][i % BITSINBLOCK];
	}

	//print decryption result
	printf("decryption result:\n");
	printBitsArr(allBlocksResult, blocksAmount * BITSINBLOCK);

	//convert bits arr to char arr
	char* decryptionResultStr = bitsArrToCharArr(allBlocksResult, bitsCount);
	printf("%s", decryptionResultStr);

	//save result to file
	saveDecryptionResult(decryptionResultStr, blocksAmount*CHARSINBLOCK, DECRYPTIONRES);

	//free memory
	free(allBlocksResult);
	for (i = 0; i < blocksAmount; i++)
	{
		free(decryptionBlocksResult[i]);
	}
	free(bitsBlocksArr);
	free(decryptionBlocksResult);
	free(decryptionResultStr);
	
	}


void threeDesEncrypt()
{
	int blocksAmount, i;
	int charCount = charsAmounInFile(PLAINTEXTFILE);
	char* charArr = fileToCharArray(PLAINTEXTFILE, charCount);
	int** encryptionBlocksResult;
	int* allBlocksResult;

	//create array of all the chars blocks from the file content
	char** charsBlocksArr = CharArrToCharBlocks(charArr, charCount, &blocksAmount);

	//create encryptionBlocksResult arr
	encryptionBlocksResult = (int**)malloc(blocksAmount * sizeof(int*));

	//iterate each chars block, convert it to bits block and encript 
	printf("\nStart encryption:\n----------------------------------------------\n");
	for (i = 0; i < blocksAmount; i++)
	{
		printf("Block #%d:\n--------------------------------\n\n", i);
		printCharBlock(charsBlocksArr[i]);

		//convert current char block to bits block
		int* bitsBlock = charArrToBitsArr(charsBlocksArr[i], CHARSINBLOCK);
		printf("bits block:\n");
		printBitsArr(bitsBlock, BITSINBLOCK);

		//perform three des encription
		int key[48] =
		{
			1, 0, 1, 0, 1, 0, 1, 0,
			1, 1, 0, 1, 0, 1, 1,1,
			1, 0, 0, 1, 0, 0, 1, 1,
			0, 1, 1, 1, 1, 0, 1, 0,
			1, 0, 1, 0, 1, 0, 1, 1,
			1, 0, 0, 1, 0, 0, 0, 1

		};
		
		//first des
		int* desEncryptionResult_a = desEncrypt(bitsBlock, key);

		//second des
		int* desEncryptionResult_b = desEncrypt(desEncryptionResult_a, key);

		//third des
		encryptionBlocksResult[i] = desEncrypt(desEncryptionResult_b, key);

		free(desEncryptionResult_a);
		free(desEncryptionResult_b);
		
	}

	//flattern the blocks to single arr of bits
	allBlocksResult = (int*)malloc((blocksAmount * BITSINBLOCK) * sizeof(int));
	for (i = 0; i < (blocksAmount * BITSINBLOCK); i++)
	{
		allBlocksResult[i] = encryptionBlocksResult[i / BITSINBLOCK][i % BITSINBLOCK];
	}

	//print encryption result
	printf("encryption result:\n");
	printBitsArr(allBlocksResult, blocksAmount * BITSINBLOCK);

	//save result to file
	saveEncryptionResult(allBlocksResult, blocksAmount * BITSINBLOCK, CYPHERFILE);

	//free memory
	free(charArr);
	free(allBlocksResult);
	for (i = 0; i < blocksAmount; i++)
	{
		free(charsBlocksArr[i]);
		free(encryptionBlocksResult[i]);
	}
	free(charsBlocksArr);
	free(encryptionBlocksResult);

}