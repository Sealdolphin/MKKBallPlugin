#ifndef NETWORK_H
#define NETWORK_H

//#define _WIN32_WINNT 0x501 //Magic
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>

#include "MKKLogger.h"

#define DEFAULT_BUFLEN 1024
//#define DEBUG_CLIENT

using std::string;
using std::cout;
using std::endl;

class Network {
	/*Data network library*/
	WSADATA wsaData;
	SOCKET ConnectSocket;
	addrinfo *addr_result;	//typedef struct addrinfo was declared
	addrinfo *addr_ptr;
	addrinfo addr_hints;
	char* sendbuffer;
	char receivebuffer[DEFAULT_BUFLEN];
	int info_result;
	int recvbuflen;

	/*Data for networking class(own)*/
	char* domain;
	char* port;
	static int net_id;
	const int self_id;

	MKKLogger networkLogger;
public:
	//Constructor
	Network(string dom = "") :ConnectSocket(INVALID_SOCKET), addr_result(NULL),
		addr_ptr(NULL), recvbuflen(0), domain(NULL), port(NULL), self_id(net_id++) {
		if (dom != "") {
			domain = new char[dom.size()];
			strcpy(domain, dom.c_str());
		}

	}

	int init(void);
	int resolve(string server = "", string port = "");
	int connect_server(void);

	int send_message(string msg);
	int recv_message(void);
	string get_server_input(void) { return receivebuffer; }

	int shutdown_server(void);

	~Network();
};

extern Network network;

#endif //NETWORK_H
