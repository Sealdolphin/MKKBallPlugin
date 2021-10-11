/**
This is an example Client using winsock2 to communicate via TCP/IP protocol.
The class Network has all the required information and function to solve two-way communication.

To use this specified library you must have windows.h and winsock2.h installed and properly linked.
**/
#ifndef NETWORK_H
#define NETWORK_H

//#define _WIN32_WINNT 0x501 //Magic
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>

//The length of the default buffer
#define DEFAULT_BUFLEN 1024
//#define DEBUG_CLIENT

using std::string;
using std::cout;
using std::endl;

//Basic enumeration for different Networking related errors
//Can be used for debugging
//-------------------------------------------------------------------------
typedef enum NetworkError {
	ERR_NOERR = 0,
	ERR_WSA_FAIL,
	ERR_NO_DOMAIN,
	ERR_GETADDRINFO_FAIL,
	ERR_INVALID_SOCK,
	ERR_CONNECT_FAIL,
	ERR_SEND_FAIL,
	ERR_CONNECTION_LOST,
	ERR_SHUTDOWN_FAIL,
	ERR_RECEIVE_FAIL,
	ERR_NULLMSG
}NetworkError;
//-------------------------------------------------------------------------
class Network {

public:
	//Public constructor of the Network class. A network is required to have a domain name.
	Network(string domain_string = "") :ConnectSocket(INVALID_SOCKET), addr_result(NULL),
		addr_ptr(NULL), recvbuflen(0), domain(NULL), port(NULL), self_id(net_id++) {
		if (domain_string != "") {
			domain = new char[domain_string.size()];
			strcpy_s(domain, domain_string.size(), domain_string.c_str());
		}
	}

	//Initializes the Network parameters and other services (such as WSA)
	int init(void);
	//Resolves the given domain from the IP address and port
	int resolve(string server = "", string port = "");
	//Establishes connection to the server. This may only called once resolve has returned without error.
	int connect_server(void);

	//Sends a null terminated message through the connection.
	int send_message(string msg);
	//Waits until a message is coming from the server.
	int recv_message(void);
	//Returns the raw received data. It can be NULL
	string get_server_input(void) { return receivebuffer; }
	//Shuts down the connection.
	int shutdown_server(void);

	~Network();
private:
	//PREMADE VARIABLES FROM WINSOCK EXAMPLE

	//The WSAData stored in the class. This is updated during WSAStartup.
	WSADATA wsaData = WSADATA();
	//The socket used for the connection.
	SOCKET ConnectSocket;
	//Information about the server address
	//typedef struct addrinfo was declared
	addrinfo *addr_result;
	addrinfo *addr_ptr;
	addrinfo addr_hints = addrinfo();
	//The buffer going to be sent
	char* sendbuffer = nullptr;
	//The buffer to be filled with the incoming response
	char receivebuffer[DEFAULT_BUFLEN] = {};
	//Contains information about various helper and sub-functions
	int info_result = 0;
	//The length of the receivebuffer
	int recvbuflen = 0;

	//ADDITIONAL VARIABLES
	
	//The domain name of the server
	char* domain = nullptr;
	//The port of the server
	char* port = nullptr;
	//Used for generating unique ID-s for different connections
	static int net_id;
	//The unique ID of the network
	const int self_id;
};
//-------------------------------------------------------------------------

//A global variable for a singleton network (Not necessary)
extern Network network;

#endif //NETWORK_H
