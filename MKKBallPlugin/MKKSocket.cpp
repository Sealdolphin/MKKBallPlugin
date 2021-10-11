/*
NETWORK.CPP
*/
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501 //Some kind of black magic

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>

#include "MKKSocket.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

//Using STD library structures
using std::string;
using std::cout;
using std::endl;


//-------------------------------------------------------------------------
int Network::init(void) {
	// Initialize Winsock
	info_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (info_result != 0) {
		//WSA Startup failed
		return ERR_WSA_FAIL;
	}
	//Free the memory and set up addrinfo hints
	ZeroMemory(&addr_hints, sizeof(addr_hints));
	addr_hints.ai_family = AF_INET;
	addr_hints.ai_socktype = SOCK_STREAM;
	addr_hints.ai_protocol = IPPROTO_TCP;

	return ERR_NOERR;
}
//-------------------------------------------------------------------------
int Network::resolve(string server, string arg_port) {
	//Setting up domain
	if (server == "") {
		//Searching for default domain
		if (domain == nullptr) {
			//No domain name was given!
			return ERR_NO_DOMAIN;
		}
	}
	else {
		
		if (domain != nullptr) delete[] domain;
		domain = new char[server.size()];
		strcpy_s(domain, strlen(server.c_str()) + 1, server.c_str());
	}
	if (port != nullptr) delete[] port;
	port = new char[arg_port.size()];
	strcpy_s(port, strlen(arg_port.c_str()) + 1, arg_port.c_str());

	//Resolving given domain name
	info_result = getaddrinfo(domain, port, &addr_hints, &addr_result);
	if (info_result != 0) {
		//Getaddrinfo failed. Cannot resolve given address
		WSACleanup();
		return ERR_GETADDRINFO_FAIL;
	}

	return ERR_NOERR;
}
//-------------------------------------------------------------------------
int Network::connect_server(void) {
	// Attempt to connect to an address until one succeeds
	for (addr_ptr = addr_result; addr_ptr != NULL; addr_ptr = addr_ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(addr_ptr->ai_family, addr_ptr->ai_socktype, addr_ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			//Socket is invalid. Maybe closed or terminated?
			WSACleanup();
			return ERR_INVALID_SOCK;
		}

		// Connect to the server.
		info_result = connect(ConnectSocket, addr_ptr->ai_addr, (int)addr_ptr->ai_addrlen);
		if (info_result == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
		}
		break;
	}
	//Clear memory
	freeaddrinfo(addr_result);

	if (ConnectSocket == INVALID_SOCKET) {
		//Connection failed, socket was invalidated.
		WSACleanup();
		return ERR_CONNECT_FAIL;
	}

	//Make socket blocking/nonblocking
	u_long block = 1;
	info_result = ioctlsocket(ConnectSocket, FIONBIO, &block);
	if (info_result != NO_ERROR) {
		//Socket is blocking, suggest termination
	}
	return ERR_NOERR;
}
//-------------------------------------------------------------------------
int Network::send_message(string msg) {

	try {
		//Sent message is null or empty
		if (msg == "0") return ERR_NULLMSG;
		//Clearing sendbuffer and filling with the message
		if (sendbuffer != nullptr) delete[] sendbuffer;
		sendbuffer = new char[msg.length()];
		strcpy_s(sendbuffer, strlen(msg.c_str()) + 1, msg.c_str());
		sendbuffer[msg.length()] = 0;

		//send buffer through the network
		info_result = send(ConnectSocket, sendbuffer, (int)strlen(sendbuffer), 0);

		if (info_result == SOCKET_ERROR) {
			//Sending failed. 
			closesocket(ConnectSocket);
			WSACleanup();
			return ERR_SEND_FAIL;
		}
	}
	catch (std::exception error) {
		
	}

	return ERR_NOERR;
}
//-------------------------------------------------------------------------
int Network::recv_message(void) {
	// Receive until the peer closes the connection
	char c;

	while ((info_result = recv(ConnectSocket, &c, 1, 0)) == 1) {
		receivebuffer[recvbuflen++] = c;
		if (c == 0) {
			//Data received
			recvbuflen = 0;
			return ERR_NOERR;
		}
	}

	if (info_result == 0) {
		return ERR_CONNECTION_LOST;
	}

	return ERR_RECEIVE_FAIL;
}
//-------------------------------------------------------------------------
int Network::shutdown_server(void) {
	// shutdown the connection since no more data will be sent
	info_result = shutdown(ConnectSocket, SD_BOTH);
	if (info_result == SOCKET_ERROR) {
		
		closesocket(ConnectSocket);
		WSACleanup();
		return ERR_SHUTDOWN_FAIL;
	}
	return ERR_NOERR;
}
//-------------------------------------------------------------------------
Network::~Network() {
	// cleanup and free memory

	closesocket(ConnectSocket);
	WSACleanup();
	if (sendbuffer != nullptr) delete[] sendbuffer;
	if (domain != nullptr) delete[] domain;
	if (port != nullptr) delete[] port;
}
//-------------------------------------------------------------------------