#include "Discover_Bulbs.hpp"
// /#define _GLIBCXX_USE_CXX11_ABI 0/1
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;
std::mutex mtx1;
int main(int argc, char *agrv[]){ 
	Discover_Bulbs webConnect("127.0.0.1",1982);
	webConnect.createSocket();
	webConnect.connect();

	webConnect.fillMultiStruct();
	webConnect.setUpMulticast();
	webConnect.sendMulticast();
   	webConnect.startListening(1);
   	string data =webConnect.getBulbData();
   	cout <<  data;
}