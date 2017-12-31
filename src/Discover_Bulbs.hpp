#ifndef DISCOVER_BULBS_H
#define DISCOVER_BULBS_H
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

//239.10.5.X -> X = group #
#define multicastGroup "239.255.255.250"

//222XX -> X = group #
#define multicastPort 1982

class Discover_Bulbs{
	private:
		int port;
		string userString;
		char receivingBuffer[BUFLEN];
		char temp[BUFLEN];
		char sendMessageBuffer[65507];
		unsigned int totalReceived, slen;
		struct  sockaddr_in serverStructure; /* structure to hold an IP address     */
		int finished = 1;
		unsigned char mc_ttl=5;     /* time to live (hop count) */
		struct  sockaddr_in myStruct;
		int i;
		int iSocket;
		struct sockaddr_in groupSock;
		string dataBuffer = "M-SEARCH * HTTP/1.1\r\nST:wifi_bulb\r\nMAN:\"ssdp:discover\"\r\n";
		int dataBufLen = dataBuffer.length();
		int count = 0;
		int packetsSent =0;
		int packetReceived =0;
		string host;
	public:
		int webSocket;
		Discover_Bulbs(string hostName, int portNumber);
		Discover_Bulbs();
		void fillMultiStruct();	
		void setUpMulticast();
		void createSocket();
		void sendMulticast();
		void connect();
		void disconnect();
		void sendBuffer();
		void sendEnd();
		void startListening(int timeout);
		string getBulbData();
		void sendString(string message);
		void writeMessage();
};
#endif