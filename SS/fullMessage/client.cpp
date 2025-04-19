#include <iostream>
#include <string>
#include <vector>
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#define serverPort 8888
#define BUF_SIZE 5
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

	if ((clientsock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		ExitWithError("Listening socket not created");

	const char* serverAddr = "127.0.0.1";

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverAddr);
	server.sin_port = htons(serverPort);

	if (connect(clientsock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		ExitWithError("connect() failed");

	while (true)
	{
		string echoBuf;
		getline(cin, echoBuf);
		echoBuf += '\n';


		if (send(clientsock, echoBuf.c_str(), echoBuf.length(), 0) != echoBuf.length())
			ExitWithError("send() failed");

		int recvMsgSize = 0;

		vector<char> buff(BUF_SIZE);

		string fullMessage = "";

		while(true)
		{
			if ((recvMsgSize = recv(clientsock, buff.data(), buff.size(), 0)) < 0)
				ExitWithError("recv() failed");

			fullMessage.append(buff.data(), recvMsgSize);

			if (fullMessage[fullMessage.length() - 1] == '\n')
			{
				printMessage(fullMessage, fullMessage.length());
				fullMessage.clear();
				break;
			}
			
		} 
	}

	closesocket(clientsock);
	WSACleanup();


}
