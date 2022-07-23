#include "StructDefination.h"
#include "Controllers.h"
#include "stdafx.h"
#include "FunctionPrototypes.h"
#include "GlobalVariable.h"
#pragma once
int main1(int argc, char* argv[]) {
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
	// Init array data
	sockaddr_in clientAddr;
	//for (int i = 0; i < MAX_CLIENT; i++) {
	//	LoginSession loginSession = {

	//	};
	//	loginSessions[i] = &loginSession;
	//}
	for (int i = 0; i < MAX_TEAM; i++) {
		Team* team = new Team;
		teams[i] = team;
	}
	for (int i = 0; i < MAX_THREAD; i++) {
		for (int j = 0; j < MAX_CLIENT_IN_A_THREAD; j++) {
			dataThread[i].loginSession[j].socketInfo.connSocket = 0;
		}
	}
	int clientAddrLen = sizeof(clientAddr);
	_beginthreadex(0, 0, &sendQuestionThread, NULL, 0, 0);
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
						for (int k = 0; k < MAX_CLIENT; k++) {
							if (loginSessions[k] == NULL) {
								loginSessions[k] = &dataThread[i].loginSession[j];
								break;
							}
						}
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
int main() {
	LoginSession lgs[9];
	SocketInfo sk[9];
	UserInfo us[9];
	for (int i = 0; i < 9; i++) {
		us[i].username = "long" + to_string(i);
		us[i].status = 1;
		lgs[i].socketInfo = sk[i];
		lgs[i].userInfo = us[i];
		loginSessions[i] = &lgs[i];
	}
	// team0 user 0 1 2
	us[0].status = 2;
	us[1].status = 3;
	us[2].status = 3;
	Team team0;
	team0.id = 0;
	team0.name = "team0";
	for (int i = 0; i < 3; i++) {
		us[i].teamId = 0;
		team0.members[i] = &lgs[i];
	}

	teams[0] = &team0;
	// team1   user 3 4
	us[3].status = 2;
	us[4].status = 3;
	Team team1;
	team1.id = 1;
	team1.name = "team1";
	for (int i = 3; i < 5; i++) {
		us[i].teamId = 1;
	}
	for (int i = 0; i < 2; i++) {
		team1.members[i] = &lgs[i + 3];
	}

	teams[1] = &team1;

	// test 

	cout << getAllTeams(&us[0]) << endl;
	cout << createTeam(&lgs[0], "team56")<<endl;
	cout << createTeam(&lgs[1], "team5") << endl;
	cout << getAllTeams(&us[0]) << endl;
	
}

unsigned __stdcall sendQuestionThread(void* params) {
	while (true) {
		createQuestion();
		Sleep(5000);
	}
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
	if (action.find(command) == action.end()) {
		cout << "Invalid command" << endl;
		return INVALID_COMMAND;
	}
	else {
		switch (action.find(command)->second) {
		case 2: {
			string responseData = loginAccount(&loginSession.userInfo,splitData(it," ")[1],splitData(it," ")[2]);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 3: {
			string responseData = registerAccount(splitData(it," ")[1],splitData(it," ")[2]);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 4: {
			string responseData = getAllTeams(&loginSession.userInfo);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 5: {
			string responseData = joinTeam(&loginSession.userInfo,atoi(splitData(it," ")[1].c_str()));
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 6: {
			string responseData = createTeam(&loginSession, splitData(it," ")[1]);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 7: {
			string responseData = accountSignOut(splitData(it," ")[1]);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 8: {
			string responseData = getOutTeam(&loginSession.userInfo);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 9: {
			string responseData = getTeamMembers(&loginSession.userInfo);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 10: {
			string responseData = getListUserInWaitingRoom(loginSession);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 11: {
			string responseData = acceptRequestJoinTeam(loginSession, splitData(it, " ")[1]);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 12: {
			string responseData = declineRequestJoinTeam(loginSession, splitData(it, " ")[1]);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 13: {
			string responseData = inviteJoinTeam(loginSession, splitData(it, " ")[1]);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 14: {
			string responseData = acceptInvitedToJoinTeam(loginSession, atoi(splitData(it, " ")[1].c_str()));
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 15: {
			string responseData = declineInvitedToJoinTeam(loginSession, atoi(splitData(it, " ")[1].c_str()));
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 16: {
			string responseData = kickUserOutRoom(loginSession, splitData(it, " ")[1]);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 17: {
			string responseData = getAllTeams(loginSession);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 18: {
			string responseData = challenge(loginSession, atoi(splitData(it, " ")[1].c_str()));
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 19: {
			string responseData = acceptChallenge(loginSession, atoi(splitData(it, " ")[1].c_str()));
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 20: {
			string responseData = declineChallenge(loginSession, atoi(splitData(it, " ")[1].c_str()));
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 21: {
			string responseData = buyItem(&loginSession.userInfo, splitData(it, " ")[1]);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 22: {
			string responseData = getAllPlayers(&loginSession.userInfo);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 23: {
			string responseData = getMine(&loginSession.userInfo);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 24: {
			string responseData = attackEnemy(loginSession, splitData(it, " ")[1]);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 26: {
			string responseData = answerQuiz(loginSession, atoi(splitData(it, " ")[1].c_str()), splitData(it, " ")[2]);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		case 27: {
			string responseData = surrender(loginSession);
			char* returnData = (char*)malloc(responseData.length() * sizeof(char));
			strcpy(returnData, responseData.c_str());
			return returnData;
		}
		}
	}
}

