
#include "Network.h"
#include "messageParser.h"
#ifdef __linux__
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#endif
int start(const char* server, const char* port)
{
#ifdef _WIN32
	u_long iMode = 0;
	while (1)
	{
		WSADATA wsaData;
		SOCKET ConnectSocket = INVALID_SOCKET;
		struct addrinfo* result = NULL,
			* ptr = NULL,
			hints;
		char recvbuf[DEFAULT_BUFLEN];
		int iResult;
		unsigned int recvbuflen = DEFAULT_BUFLEN;
		int parsedVersion = 0;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port

		iResult = getaddrinfo(server, port, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			if (ConnectSocket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return 1;
			}

			ioctlsocket(ConnectSocket, FIONBIO, &iMode);

			// Connect to server.
			iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(result);

		// Send an initial buffer
		iResult = send(ConnectSocket, HELO, (int)strlen(HELO), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			return 1;
		}

		// Receive until the peer closes the connection
		do {

			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0)
			{
					parseMessage(recvbuf, iResult);
			}
			else if (iResult == 0)
				printf("Connection closed\n");
			else
				printf("recv failed with error: %d\n", WSAGetLastError());

		} while (iResult > 0);

		// cleanup
		closesocket(ConnectSocket);
		WSACleanup();
		Sleep(5000);
	}
	return 0;
#elif __linux__
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		return errno;
	}
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT 
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(server);
	servaddr.sin_port = htons(atoi(port));

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed\n");
		return errno;
	}

	char buff[1024];
	int n;
	write(sockfd, HELO, sizeof(HELO));
	for (;;) {
		bzero(buff, sizeof(buff));
		n = read(sockfd, buff, sizeof(buff));
		parseMessage(buff, n);
	}
#endif
}

bool sendPacket(const char* server, const char* port, const char* message, unsigned int messagelen, char* recvbuf, unsigned int recvbuflen)
{
#ifdef _WIN32
	unsigned int recvhead = 0;
	u_long iMode = 0;
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	int iResult;
	int parsedVersion = 0;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port

	iResult = getaddrinfo(server, port, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return false;
		}

		ioctlsocket(ConnectSocket, FIONBIO, &iMode);

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	// Send an initial buffer
	//iResult = send(ConnectSocket, HELO, (int)strlen(HELO), 0);
	iResult = send(ConnectSocket, message, messagelen, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return false;
	}

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return false;
	}
	DWORD timeout = 1000;
	setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	recvbuf[0] = 0;
	iResult = recv(ConnectSocket, recvbuf, recvbuflen-1, 0);
	if (iResult > 0)
	{
		recvbuf[iResult] = 0;
	}
	else if (iResult != 0)
		printf("recv failed with error: %d\n", WSAGetLastError());
	closesocket(ConnectSocket);
	WSACleanup();
	return iResult;
#elif __linux__
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		return errno;
	}
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT 
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(server);
	servaddr.sin_port = htons(atoi(port));

	// connect the client socket to server socket 
//	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
	 // Set non-blocking 
   	long arg; 
	int res; 
	fd_set myset;
	struct timeval tv; 
	int valopt; 
	socklen_t lon; 
	bool connected = false;

	 if( (arg = fcntl(sockfd, F_GETFL, NULL)) < 0) { 
	    fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno)); 
		return errno;
	 } 
	 arg |= O_NONBLOCK; 
	 if( fcntl(sockfd, F_SETFL, arg) < 0) { 
	    fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno)); 
		return errno;
	 } 
	 // Trying to connect with timeout 
	 res = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)); 
	 if (res < 0) { 
	    if (errno == EINPROGRESS) { 
	       do { 
	          tv.tv_sec = 1; 
	          tv.tv_usec = 0; 
	          FD_ZERO(&myset); 
	          FD_SET(sockfd, &myset); 
	          res = select(sockfd+1, NULL, &myset, NULL, &tv); 
	          if (res < 0 && errno != EINTR) { 
	             fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); 
	            break; 
	          } 
	          else if (res > 0) { 
	             // Socket selected for write 
	             lon = sizeof(int); 
	             if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) { 
	       	 fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno)); 
	            break; 
	             } 
	             // Check the value returned... 
	             if (valopt) { 
	       	 fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt));
	            break; 
	            } 
		    connected = true;
	            break; 
	         } 
	         else { 
	            fprintf(stderr, "Timeout in select() - Timeout Connecting\n");
	            break; 
	         } 
	      	   sleep(100);
	      } while (1); 
	   } 
	   else { 
	      fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno)); 
	   } 
	}
	if (!connected)
	{
		printf("Not Connected\n");
		close(sockfd);
		return errno;
	}
	
	// Set to blocking mode again... 
	if( (arg = fcntl(sockfd, F_GETFL, NULL)) < 0) { 
	   fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno)); 
	} 
	arg &= (~O_NONBLOCK); 
	if( fcntl(sockfd, F_SETFL, arg) < 0) { 
	   fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno)); 
	} 

	int n;
	write(sockfd, message, messagelen);
	n = read(sockfd, recvbuf, recvbuflen - 1);
	recvbuf[n] = 0;
	close(sockfd);
	return n;
#endif
}
