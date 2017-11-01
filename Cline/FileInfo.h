#pragma once
#include <fstream> 

//Struct for File 
struct FileInfo
{
	static const int buffer = 17000; 
	char fileName[100];
	int fileSize;
	char memory[buffer];
	int remainingByte;
	int byteRead = 0;
	std::ifstream inputFile;
	std::ofstream outputFile;

	void empty()
	{
		fileSize = 0;
		remainingByte = 0;
		byteRead = 0;
		memory[buffer] = 0; 

		//Close file which if this client open any file.
		if (inputFile.is_open())
			inputFile.close();
		if (outputFile.is_open())
			outputFile.close();
	}
};