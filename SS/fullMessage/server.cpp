#include <iostream>
#include <string>
#include <vector>
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#define SERVER_PORT 8888
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
	cout << "Primljena poruka: ";
	for (auto i = 0; i < len; cout << message[i++]);
	cout << endl;
}

int main() {

	cout << "Server" << endl;

	WSAData wsa;
	SOCKET listensock;

	if (WSAStartup(0x0202, &wsa) != 0)
		ExitWithError("Startup failed");

	if ((listensock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		ExitWithError("Listening socket not created");

	//sada kada imamo listen sock potrebno je da mu dodelimo port i da ga bindujemo

	sockaddr_in server;
	server.sin_family = AF_INET; //ovo je za IPV4
	server.sin_addr.s_addr = INADDR_ANY; //LISTEN FROM ANYONE
	server.sin_port = htons(SERVER_PORT); // NA OVAJ PORT

	if (bind(listensock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		ExitWithError("bind() failed!");

	//bind(ovaj socket, adresa, velicina)

	if ((listen(listensock, 3)) < 0)
		ExitWithError("listen() failed!");

	//max 3 konekcije

	while (true) //radi ovo zauvek
	{
		sockaddr_in client; //imali smo za server sada za klijent ali ovde ga ne setujemo jer je klijent
		int cLen = sizeof(struct sockaddr_in); //koliko mesta struktura zauzima
		SOCKET clientSock; //ovaj socket ce da prica sa klijentom

		if ((clientSock = accept(listensock, (struct sockaddr*)&client, &cLen)) < 0)
			ExitWithError("accept() failed");

		//kad se neko poveze napravi socket
		//accept(listensocket, clientova adresa,

		string fullMessage = ""; 
		vector<char> echoBuf(BUF_SIZE); // gde se cuva poruka

		int recvMsgSize = recv(clientSock, echoBuf.data(), echoBuf.size(), 0);

		//recv(od koga, gde, kolko max, default flag)

		if (recvMsgSize < 0)
			ExitWithError("recv() failed");

		printMessage(echoBuf.data(), recvMsgSize);

		fullMessage.append(echoBuf.data(), recvMsgSize);

		while (recvMsgSize > 0)
		{
			if (fullMessage[fullMessage.length() - 1] == '\n')
			{
				if (send(clientSock, fullMessage.c_str(), fullMessage.length(), 0) != fullMessage.length())
					ExitWithError("send() failed");

				fullMessage.clear();

			}

			//send(kome, sta, koliko, 0)

			if ((recvMsgSize = recv(clientSock, echoBuf.data(), echoBuf.size(), 0)) < 0)
				ExitWithError("recv() failed");

			printMessage(echoBuf.data(), recvMsgSize);

			fullMessage.append(echoBuf.data(), recvMsgSize);

		}

		closesocket(clientSock);
	}

	closesocket(listensock);
	WSACleanup();
}
