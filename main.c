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
	char fileContent[255];
	char** blocksArray;
	int blocksCount;

	//read the ascii string from the file to fileContent
	readAsciiFile(FILE, fileContent);

	//creates blocks array
	//each block is a string(char*) of 8 characters (64 bits) or less (if the file string not divided by 8)
	blocksArray = stringToBlocksArray(fileContent, &blocksCount);
	
	//print the first block
	printBlockBits(blocksArray[0]);

}


