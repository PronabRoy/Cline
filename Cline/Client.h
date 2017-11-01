#pragma once
//This is for use "Winsock" function. 
#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)
#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <string>  
#include "SendAndRecv.h"
#include "FileInfo.h"
#include <mutex>

#define SIZE_OF_MSG 255

//This function belong to "HelperFunction.cpp" file.
bool checkMsg(char* , char*);
void copyMsg(char* input, char* output);
bool writeFile(FileInfo& fileInfo);

using std::cout;
using std::endl;
using std::cerr;

enum PacketType {
	p_message,
	p_connected,
	p_fileRequest,				//Client send a file request with this ID and file name.
	p_unknownFileName,			//Server reuturn this ID, if server don't have that particular file which is client requested
	p_readyToTransmitFile,		//Server return this ID when client requested file exist in server.
	p_ackOkForFileRequest,		//Client send this ID when client ready to receive and also after receive a packet.
	p_fileTransmiting,			//Server reurn this ID after send a packet.
	p_ackForEndFile,			//Client send this ID when it receive full file. 
	p_availableFile
};

class Client
{
public:
	Client(std::string, int);
	bool connectServer();
	void sendNRecv();
	friend void messageHandelerThread(Client* client);
	//Method.
	bool processMessage(PacketType&);
	~Client();


private:
	SOCKET _clientSock;
	std::string _ip;
	int _port, _remByte;
	sockaddr_in _serverAddr;
	WSAData  _wsData;
	WORD _var;
	std::thread _clientThread;
	std::mutex _clientMutex;
	FileInfo _fileInfo;


};

static Client* thisClient;
