
#include "stdafx.h"
#ifndef Controllers_h
#define Controllers_h

#ifndef Handle WSA Event

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

	// 2. Login
	string loginAccount(string username, string password, UserInfo* userInfo) {

	// check if account is logged in
	for (int i = 0; i < sizeof(loginSessions); i++) {
		if (!strcmp((loginSessions[i]->userInfo.username).c_str, username.c_str)) return "113";
	}
	// if this account has not logged in
	for (int i = 0; i < sizeof(accounts); i++) {
		if (!strcmp(accounts[i].username.c_str, username.c_str)) {
			if (!strcmp(accounts[i].password.c_str, password.c_str)) { // if username and password matched
																	   // update user info
				userInfo->username = username;
				userInfo->status = 1;
				return "110";
			}
			else return "112";
		}
	}
	return "111";
}

// 3. Register

string registerAccount(string username, string password) {
	for (int i = 0; i < sizeof(accounts); i++) {
		if (!strcmp(accounts[i].username.c_str, username.c_str)) // if username is existed
			return "121";
	}

	// save account to data file

	try
	{
		ofstream file(fileDirectory);
		file << username << " " << password<<"\n";

	}
	catch (const std::exception&)
	{
		cout << "Error at function 3.Register: Save data error\n";
		return "122";
	}
	return "120";
}

// 4. Get list all teams

string getAllTeams(UserInfo* userInfo) {

	// login check
	if (userInfo->status == 0) return "211";

	string response= "210|";
	for (int i = 0; i < sizeof(teams); i++) {
		if (teams[i]->members[0] != NULL) { // if team has linked to team leader
		response += std::to_string(teams[i]->id);
		response += " "+teams[i]->name;
		int numOfMems = 0;
		for (int j = 0; j < 3; j++) {
			if (teams[i]->members[j] != NULL) numOfMems++;
		}
		response += " "+ std::to_string(numOfMems) +"|";
		}
	}
	return response;
}

// 5. Join team

string joinTeam(UserInfo* userInfo, unsigned int teamId) {
	for (int i = 0; i < sizeof(teams); i++) {
		if (teams[i]->id == teamId) {
			if (sizeof(teams[i]->members) < 3) {
				string s = "230|" + userInfo->username;
				char * _s = s.c_str;
				// send join request to team leader
				Send((teams[i]->members)[0]->socketInfo.connSocket, _s,sizeof(_s),0);
				return "220";
			}
			else return "221";
		}
	}
	// teamId not existed
	return "222";
}

//6. create team
string createTeam(LoginSession* userInfo, string teamName) {
	Team newTeam;
	int i;
	for (i = 0; i < MAX_TEAM; i++) {
		// find the team hasnt link to any team leader
		if (teams[i]->members[0] == NULL) {
			newTeam.id = i;
			newTeam.members[0] = userInfo;
			newTeam.name = teamName;
			break;
		}
	}
	if (i >= MAX_TEAM) return "231";
	else return "230";
}

//7. Sign out
string accountSignOut(string username) {
	int i;
	for (i = 0; i < MAX_CLIENT; i++) {
		// find logginsession has same username
		if (!strcmp(loginSessions[i]->userInfo.username.c_str, username.c_str)) {
			// check status
			switch (loginSessions[i]->userInfo.status){
			case 0: return "211";
			case 1: { 
				resetUserInfo(loginSessions[i]);
				return "240";
			};
			case 2: { // room member
				// pop the user out of team 
				for (int j = 1; j < 3; j++) {
					LoginSession* member = teams[loginSessions[i]->userInfo.teamId]->members[j];
					if (member != NULL && ! strcmp(member->userInfo.username.c_str, username.c_str)) {
						teams[loginSessions[i]->userInfo.teamId]->members[j] = NULL;
					}
				}
				resetUserInfo(loginSessions[i]);
				return "240";
			case 3: {

			}
			};
			default:
				break;
			}
		}
	}
	// if this account hasnt logged in
	return "211";
}

void resetUserInfo(LoginSession* loginSession) {
	loginSession->userInfo.coin = 0;
	loginSession->userInfo.HP[0] = 1000;
	loginSession->userInfo.HP[1] = 0;
	loginSession->userInfo.HP[2] = 0;
	loginSession->userInfo.laze[0] = -90;
	loginSession->userInfo.laze[1] = -90;
	loginSession->userInfo.laze[2] = -90;
	loginSession->userInfo.laze[3] = -90;
	loginSession->userInfo.rocket = 0;
	loginSession->userInfo.status = 0;
	loginSession->userInfo.sungtudong[0] = 50;
	loginSession->userInfo.sungtudong[1] = -200;
	loginSession->userInfo.sungtudong[2] = -200;
	loginSession->userInfo.sungtudong[3] = -200;
	loginSession->userInfo.teamId = -1;
	loginSession->userInfo.username = "";
}

#endif // !Handle user statement






#endif 
