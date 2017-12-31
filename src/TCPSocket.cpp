#include "TCP_Connection.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <thread>
#include <pthread.h>
#include <sys/time.h>
#include <mutex>
#include <iostream>
#include <vector>
#define BUFLEN 66507
std::mutex mtx1;
std::mutex mtx2;
TCPSocket* TCPSocket::singleton = NULL;
TCPSocket::TCPSocket(){


}
TCPSocket::TCPSocket(string bulb_ip, string host, int port){
	host = host;
	bulb_ip = bulb_ip;
	port = port;
}

void TCPSocket::createSocket(string bulb_ip){
	/*Create client Scocket*/
	clientSocket = socket(PF_INET,SOCK_STREAM,0);
	/*zero out the  serverAddr structure and assign the ip addres and port*/
	memset((char*)&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((u_short)port);
	ptrh = gethostbyname(bulb_ip.c_str()); //convert host name to IP address
	memcpy(&serverAddr.sin_addr,ptrh->h_addr,ptrh->h_length);
	connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
}
void TCPSocket::sendToBulb(string command){
	char *buffer = (char*) malloc(1024);
	//int buffLen = 0;
	if(buffer == NULL){
		cerr<<"No Buffer allocation";
	}
	while(1){
		memset(buffer,'\0',1024);
		fgets(buffer,1024,stdin);
		if ((strlen(buffer) > 0) && (buffer[strlen (buffer) - 1] == '\n'))
        	memcpy(buffer +(strlen (buffer) - 1), "\r\n",2);
        cout << buffer;
		 if( (bytes_send = send(clientSocket,buffer,strlen(buffer)+1,0)) <	0){
		 	cerr << "Error sending Command";
		}

	}

	
}

void TCPSocket::receiveFromBulb(){
	//long tid = (long)t;
	while(1){
		fflush(stdout);
		memset(server_response, '\0', sizeof(server_response));
		bytes_receive = recv(clientSocket, server_response, sizeof(server_response),0);
		if(bytes_receive < 0){
			continue;
		}else		
			printf("%s\n",server_response);
	}
}

TCPSocket* TCPSocket::getInstance(){
	if(singleton == NULL)
		singleton = new TCPSocket();
	return singleton;
}




int main(int argc, char *argv[]){
	TCPSocket *connection = TCPSocket::getInstance();
	connection->createSocket("192.168.1.2");
	string command = "{ \"id\": \"0x000000000335e0b0\", \"method\": \"set_power\", \"params\":[\"off\", \"smooth\", 500]}\r\n";

	/*while(1){
		connection->sendToBulb(command);
		connection->receiveFromBulb();
	}*/


		mtx1.lock();
		std::thread sendTo(&TCPSocket::sendToBulb, connection,command);
		mtx1.unlock();
		mtx2.lock();
		std::thread receiv(&TCPSocket::receiveFromBulb, connection);
		mtx2.lock();
		sendTo.join();
		receiv.join();
	
	
	
}