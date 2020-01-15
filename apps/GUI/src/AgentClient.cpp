#include "AgentClient.h"
#include <ctime>
#include <sstream>
#include <iostream>
#include <vector>

AgentClient::AgentClient(QMainWindow *parent)
{
	//myPort = DEFAULT_PORT; // char -> int
	
	ConnectSocket = INVALID_SOCKET;
	myClientName = "CppClient";
}


bool AgentClient::ConnectToLocalHostServer()
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("localHost", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return false;
		}

		// Connect to server.
		iResult = ::connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return false;
	}
	printf("Connected to server!\n");

	// send a test message
	this->sendMessage("test");

	thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)messageThread, this, 0, 0);
	//thread_started = true;
	return true;
}

void AgentClient::DisconnectFromLocalHostServer()
{
	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}
	
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
}

int AgentClient::makeCheckSum(const char *dataAsBytes)
{
	int checkSum = 0;
	for (int i = 0; i < std::strlen(dataAsBytes); i++)
	{
		checkSum += (int)dataAsBytes[i];
	}
	return checkSum;
}

std::string AgentClient::getTimeStamp()
{
	std::time_t now = std::time(nullptr);
	std::string timestamp = std::ctime(&now);
	// TODO: this is not in the correct format
	std::string timeTest = std::to_string(now);
	return timeTest;
}

void AgentClient::sendMessage(std::string msg)
{
	
	// first we build the message to send
	std::string package;
	package.append(getTimeStamp());
	package.append(SEPARATION_CHARACTER);
	package.append(myClientName);
	package.append(SEPARATION_CHARACTER);
	package.append(msg);
	package.append(SEPARATION_CHARACTER);

	checkSum = makeCheckSum(package.c_str());
	std::string checkSumStr = std::to_string(checkSum);

	char* sendbuf = new char[std::strlen(package.c_str()) + std::strlen(checkSumStr.c_str()) + 1];
	std::strcpy(sendbuf, package.c_str());
	std::strcat(sendbuf, checkSumStr.c_str());

	//sendbuf = "s_s_s_4";
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	Sleep(100);
	printf("Bytes Sent: %ld\n", iResult);

	return;
}



bool AgentClient::interpreteMessage(const std::string &message)
{
	if (message.size() < 1) return false;

	std::stringstream messageStream(message);
	std::vector<std::string> stringList;
	std::string tmpString;
	while (std::getline(messageStream, tmpString, '_'))//char(SEPARATION_CHARACTER)))
	{
		stringList.push_back(tmpString);
	}

	if (stringList.size() < 3) // TODO: this is the min number of split if we have correctly received the message
		return false;

	// this method modifies a class member
	myCurrentMessage.timestamp = stringList.at(0);
	myCurrentMessage.sender = stringList.at(1);
	myCurrentMessage.message = stringList.at(2);
	myCurrentMessage.checksum = std::atoi(stringList.at(3).c_str());

	return true;
}

void AgentClient::startThread()
{
	while (1)
	{
		if (connected)
		{
			// the function recv actually wait for incoming messages without loading 
			// any computation on the cpu,
			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
				//std::cout << "Bytes received: " << iResult << std::endl;
				// ok, we do not need to print anything
			}
			else { // something went wrong, let's figure out what
				if (iResult == 0)
					std::cout << "Connection closed" << std::endl;
				else
					std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
			}
			if (interpreteMessage(recvbuf))
				emit messageReceived();
		}
	}
}

