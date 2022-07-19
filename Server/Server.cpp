#include "StructDefination.h"
#include "stdafx.h"




LoginSession acc[MAX_NUM_ACCOUNT];
CRITICAL_SECTION critical;
list<char*> splitMsg(char* msg);
DataThread dataThread[MAX_THREAD];
int numOfAccount = 0;
int numOfThread = 0;
int numOfConn = 0;
int SERVER_PORT = 5500;

/**
* The freeSockInfo function remove a socket from array
* @param	siArray		An array of pointers of socket information struct
* @param	n	Index of the removed socket
*/
void freeSockInfo(LoginSession* siArray, int n);

queue<char*> recvStreamProcessing(LoginSession &loginSession, char buff[BUFF_SIZE]);
void closeEventInArray(WSAEVENT* eventArr, int n);
vector<string> splitData(string inlineData, string del);
int Send(SOCKET s, char *buff, int size, int flags);
int Receive(SOCKET s, char *buff, int size, int flags);
unsigned __stdcall workingThread(void* params);
void interactWithClient(LoginSession &loginSession, char buff[BUFF_SIZE]);
list<char*> splitMsg(char* msg);
int isNumber(char* text);
char* handleResponse(char* it, LoginSession &loginSession);


int main(int argc, char* argv[]) {
	if (argc == 2) {
		if (isNumber(argv[1])) {
			if (atoi(argv[1]) >= 0 && atoi(argv[1]) <= 65535) {
				SERVER_PORT = atoi(argv[1]);
			}
			else {
				printf("Port is in range 0 to 65535, use 5500 as default port!\n");
			}
		}
		else {
			printf("Port must be number, use 5500 as default port!\n");
		}
	}
	else {
		printf("Command line parameter is not valid, use 5500 as default port!\n");
	}
	// Initiate WinSock
	WSADATA wsaData;
	DWORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		printf("Version is not supported\n");
	}

	// Contruct socket
	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET) {
		printf("Error %d: Cannot create server socket.", WSAGetLastError());
		return 0;
	}
	// Bind address to socket
	sockaddr_in serverAddr;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr))) {
		printf("(Error: %d)Cannot associate a local address with server socket.", WSAGetLastError());
		return 0;
	}

	//  Listen request from client
	if (listen(listenSock, 10)) {
		printf("(Error: %d)Cannot place server socket in state LISTEN.\n", WSAGetLastError());
		return 0;
	}

	printf("Server start on port: %d\n", SERVER_PORT);

	// Communicate with client
	SOCKET connSock;
	char buff[BUFF_SIZE + 1];
	sockaddr_in clientAddr;
	int clientAddrLen = sizeof(clientAddr);
	for (int i = 0; i < MAX_THREAD; i++) {
		for (int j = 0; j < MAX_CLIENT_IN_A_THREAD; j++) {
			dataThread[i].loginSession[j].socketInfo.connSocket = 0;
		}
	}

	InitializeCriticalSection(&critical);
	while (1) {
		connSock = accept(listenSock, (sockaddr*)&clientAddr, &clientAddrLen);
		if (connSock == SOCKET_ERROR) {
			printf("Cann't permit incomming connection.\n");
		}
		else {
			int i, j;
			bool isFinded = false;
			cout << "Accept from client: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;
			for (i = 0; i < MAX_THREAD; i++) {
				for (j = 0; j < MAX_CLIENT_IN_A_THREAD; j++) {
					if (dataThread[i].loginSession[j].socketInfo.connSocket == 0) {
						dataThread[i].loginSession[j].socketInfo.connSocket = connSock;
						dataThread[i].loginSession[j].socketInfo.clientPort = ntohs(clientAddr.sin_port);
						strcpy(dataThread[i].loginSession[j].socketInfo.clientIP, inet_ntoa(clientAddr.sin_addr));
						dataThread[i].events[j] = WSACreateEvent();
						dataThread[i].nEvents++;
						WSAEventSelect(dataThread[i].loginSession[j].socketInfo.connSocket, dataThread[i].events[j], FD_READ | FD_CLOSE);
						EnterCriticalSection(&critical);
						numOfConn++;
						LeaveCriticalSection(&critical);
						isFinded = true;
						break;
					}
				}
				if (isFinded) break;
			}
			if (i == MAX_THREAD) {
				printf("Too client connect!\n");
				closesocket(connSock);
			}
			EnterCriticalSection(&critical);
			if (j == 0 && dataThread[i].hasThread == 0) {
				_beginthreadex(0, 0, &workingThread, (void*)i, 0, 0);
				dataThread[i].hasThread = 1;
				numOfThread++;
			}
			LeaveCriticalSection(&critical);
		}
	}
	DeleteCriticalSection(&critical);
	closesocket(listenSock);
	WSACleanup();
	return 0;
}


