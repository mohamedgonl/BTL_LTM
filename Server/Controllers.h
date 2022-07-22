
#include "stdafx.h"
#include "StructDefination.h"
#include "GlobalVariable.h"
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
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (loginSessions[i]) {
			if (! ((loginSessions[i]->userInfo.username).compare(username))) return "113";
		}
	}
	// if this account has not logged in
	for (int i = 0; i < MAX_NUM_ACCOUNT; i++) {
		if (&accounts[i]) {
			if (!accounts[i].username.compare(username)) {
				if (!accounts[i].password.compare(password)) { // if username and password matched
				// update user info
					userInfo->username = username;
					userInfo->status = 1;
					return "110";
				}
				else return "112";
			}
		}
	}
	return "111";
}

// 3. Register

string registerAccount(string username, string password) {
	for (int i = 0; i < MAX_NUM_ACCOUNT; i++) {
		if (&accounts[i])
			if (!(accounts[i].username.compare(username)) ) // if username is existed
				return "121";
		}
	// save account to data file
	try
	{
		fstream file;
		string account = username + " " + password;
		file.open(fileDirectory,ios::app);
		if (file) {
			file << account << endl;
			file.close();
			return "120";
		}
		else {
			cout << "File not existed";
			file.close();
			return "122";
		}
	}
	catch (const std::exception&)
	{
		cout << "Error at function 3.Register: Save data error\n";
		return "122";
	}
}

// 4. Get list all teams

