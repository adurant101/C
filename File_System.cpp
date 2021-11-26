/*
	This program creates files with names in a directory that can carry a string.
	Some operations for the file system include writing to the file, reading the file
	for a certain number of characters, closing a file, seeking through a file, and 
	deleting a file
*/

#include <iostream>
#include <string>
#include <sstream>
#include "Block.h"
#include "DirectoryEntry.h"

using namespace std;

//used to traverse files
struct FilePointer {
	char mode;
	char* fp;
	int blockNumber;
	int offset;
};

//blocks for use
Block b[100];
static int nextFreeBlock = 1;

//create file, open, write, read, close, seek, get offset, print
//file, split path, delete file
int create(char type, string name);
struct FilePointer* open(char mode, string name);
int write(struct FilePointer* fp, int n, string data);
void printFilePointer(struct FilePointer* FP);
void read(struct FilePointer* FP, int n);
void close(struct FilePointer** FP);
void seek(struct FilePointer* FP, int whence, int offset);
int getBlockNumberOfOffset(struct FilePointer* FP);
vector<string> splitPath(string path);
void deleteFile(string name);

int main()
{
	//initialize
	for (int i = 0; i < 100; i++)
	{
		b[i] = Block();
	}

	b[0].setInUse(true);
	b[0].initializeDirectory();
	string input;
	struct FilePointer* FP = nullptr;
	
	/*
		get input from program to create, open, close, delete, read, write, seek,
		or quit program
	*/
	do {
		cout << "Enter command >> ";
		fflush(stdout);
		getline(cin, input);
		stringstream ss(input);
		istream_iterator<string> begin(ss);
		istream_iterator<string> end;
		vector<std::string> vstrings(begin, end);
		if (vstrings[0].compare("CREATE") == 0) {
			create(vstrings[1][0], vstrings[2]);
		}
		else if (vstrings[0].compare("OPEN") == 0) {
			FP = open(vstrings[1][0], vstrings[2]);
			if (FP == nullptr) {
				cout << "Failed to open file " << vstrings[2] << endl;
			}
			else {
			}
		}
		else if (vstrings[0].compare("CLOSE") == 0) {
			close(&FP);
		}
		else if (vstrings[0].compare("DELETE") == 0) {
			deleteFile(vstrings[1]);
		}
		else if (vstrings[0].compare("READ") == 0) {
			if (FP != NULL) {
				read(FP, stoi(vstrings[1]));
			}
			else {
				cout << "ERROR: No file open!" << endl;
			}
		}
		else if (vstrings[0].compare("WRITE") == 0) {
			string input = "";
			for (int i = 2; i < vstrings.size(); i++) {
				input += vstrings[i] + " ";
			}
			int numWritten = write(FP, stoi(vstrings[1]), input);
		}
		else if (vstrings[0].compare("SEEK") == 0) {
			seek(FP, stoi(vstrings[1]), stoi(vstrings[2]));
		}
	} while (input.compare("quit") != 0);

	return 0;
}

//changes offset to string by spliting to desired destination
vector<string> splitPath(string path) {
	size_t old_offset = 0;
	size_t offset = 0;
	vector<string> output;
	while ((offset = path.find("/", old_offset)) != string::npos) {
		output.push_back(path.substr(old_offset, offset-old_offset));
		old_offset = offset+1;
	}
	output.push_back(path.substr(old_offset, offset-old_offset));
	return output;
}

/*
	create new file in directory, either file U or directory D
*/
int create(char type, string name)
{
	DirectoryEntry *d = nullptr;
	vector<string> path = splitPath(name);
	if (type == 'U' || type == 'D')
	{
		int blockNumber = 0;
		for (int i = 0; i < path.size(); i++) {
			if ((d = b[blockNumber].findFile(path[i])) != nullptr) {
				blockNumber = d->getStartSector();
			}
			else {
				DirectoryEntry newEntry;
				newEntry.setfileName(path[i].c_str());
				if (i != path.size() - 1) {
					newEntry.setfType('D');
				}
				else {
					newEntry.setfType(type);
				}
				newEntry.setAccess('O');
				newEntry.setStartSector(nextFreeBlock++);
				b[blockNumber].setDirectory(newEntry);
				blockNumber = newEntry.getStartSector();
			}
		}
	}
	else
	{
		return 1;
	}

	return 0;
}

