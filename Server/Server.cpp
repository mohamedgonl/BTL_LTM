

#include "StructDefination.h"
#include "stdafx.h"

// Define global variables
Account accounts[];
LogginSession* logginSessions[];
Team* teams[];
Room* rooms[];
Question* questions[];



UserInfo acc[MAX_NUM_ACCOUNT];
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
void freeSockInfo(SocketInfo* siArray, int n);

queue<char*> recvStreamProcessing(SocketInfo &userInfo, char buff[BUFF_SIZE]);
/**
* The closeEventInArray function release an event and remove it from an array
* @param	eventArr	An array of event object handles
* @param	n	Index of the removed event object
*/

void closeEventInArray(WSAEVENT* eventArr, int n);

/**
* @function login: Check case in login situation and respond to the corresponding message
*
* @param it: a pointer to client input
* @param clientInfo: a LoginState variable with the user's infomation
*
* return: 10 if login successful
11 if the account is blocked
12 if the account is not available
**/
char* login(char* it, SocketInfo &clientInfo);

/**
* @function logout: The function to logout the user
*
* @param clientInfo: a LoginState variable with the user's infomation
*
* return: 30 if login successful
*
**/
char* logout(SocketInfo &clientInfo);

/**
* @function readfile: Get the list of users and their information and push them to an array
*
* @param pathname: The path of account.txt file
*
* return: Number of account
*
**/
int readfile(string pathname);

/**
* @function splitAccountData: Split inline data with account and status into account and status of the account
*
* @param inlineData: Data to split
* @param del: the delimiter in the inline data
*
* return: account's information with Account type
**/
UserInfo splitAccountData(string inlineData, string del);

/**
* @function splitData: Split inline data into vector of token by delimiter
*
* @param inlineData: Data to split
* @param del: the delimiter in the inline data
*
* return: Vector of token
**/
vector<string> splitData(string inlineData, string del);

/**
* @function checkLogin: check the username and status of the
*
* @param username: the username that the user provides
* @param accounts: The vector of vector of valid account
*
* return: 10 if username is available and is not block
11 if username is available and block
12 if username is not available
**/
int checklogin(string username, UserInfo accounts[], int numOfAccount);

/**
* @function Send: The send() wrapper function
*
**/
int Send(SOCKET s, char *buff, int size, int flags);

/**
* @function Receive: The receive() wrapper function
**/
int Receive(SOCKET s, char *buff, int size, int flags);

/**
* @function workingThread: The function to use in the child process
*
* @param param: The start index of LoginState array that child process manages
**/
unsigned __stdcall workingThread(void* params);

/**
* @function interactWithClient: The function to get user input, handle input and send result to client
*
* @param buff: the data receive from client
* @param clientInfo: a LoginState variable with the user's infomation
*
**/
void interactWithClient(SocketInfo &userInfo, char buff[BUFF_SIZE]);

/**
* @function splitMsg: Split a string into substrings by ending delimiter
*
* @param msg: a pointer to input string
*
* @return: a pointer to link list of string after split
**/
list<char*> splitMsg(char* msg);

/**
* @function isNumber: Check if a string is a number
*
* @param text: A pointer to a text
*
* @return: 0 if string isn't a number
1 if string is a number.
**/
int isNumber(char* text);

