
#ifndef AGENT_CLIENT_H_
#define AGENT_CLIENT_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include <QMainWindow>
#include <QApplication>
#include <qthread.h>


struct messageFromAgent {
	std::string timestamp;
	std::string sender;
	std::string message;
	int checksum;

};


class commandFromAgent
{
public:
	const std::string MOVE_TO_POSE_PREFIX = "MoveToPose";
	const std::string MOVE_TO_NAMED_LOCATION_PREFIX = "MoveToNamedLocation";
	const std::string POSE_PREFIX = "Pose";
	const std::string POSE_REQUEST = "RequestPose";
	const std::string ROBOT_RADIUS_PREFIX = "RobotRadius";
	const std::string ROBOT_ARRIVED_PREFIX = "Arrived"; // Not (yet) used - should be used as "Arrived (x,y, headingangle)" (send to agent).
	const std::string ROBOT_SPEED_PREFIX = "Speed"; // Not (yet) used - should be used when sending the robot's current speed ("Speed: (vx,vy)") to the agent)
	const std::string UNABLE_TO_MOVE = "UnableToMove";  // Not (yet) used - should be used in the RobotHandler, when necessary
	const std::string VISION_PREFIX = "Vision";  // Not currently used (will be used if the Kinect is placed in the RobotHandler)
	const std::string SPEECH_PREFIX = "Speech";  // Not currently used (will be used if the Kinect is placed in the RobotHandler)
	const std::string MESSAGE_SPLIT_CHARACTER = ":";
	const std::string EMPTY_MESSAGE_STRING = "EmptyMessageReceived";
	const std::string INVALID_MESSAGE_STRING = "InvalidMessageReceived";
};


class AgentClient : public QThread
{
	Q_OBJECT

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "7"
#define SEPARATION_CHARACTER "_"

public:
	AgentClient(QMainWindow *parent = nullptr);
	

	void setPort(int port) { myPort = port; }

	bool ConnectToLocalHostServer();

	void DisconnectFromLocalHostServer();

	void sendMessage(std::string msg);

	void setClientName(std::string name) { myClientName = name; }

	bool isConnected() { return connected; }

	messageFromAgent getMessageFromAgent() { return myCurrentMessage; }

signals:
	void messageReceived();                //!< emit a string when the result is ready

private:
	int makeCheckSum(const char *dataAsBytes);

	void startThread();

	bool connected;

	bool interpreteMessage(const std::string &message);
	std::string getTimeStamp();

	int myPort;
	SOCKET ConnectSocket;
	HANDLE thread;
	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	int checkSum;

	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	std::string myClientName;

	messageFromAgent myCurrentMessage;

	static DWORD messageThread(LPVOID param) {
		AgentClient *client = (AgentClient*)param;
		client->startThread();
		return 0;
	}
};

#endif //AGENT_CLIENT_H_