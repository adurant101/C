#pragma once
#include <iostream>
#include <vector>

using namespace std;
using byte = unsigned char;
class DirectoryEntry
{
private:
	//false is a normal file, true directory
	char fType;
	string fileName, ext;
	size_t startSector, sectorOffset, fileSize;
	byte links[80];
	char access;

public:
	//getters and setters for variables
	DirectoryEntry();
	void setfType(char);
	char getfType();
	void setfileName(string);
	string getfileName();
	void setExt(string);
	string getExt();
	void setStartSector(size_t);
	size_t getStartSector();
	void setSectorOffset(size_t);
	size_t getSectorOffset();
	void setFileSize(size_t);
	size_t getFileSize();
	void setNextLink(byte);
	byte* getLinks();
	void setAccess(char);
	char getAccess();
	string asString();

};



