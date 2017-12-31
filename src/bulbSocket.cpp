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
using namespace std;
std::mutex mtx1;



struct timeval currTimeVal;

class Discover_Bulbs{
	private:
		string host;
		

	public:
				// Constructor for UDPClient, start with IP address and portNumber
		Discover_Bulbs(string hostName, int portNumber){
			host = hostName;
			port = portNumber;
		}

		void connectSocketDiscover_Bulbs(){
			if ( (webSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
				perror("Opening datagram socket error");
				exit(1);
			} else
				printf("Opening the datagram socket...OK.\n");
		}

		void fillMultiStruct(){
			memset((char *) &groupSock, 0, sizeof(groupSock));
			groupSock.sin_family = AF_INET;
			groupSock.sin_addr.s_addr = inet_addr(multicastGroup);
			groupSock.sin_port = htons(multicastPort);
			bind(webSocket, (struct sockaddr *)&groupSock, sizeof(groupSock));
		}

		void setUpMulticast(){
			if( (setsockopt(webSocket, IPPROTO_IP, IP_MULTICAST_TTL, (void*) &mc_ttl, sizeof(mc_ttl))) < 0){
				perror("Setting local interface error");
				exit(1);
			}
			else
				printf("Setting the local interface...OK\n");
		}


		void sendMulticast(){

			printf("Count: %d ", count++);
			if( (sendto(webSocket, dataBuffer.c_str(), dataBufLen, 0, (struct sockaddr*)&groupSock, sizeof(groupSock))) < 0)
				perror("Sending datagram message error");
			else
				printf("Sending datagram message...OK\n");
			sleep(1);

		}
		// Connect
		void connect(){	
			memset((char *)&serverStructure,0,sizeof(serverStructure)); /* clear sockaddr structure */

			serverStructure.sin_family = AF_INET;
			serverStructure.sin_port = htons(port);

			myStruct.sin_family = PF_INET;
			myStruct.sin_port = htons(0);
			myStruct.sin_addr.s_addr = htonl(INADDR_ANY);

			i = bind(webSocket, (struct sockaddr *) &myStruct, sizeof(myStruct));
			if (i < 0){
				cerr << "Bind result " << i << "\n";
				exit(-1);
			}
		}

		void sendUDPXTimes(int x){
			int i = 0;
				for(i = 0; i < x; i++){
					sendMulticast();
				}
				sendEnd();
				usleep(sleepTimeInMicroS);
		}

		// This will send a c++ string to the bounded port
		void sendBuffer(){
				sendto(webSocket, sendMessageBuffer, sizeof(sendMessageBuffer),0 ,
				(struct sockaddr *) &(serverStructure), sizeof(struct sockaddr));
				packetsSent++;
		}

		void sendEnd(){
			memset(sendMessageBuffer,0,sizeof(sendMessageBuffer)); // Clear buffer
			strcpy(sendMessageBuffer, "end");
			sendBuffer();
		}

		// This will listen for messages
		void startListening(){
			while(true){
			
				fflush(stdout);
				memset(receivingBuffer,0,sizeof(receivingBuffer));         
				//try to receive some data, this is a blocking call
				totalReceived = recvfrom(webSocket, receivingBuffer, BUFLEN, 0, (struct sockaddr *) &serverStructure, &slen);
				if(totalReceived <= 0){
					break;
				}	
				cout << "Message Received: " << receivingBuffer << "\n";

				// If we recieve a end message end
				if(strcmp(receivingBuffer,"end") == 0){
					packetReceived++;
					return;
				}
			}
		}

		/****
		*Returns metaData from bulb
		*
		*
		****/
		void getBulbData(){
			if(receivingBuffer != "")
				return receivingBuffer;
		}
		// Takes a string as input then sends the string to the specified server.
		void sendString(string message){
			int numberOfSentBytes;
			numberOfSentBytes=sendto(webSocket, message.c_str(), message.length()+1,0 ,
			(struct sockaddr *) &(serverStructure), sizeof(struct sockaddr));
		
			if (numberOfSentBytes < 0){
				cerr << "Error sending message\n";
				exit(-1);
			}
			//cout << "Client " << numberOfSentBytes << " bytes to the server\n";
		}

		// Call this message to prompt the user to enter a string
		void writeMessage(){
			cout << "Please enter a valid sentence:\n>";
			getline(cin, userString);
			//cout << "You entered: " << userString << "\n";
			sendString(userString);
		}


};


int main(int argc, char *agrv[]){ 

	Discover_Bulbs mainServer("127.0.0.1", 5555);
	mainServer.connectSocketDiscover_Bulbs();
	mainServer.connect();

	mainServer.fillMultiStruct();
	mainServer.setUpMulticast();
	mainServer.sendMulticast();
   	mainServer.startListening();

	//mtx1.lock();
	//std::thread t1(&Discover_Bulbs::startListening, &mainServer);
	//mtx1.unlock();
//	std::thread t2(&Discover_Bulbs::sendUDPXTimes, &mainServer, 5);
	


	//t1.join();
//	t2.join();
	cout << "----------------------------------------\n";




	close(mainServer.webSocket);
	return 0;
}

