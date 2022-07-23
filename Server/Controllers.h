#include "stdafx.h"
#include "GlobalVariable.h"
#pragma once
#ifndef Controllers_h
#define Controllers_h

#ifndef Handle WSA Event

void endGame(Team* team);
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

vector<string> splitData(string inlineData, string del)
{
	vector<string> data;
	//int end = inlineData.find(del);
	//data.push_back(inlineData.substr(0, end));
	//data.push_back(inlineData.substr(end + del.size()));
	int start = 0;
	int end = inlineData.find(del);
	while (end != -1) {
		data.push_back(inlineData.substr(start, end - start));
		start = end + del.size();
		end = inlineData.find(del, start);
	}
	data.push_back(inlineData.substr(start, end - start));
	for (int i = 0; i < data.size(); i++) {
		cout << data[i] << endl;
	}
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

int Receive(SOCKET s, char *buff, int size, int flags) {
	int n;

	n = recv(s, buff, size, flags);
	if (n == SOCKET_ERROR) {
		printf("Error: %", WSAGetLastError());
	}
	return n;
}

int Send(SOCKET s, char *buff, int size, int flags) {
	int n;

	n = send(s, buff, size, flags);
	if (n == SOCKET_ERROR) {
		printf("Error: %", WSAGetLastError());
	}

	return n;
}

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


#endif // !Handle WSA Event
#ifndef Handle user statement


/*
1. Handle stream data
*/
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


/*
10. Get list user in waiting room
*/

string getListUserInWaitingRoom(LoginSession &loginSession) {
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return RES_GETUSERS_NOT_TEAM_LEAD;
	}
	else if (loginSession.userInfo.status > 3) {
		return RES_GETUSERS_IN_GAME;
	}
	string list_team = "|";
	cout << list_team << endl;
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (loginSessions[i] != NULL) {
			if (loginSessions[i]->userInfo.status == 1) {
				list_team = list_team + loginSessions[i]->userInfo.username + " ";
			}
		}
	}
	return RES_GETUSERS_SUCCESS + list_team;
}

/*
11. Accept user request to go to our team
*/

string acceptRequestJoinTeam(LoginSession &loginSession, string nameOfRequestUser) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return RES_GETUSERS_NOT_TEAM_LEAD;
	}
	else if (loginSession.userInfo.status > 3) {
		return RES_GETUSERS_IN_GAME;
	}

	// Handle user not available
	int userIndex = -1;
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (loginSessions[i] != NULL) {
			if (loginSessions[i]->userInfo.username == nameOfRequestUser) {
				userIndex = i;
				break;
			}

		}
	}
	if (userIndex == -1) {
		return RES_M_ACCEPT_USER_NOT_ONLINE;
	}

	// Handle user in another room
	if (loginSessions[userIndex]->userInfo.status >= 2) {
		return RES_M_ACCEPT_USER_IN_ANOTHER_ROOM;
	}

	// Handle team is full
	int teamID = loginSession.userInfo.teamId;
	int availableSlotIndex = -1;
	for (int i = 0; i < 3; i++) {
		if (teams[teamID]->members[i] == NULL) {
			availableSlotIndex = i;
			break;
		}
	}
	if (availableSlotIndex == -1) {
		return RES_M_ACCEPT_TEAM_FULL;
	}

	// Handle send response to user request
	string sendBackData = RES_M_ACCEPT_SEND_BACK_TO_USER_REQUEST;
	sendBackData = sendBackData + "|" + to_string(teamID);
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	int ret = Send(loginSessions[userIndex]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return RES_M_ACCEPT_SEND_FAIL;
	}

	// Handle success accept user
	teams[teamID]->members[availableSlotIndex] = loginSessions[userIndex];
	loginSessions[userIndex]->userInfo.status = 2;
	loginSessions[userIndex]->userInfo.teamId = teamID;
	return RES_M_ACCEPT_SUCCESS;
}


/*
12. Decline user request to go to our team
*/