/**
* @function handleResponse: handle data before the response to the
*
* @param it: a char pointer to client input
* @param clientInfo: a LoginState variable with the user's infomation
*
* return: Message to send to client after the handle
**/
char* handleResponse(char* it, SocketInfo &clientInfo);


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
			dataThread[i].clientInfo[j].connSocket = 0;
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
					if (dataThread[i].clientInfo[j].connSocket == 0) {
						dataThread[i].clientInfo[j].connSocket = connSock;
						dataThread[i].clientInfo[j].clientPort = ntohs(clientAddr.sin_port);
						strcpy(dataThread[i].clientInfo[j].clientIP, inet_ntoa(clientAddr.sin_addr));
						dataThread[i].events[j] = WSACreateEvent();
						dataThread[i].nEvents++;
						WSAEventSelect(dataThread[i].clientInfo[j].connSocket, dataThread[i].events[j], FD_READ | FD_CLOSE);
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
			WSAEnumNetworkEvents(dataThread[startIndex].clientInfo[index].connSocket, dataThread[startIndex].events[index], &sockEvent);

			int ret;
			char rcvBuff[BUFF_SIZE + 1];
			if (sockEvent.lNetworkEvents & FD_READ) {
				if (sockEvent.iErrorCode[FD_READ_BIT] != 0) {
					printf("FD_READ failed with error %d\n", sockEvent.iErrorCode[FD_READ_BIT]);
					continue;
				}
				ret = Receive(dataThread[startIndex].clientInfo[index].connSocket, rcvBuff, BUFF_SIZE, 0);
				if (ret <= 0) {
					// Close socket
					freeSockInfo(dataThread[startIndex].clientInfo, index);
					closeEventInArray(dataThread[startIndex].events, index);
					EnterCriticalSection(&critical);
					dataThread[startIndex].nEvents--;
					numOfConn--;
					LeaveCriticalSection(&critical);
				}
				else {
					rcvBuff[ret] = 0;
					interactWithClient(dataThread[startIndex].clientInfo[index], rcvBuff);
					WSAResetEvent(dataThread[startIndex].events[index]);
				}
			}

			if (sockEvent.lNetworkEvents & FD_CLOSE) {
				if (sockEvent.iErrorCode[FD_CLOSE_BIT] != 0 && sockEvent.iErrorCode[FD_CLOSE_BIT] != 10053) {
					printf("FD_READ failed with error %d\n", sockEvent.iErrorCode[FD_READ_BIT]);
					continue;
				}

				freeSockInfo(dataThread[startIndex].clientInfo, index);
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



void freeSockInfo(SocketInfo* siArray, int n) {
	closesocket(siArray[n].connSocket);
	for (int i = n; i < MAX_CLIENT_IN_A_THREAD; i++) {
		siArray[i] = siArray[i + 1];
	}
}

void closeEventInArray(WSAEVENT* eventArr, int n) {
	WSACloseEvent(eventArr[n]);
	for (int i = n; i < MAX_CLIENT_IN_A_THREAD; i++)
		eventArr[i] = eventArr[i + 1];
}

queue<char*> recvStreamProcessing(SocketInfo &userInfo, char buff[BUFF_SIZE]) {
	queue<char*> statements;
	list<char*> tokens;
	strcat(userInfo.buff, buff);
	string stringConvert = string(userInfo.buff);
	tokens = splitMsg(userInfo.buff);

	int posOfEndED = stringConvert.length() - strlen(ENDING_DELIMITER);
	if (stringConvert.find(ENDING_DELIMITER, posOfEndED) == posOfEndED) {
		for (list<char*>::iterator it = tokens.begin(); it != tokens.end(); it++) {
			char* temp = (char*)malloc(strlen(*it));
			strcpy(temp, *it);
			statements.push(temp);
		}
		strcpy(userInfo.buff, "");
	}
	// Case ENDING_DELIMITER is not in the end of message
	else {
		for (list<char*>::iterator it = tokens.begin(); it != tokens.end(); it++) {
			// Token is the last element of list of token
			if (it == (--tokens.end())) {
				strcpy(userInfo.buff, *it);
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


void interactWithClient(SocketInfo &userInfo, char buff[BUFF_SIZE]) {
	SOCKET connectedSocket = userInfo.connSocket;
	char* sendData;
	char* statement;
	queue<char*> statements = recvStreamProcessing(userInfo, buff);
	while (!statements.empty()) {
		statement = statements.front();
		statements.pop();
		sendData = handleResponse(statement, userInfo);
		Send(connectedSocket, sendData, strlen(sendData), 0);
	}
}

char* handleResponse(char* it, SocketInfo &clientInfo) {
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


