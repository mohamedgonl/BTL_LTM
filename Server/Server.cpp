#include "StructDefination.h"
#include "Controllers.h"
#include "stdafx.h"
#include "FunctionPrototypes.h"
#include "GlobalVariable.h"
#pragma once
int main(int argc, char* argv[]) {
	if (argc == 3) {
		if (isNumber(argv[2])) {
			if (atoi(argv[2]) >= 0 && atoi(argv[2]) <= 65535) {
				SERVER_PORT = atoi(argv[2]);
			}
			else {
				printf("Port is in range 0 to 65535, use 5500 as default port!\n");
			}
		}
		else {
			printf("Port must be number, use 5500 as default port!\n");
		}
		SERVER_ADDR = argv[1];
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

	cout << "Please input the account file directory: ";
	getline(cin, accountFileDirectory);
	numOfAccount = getAccountFromTxtFile(accountFileDirectory);
	cout << "Please input the question file directory: ";
	getline(cin, questionFileDirectory);
	numOfQuestion = getQuestionFromTxtFile(questionFileDirectory);

	printf("Server start on port: %d\n", SERVER_PORT);

	// Communicate with client
	SOCKET connSock;
	char buff[BUFF_SIZE + 1];
	// Init array data
	sockaddr_in clientAddr;

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

unsigned __stdcall sendQuestionThread(void* params) {
	while (true) {
		createQuestion();
		Sleep(30000);
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
					cout << "(Debug) Receive:  " << rcvBuff << endl;;
					interactWithClient(dataThread[startIndex].loginSession[index], rcvBuff);
					WSAResetEvent(dataThread[startIndex].events[index]);
				}
			}

			if (sockEvent.lNetworkEvents & FD_CLOSE) {
				if (sockEvent.iErrorCode[FD_CLOSE_BIT] != 0 && sockEvent.iErrorCode[FD_CLOSE_BIT] != 10053) {
					printf("FD_READ failed with error %d\n", sockEvent.iErrorCode[FD_READ_BIT]);
					continue;
				}
				LoginSession* loginSession = &dataThread[startIndex].loginSession[index];
				// User in team
				if (loginSession->userInfo.teamId != -1) {
					int teamId = loginSession->userInfo.teamId;
					// User is leader
					if (teams[teamId]->members[0] == loginSession) {
						// Team in game
						if (teams[teamId]->roomId != -1) {
							int roomId = teams[teamId]->roomId;
							string sendBackData = SEND_TO_ALL_USERS_WINNER_TEAM_ID;
							int idTeamWin = rooms[roomId]->team1->id == teamId ? rooms[roomId]->team2->id : rooms[roomId]->team1->id;
							sendBackData = sendBackData + "|" + teams[idTeamWin]->name;
							char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
							strcpy(dataSend, sendBackData.c_str());
							cout << "(Debug) Send to all member in team about winner team when lead disconnect: " << dataSend << endl;

							for (int i = 1; i < 3; i++) {
								if (teams[teamId]->members[i] != NULL) {
									Send(teams[teamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
								}
							}
							for (int i = 1; i < 3; i++) {
								if (teams[idTeamWin]->members[i] != NULL) {
									Send(teams[idTeamWin]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
								}
							}
							string sendBackDataToLeader = SEND_TO_TEAM_LEAD_WINNER_TEAM_ID;
							idTeamWin = rooms[roomId]->team1->id == teamId ? rooms[roomId]->team2->id : rooms[roomId]->team1->id;
							sendBackDataToLeader = sendBackDataToLeader + "|" + teams[idTeamWin]->name;
							dataSend = (char*)malloc(sendBackDataToLeader.length() * sizeof(char));
							strcpy(dataSend, sendBackDataToLeader.c_str());
							cout << "(Debug) Send to leader in team about winner team when lead disconnect: " << dataSend << endl;
							endGame(teams[teamId]);
							endGame(teams[idTeamWin]);

							if (teams[teamId]->members[0] != NULL) {
								Send(teams[teamId]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
							}
							if (teams[idTeamWin]->members[0] != NULL) {
								Send(teams[idTeamWin]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
							}

							rooms[roomId] = NULL;

						}
						string sendBackData = SEND_TO_TEAM_DISSOLVE;
						char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
						strcpy(dataSend, sendBackData.c_str());
						cout << "(Debug) Send to all member in team about dissolve: " << dataSend << endl;
						for (int i = 2; i >= 1; i--) {
							if (teams[teamId]->members[i] != NULL) {
								Send(teams[teamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
								teams[teamId]->members[i]->userInfo.status = 1;
								teams[teamId]->members[i]->userInfo.teamId = -1;
							}
						}
						//reset team
						teams[teamId] = NULL;
					}
					else { // not team lead
						// In game
						if (teams[teamId]->roomId != -1) {
							int roomId = teams[teamId]->roomId;
							string sendBackData = SEND_TO_HAS_MEMBER_DISCONNECT;
							sendBackData = sendBackData + "|" + loginSession->userInfo.username;
							char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
							strcpy(dataSend, sendBackData.c_str());
							cout << "(Debug) Send to all member in room about member disconnect: " << dataSend << endl;
							int indexOfLeaveUser = -1;
							for (int i = 0; i < 3; i++) {
								if (teams[teamId]->members[i] == loginSession) {
									indexOfLeaveUser = i;
									break;
								}
							}
							int idTeamOP = rooms[roomId]->team1->id == teamId ? rooms[roomId]->team2->id : rooms[roomId]->team1->id;
							for (int i = 0; i < 3; i++) {
								if (indexOfLeaveUser != i) {
									if (teams[teamId]->members[i] != NULL) {
										Send(teams[teamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
									}
								}
							}
							for (int i = 0; i < 3; i++) {
								if (teams[idTeamOP]->members[i] != NULL) {
									Send(teams[idTeamOP]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
								}
							}
							teams[teamId]->members[indexOfLeaveUser] = NULL;
						}
						else {// Not in game
							string sendBackData = SEND_TO_HAS_MEMBER_DISCONNECT;
							sendBackData = sendBackData + "|" + loginSession->userInfo.username;
							char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
							strcpy(dataSend, sendBackData.c_str());
							cout << "(Debug) Send to all member in team about member disconnect: " << dataSend << endl;
							int indexOfLeaveUser = -1;
							for (int i = 0; i < 3; i++) {
								if (teams[teamId]->members[i] == loginSession) {
									indexOfLeaveUser = i;
									break;
								}
							}
							if (indexOfLeaveUser != -1) {
								teams[teamId]->members[indexOfLeaveUser] = NULL;
							}

							for (int i = 0; i < 3; i++) {
								if (teams[teamId]->members[i] != NULL) {
									Send(teams[teamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
								}
							}
						}
					}
				}
				for (int i = 0; i < MAX_CLIENT; i++) {
					if (loginSessions[i] == loginSession) {
						loginSessions[i] = NULL;
						break;
					}
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
	int ret;
	char rcvBuff[BUFF_SIZE];
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
		return INVALID_COMMAND;
	}
	else {
		string responseData;
		switch (action.find(command)->second) {
		case 2: {
			if (splitData(it, " ").size() == 3) {
				responseData = loginAccount(&loginSession.userInfo, splitData(it, " ")[1], splitData(it, " ")[2]);
				break;
			}
			else {
				responseData = INVALID_COMMAND;
				break;
			}
		}
		case 3: {
			if (splitData(it, " ").size() == 3) {
				responseData = registerAccount(&loginSession.userInfo, splitData(it, " ")[1], splitData(it, " ")[2]);
				break;
			}
			else {
				responseData = INVALID_COMMAND;
				break;
			}
		}
		case 4: {
			responseData = getAllTeams(&loginSession.userInfo);
			break;
		}
		case 5: {
			if (splitData(it, " ").size() == 2) {
				if (isNumber((char*)splitData(it, " ")[1].c_str())) {
					responseData = joinTeam(&loginSession.userInfo, atoi(splitData(it, " ")[1].c_str()));
					break;
				}
				else {
					responseData = INVALID_COMMAND;
					break;

				}
			}
			else {
				responseData = INVALID_COMMAND;
				break;
			}
		}
		case 6: {

			if (splitData(it, " ").size() == 2) {
				responseData = createTeam(&loginSession, splitData(it, " ")[1]);
				break;
			}
			else {
				responseData = INVALID_COMMAND;
				break;

			}
		}
		case 7: {
			responseData = accountSignOut(&loginSession.userInfo);
			break;
		}
		case 8: {
			responseData = getOutTeam(&loginSession.userInfo);
			break;
		}
		case 9: {
			responseData = getTeamMembers(&loginSession.userInfo);
			break;
		}
		case 10: {
			responseData = getListUserInWaitingRoom(loginSession);
			break;
		}
		case 11: {
			if (splitData(it, " ").size() == 2) {
				responseData = acceptRequestJoinTeam(loginSession, splitData(it, " ")[1]);
				break;
			}
			else {
				responseData = INVALID_COMMAND;
				break;

			}
		}
		case 12: {
			if (splitData(it, " ").size() == 2) {
				responseData = declineRequestJoinTeam(loginSession, splitData(it, " ")[1]);
				break;
			}
			else {
				responseData = INVALID_COMMAND;
				break;

			}
		}
		case 13: {
			if (splitData(it, " ").size() == 2) {
				responseData = inviteJoinTeam(loginSession, splitData(it, " ")[1]);
				break;
			}
			else {
				responseData = INVALID_COMMAND;
				break;

			}
		}
		case 14: {
			if (splitData(it, " ").size() == 2) {
				if (isNumber((char*)splitData(it, " ")[1].c_str())) {
					responseData = acceptInvitedToJoinTeam(loginSession, atoi(splitData(it, " ")[1].c_str()));
					break;
				}
				else {
					responseData = INVALID_COMMAND;
					break;

				}
			}
			else {
				responseData = INVALID_COMMAND;
				break;
			}
		}
		case 15: {
			if (splitData(it, " ").size() == 2) {
				if (isNumber((char*)splitData(it, " ")[1].c_str())) {
					responseData = declineInvitedToJoinTeam(loginSession, atoi(splitData(it, " ")[1].c_str()));
					break;
				}
				else {
					responseData = INVALID_COMMAND;
					break;

				}
			}
			else {
				responseData = INVALID_COMMAND;
				break;
			}
		}
		case 16: {
			if (splitData(it, " ").size() == 2) {
				responseData = kickUserOutRoom(loginSession, splitData(it, " ")[1]);
				break;
			}
			else {
				responseData = INVALID_COMMAND;
				break;

			}
		}
		case 17: {
			responseData = getAllTeams(loginSession);
			break;
		}
		case 18: {
			if (splitData(it, " ").size() == 2) {
				if (isNumber((char*)splitData(it, " ")[1].c_str())) {
					responseData = challenge(loginSession, atoi(splitData(it, " ")[1].c_str()));
					break;
				}
				else {
					responseData = INVALID_COMMAND;
					break;

				}
			}
			else {
				responseData = INVALID_COMMAND;
				break;
			}
		}
		case 19: {
			if (splitData(it, " ").size() == 2) {
				if (isNumber((char*)splitData(it, " ")[1].c_str())) {
					responseData = acceptChallenge(loginSession, atoi(splitData(it, " ")[1].c_str()));
					break;
				}
				else {
					responseData = INVALID_COMMAND;
					break;

				}
			}
			else {
				responseData = INVALID_COMMAND;
				break;
			}
		}
		case 20: {
			if (splitData(it, " ").size() == 2) {
				if (isNumber((char*)splitData(it, " ")[1].c_str())) {
					responseData = declineChallenge(loginSession, atoi(splitData(it, " ")[1].c_str()));
					break;
				}
				else {
					responseData = INVALID_COMMAND;
					break;

				}
			}
			else {
				responseData = INVALID_COMMAND;
				break;
			}
		}
		case 21: {
			if (splitData(it, " ").size() == 2) {
				responseData = buyItem(&loginSession.userInfo, splitData(it, " ")[1]);
				break;
			}
			else {
				responseData = INVALID_COMMAND;
				break;

			}
		}
		case 22: {
			responseData = getAllPlayers(&loginSession.userInfo);
			break;
		}
		case 23: {
			responseData = getMine(&loginSession.userInfo);
			break;
		}
		case 24: {
			if (splitData(it, " ").size() == 2) {
				responseData = attackEnemy(loginSession, splitData(it, " ")[1]);
				break;
			}
			else {
				responseData = INVALID_COMMAND;
				break;

			}
		}
		case 26: {
			if (splitData(it, " ").size() == 3) {
				if (isNumber((char*)splitData(it, " ")[1].c_str())) {
					responseData = answerQuiz(loginSession, atoi(splitData(it, " ")[1].c_str()), splitData(it, " ")[2]);
					break;
				}
				else {
					responseData = INVALID_COMMAND;
					break;

				}
			}
			else {
				responseData = INVALID_COMMAND;
				break;
			}
		}
		case 27: {
			responseData = surrender(loginSession);
			break;
		}
		}
		char* returnData = (char*)malloc(responseData.length() * sizeof(char));
		strcpy(returnData, responseData.c_str());
		cout << "(Debug) Response: " << returnData << endl;
		return returnData;
	}
}