string getAllTeams(UserInfo* userInfo) {

	// login check
	if (userInfo->status == 0) return "211";

	string response= "210|";
	for (int i = 0; i < MAX_TEAM; i++) {
		if (teams[i]!=NULL && teams[i]->members[0] != NULL) { // if team has linked to team leader
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
	for (int i = 0; i < MAX_TEAM; i++) {
		if (teams[i]!=NULL && teams[i]->id == teamId) {
			// find numbers of member in team
			int numOfMems = 0;
			for (int j = 0; j < 3; j++) {
				if (teams[i]->members[j] != NULL) numOfMems++;
			}
			if (numOfMems < 3) {
				string s = "230|" + userInfo->username;
				// send join request to team leader
				char* _s = (char*)malloc(s.length() * sizeof(char));
				strcpy(_s, s.c_str());
				Send((teams[i]->members)[0]->socketInfo.connSocket, _s , sizeof(_s),0);
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
		if (loginSessions[i] && !strcmp(loginSessions[i]->userInfo.username.c_str(), username.c_str())) {
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
					if (member != NULL && !strcmp(member->userInfo.username.c_str(), username.c_str())) {
						teams[loginSessions[i]->userInfo.teamId]->members[j] = NULL;
					}
				}
				resetUserInfo(loginSessions[i]);
				return "240"; 
			};
			case 3: { // team leader
				// reset team members
				Team* team = teams[loginSessions[i]->userInfo.teamId];
				team->members[1]->userInfo.teamId = -1;
				team->members[2]->userInfo.teamId = -1;
				team->members[1]->userInfo.status = 1;
				team->members[2]->userInfo.status = 1;
				// kick team member
				team->members[1] = NULL;
				team->members[2] = NULL;
				// reset team leader info
				LoginSession* teamLeader = team->members[0];
				resetUserInfo(teamLeader);
				return "240";
			};
			default:
				return "241";
			}
		}
	}
	// if this account hasnt logged in
	return "211";
}
// 8. Get out of team
string getOutTeam(UserInfo* userInfo) {

	if (userInfo->teamId == -1) return "311";
	switch (userInfo->status){
	case 0: {
		return "211";
	}
	case 1: {
		return "311";
	}
	case 2: {
		// kick member out of team
		if (!strcmp(teams[userInfo->teamId]->members[1]->userInfo.username.c_str(), userInfo->username.c_str())) {
			teams[userInfo->teamId]->members[1] = NULL;
		}
		else teams[userInfo->teamId]->members[2] = NULL;
		// reset member status and roomId
		userInfo->status = 1;
		userInfo->teamId = -1;
		return "310";
	}
	case 3: {// team leader
		// update member info and kick out of team
		Team* team = teams[userInfo->teamId];
		team->members[1]->userInfo.status = 1;
		team->members[1]->userInfo.teamId = -1;
		team->members[2]->userInfo.status = 1;
		team->members[2]->userInfo.teamId = -1;
		for (int i = 0; i < 3; i++) {
			team->members[i] = NULL;
		}
		//update user info
		userInfo->status = 1;
		userInfo->teamId = -1;
		return "310";
	}
	case 4: 
	case 5: {
		return "312";
	};
	default:
		cout << "Error at func 8.Get out of team: Status undefined\n";
		break;
	}
}

// 9. Get team members
string getTeamMembers(UserInfo* userInfo) {
	if (userInfo->status == 0) return "211";
	if (userInfo->status == 1 || userInfo->teamId == -1) return "311";
	
	Team* team = teams[userInfo->teamId];
	string response = "320|";
	for (int i = 0; i < MAX_TEAM; i++) {
		LoginSession* mem = team->members[i];
		if (mem != NULL) {
			response += mem->userInfo.username+" ";
		}
	}
	return response;
}
// 10. Get players are free
string getFreePlayers(UserInfo* userInfo) {
	if (userInfo->status == 0) return "221";
	if (userInfo->status == 1) return "311";
	if (userInfo->status == 2) return "331";
	if (userInfo->status != 3) return "312";
	else {
		string response = "330|";
		for (int i = 0; i < MAX_CLIENT; i++) {
			if (loginSessions[i]->userInfo.status == 1) {
				response += loginSessions[i]->userInfo.username + " ";
			}
		}
		return response;
	}
}
// 21. Buy item

string buyItem(string item, UserInfo* userInfo) {
	switch (userInfo->status){
	case 0: return "221";
	case 1: return "311";
	case 2:
	case 3: return "413";
	case 5: return "414";
	case 4: {

	}
	default:
		cout << "Error at 21. Buy item function\n";
		break;
	}
}

// 22. Get info of all players in the game
string getAllPlayers(UserInfo* userInfo) {
	if (userInfo->status == 0) return "221";
	if (userInfo->status == 1) return "311";
	if (userInfo->status == 2 || userInfo->status == 3) return "413";

	// get two team info
	Room* room = rooms[teams[userInfo->teamId]->roomId];
	Team* team1 = room->team1;
	Team* team2 = room->team2;
	string response = "420|Team 1:\n";
	// get member username and hp 
	//team1
	for (int i = 0; i < 3; i++) {
		response += team1->members[i]->userInfo.username+ " ";
		for (int j = 0; j < 3; j++) {
			response += to_string(team1->members[i]->userInfo.HP[j]) + " ";
		}
		response += "\n";	
	}
	//team2;
	response += "Team2:\n";
	for (int i = 0; i < 3; i++) {
		response += team2->members[i]->userInfo.username + " ";
		for (int j = 0; j < 3; j++) {
			response += to_string(team2->members[i]->userInfo.HP[j]) + " ";
		}
	}
	return response;
}

// 23. Get my ingame-info
string getMine(UserInfo* userInfo) {
	if (userInfo->status == 0) return "221";
	if (userInfo->status == 1) return "311";
	if (userInfo->status == 2 || userInfo->status == 3) return "413";

	string response = "430|";
	// get hp and armor
	for (int i = 0; i < 3; i++) {
		response += to_string(userInfo->HP[i]) + " ";
	}
	// get sungtudong
	for (int i = 0; i < 4; i++) {
		response += to_string(userInfo->sungtudong[i]) + " ";
	}
	// get laze 
	for (int i = 0; i < 4; i++) {
		response += to_string(userInfo->laze[i]) + " ";
	}
	// get rocket
	response += to_string(userInfo->rocket) + " ";
	// get coin
	response += to_string(userInfo->coin);
	return response;
}

#endif // !Handle user statement







#endif 