string declineRequestJoinTeam(LoginSession &loginSession, string nameOfRequestUser) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return RES_GETUSERS_NOT_TEAM_LEAD;
	}
	else if (loginSession.userInfo.status > 3) {
		return RES_GETUSERS_IN_GAME;
	}

	// Handle user not available
	int userIndex = -1;
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (loginSessions[i] != NULL) {
			if (loginSessions[i]->userInfo.username == nameOfRequestUser) {
				userIndex = i;
				break;
			}

		}
	}
	if (userIndex == -1) {
		return RES_M_ACCEPT_USER_NOT_ONLINE;
	}

	// Handle user in another room
	if (loginSessions[userIndex]->userInfo.status >= 2) {
		return RES_M_ACCEPT_USER_IN_ANOTHER_ROOM;
	}

	// Handle send response to user request
	string sendBackData = RES_M_DECLINE_SEND_BACK_TO_USER_REQUEST;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	int ret = Send(loginSessions[userIndex]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return RES_M_ACCEPT_SEND_FAIL;
	}

	// Handle success accept user
	return RES_M_DECLINE_SUCCESS;
}

/*
13. Invite user to join room
*/

string inviteJoinTeam(LoginSession &loginSession, string usernameUser) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return RES_GETUSERS_NOT_TEAM_LEAD;
	}
	else if (loginSession.userInfo.status > 3) {
		return RES_GETUSERS_IN_GAME;
	}

	// Handle user not available
	int userIndex = -1;
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (loginSessions[i] != NULL) {
			if (loginSessions[i]->userInfo.username == usernameUser) {
				userIndex = i;
				break;
			}
		}
	}
	if (userIndex == -1) {
		return RES_M_ACCEPT_USER_NOT_ONLINE;
	}

	// Handle user in another room
	if (loginSessions[userIndex]->userInfo.status >= 2) {
		return RES_M_ACCEPT_USER_IN_ANOTHER_ROOM;
	}

	// Handle send response to user request
	string sendBackData = RES_INVITE_SEND_TO_USER;
	sendBackData = sendBackData + "|" + to_string(loginSession.userInfo.teamId);
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	int ret = Send(loginSessions[userIndex]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return RES_M_ACCEPT_SEND_FAIL;
	}

	// Handle success accept user
	return RES_INVITE_SUCCESS;
}


/*
14. Accept invited to join room
*/

string acceptInvitedToJoinTeam(LoginSession &loginSession, int teamID) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status > 2) {
		return RES_M_ACCEPT_USER_IN_ANOTHER_ROOM;
	}


	// Handle team is full
	int availableSlotIndex = -1;
	for (int i = 0; i < 3; i++) {
		if (teams[teamID]->members[i] == NULL) {
			availableSlotIndex = i;
			break;
		}
	}

	if (availableSlotIndex == -1) {
		return RES_M_ACCEPT_TEAM_FULL;
	}

	if (teams[teamID]->status == 1) {
		return RES_TEAM_IN_GAME;
	}

	// Handle send response to user request
	string sendBackData = RES_ACCEPT_INVITE_SEND_TO_LEADER;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	int ret = Send(teams[teamID]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return RES_M_ACCEPT_SEND_FAIL;
	}

	teams[teamID]->members[availableSlotIndex] = &loginSession;
	loginSession.userInfo.status = 2;
	loginSession.userInfo.teamId = teamID;

	// Handle success accept user
	return RES_ACCEPT_SUCCESS;
}


/*
15. Decline invited to join room
*/

string declineInvitedToJoinTeam(LoginSession &loginSession, int teamID) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status > 2) {
		return RES_M_ACCEPT_USER_IN_ANOTHER_ROOM;
	}

	if (teams[teamID]->status == 1) {
		return RES_TEAM_IN_GAME;
	}

	// Handle send response to user request
	string sendBackData = RES_DECLINE_INVITE_SEND_TO_LEADER;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	int ret = Send(teams[teamID]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return RES_M_ACCEPT_SEND_FAIL;
	}
	// Handle success accept user
	return RES_DECLINE_SUCCESS;
}

/*
16. Kick user out room
*/

