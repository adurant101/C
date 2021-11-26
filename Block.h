#pragma once
#include "DirectoryEntry.h"
class Block
{
	/*
		block number, data, directory for blocks, free data, sequence, 
		next block, in use.  setters and getters, find file, get directory 
		entry, getter and setter for directory entry and next block
	*/
public:
	static const size_t blockSize = 512;
private:
	static int blockNumber;
	char data[blockSize+1];
	DirectoryEntry directory[31];
	size_t freeData;
	int sequence;
	int nextBlock;
	bool inUse;
public:
	Block();
	void setInUse(bool);
	bool getinUse();
	void setFreeData(size_t);
	size_t getFreeData();
	void setSequence(int);
	int getSequence();
	DirectoryEntry* findFile(string name);
	void initializeDirectory();
	void setDirectory(DirectoryEntry);
	bool eraseDirEntry(string name);
	DirectoryEntry* getDirectoryEntry();
	void setData(char val, size_t offset);
	char* getData(size_t offset);
	void debugPrint();
	void setNextBlock(int nb);
	int getNextBlock();




};

