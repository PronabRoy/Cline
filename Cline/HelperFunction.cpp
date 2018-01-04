#include <string>
#include <iostream>
#include "FileInfo.h"

//Check two array of char.
bool checkMsg(char* org, char* check)
{
	int offst = 0;
	while ((*(org + offst)) != '\0')
	{
		if (*(org + offst) == *(check + offst))
		{
			offst++;
		}
		else
			return false;
	}

	return true;
}

//Copy two buffer of char.
void copyMsg(char* output, char* input)
{
	int i = 0;
	while ((*(input + i)) != '\0')
	{
		*(output + i) = *(input + i);
		i++;
	}
}

//Open a file for read.
bool openFile(FileInfo& fileInfo)
{
	std::string fileName = "../file/" + (std::string)fileInfo.fileName;
	fileInfo.inputFile.open(fileName, std::ios::ate | std::ios::binary);
	if (fileInfo.inputFile.fail())
	{
		std::cout << "Error to open file" << std::endl;
		return false;
	}

	fileInfo.fileSize = fileInfo.inputFile.tellg();		//Total size of file.
	fileInfo.inputFile.seekg(0, std::ios::beg);			//Make input pointer begaining.
	while (fileInfo.byteRead < fileInfo.fileSize)
	{
		fileInfo.remainingByte = fileInfo.fileSize - fileInfo.byteRead;
		if (fileInfo.remainingByte > fileInfo.byteRead)
		{
			fileInfo.inputFile.read(fileInfo.memory, fileInfo.buffer);
			fileInfo.byteRead += fileInfo.buffer;
		}
		else
		{
			fileInfo.inputFile.read(fileInfo.memory, fileInfo.remainingByte);
			fileInfo.byteRead += fileInfo.remainingByte;
		}
	}

	return true;
}

//Write on a file
bool writeFile(FileInfo& fileInfo)
{
	std::string fileName = "../file/" + (std::string)fileInfo.fileName;
	if (!fileInfo.outputFile.is_open())
	{
		fileInfo.outputFile.open(fileName, std::ios::binary);
		if (fileInfo.outputFile.fail())
		{
			std::cout << "Error to open file" << std::endl;
			return false;
		} 
	}

	if (fileInfo.remainingByte > fileInfo.buffer)
	{
		fileInfo.outputFile.write(fileInfo.memory, fileInfo.buffer);
	}
	else
	{
		fileInfo.outputFile.write(fileInfo.memory, fileInfo.remainingByte);
	}

	return true;
}


