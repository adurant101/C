#include "Block.h"
#include <string.h>

int Block::blockNumber = 0;

//initialize
Block::Block()
{
	sequence = blockNumber++;
	inUse = false;
	freeData = blockSize;
	memset(data, 0, blockSize);
	nextBlock = -1;
}

//setters and getters
void Block::setNextBlock(int nb) {
	nextBlock = nb;
}

int Block::getNextBlock() {
	return nextBlock;
}

void Block::setInUse(bool iu)
{
	inUse = iu;
}
bool Block::getinUse()
{
	return inUse;
}
void Block::setFreeData(size_t fd)
{
	freeData = fd;
}
size_t Block::getFreeData()
{
	return freeData;
}
void Block::setSequence(int s)
{
	sequence = s;
}
int Block::getSequence()
{
	return sequence;
}

void Block::initializeDirectory()
{
	for (int i = 0; i < 31; i++)
	{
		directory[i] = DirectoryEntry();
	}
}


void Block::setDirectory(DirectoryEntry de)
{
	int i = 0;
	while (directory[i].getfType() != 'F')
	{
		i++;
	}

	directory[i] = de;
}

DirectoryEntry* Block::getDirectoryEntry()
{
	return directory;
}

//erase directory
bool Block::eraseDirEntry(string name) {
	if (!findFile(name)) {
		return false;
	}
	int i;
	for (i = 0; directory[i].getfileName() != name; i++) { /* LEFT EMPTY */ }
	for (/* LEFT EMPTY */; directory[i].getfType() != 'F'; i++) {
		directory[i] = directory[i + 1];
	}
	return true;
}

//find file
DirectoryEntry* Block::findFile(string name)
{ 
	int i = 0;
	while (i < 31 && directory[i].getfType() != 'F')
	{
		if (directory[i].getfileName().compare(name) == 0) {
			return &directory[i];
		}
		i++;
	}
	return nullptr;

}

void Block::setData(char val, size_t offset)
{
	data[offset] = val;
}



char* Block::getData(size_t offset)
{
	return &(data[offset]);
}

void Block::debugPrint() {
	int i = 0;
	cout << "BEGIN DEBUG PRINT" << endl;
	while (directory[i].getfType() != 'F') {
		cout << directory[i].asString() << endl;
		i++;
	}
	cout << "END DEBUG PRINT" << endl;
}