string kickUserOutRoom(LoginSession &loginSession, string username) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return RES_GETUSERS_NOT_TEAM_LEAD;
	}
	else if (loginSession.userInfo.status > 3) {
		return RES_GETUSERS_IN_GAME;
	}
	int userIndex = -1;
	for (int i = 0; i < 3; i++) {
		if (teams[loginSession.userInfo.teamId]->members[i] != NULL) {
			if (teams[loginSession.userInfo.teamId]->members[i]->userInfo.username == username) {
				userIndex = i;
				break;
			}
		}
	}

	if (userIndex == -1) {
		return RES_USER_NOT_IN_TEAM;
	}

	// Handle send response to user request
	string sendBackData = RES_SEND_KICK_ANNOUNCE;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	int ret = Send(teams[loginSession.userInfo.teamId]->members[userIndex]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return RES_M_ACCEPT_SEND_FAIL;
	}
	teams[loginSession.userInfo.teamId]->members[userIndex]->userInfo.status = 1;
	teams[loginSession.userInfo.teamId]->members[userIndex]->userInfo.teamId = -1;
	teams[loginSession.userInfo.teamId]->members[userIndex] = NULL;

	// Handle success accept user
	return RES_USER_NOT_IN_TEAM;
}

/*
17. Get all teams
*/

string getAllTeams(LoginSession &loginSession) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return RES_GETUSERS_NOT_TEAM_LEAD;
	}
	else if (loginSession.userInfo.status > 3) {
		return RES_GETUSERS_IN_GAME;
	}

	int teamIndex = loginSession.userInfo.teamId;

	string list_team = "|";
	for (int i = 0; i < MAX_TEAM; i++) {
		if (i != teamIndex && teams[i]->name != "") {
			int numOfUser = 0;
			for (int j = 0; j < 3; j++) {
				if (teams[i]->members[j] != NULL) {
					if (teams[i]->members[j]->userInfo.username != "") {
						numOfUser++;
					}

				}
			}
			list_team = list_team + to_string(i) + " " + teams[i]->name + " " + to_string(numOfUser) + "|";
		}
	}
	return 	RES_GETTEAMS_SUCCESS + list_team;
}

/*
18. Challenge
*/

string challenge(LoginSession &loginSession, int enemyTeamId) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return RES_GETUSERS_NOT_TEAM_LEAD;
	}
	else if (loginSession.userInfo.status > 3) {
		return RES_GETUSERS_IN_GAME;
	}

	int teamIndex = loginSession.userInfo.teamId;
	int numOfMember = 0;
	for (int i = 0; i < 3; i++) {
		if (teams[teamIndex]->members[i] != NULL) {
			numOfMember++;
		}
	}

	if (numOfMember < 3) {
		return RES_CHALLENGE_NOT_ENOUGH_MEMBER;
	}

	if (teams[enemyTeamId]->name == "") {
		return RES_CHALLENGE_ENEMY_NOT_EXIST;
	}

	if (teams[enemyTeamId]->status == 1) {
		return RES_CHALLENGE_ENEMY_IN_GAME;
	}

	int numOfEnemyMember = 0;
	for (int i = 0; i < 3; i++) {
		if (teams[enemyTeamId]->members[i] != NULL) {
			numOfEnemyMember++;
		}
	}

	if (numOfEnemyMember < 3) {
		return RES_CHALLENGE_ENEMY_NOT_ENOUGH_MEMBER;
	}

	// Handle send response to user request
	string sendBackData = RES_CHALLENGE_SEND_CHALLENGE_TO_ANOTHER_TEAM;
	sendBackData = sendBackData + "|" + to_string(loginSession.userInfo.teamId);
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	int ret = Send(teams[enemyTeamId]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return RES_M_ACCEPT_SEND_FAIL;
	}
	return RES_CHALLENGE_SUCCESS;
}

/*
19. Accept challenge
*/

