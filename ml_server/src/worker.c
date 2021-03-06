// Michael Landes
// GaTech : GOS : Project 1
///////////////////////////
#include "globals.h"

#include "worker.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include "safeq.h"
#include "http.h"

/* DATA */
static const int BUFFER_SIZE = 1024;
static char FAILURE_TO_INTERPRET[] = "SERVER failed to interpret your request, closing connection\n";
//static char UNHANDLED_PROTOCOL[] = "SERVER does not implement your protocol, closing connection\n";

/* PRIVATE INTERFACE */
static void processConnection(int, char[]);

/* PUBLIC INTERFACE */
void* ml_worker(void* argument)
{
	//int tid = *((int*)argument);;
	unsigned int hSocket = 0;
	char inBuffer[BUFFER_SIZE];

	//printf("Hello World from thread (%d)\n", tid);

	while(1)
	{
		hSocket = ml_safeq_get();

		//printf("Thread (%d) handling socket (%d)\n", tid, hSocket);

		processConnection(hSocket, inBuffer);
		//usleep(50000);

		// close socket and move on
        if (close(hSocket) == SOCKET_ERROR)
        {
			printf("ERROR: Failed to close the socket\n");
			//return -1; (no error return, simply move on)
        }
		hSocket = 0;
	}

	return NULL;
}

/* IMPLEMENTATION */
static void processConnection(int hSocket, char inBuffer[])
{
	int result;
	char* statusLine;

	// check the socket
	if (hSocket <= 0) return;

	// read the request, if there is no request line, END
	result = ml_http_readLine(hSocket, inBuffer);
	if (result != SUCCESS)
	{
		write(hSocket, FAILURE_TO_INTERPRET, strlen(FAILURE_TO_INTERPRET)+1);
		return;
	}

///	// if protocol is not HTTP, return un-handled protocol message END
///	if (!ml_http_isHTTP(inBuffer))
///	{
///		write(hSocket, UNHANDLED_PROTOCOL, strlen(UNHANDLED_PROTOCOL)+1);
///		return;
///	}
/// removed because the specs ask us to handle request in format: [GET <path>\r\n]

	// call processHTTPRequest()
	statusLine = (char*) malloc (sizeof(char) * (strlen(inBuffer)+1));
	strcpy(statusLine, inBuffer);
	ml_http_processHTTPRequest(hSocket, inBuffer, statusLine);
	free(statusLine);
}