//close file
void close(struct FilePointer** FP) {
	if (*FP == NULL) {
		cout << "No file open that we can close!" << endl;
	}
	else {
		*FP = NULL;
	}
}

/*
	open file in file system for use from filename, get all file info,
	put in filepointer
*/
struct FilePointer* open(char mode, string name)
{
	DirectoryEntry *d = nullptr;
	vector<string> path = splitPath(name);
	int blockNumber = 0;
	for (int i = 0; i < path.size(); i++) {
		d = b[blockNumber].findFile(path[i]);
		if (d == nullptr) {
			return nullptr;
		}
		blockNumber = d->getStartSector();
	}
	if (d == nullptr)
	{
		return nullptr;
	}
	else
	{
		struct FilePointer* FP = new struct FilePointer;
		d->setAccess(mode);
		size_t start, offset;
		offset = d->getSectorOffset();
		start = d->getStartSector();
		FP->mode = mode;
		FP->blockNumber = start;
		FP->offset = 0;
		switch (mode)
		{
		case 'I':
			FP->fp = b[start].getData(offset);
			return FP;
			break;
		case 'O':
			{
				size_t size = d->getFileSize();	
				if (size < Block::blockSize - offset) {
					FP->fp = b[start].getData(offset + size);
				}
				return FP;
				break;
			}
		case 'U':
			FP->fp = b[start].getData(offset);
			return FP;
			break;

		default:
			return nullptr;
		}
	}

}

/*
	write to file, file end of empty unused string with spaces, put string into
	block for file in data for given amount of characters
*/
int write(struct FilePointer *fp, int n, string data)
{
	int i = 0;
	if (fp == nullptr)
	{
		cout << "No file open!" << endl;
		return 0;
	}
	if (fp->mode == 'O')
	{
		cout << "File open in READ mode! Cannot write!" << endl;
		return 0;
	}
	if (n > data.length()) {
		for (int i = data.length(); i <= n; i++) {
			data += " ";
		}
	}
	else if (n < data.length()) {
		data = data.substr(0, n);
	}
	int chunk = 0;
	fp->fp = b[fp->blockNumber].getData(0);
	int curBlock = fp->blockNumber;
	int size = 0;
	do {
		int size = min(min(data.size() - chunk*Block::blockSize, (size_t)n), b[curBlock].getFreeData());
		strncpy_s(b[curBlock].getData(0), Block::blockSize+1, 
				  data.c_str()+(chunk*Block::blockSize), size);
		b[curBlock].setFreeData(Block::blockSize - size);
		if ((chunk+1) * Block::blockSize < data.size()) {
			int nextBlock = nextFreeBlock++;
			b[curBlock].setNextBlock(nextBlock);
			curBlock = nextBlock;
		}
		chunk++;
	} while (chunk * Block::blockSize < data.size());
	curBlock = fp->blockNumber;
	do {
		cout << b[curBlock].getData(0);
		curBlock = b[curBlock].getNextBlock();
	} while (curBlock != -1);
	cout << endl;
	b[fp->blockNumber].setFreeData(b[fp->blockNumber].getFreeData() - size);

	return i;
}

/*
	read file from filename and number of characters, print out string data
	in file
*/
void read(struct FilePointer* FP, int n) {
	if (FP == nullptr)
	{
		cout << "No file open!" << endl;
		return;
	}
	if (FP->mode == 'I')
	{
		cout << "File open in WRITE mode! Cannot read!" << endl;
		return;
	}
	int n_bytes_encountered = 0;
	int n_bytes_printed = 0;
	int curBlock = FP->blockNumber;
	int curOffset = FP->offset;
	do {
		if (FP->offset < n_bytes_encountered + Block::blockSize) {
			char* data = b[curBlock].getData(curOffset % Block::blockSize);
			cout << string(data).substr(0, min((int)strlen(data), n - n_bytes_printed));
			curOffset = 0;
			n_bytes_printed += min((int)strlen(data), n - n_bytes_printed);
		}
		if (n_bytes_printed >= n) {
			break;
		}
		n_bytes_encountered += Block::blockSize;
		curBlock = b[curBlock].getNextBlock();
	} while (curBlock != -1);
	cout << endl;
}

