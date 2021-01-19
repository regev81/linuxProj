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

	readAsciiFile(FILE, fileContent);
	unsigned long* blocksArr = asciiToBlocks(fileContent);

}


