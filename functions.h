#pragma once
#define BITSINCHAR 8
#define CHARSINBLOCK 8
#define BITSINBLOCK 64
#define BITSINSUBKEY 48
#define HALF_KEY_SIZE 28
#define KEY_SIZE 56
#define ROUNDS 16

typedef enum {
	LEFT, RIGHT
} HalfBlockSide;

int charsAmounInFile(char* filePath);
char* fileToCharArray(char* filePath, int charArrSize);
int* charArrToBitsArr(char* charArr, int charArrSize);
char* bitsArrToCharArr(int* bitsArr, int bitsArrSize);
int* charToBitsArr(char c);
char bitsArrToChar(int* bits);
void printBitsArr(int* bitsArr, int size);
char** CharArrToCharBlocks(char* charArr, int charCount, int* blockCount_p);
void printCharsBlock(char** charsBlocksArr, int blocksAmount);
void printCharBlock(char* charBlock);
int* initialPermutation(int* bitsBlock);
int* expansion(int* halfBlock);
int* feistelFunction(int* halfBlock, int* subkey);
int bitXorOperation(int bit, int bit2);
int* keyMixing(int* halfBlock, int* subkey);
int* getHalfBlock(int* block, HalfBlockSide side);
int* halfBlockPermutation(int* halfBlock);
int getRowInBox(int bit1, int bit6);
int getColInBox(int bit2, int bit3, int bit4, int bit5);
int getBoxResult(int* sixBitsInput, int boxIndex);
int* intTo4bits(int num);
int* substitution(int* bitsArr, int size);
int** divideBitsArr(int* bitsArr, int bitsInArr, int grousCount);
int** performSboxSubstitution(int** arrOf6bitsArrays, int arrSize);
void prformRound(int* leftInput, int* rightInput, int** leftOutput_p, int** rightOutput_p, int* subkey);
int* halfBlockXorOperation(int* halfBlock1, int* halfBlock2);
int* joinHalfblocks(int* firstHalfBlock, int* lastHalfBlock);
int* finalPermutation(int* bitsBlock);
void saveEncryptionResult(int* decryptionResult, int size, char* filePath);
void saveDecryptionResult(char* decryptionResult, int size, char* filePath);
void printCharsArr(char* charsArr, int size);
int* fileToBitsArray(char* filePath, int bitsArrSize);
int** bitsArrToBitsBlocks(int* bitsArr, int bitsCount, int* blockCount_p);
int* desEncrypt(int* bitsBlock, int* key);
int* keySecondPermutation(int* keyBlock);
int* keyfirstPermutation(int* keyBlock);
int** generateAllKeys(int* key);
int* leftShift(int* Key, int numOfShift);
int* desDecrypt(int* bitsBlock, int* key);