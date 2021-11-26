#include "DirectoryEntry.h"
DirectoryEntry::DirectoryEntry()
{
	fType = 'F';
	startSector = 0;
	sectorOffset = 0;
	fileSize = 0;
}

//getters and setters
void DirectoryEntry::setfType(char b)
{
	fType = b;
}
char DirectoryEntry::getfType()
{
	return fType;
}
void DirectoryEntry::setfileName(string fn)
{
	fileName = fn;
}
string DirectoryEntry::getfileName()
{
	return fileName;
}
void DirectoryEntry::setExt(string e)
{
	ext = e;
}
string DirectoryEntry::getExt()
{
	return ext;
}
void DirectoryEntry::setStartSector(size_t ss)
{
	startSector = ss;
}
size_t DirectoryEntry::getStartSector()
{
	return startSector;
}

void DirectoryEntry::setSectorOffset(size_t so)
{
	sectorOffset = so;
}
size_t DirectoryEntry::getSectorOffset()
{
	return sectorOffset;
}
void DirectoryEntry::setFileSize(size_t fs)
{
	fileSize = fs;
}
size_t DirectoryEntry::getFileSize()
{
	return fileSize;
}

void DirectoryEntry::setNextLink(byte b)
{
	int i = 0;
	while (i < 80 && links[i] != 0)
	{
		i++;
	}

	if (i < 80)
		links[i] = b;

}
byte* DirectoryEntry::getLinks()
{
	return links;
}

void DirectoryEntry::setAccess(char c)
{
	access = c;
}
char DirectoryEntry::getAccess()
{
	return access;
}

//debug
string DirectoryEntry::asString() {
	char buffer[1000];
	sprintf_s(buffer, "fType: %c, fileName: %s, ext: %s, startSector(block): %ld, sectorOffset: %ld, fileSize: %ld, access: %c", 
		fType, fileName.c_str(), ext.c_str(), startSector, sectorOffset, fileSize, access);
	return string(buffer);
}