string acceptChallenge(LoginSession &loginSession, int enemyTeamId) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return RES_GETUSERS_NOT_TEAM_LEAD;
	}
	else if (loginSession.userInfo.status > 3) {
		return RES_GETUSERS_IN_GAME;
	}

	if (teams[enemyTeamId]->name == "") {
		return RES_CHALLENGE_ENEMY_NOT_EXIST;
	}

	if (teams[enemyTeamId]->status == 1) {
		return RES_CHALLENGE_ENEMY_IN_GAME;
	}


	int teamIndex = loginSession.userInfo.teamId;
	int numOfMember = 0;
	for (int i = 0; i < 3; i++) {
		if (teams[teamIndex]->members[i] != NULL) {
			numOfMember++;
		}
	}

	if (numOfMember < 3) {
		return RES_CHALLENGE_NOT_ENOUGH_MEMBER;
	}

	int numOfEnemyMember = 0;
	for (int i = 0; i < 3; i++) {
		if (teams[enemyTeamId]->members[i] != NULL) {
			numOfEnemyMember++;
		}
	}

	if (numOfEnemyMember < 3) {
		return RES_CHALLENGE_ENEMY_NOT_ENOUGH_MEMBER;
	}

	// Handle send response to user request
	string sendBackData = RES_ACCEPTCHALLENGE_SEND_TO_ALL_USER;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	for (int i = 0; i < 3; i++) {
		teams[enemyTeamId]->members[i]->userInfo.status = 4;
		Send(teams[enemyTeamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	}
	for (int i = 1; i < 3; i++) {
		teams[teamIndex]->members[i]->userInfo.status = 4;
		Send(teams[teamIndex]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	}
	loginSession.userInfo.status = 4;
	int roomIndex = -1;
	for (int i = 0; i < MAX_ROOM; i++) {
		if (rooms[i] == NULL) {
			Room* room = new Room;
			room->id = i;
			room->status = 1;
			room->team1 = teams[teamIndex];
			room->team2 = teams[enemyTeamId];
			rooms[i] = room;
			roomIndex = i;
			EnterCriticalSection(&critical);
			numOfRoom++;
			LeaveCriticalSection(&critical);
			break;
		}
	}

	if (roomIndex != -1) {
		teams[enemyTeamId]->roomId = roomIndex;
		teams[enemyTeamId]->status = 1;
		teams[teamIndex]->roomId = roomIndex;
		teams[teamIndex]->status = 1;
	}

	return RES_ACCEPTCHALLENGE_SUCCESS;
}

/*
20. Decline challenge
*/

string declineChallenge(LoginSession &loginSession, int enemyTeamId) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return RES_GETUSERS_NOT_TEAM_LEAD;
	}
	else if (loginSession.userInfo.status > 3) {
		return RES_GETUSERS_IN_GAME;
	}

	if (teams[enemyTeamId] == NULL) {
		return RES_CHALLENGE_ENEMY_NOT_EXIST;
	}

	if (teams[enemyTeamId]->status == 1) {
		return RES_CHALLENGE_ENEMY_IN_GAME;
	}

	// Handle send response to user request
	string sendBackData = RES_DECLINECHALLENGE_SEND_TO_ANOTHER;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	int ret = Send(teams[enemyTeamId]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return RES_M_ACCEPT_SEND_FAIL;
	}

	return RES_DECLINECHALLENGE_SUCCESS;
}

/*
24. Attack enemy
*/

