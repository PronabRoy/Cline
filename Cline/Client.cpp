#include "Client.h"

Client::Client(std::string ip, int port)
{
	this->_ip = ip;
	this->_port = port;
	thisClient = this;
	_remByte = 0;
}

bool Client::connectServer()
{
	//Initialize 
	//........................................................................
	_var = MAKEWORD(2, 1);
	int wsOk = WSAStartup(_var, &_wsData); 
	if (wsOk != 0)
	{
		cout << "Faild to initialize wsData " << endl;
		exit(0);
	}

	//Details about server.............................
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	_serverAddr.sin_addr.s_addr = inet_addr(_ip.c_str()); 

	//Creating Socket.....................................
	_clientSock = socket(AF_INET, SOCK_STREAM, 0);
	//Connect to the server................................
	if (connect(_clientSock, (sockaddr*)&_serverAddr, sizeof(_serverAddr)) != 0)
	{
		cout << "Faild to connect server!" << endl; 
		return false;
	}
	else
		return true;
}

void Client::sendNRecv()
{
	//Receiving message from server.
	_clientThread = std::thread(messageHandelerThread, this);
	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)messageHandelerThread, NULL, NULL, NULL);

	//Sending message to the server.
	PacketType packetType;
	char msg[SIZE_OF_MSG];  
	char check[] = "File";
	while (true)
	{ 
		std::cin.getline(msg, sizeof(msg));    
		//If we type "File" then send a message to the server for "p_fileRequest" for download a file from server.
		if (checkMsg(msg, check))	//We create "checkMsg()" function in "HelperFunction" header file for check two arry of charecter.
		{
			cout << "Name of File : " << endl;
			std::cin.getline(msg, sizeof(msg));
			packetType = PacketType::p_fileRequest;
			sendAll(_clientSock, (char*)&packetType, sizeof(packetType));	//Sending a request for file name.
			sendAll(_clientSock, (char*)&msg, sizeof(_fileInfo.fileName));					//Sending name of file that we want.
		}
		else if (checkMsg(msg, "available"))
		{
			packetType = PacketType::p_availableFile;
			sendAll(_clientSock, (char*)&packetType, sizeof(packetType));
		}
		else
		{ 
			//First send type of PacketType that is sending and then send actual message.
			packetType = PacketType::p_message;
			sendAll(_clientSock, (char*)&packetType, sizeof(packetType));
			if(sendAll(_clientSock, msg, sizeof(msg)) == false)
				break;
		}
	}
}


//Client therad for recv data form server and process Data.
void messageHandelerThread(Client* client)
{
	char msg[SIZE_OF_MSG];
	//Use this mutex so that file dosen't corrept and make sure at a time we are receiving only one packet in this case "Message of file".
	std::unique_lock<std::mutex> uniqueLock(client->_clientMutex, std::defer_lock);
	PacketType packetType;
	while (true)
	{ 
		if (!recvAll(client->_clientSock, (char*)&packetType, sizeof(packetType)))
			break; 

		uniqueLock.lock();
		if (!client->processMessage(packetType))
		{
			uniqueLock.unlock();
			break; 
		}
		uniqueLock.unlock();
	}

	cout << "Server expaired!" << endl;
	closesocket(client->_clientSock);
}

bool Client::processMessage(PacketType& packetType)
{ 
	int recvData = 0;
	PacketType pacTyp;
	switch (packetType)
	{
	case p_connected:
	{
		cout << "Wellcome to the Server........" << endl;
		break;
	}

	case p_message:
	{
		char msg[SIZE_OF_MSG];
		if (!recvAll(_clientSock, msg, SIZE_OF_MSG))
			return false;
		cout << "Respons back from server : " << msg << endl;
		break;
	}

	case p_availableFile:
	{
		int fileSize = 0;  
		recvAll(_clientSock, (char*)&fileSize, sizeof(int));
		fileSize = ntohs(fileSize);
		cout << fileSize << endl;
		break;
	}

	case p_readyToTransmitFile:
	{
		char msg[50];
		//Received File name from server.
		recvAll(_clientSock, _fileInfo.fileName, sizeof(_fileInfo.fileName));
		//Send ack to the server that we ready to receive file.
		pacTyp = PacketType::p_ackOkForFileRequest;
		sendAll(_clientSock, (char*)&pacTyp, sizeof(pacTyp));
		 
		break;
	}

	case p_fileTransmiting:
	{
		//Recv a block of file form server
		{
			/*while (recvData < sizeof(_fileInfo.memory))
			{
				//recv() does not always receive the data in the chunks that it was sent by send(), 
				int data = recv(_clientSock, _fileInfo.memory, sizeof(_fileInfo.memory), 0);
				recvData += data;
			}*/
		}
		recvAll(_clientSock, _fileInfo.memory, sizeof(_fileInfo.memory));
		recvAll(_clientSock, (char*)&_fileInfo.remainingByte, sizeof(_fileInfo.remainingByte)); 
		recvAll(_clientSock, (char*)&_fileInfo.fileSize, sizeof(_fileInfo.fileSize));  

		writeFile(_fileInfo);
		if (_fileInfo.remainingByte < _fileInfo.buffer)
		{
			pacTyp = PacketType::p_ackForEndFile;
			sendAll(_clientSock, (char*)&pacTyp, sizeof(pacTyp));
		}
		else
		{
			pacTyp = PacketType::p_ackOkForFileRequest;
			sendAll(_clientSock, (char*)&pacTyp, sizeof(pacTyp));
		}
		_remByte = _fileInfo.remainingByte;
		std::cout << _fileInfo.remainingByte << std::endl; 
		break;
	}
	 	
	case p_ackForEndFile:
	{
		std::cout << "File Received" << endl;
		_fileInfo.empty(); 
		break;
	}

	case p_unknownFileName:
	{
		std::cout << "ERROR : File dosen't exist in server!" << std::endl;
		break;
	}

	default:
	{
		cout << "Can't process this message!" << endl;
		Sleep(10);
		break;
	}


	}

	return true;
}

Client::~Client()
{
	if (_fileInfo.inputFile.is_open())
		_fileInfo.inputFile.close();

	if (_fileInfo.outputFile.is_open())
		_fileInfo.outputFile.close();
}