unsigned __stdcall workingThread(void* params) {
	int startIndex = (int)params;
	WSANETWORKEVENTS sockEvent;
	DWORD index;
	while (1) {
		if (dataThread[startIndex].nEvents > 0) {
			index = WSAWaitForMultipleEvents(dataThread[startIndex].nEvents, dataThread[startIndex].events, FALSE, 1, FALSE);
			if (index == WSA_WAIT_FAILED) {
				printf("Error %d: WSAWaitForMultipleEvents() failed\n", WSAGetLastError());
				continue;
			}

			if (index == WSA_WAIT_TIMEOUT) {
				continue;
			}

			// Chi so chinh xac cua bo bao su kien	
			index = index - WSA_WAIT_EVENT_0;
			WSAEnumNetworkEvents(dataThread[startIndex].loginSession[index].socketInfo.connSocket, dataThread[startIndex].events[index], &sockEvent);
			int ret;
			char rcvBuff[BUFF_SIZE + 1];
			if (sockEvent.lNetworkEvents & FD_READ) {
				if (sockEvent.iErrorCode[FD_READ_BIT] != 0) {
					printf("FD_READ failed with error %d\n", sockEvent.iErrorCode[FD_READ_BIT]);
					continue;
				}
				ret = Receive(dataThread[startIndex].loginSession[index].socketInfo.connSocket, rcvBuff, BUFF_SIZE, 0);
				if (ret <= 0) {
					// Close socket
					freeSockInfo(dataThread[startIndex].loginSession, index);
					closeEventInArray(dataThread[startIndex].events, index);
					EnterCriticalSection(&critical);
					dataThread[startIndex].nEvents--;
					numOfConn--;
					LeaveCriticalSection(&critical);
				}
				else {
					rcvBuff[ret] = 0;
					interactWithClient(dataThread[startIndex].loginSession[index], rcvBuff);
					WSAResetEvent(dataThread[startIndex].events[index]);
				}
			}

			if (sockEvent.lNetworkEvents & FD_CLOSE) {
				if (sockEvent.iErrorCode[FD_CLOSE_BIT] != 0 && sockEvent.iErrorCode[FD_CLOSE_BIT] != 10053) {
					printf("FD_READ failed with error %d\n", sockEvent.iErrorCode[FD_READ_BIT]);
					continue;
				}

				freeSockInfo(dataThread[startIndex].loginSession, index);
				closeEventInArray(dataThread[startIndex].events, index);
				EnterCriticalSection(&critical);
				dataThread[startIndex].nEvents--;
				numOfConn--;
				LeaveCriticalSection(&critical);
			}
		}
		else {
			EnterCriticalSection(&critical);
			dataThread[startIndex].hasThread = 0;
			LeaveCriticalSection(&critical);
			break;
		}
	}
}



void freeSockInfo(LoginSession* siArray, int n) {
	closesocket(siArray[n].socketInfo.connSocket);
	for (int i = n; i < MAX_CLIENT_IN_A_THREAD; i++) {
		siArray[i] = siArray[i + 1];
	}
}

void closeEventInArray(WSAEVENT* eventArr, int n) {
	WSACloseEvent(eventArr[n]);
	for (int i = n; i < MAX_CLIENT_IN_A_THREAD; i++)
		eventArr[i] = eventArr[i + 1];
}

queue<char*> recvStreamProcessing(LoginSession &loginSession, char buff[BUFF_SIZE]) {
	queue<char*> statements;
	list<char*> tokens;
	strcat(loginSession.buff, buff);
	string stringConvert = string(loginSession.buff);
	tokens = splitMsg(loginSession.buff);
	int posOfEndED = stringConvert.length() - strlen(ENDING_DELIMITER);
	if (stringConvert.find(ENDING_DELIMITER, posOfEndED) == posOfEndED) {
		for (list<char*>::iterator it = tokens.begin(); it != tokens.end(); it++) {
			char* temp = (char*)malloc(strlen(*it));
			strcpy(temp, *it);
			statements.push(temp);
		}
		strcpy(loginSession.buff, "");
	}
	// Case ENDING_DELIMITER is not in the end of message
	else {
		for (list<char*>::iterator it = tokens.begin(); it != tokens.end(); it++) {
			// Token is the last element of list of token
			if (it == (--tokens.end())) {
				strcpy(loginSession.buff, *it);
			}
			else {
				char* temp = (char*)malloc(strlen(*it));
				strcpy(temp, *it);
				statements.push(temp);
			}
		}
	}
	return statements;
}


void interactWithClient(LoginSession &loginSession, char buff[BUFF_SIZE]) {
	SOCKET connectedSocket = loginSession.socketInfo.connSocket;
	char* sendData;
	char* statement;
	queue<char*> statements = recvStreamProcessing(loginSession, buff);
	while (!statements.empty()) {
		statement = statements.front();

		statements.pop();
		sendData = handleResponse(statement, loginSession);
		Send(connectedSocket, sendData, strlen(sendData), 0);
	}
}

char* handleResponse(char* it, LoginSession &loginSession) {
	string command = splitData(it, " ")[0];
	if (command != "USER") {
		cout << "You are not login!" << endl;
		return NOT_LOGIN;
	}
	else {
		if (command == "USER") {
			return SUCCESS_LOGIN;
		}
	}
}

vector<string> splitData(string inlineData, string del)
{
	vector<string> data;
	int end = inlineData.find(del);
	data.push_back(inlineData.substr(0, end));
	data.push_back(inlineData.substr(end + del.size()));
	return data;
}


list<char*> splitMsg(char* msg) {
	list<char*> tmp;
	char* token = strtok(msg, ENDING_DELIMITER);
	// loop through the string to extract all other tokens
	while (token != NULL) {
		tmp.push_back(token);
		token = strtok(NULL, ENDING_DELIMITER);
	}
	return tmp;
}

/* The recv() wrapper function */
int Receive(SOCKET s, char *buff, int size, int flags) {
	int n;

	n = recv(s, buff, size, flags);
	if (n == SOCKET_ERROR) {
		printf("Error: %", WSAGetLastError());
	}

	return n;
}

/* The send() wrapper function*/
int Send(SOCKET s, char *buff, int size, int flags) {
	int n;

	n = send(s, buff, size, flags);
	if (n == SOCKET_ERROR) {
		printf("Error: %", WSAGetLastError());
	}

	return n;
}

//Check if string is number
int isNumber(char* text) {
	int j = strlen(text);

	while (j--)
	{
		if (text[j] > 47 && text[j] < 58) {
			continue;
		}
		return 0;
	}
	return 1;
}