string attackEnemy(LoginSession &loginSession, string username) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status <= 3) {
		return RES_SURR_NOT_IN_GAME;
	}

	if (loginSession.userInfo.status == 5) {
		return RES_SURR_DEADTH;
	}


	int teamId = loginSession.userInfo.teamId;
	int roomId = teams[teamId]->roomId;

	int enemyTeamId = rooms[roomId]->team1->id == teamId ? rooms[roomId]->team2->id : rooms[roomId]->team1->id;
	int enemyIndex = -1;
	for (int i = 0; i < 3; i++) {
		if (teams[enemyTeamId]->members[i]->userInfo.username == username) {
			enemyIndex = i;
		}
	}
	if (enemyIndex == -1) {
		return RES_ATK_MEMBER_NOT_EXIST;
	}

	if (teams[enemyTeamId]->members[enemyIndex]->userInfo.status == 5) {
		return RES_ATK_MEMBER_HAS_DEATH;
	}

	// get current time and check if last attack < 5s
	clock_t atkTime;
	atkTime = clock();
	if ((float)(atkTime - loginSession.userInfo.lastTimeATK) / CLOCKS_PER_SEC < 5) {
		return RES_ATK_IN_PENDING;
	}
	loginSession.userInfo.lastTimeATK = atkTime;

	// Caculate damage of attack ??????
	int damage = 0;
	


	// Caculate user info
	LoginSession* enemy = teams[enemyTeamId]->members[enemyIndex];
	int i = 2;
	while (damage > 0) {
		if (enemy->userInfo.HP[i] > 0) {
			int reducedHP = damage >= enemy->userInfo.HP[i] ? enemy->userInfo.HP[i] : damage;
			enemy->userInfo.HP[i] -= reducedHP;
			damage -= reducedHP;
		}
		else {
			i--;
		}
	}

	if (enemy->userInfo.HP[0] == 0) {
		enemy->userInfo.status == 5;
		string sendBackData = RES_ATK_SEND_TO_DEATH_MEMBER;
		sendBackData = sendBackData + "|" + loginSession.userInfo.username;
		char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
		strcpy(dataSend, sendBackData.c_str());
		int ret = Send(enemy->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
		if (ret == SOCKET_ERROR) {
			return RES_M_ACCEPT_SEND_FAIL;
		}
		sendBackData = RES_ATK_SEND_TO_ALL_MEMBER_ABOUT_DEATH_MEMBER;
		sendBackData = sendBackData + "|" + loginSession.userInfo.username + " " + enemy->userInfo.username;
		dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
		strcpy(dataSend, sendBackData.c_str());
		for (int i = 0; i < 3; i++) {
			if (i != enemyIndex) {
				Send(teams[enemyTeamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
			}
		}
		for (int i = 0; i < 3; i++) {
			Send(teams[teamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
		}

		int numOfDeathPerson = 0;

		for (int i = 0; i < 3; i++) {
			if (teams[enemyTeamId]->status == 5) {
				numOfDeathPerson++;
			}
		}

		if (numOfDeathPerson == 3) {
			string sendBackData = RES_ATK_WIN;
			sendBackData = sendBackData + "|" + to_string(teamId);
			dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
			strcpy(dataSend, sendBackData.c_str());
			for (int i = 0; i < 3; i++) {
				Send(teams[enemyTeamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
			}
			for (int i = 0; i < 3; i++) {
				if (teams[teamId]->members[i] != &loginSession) {
					Send(teams[teamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
				}
			}
			endGame(teams[teamId]);
			endGame(teams[enemyTeamId]);
			rooms[roomId] = NULL;
			EnterCriticalSection(&critical);
			numOfRoom--;
			LeaveCriticalSection(&critical);
			return sendBackData;
		}

	}
	string sendBackData = RES_ATK_SEND_TO_ALL_MEMBER;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username + " " + teams[enemyTeamId]->members[enemyIndex]->userInfo.username + " " + to_string(damage);
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	for (int i = 0; i < 3; i++) {
		Send(teams[enemyTeamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	}
	for (int i = 0; i < 3; i++) {
		if (teams[teamId]->members[i] != &loginSession) {
			Send(teams[teamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
		}
	}


	sendBackData = RES_ATK_SUCCESS;
	return sendBackData + "|" + loginSession.userInfo.username + " " + teams[enemyTeamId]->members[enemyIndex]->userInfo.username + " " + to_string(damage);
}


/*
26. Create quiz
*/

void createQuestion() {
	int roomHasTravel = 0;
	for (int i = 0; i < MAX_ROOM; i++) {
		if (roomHasTravel == numOfRoom) {
			break;
		}
		else {
			if (rooms[i] != NULL) {
				roomHasTravel++;
				int descriptionQuesionID = (std::rand() % (MAX_QUESTION));
				Question question;
				question.description = &questionDescriptions[descriptionQuesionID];
				for (int j = 0; j < MAX_QUESTION; j++) {
					if (rooms[i]->questions[j].id != -1) {
						question.id = j;
						rooms[i]->questions[j] = question;
						// Handle send response to user request
						string sendBackData = RES_NEW_QUESTION;
						sendBackData += "|" + question.description->question + "|";
						for (int k = 0; k < 4; k++) {
							if (question.description->answers[k] != "") {
								sendBackData += question.description->answers[k] + "|";
							}
						}
						char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
						strcpy(dataSend, sendBackData.c_str());
						for (int k = 0; k < 3; k++) {
							Send(rooms[i]->team1->members[k]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
						}
						for (int k = 0; k < 3; k++) {
							Send(rooms[i]->team2->members[k]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
						}
						break;
					}
				}
			}
		}
	}
}


/*
26. Answer quiz
*/

string answerQuiz(LoginSession &loginSession, int quizId, string key) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status <= 3) {
		return RES_SURR_NOT_IN_GAME;
	}

	if (loginSession.userInfo.status == 5) {
		return RES_SURR_DEADTH;
	}

	int teamID = loginSession.userInfo.teamId;
	int roomID = teams[teamID]->roomId;

	if (rooms[roomID]->questions[quizId].id == -1) {
		return RES_ANS_NOT_EXIST;
	}

	if (rooms[roomID]->questions[quizId].status == 1) {
		return RES_ANS_HAS_ANSWER;
	}

	if (rooms[roomID]->questions[quizId].description->key == key) {
		loginSession.userInfo.coin += rooms[roomID]->questions[quizId].description->coin;
		rooms[roomID]->questions[quizId].status = 1;
		return RES_ANS_CORRECT;
	}
	else {
		return RES_ANS_INCORRECT;
	}
}

/*
27. Surrender match
*/

string surrender(LoginSession &loginSession) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_LOGIN_NOT_LOGIN;
	}
	else if (loginSession.userInfo.status == 1) {
		return RES_GETUSERS_NOT_IN_TEAM;
	}
	else if (loginSession.userInfo.status <= 3) {
		return RES_SURR_NOT_IN_GAME;
	}

	if (loginSession.userInfo.username != teams[loginSession.userInfo.teamId]->members[0]->userInfo.username) {
		return RES_GETUSERS_NOT_TEAM_LEAD;
	}

	if (loginSession.userInfo.status == 5) {
		return RES_SURR_DEADTH;
	}

	int teamIndexUserSurr = loginSession.userInfo.teamId;

	// Handle send response to user request
	string sendBackData = RES_SURR_ANNOUNCE;
	int roomID = teams[teamIndexUserSurr]->roomId;
	int idTeamWin = rooms[teams[teamIndexUserSurr]->roomId]->team1->id == teamIndexUserSurr ? rooms[teams[teamIndexUserSurr]->roomId]->team2->id : rooms[teams[teamIndexUserSurr]->roomId]->team1->id;
	sendBackData = sendBackData + "|" + to_string(idTeamWin);
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());

	endGame(teams[teamIndexUserSurr]);
	endGame(teams[idTeamWin]);

	for (int i = 1; i < 3; i++) {
		Send(teams[teamIndexUserSurr]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	}
	for (int i = 0; i < 3; i++) {
		Send(teams[idTeamWin]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	}

	rooms[roomID] = NULL;

	EnterCriticalSection(&critical);
	numOfRoom--;
	LeaveCriticalSection(&critical);
	return RES_SURR_SUCCESS;
}

/*
28. End game and reset team
*/

void endGame(Team* team) {
	// Handle send response to user request
	team->status = 0;
	team->roomId = -1;
	for (int i = 0; i < 3; i++) {
		team->members[i]->userInfo.coin = 0;
		for (int j = 0; j < 4; j++) {
			team->members[i]->userInfo.laze[j] = -90;
		}
		team->members[i]->userInfo.rocket = 0;
		team->members[i]->userInfo.HP[0] = 1000;
		team->members[i]->userInfo.HP[1] = 0;
		team->members[i]->userInfo.HP[2] = 0;
		team->members[i]->userInfo.sungtudong[0] = 50;
		team->members[i]->userInfo.sungtudong[1] = -200;
		team->members[i]->userInfo.sungtudong[2] = -200;
		team->members[i]->userInfo.sungtudong[3] = -200;
		team->members[i]->userInfo.lastTimeATK = 0;
	}
	team->members[0]->userInfo.status = 3;
	team->members[1]->userInfo.status = 2;
	team->members[2]->userInfo.status = 2;
}
#endif // !Handle user statement






#endif 
