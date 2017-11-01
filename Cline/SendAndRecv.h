#pragma once
#include <WinSock2.h>
#include "FileInfo.h"

static inline bool sendAll(SOCKET& clientSock, char memory[], int fileSize)
{
	int sendData = 0;
	while (sendData < fileSize)
	{
		//Received function dosen't received all data at onece.
		int data = send(clientSock, memory, fileSize, 0);
		if (data == -1)				//-1 = SOCKET_ERROR
			return false;

		sendData += data;
	}
	return true;
}

static inline bool recvAll(SOCKET& clientSock, char memory[], int fileSize)
{
	int recvData = 0;
	while (recvData < fileSize)
	{
		//recv() does not always receive the data in the chunks that it was sent by send(), 
		int data = recv(clientSock, memory, fileSize, 0);
		if (data == -1)				//-1 = SOCKET_ERROR
			return false;

		recvData += data;
	}

	return true;
}