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
	int charCount = findFileSize(FILE);
	char* charArr = fileToCharArray(FILE, charCount);
	int* bitsArr = charArrToBitsArr(charArr, charCount);
	int bitsCount = charCount * BITSINCHAR;
	printBitsArr(bitsArr, bitsCount);

}
