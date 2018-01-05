#include "Client.h"			

int main()
{

	std::string ip = "127.0.0.1";
	int port = 1111;

	std::cin >> ip;

	Client client(ip, port);
	if (client.connectServer() == true)
		client.sendNRecv();
	else
	{
		cout << "Unable to connect server" << endl; 
	}
	
		
	std::cin.get();
	return 0;
}







