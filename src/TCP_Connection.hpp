#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <thread>
#include <sys/time.h>
#include <mutex>
#define BUFLEN 66507 
using namespace std;
class TCPSocket{
	private:
		struct sockaddr_in serverAddr; //structure to hold serverAddr IP
		int clientSocket; //client scoket descriptor
		struct hostent *ptrh;
		char buffer[BUFLEN],chDecision[BUFLEN],server_response[BUFLEN];
		ssize_t bytes_send, bytes_receive;
		int port = 55443;
		string bulb_ip;
		static TCPSocket* singleton;
		TCPSocket();
		TCPSocket(TCPSocket const&)=delete;
		void operator=(TCPSocket const&)=delete;
		string host;
	public:
		TCPSocket(string bulb_ip, string host, int port);
		void createSocket(string bulb_ip);
		void sendToBulb(string command);
		void receiveFromBulb();
		//void sockListen();
		static TCPSocket* getInstance();
};

#endif