#include "Client.h"			

int main()
{
	//Global IPV4 -> 103.77.19.99			103.77.19.101
	//Local IPV4 -> 192.168.1.100			192.168.0.104

	std::string ip = "192.168.1.100";
	int port = 1111;

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







