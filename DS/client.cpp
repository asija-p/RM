#include <iostream>
#include <string>
#include <vector>
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#define serverPort 8888
#define BUF_SIZE 1024
using namespace std;


void ExitWithError(const string& message)
{
	cout << message << "Error code: " << WSAGetLastError << endl;
	WSACleanup(); //Stop and release resources used by Winsock.
	exit(1);
}

void printMessage(const string& message, int len)
{
	cout << "Echoed poruka: ";
	for (auto i = 0; i < len; cout << message[i++]);
	cout << endl;
}

int main()
{


	cout << "Client" << endl;

	WSAData wsa;

	SOCKET clientsock;

	if (WSAStartup(0x0202, &wsa) != 0)
		ExitWithError("Startup failed.");

	if ((clientsock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		ExitWithError("Listening socket not created");

	const char* serverAddr = "127.0.0.1";

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverAddr);
	server.sin_port = htons(serverPort);

	string echoBuf;
	int sz = sizeof(sockaddr);

	while (true)
	{
		
		getline(cin, echoBuf);

		if (sendto(clientsock, echoBuf.c_str(), echoBuf.length(), 0, (sockaddr*)&server, sz) != echoBuf.length())
			ExitWithError("send() failed");

		int recvMsgSize = 0;

		char buff[BUF_SIZE];

		if ((recvMsgSize = recvfrom(clientsock, buff, BUF_SIZE, 0, (sockaddr*)&server, &sz))< 0)
				ExitWithError("recv() failed");

		printMessage(buff, recvMsgSize);

	}

	closesocket(clientsock);
	WSACleanup();


}