//get file pointer info
void printFilePointer(struct FilePointer* FP) {
	if (FP == nullptr || FP->mode == 'I')
	{
		return;
	}
	int nextBlock = FP->blockNumber;
	Block curBlock;
	do {
		curBlock = b[nextBlock];
		cout << "MODE: " << FP->mode << ", BLOCK NUMBER: " << nextBlock << ", CONTENT: " << curBlock.getData(0) << endl;
		nextBlock = curBlock.getNextBlock();
	} while (curBlock.getNextBlock() != -1);
}

//get block number from offset  
int getBlockNumberOfOffset(struct FilePointer* FP) {
	int curoffset = FP->offset;
	int blockNumber = FP->blockNumber;
	while (curoffset > Block::blockSize) {
		blockNumber = b[blockNumber].getNextBlock();
		curoffset -= Block::blockSize;
	}
	return blockNumber;
}

/*
	get size of file
*/
int getFileSize(struct FilePointer* FP) {
	int curBlock = FP->blockNumber;
	FilePointer tmpFP;
	tmpFP.blockNumber = FP->blockNumber;
	tmpFP.fp = FP->fp;
	tmpFP.mode = FP->mode;
	tmpFP.offset = 0;
	int size = 0;
	
	while (b[curBlock].getData(0)[tmpFP.offset] != '\0') {
		tmpFP.offset++;
		size++;
		curBlock = getBlockNumberOfOffset(&tmpFP);
	}
	return size;
}

/*
	seek file offset to desired destination, from either beginning
	or specified destination, -1 to go to beginning
*/
void seek(struct FilePointer* FP, int whence, int offset) {
	int fileSize = getFileSize(FP);
	if (FP == nullptr) {
		cout << "Could not seek. File not open!" << endl;
		return;
	}
	if (whence < -1 || whence > 1) {
		return;
	}
	if (whence == -1) {
		FP->offset = min(offset, fileSize);
	}
	else if (whence == 1) {
		FP->offset = max(fileSize - offset, 0);
	} else {
		FP->offset += offset;
	}
	if (FP->offset > fileSize) {
		FP->offset = fileSize;
	}
	else if (FP->offset < 0) {
		FP->offset = 0;
	}
	cout << "NEW FILE OFFSET: " << FP->offset << endl;
}

//erase block
void eraseBlock(int blockNumber) {
	if (blockNumber == -1) {
		return;
	}
	eraseBlock(b[blockNumber].getNextBlock());
	b[blockNumber] = Block();
	b[blockNumber].setSequence(blockNumber);
}

/*
	delete all file information from directory entry and block
	from the file system
*/
void deleteFile(string name) {
	DirectoryEntry* d = nullptr;
	vector<string> path = splitPath(name);
	int blockNumber = 0;
	int residingDirNumber = 0;
	int i;
	for (i = 0; i < path.size(); i++) {
		d = b[blockNumber].findFile(path[i]);
		if (d == nullptr) {
			cout << "DELETE: Could not find file to delete." << endl;
			return;
		}
		residingDirNumber = blockNumber;
		blockNumber = d->getStartSector();
	}
	if (d == nullptr) {
		cout << "DELETE: Could not find file to delete." << endl;
		return;
	}
	if (d->getfType() == 'F') {
		eraseBlock(d->getStartSector());
	}
	else {
		int curBlockNum = d->getStartSector();
		int nextBlock;
		do {
			nextBlock = b[curBlockNum].getNextBlock();
			eraseBlock(curBlockNum);
		} while (nextBlock != -1);
		eraseBlock(d->getStartSector());
		b[residingDirNumber].eraseDirEntry(d->getfileName());
	}
}
