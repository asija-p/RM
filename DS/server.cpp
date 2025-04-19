#include <iostream>
#include <string>
#include <vector>
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#define SERVER_PORT 8888
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
	cout << "Primljena poruka: ";
	for (auto i = 0; i < len; cout << message[i++]);
	cout << endl;
}

int main() {

	cout << "Server" << endl;

	WSAData wsa;
	SOCKET serverSock;

	if (WSAStartup(0x0202, &wsa) != 0)
		ExitWithError("Startup failed");

	if ((serverSock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		ExitWithError("Listening socket not created");

	//sada kada imamo listen sock potrebno je da mu dodelimo port i da ga bindujemo

	sockaddr_in server;
	server.sin_family = AF_INET; //ovo je za IPV4
	server.sin_addr.s_addr = INADDR_ANY; //LISTEN FROM ANYONE
	server.sin_port = htons(SERVER_PORT); // NA OVAJ PORT

	if (bind(serverSock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		ExitWithError("bind() failed!");

	//bind(ovaj socket, adresa, velicina)

	sockaddr_in client;
	int addrlen = sizeof(sockaddr);

	//max 3 konekcije

	while (true) //radi ovo zauvek
	{
		
		char echoBuf[BUF_SIZE]; // gde se cuva poruka

		int recvMsgSize = recvfrom(serverSock, echoBuf, BUF_SIZE, 0, (sockaddr*)&client, &addrlen);

		//recvfrom(od koga, gde, kolko max, default flag, ko salje, velicina drese)

		if (recvMsgSize < 0)
			ExitWithError("recv() failed");

		printMessage(echoBuf, recvMsgSize);

		if (sendto(serverSock, echoBuf, recvMsgSize, 0, (sockaddr*)&client, addrlen) != recvMsgSize)
			ExitWithError("send() failed");

	}

	closesocket(serverSock);
	WSACleanup();
}
