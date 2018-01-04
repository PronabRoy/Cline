#pragma once
#include <fstream> 
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

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

struct stream
{
	bool t = false;
	cv::VideoCapture cap;
	cv::Mat frame; 
	int frameDataSize = 0;
	int totalFrame = 0;
	int remainingFrame;
	int frameRead = 0;
	int height = 0;
	int width = 0;
	char *c = new char[1000000];
};