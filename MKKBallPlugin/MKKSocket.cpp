/*
NETWORK.CPP
*/
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501 //Magic

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

using std::string;
using std::cout;
using std::endl;

int Network::init(void) {		// Initialize Winsock
	info_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (info_result != 0) {
		
		networkLogger.createLog(LL_ERROR, "WSAStartUp failed.");

		return 1;
	}
	//Free the memory and set up addrinfo hints
	ZeroMemory(&addr_hints, sizeof(addr_hints));
	addr_hints.ai_family = AF_INET;
	addr_hints.ai_socktype = SOCK_STREAM;
	addr_hints.ai_protocol = IPPROTO_TCP;

	networkLogger.createLog(INFO, "WSAStartUp successful.");

	return 0;
}

int Network::resolve(string server, string arg_port) {		// Resolve the server address and port
	//Setting domain
	if (server == "") {
		//Searching default domain
		if (domain == nullptr) {

			networkLogger.createLog(LL_ERROR, "No domain was given!");

			return 1;
		}
	}
	else {
		if (domain != nullptr) delete[] domain;
		domain = new char[server.size() + 1];
		strcpy(domain, server.c_str());
	}
	if (port != nullptr) delete[] port;
	port = new char[arg_port.size() + 1];
	strcpy(port, arg_port.c_str());

	info_result = getaddrinfo(domain, port, &addr_hints, &addr_result);
	if (info_result != 0) {

		networkLogger.createLog(LL_ERROR, "Getaddrinfo failed, cannot resolve domain.");

		WSACleanup();
		return 1;
	}

	networkLogger.createLog(INFO, "Domain resolved");

	return 0;
}

int Network::connect_server(void) {
	// Attempt to connect to an address until one succeeds
	for (addr_ptr = addr_result; addr_ptr != NULL; addr_ptr = addr_ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(addr_ptr->ai_family, addr_ptr->ai_socktype, addr_ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			
			networkLogger.createLog(LL_ERROR, "Socket is invalid.");

			WSACleanup();
			return 1;
		}

		// Connect to server.
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
		
		networkLogger.createLog(LL_ERROR, "Connection failed.");

		WSACleanup();
		return 1;
	}
	else {

		networkLogger.createLog(INFO, "Connection established.");

	}

	//Make socket blocking/nonblocking
	u_long block = 1;
	info_result = ioctlsocket(ConnectSocket, FIONBIO, &block);
	if (info_result != NO_ERROR) {
		
		networkLogger.createLog(WARNING, "Socket is not non-blocking. Suggest termination.");

	}
	return 0;
}

int Network::send_message(string msg) {
	if (msg == "0") return 1;
	if (sendbuffer != nullptr) delete[] sendbuffer;
	sendbuffer = new char[msg.length() + 1];
	strcpy(sendbuffer, msg.c_str());
	sendbuffer[msg.length()] = 0;

	info_result = send(ConnectSocket, sendbuffer, (int)strlen(sendbuffer) + 1, 0);

	if (info_result == SOCKET_ERROR) {
	
		networkLogger.createLog(LL_ERROR, "Sending data failed.");

		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}


	networkLogger.createLog(INFO, "Data Sent: " + msg);


	return 0;
}

int Network::recv_message(void) {
	// Receive until the peer closes the connection
	char c;

	while ((info_result = recv(ConnectSocket, &c, 1, 0)) == 1) {
		receivebuffer[recvbuflen++] = c;

		if (c == 0) {

			networkLogger.createLog(INFO, "Data received.");

			recvbuflen = 0;
			return 0;

		}

	}

	if (info_result == 0) {
		
		networkLogger.createLog(LL_ERROR, "Connection lost to server.");

		return 1;
	}

	return 2;
}

int Network::shutdown_server(void) {
	// shutdown the connection since no more data will be sent
	info_result = shutdown(ConnectSocket, SD_BOTH);
	if (info_result == SOCKET_ERROR) {
		
		networkLogger.createLog(LL_ERROR, "Shutdown failed.");

		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	
	networkLogger.createLog(INFO, "Server closed.");

	return 0;
}

Network::~Network() {
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	networkLogger.createLog(INFO, "Network has been destroyed.");

	if (sendbuffer != nullptr) delete[] sendbuffer;
	delete[] domain;
	delete[] port;
}
