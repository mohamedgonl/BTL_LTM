#include "stdafx.h"
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
	int start = 0;
	int end = inlineData.find(del);
	while (end != -1) {
		data.push_back(inlineData.substr(start, end - start));
		start = end + del.size();
		end = inlineData.find(del, start);
	}
	data.push_back(inlineData.substr(start, end - start));
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
		//printf("Error: %", WSAGetLastError());
	}
	return n;
}

int Send(SOCKET s, char *buff, int size, int flags) {
	int n;

	n = send(s, buff, size, flags);
	if (n == SOCKET_ERROR) {
		//printf("Error: %", WSAGetLastError());
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

vector<string> readFile(string pathname) {
	// read account
	vector<string> inlineData;
	string line;
	ifstream myfile(pathname);
	int count = 0;
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line != "") {
				inlineData.push_back(line);
			}
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file. Please confirm your path to account.txt file./n";
	}
	return inlineData;
}


int getAccountFromTxtFile(string pathname) {
	int count = 0;
	vector<string> inlineData;
	inlineData = readFile(pathname);
	for (int i = 0; i < inlineData.size(); i++) {
		vector<string> accountData = splitData(inlineData[i], " ");
		Account account;
		account.username = accountData[0];
		account.password = accountData[1];
		accounts[count] = account;
		count++;
	}
	return count;
}

int getQuestionFromTxtFile(string pathname) {
	int count = 0;
	vector<string> inlineData;
	inlineData = readFile(pathname);
	for (int i = 0; i < inlineData.size(); i++) {
		vector<string> questionData = splitData(inlineData[i], "|");
		QuestionDescription questionDescription;
		questionDescription.question = questionData[0];
		questionDescription.key = questionData[1];
		questionDescription.coin = atoi(questionData[2].c_str());
		int numOfAnswer = 0;
		for (int i = 3; i < questionData.size(); i++) {
			questionDescription.answers[numOfAnswer] = questionData[i];
			numOfAnswer++;
		}
		questionDescriptions[count] = questionDescription;
		count++;
	}
	return count;
}

#endif // !Handle WSA Event

#ifndef Handle user statement
void endGame(Team* team) {
	// Handle send response to user request
	team->status = 0;
	team->roomId = -1;
	for (int i = 0; i < 3; i++) {
		if (team->members[i] != NULL) {
			team->members[i]->userInfo.coin = 0;
			for (int j = 0; j < 4; j++) {
				team->members[i]->userInfo.laze[j] = -90;
			}
			team->members[i]->userInfo.rocket = 0;
			team->members[i]->userInfo.HP[0] = 1000;
			team->members[i]->userInfo.HP[1] = 0;
			team->members[i]->userInfo.HP[2] = 0;
			team->members[i]->userInfo.autogun[0] = 50;
			team->members[i]->userInfo.autogun[1] = -200;
			team->members[i]->userInfo.autogun[2] = -200;
			team->members[i]->userInfo.autogun[3] = -200;
			team->members[i]->userInfo.lastTimeATK = 0;
		}
	}
	if (team->members[0] != NULL) {
		team->members[0]->userInfo.status = 3;
	}
	if (team->members[1] != NULL) {
		team->members[1]->userInfo.status = 2;
	}
	if (team->members[2] != NULL) {
		team->members[2]->userInfo.status = 2;
	}
}
void resetUserInfo(UserInfo* userInfo) {
	userInfo->coin = 0;
	userInfo->HP[0] = 1000;
	userInfo->HP[1] = 0;
	userInfo->HP[2] = 0;
	userInfo->laze[0] = -90;
	userInfo->laze[1] = -90;
	userInfo->laze[2] = -90;
	userInfo->laze[3] = -90;
	userInfo->rocket = 0;
	userInfo->status = 0;
	userInfo->autogun[0] = 50;
	userInfo->autogun[1] = -200;
	userInfo->autogun[2] = -200;
	userInfo->autogun[3] = -200;
	userInfo->teamId = -1;
	userInfo->username = "";
	userInfo->lastTimeATK = 0;
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
string loginAccount(UserInfo* userInfo, string username, string password) {
	if (userInfo->status >= 1) {
		return USER_LOGINNED;
	}

	// check if account is logged in
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (loginSessions[i]) {
			if (!((loginSessions[i]->userInfo.username).compare(username))) return ACC_IN_ANOTHER_SESSION;
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
					return LOGIN_SUCCESS;
				}
				else return WRONG_PASSWORD;
			}
		}
	}
	return ACC_INVALID;
}

// 3. Register
string registerAccount(UserInfo* userInfo, string username, string password) {

	//! đã đăng nhập 
	if (userInfo->status != 0) return USER_LOGINNED;
	else {
		for (int i = 0; i < MAX_NUM_ACCOUNT; i++) {
			if (&accounts[i])
				if (!(accounts[i].username.compare(username))) // if username is existed
					return USERNAME_EXISTED;
		}
		// save new account 
		try
		{
			// push new account to accounts
			for (int i = 0; i < MAX_NUM_ACCOUNT; i++) {
				if (!accounts[i].username.compare("")) {
					accounts[i].username = username;
					accounts[i].password = password;
					break;
				}
			}
			// save to data file
			fstream file;
			string account = username + " " + password;
			file.open(accountFileDirectory, ios::app);
			if (file) {
				file << account << endl;
				file.close();
				return SIGNUP_SUCCESS;
			}
			else {
				cout << "File not existed";
				file.close();
				return SERVER_ERROR;
			}
		}
		catch (const std::exception&)
		{
			cout << "Error at function 3.Register: Save data error\n";
			return SERVER_ERROR;
		}
	}


}

// 4. Get list all teams
string getAllTeams(UserInfo* userInfo) {
	// login check
	if (userInfo->status == 0) return RES_NOT_AUTHORIZE;
	// in a game
	if (userInfo->status == 4 || userInfo->status == 5) return USER_IN_GAME;
	string response = RES_GETTEAMS_SUCCESS;
	response += "|";
	for (int i = 0; i < MAX_TEAM; i++) {
		if (teams[i] != NULL) {
			if (teams[i]->id != -1) {
				response += std::to_string(teams[i]->id);
				response += " " + teams[i]->name;
				int numofMems = 0;
				for (int j = 0; j < 3; j++) {
					if (teams[i]->members[j]) numofMems++;
				}
				response += " " + std::to_string(numofMems) + "|";
			}
		}
	}
	return response;
}

// 5. Join team
string joinTeam(UserInfo* userInfo, unsigned int teamId) {
	//login check
	if (userInfo->status == 0) return RES_NOT_AUTHORIZE;
	// in a team
	if (userInfo->status == 2 || userInfo->status == 3) return USER_ALREADY_INTEAM;
	// in a game
	if (userInfo->status == 4 || userInfo->status == 5) return USER_IN_GAME;

	for (int i = 0; i < MAX_TEAM; i++) {
		if (teams[i] != NULL) {
			if (teams[i]->id == teamId) {
				int numofMems = 0;
				for (int j = 0; j < 3; j++) {
					if (teams[i]->members[j]) numofMems++;
				}
				if (numofMems < 3) { // team member max 
					// Handle send response to user request
					string sendBackData = SEND_TO_INVITATION_JOIN_TEAM;
					sendBackData = sendBackData + "|" + userInfo->username;
					char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
					strcpy(dataSend, sendBackData.c_str());
					cout << "(Debug) Send to user invitation: " << dataSend << endl;
					int ret = Send(teams[teamId]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
					if (ret == SOCKET_ERROR) {
						return REQUEST_FAIL;
					}
					for (int i = 0; i < MAX_CLIENT; i++) {
						if (teams[teamId]->userRequestJoinTeam[i] == "") {
							teams[teamId]->userRequestJoinTeam[i] = userInfo->username;
							break;
						}
					}
					return SEND_REQUEST_JOINTEAM_SUCCESS;
				}
				else return TEAM_IS_FULL;
			}
		}
	}
	// team not existed
	return TEAM_INVALID;
}

//6. create team
string createTeam(LoginSession* logginSession, string teamName) {
	//login check
	if (logginSession->userInfo.status == 0) return RES_NOT_AUTHORIZE;
	// in a team
	if (logginSession->userInfo.status == 2 || logginSession->userInfo.status == 3) return USER_ALREADY_INTEAM;
	// in a game
	if (logginSession->userInfo.status == 4 || logginSession->userInfo.status == 5) return USER_IN_GAME;

	int firstBlank = -1;
	for (int i = 0; i < MAX_TEAM; i++) {
		if (firstBlank == -1 && teams[i] == NULL) {
			firstBlank = i;
		}
		if (teams[i] != NULL) {
			if (teams[i]->name == teamName) {
				return TEAM_NAME_ALREADY_EXIST;
			}
		}

	}

	if (firstBlank != -1) {
		Team* newTeam = new Team;
		newTeam->id = firstBlank;
		newTeam->members[0] = logginSession;
		newTeam->name = teamName;
		teams[firstBlank] = newTeam;
		logginSession->userInfo.teamId = firstBlank;
		logginSession->userInfo.status = 3;
		string res = CREATE_TEAM_SUCCESS;
		res += "|" + to_string(firstBlank);
		return res;
	}
	else {
		return NUMBER_OF_TEAM_LIMIT;
	}
}

//7. Sign out
string accountSignOut(UserInfo* userInfo) {
	//login check
	if (userInfo->status == 0) return RES_NOT_AUTHORIZE;
	// in a team
	if (userInfo->status == 2 || userInfo->status == 3) return USER_ALREADY_INTEAM;
	// in a game
	if (userInfo->status == 4 || userInfo->status == 5) return USER_IN_GAME;

	// reset user info
	resetUserInfo(userInfo);
	return LOGOUT_SUCCESS;
}

// 8. Get out of team
string getOutTeam(UserInfo* userInfo) {

	if (userInfo->teamId == -1) return NOT_IN_A_TEAM;
	switch (userInfo->status) {
	case 0: {
		return RES_NOT_AUTHORIZE;
	}
	case 1: {
		return NOT_IN_A_TEAM;
	}
	case 2: {// team member
			 // member out of team
		int teamIndex = userInfo->teamId;
		for (int i = 1; i <= 2; i++) {
			if (teams[teamIndex]->members[i] != NULL) {
				if (teams[teamIndex]->members[i]->userInfo.username == userInfo->username) {
					teams[teamIndex]->members[i] = NULL;
					break;
				}
			}
		}

		string sendBackData = SEND_TO_HAS_MEMBER_LEAVE;
		sendBackData = sendBackData + "|" + userInfo->username;
		char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
		strcpy(dataSend, sendBackData.c_str());

		cout << "(Debug) Send to member in team: " << dataSend << endl;
		for (int i = 0; i < 3; i++) {
			if (teams[teamIndex]->members[i] != NULL) {
				Send(teams[teamIndex]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
			}
		}
		// reset member status and roomId
		userInfo->status = 1;
		userInfo->teamId = -1;
		return LEAVE_TEAM_SUCCESS;
	}
	case 3: {// team leader
			 // find team  and reset members
		int teamIndex = userInfo->teamId;
		string sendBackData = SEND_TO_TEAM_DISSOLVE;
		char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
		strcpy(dataSend, sendBackData.c_str());

		cout << "(Debug) Send to member in team: " << dataSend << endl;
		for (int i = 2; i >= 0; i--) {
			if (teams[teamIndex]->members[i] != NULL) {
				if (i != 0) {
					Send(teams[teamIndex]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
				}
				teams[teamIndex]->members[i]->userInfo.status = 1;
				teams[teamIndex]->members[i]->userInfo.teamId = -1;
				teams[teamIndex]->members[i] = NULL;

			}
		}

		//reset team
		teams[teamIndex] = NULL;
		return LEAVE_TEAM_SUCCESS;
	}
	case 4: {
		return USER_IN_GAME;
	}
	case 5: {
		return USER_IN_GAME;
	};
	default:
		cout << "Error at func 8.Get out of team: Status undefined\n";
		break;
	}
}

// 9. Get team members
string getTeamMembers(UserInfo* userInfo) {
	//login check
	if (userInfo->status == 0) return RES_NOT_AUTHORIZE;
	// in a team
	if (userInfo->status == 1) return NOT_IN_A_TEAM;
	// in a game
	if (userInfo->status == 4 || userInfo->status == 5) return USER_IN_GAME;

	Team* team = teams[userInfo->teamId];
	if (!team) return TEAM_INVALID;
	else {
		string response = GET_TEAMMBER_SUCCESS;
		response += "|";
		for (int i = 0; i < 3; i++) {
			if (team->members[i]) {
				LoginSession* mem = team->members[i];
				if (mem != NULL) {
					response += mem->userInfo.username + " ";
				}
			}
		}
		return response;
	}
}


/*
10. Get list user in waiting room
*/
string getListUserInWaitingRoom(LoginSession &loginSession) {
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return USER_IS_NOT_HOST;
	}
	else if (loginSession.userInfo.status > 3) {
		return USER_IN_GAME;
	}
	string list_team = "|";
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (loginSessions[i] != NULL) {
			if (loginSessions[i]->userInfo.status == 1) {
				list_team = list_team + loginSessions[i]->userInfo.username + " ";
			}
		}
	}
	return GETUSERS_IN_WAITINGROOM_SUCCESS + list_team;
}

/*
11. Accept user request to go to our team
*/
string acceptRequestJoinTeam(LoginSession &loginSession, string nameOfRequestUser) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return USER_IS_NOT_HOST;
	}
	else if (loginSession.userInfo.status > 3) {
		return USER_IN_GAME;
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
		return USER_NOT_ONLINE;
	}

	// Handle user in another room
	if (loginSessions[userIndex]->userInfo.status >= 2) {
		return USER_IN_ANOTHER_TEAM;
	}

	// Handle team is full
	int teamID = loginSession.userInfo.teamId;
	int indexOfArray = -1;
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (teams[teamID]->userRequestJoinTeam[i] == nameOfRequestUser) {
			indexOfArray = i;
			break;
		}
	}

	if (indexOfArray == -1) {
		return USER_NEVER_REQUEST_JOIN_TEAM;
	}

	int availableSlotIndex = -1;
	for (int i = 0; i < 3; i++) {
		if (teams[teamID]->members[i] == NULL) {
			availableSlotIndex = i;
			break;
		}
	}
	if (availableSlotIndex == -1) {
		return TEAM_IS_FULL;
	}

	// Handle send response to user request
	string sendBackData = SEND_TO_JOIN_TEAM_SUCCESS;
	sendBackData = sendBackData + "|" + to_string(teamID);
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	cout << "(Debug) Send to user want to join team: " << dataSend << endl;
	int ret = Send(loginSessions[userIndex]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return REQUEST_FAIL;
	}

	// Handle success accept user
	teams[teamID]->members[availableSlotIndex] = loginSessions[userIndex];
	loginSessions[userIndex]->userInfo.status = 2;
	loginSessions[userIndex]->userInfo.teamId = teamID;
	teams[teamID]->userRequestJoinTeam[indexOfArray] = "";
	return ACCEPT_USER_JOIN_TEAM_SUCCESS;
}

/*
12. Decline user request to go to our team
*/
string declineRequestJoinTeam(LoginSession &loginSession, string nameOfRequestUser) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return USER_IS_NOT_HOST;
	}
	else if (loginSession.userInfo.status > 3) {
		return USER_IN_GAME;
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
		return USER_NOT_ONLINE;
	}

	// Handle user in another room
	if (loginSessions[userIndex]->userInfo.status >= 2) {
		return USER_IN_ANOTHER_TEAM;
	}

	int indexOfArray = -1;
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (teams[loginSession.userInfo.teamId]->userRequestJoinTeam[i] == nameOfRequestUser) {
			indexOfArray = i;
			break;
		}
	}

	if (indexOfArray == -1) {
		return USER_NEVER_REQUEST_JOIN_TEAM;
	}
	// Handle send response to user request
	string sendBackData = SEND_TO_JOIN_TEAM_FAIL;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	cout << "(Debug) Send to user want to join team: " << dataSend << endl;
	strcpy(dataSend, sendBackData.c_str());
	int ret = Send(loginSessions[userIndex]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return REQUEST_FAIL;
	}

	// Handle success accept user
	teams[loginSession.userInfo.teamId]->userRequestJoinTeam[indexOfArray] = "";
	return DENY_JOIN_TEAM_SUCCESS;
}

/*
13. Invite user to join room
*/
string inviteJoinTeam(LoginSession &loginSession, string usernameUser) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return USER_IS_NOT_HOST;
	}
	else if (loginSession.userInfo.status > 3) {
		return USER_IN_GAME;
	}

	int numOfMember = 0;
	for (int i = 0; i < 2; i++) {
		if (teams[loginSession.userInfo.teamId]->members[i] != NULL) {
			numOfMember++;
		}
	}

	if (numOfMember == 3) {
		return TEAM_IS_FULL;
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
		return USER_NOT_ONLINE;
	}

	// Handle user in another room
	if (loginSessions[userIndex]->userInfo.status >= 2) {
		return USER_IN_ANOTHER_TEAM;
	}

	// Handle send response to user request
	string sendBackData = SEND_TO_RECEIVE_INVITATION_SUCCESS;
	sendBackData = sendBackData + "|" + to_string(loginSession.userInfo.teamId);
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	cout << "(Debug) Send to user invitation: " << dataSend << endl;
	int ret = Send(loginSessions[userIndex]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return REQUEST_FAIL;
	}

	for (int i = 0; i < MAX_CLIENT; i++) {
		if (teams[loginSession.userInfo.teamId]->userInviteJoinTeam[i] == "") {
			teams[loginSession.userInfo.teamId]->userInviteJoinTeam[i] = usernameUser;
			break;
		}

	}

	// Handle success accept user
	return INVITE_USER_JOINTEAM_SUCCESS;
}

/*
14. Accept invited to join room
*/
string acceptInvitedToJoinTeam(LoginSession &loginSession, int teamID) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status > 2) {
		return USER_ALREADY_INTEAM;
	}

	if (teams[teamID] == NULL) {
		return TEAM_INVALID;
	}

	int indexInArray = -1;
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (teams[teamID]->userInviteJoinTeam[i] == loginSession.userInfo.username) {
			indexInArray = i;
		}
	}

	if (indexInArray == -1) {
		return USER_NEVER_HAS_INVITATION;
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
		return TEAM_IS_FULL;
	}

	if (teams[teamID]->status == 1) {
		return TEAM_IN_GAME;
	}

	// Handle send response to user request
	string sendBackData = SEND_TO_ACCEPT_INVITATION;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	cout << "(Debug) Send to lead of team: " << dataSend << endl;
	int ret = Send(teams[teamID]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return REQUEST_FAIL;
	}

	teams[teamID]->userInviteJoinTeam[indexInArray] = "";
	teams[teamID]->members[availableSlotIndex] = &loginSession;
	teams[teamID]->userInviteJoinTeam[indexInArray] = "";
	loginSession.userInfo.status = 2;
	loginSession.userInfo.teamId = teamID;

	// Handle success accept user
	string responseData = USER_ACCEPT_JOINTEAM_SUCCESS;
	responseData = responseData + "|" + to_string(teamID);
	return  responseData;
}

/*
15. Decline invited to join room
*/
string declineInvitedToJoinTeam(LoginSession &loginSession, int teamID) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status >= 4) {
		return USER_IN_GAME;
	}

	if (teams[teamID]->status == 1) {
		return TEAM_IN_GAME;
	}

	int indexInArray = -1;
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (teams[teamID]->userInviteJoinTeam[i] == loginSession.userInfo.username) {
			indexInArray = i;
		}
	}

	if (indexInArray == -1) {
		return USER_NEVER_HAS_INVITATION;
	}

	// Handle send response to user request
	string sendBackData = SEND_TO_HOST_USER_REFUSE_INVITATION;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	cout << "(Debug) Send to lead of team: " << dataSend << endl;
	int ret = Send(teams[teamID]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return REQUEST_FAIL;
	}
	teams[teamID]->userInviteJoinTeam[indexInArray] = "";
	// Handle success accept user
	return USER_REFUSE_JOINTEAM_SUCCESS;
}

/*
16. Kick user out room
*/
string kickUserOutRoom(LoginSession &loginSession, string username) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return USER_IS_NOT_HOST;
	}
	else if (loginSession.userInfo.status > 3) {
		return USER_IN_GAME;
	}
	if (loginSession.userInfo.username == username) {
		return YOU_CAN_NOT_KICK_YOU;
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
		return USER_WANT_TOKICK_NOT_INTEAM;
	}

	// Handle send response to user request
	string sendBackData = SEND_TO_USER_KICKED;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	cout << "(Debug) Send to user has been kicked: " << dataSend << endl;
	int ret = Send(teams[loginSession.userInfo.teamId]->members[userIndex]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return REQUEST_FAIL;
	}
	teams[loginSession.userInfo.teamId]->members[userIndex]->userInfo.status = 1;
	teams[loginSession.userInfo.teamId]->members[userIndex]->userInfo.teamId = -1;
	teams[loginSession.userInfo.teamId]->members[userIndex] = NULL;

	// Handle success accept user
	return KICK_USER_SUCCESS;
}

/*
17. Get all teams
*/
string getAllTeams(LoginSession &loginSession) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return USER_IS_NOT_HOST;
	}
	else if (loginSession.userInfo.status > 3) {
		return USER_IN_GAME;
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
	return 	GET_ALL_TEAMS + list_team;
}

/*
18. Challenge
*/
string challenge(LoginSession &loginSession, int enemyTeamId) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return USER_IS_NOT_HOST;
	}
	else if (loginSession.userInfo.status > 3) {
		return USER_IN_GAME;
	}

	int teamIndex = loginSession.userInfo.teamId;
	int numOfMember = 0;
	for (int i = 0; i < 3; i++) {
		if (teams[teamIndex]->members[i] != NULL) {
			numOfMember++;
		}
	}


	if (teamIndex == enemyTeamId) {
		return YOU_CAN_NOT_CHALLENGE_YOURSELF;
	}

	if (numOfMember < 3) {
		return TEAM_LACK_MEMBER;
	}

	if (teams[enemyTeamId] == NULL) {
		return OPONENT_INVALID;
	}

	if (teams[enemyTeamId]->status == 1) {
		return OPONENT_IN_GAME;
	}

	for (int i = 0; i < MAX_TEAM; i++) {
		if (teams[teamIndex]->teamInviteToChallenge[i] == -1) {
			teams[teamIndex]->teamInviteToChallenge[i] = i;
			break;
		}
	}

	int numOfEnemyMember = 0;
	for (int i = 0; i < 3; i++) {
		if (teams[enemyTeamId]->members[i] != NULL) {
			numOfEnemyMember++;
		}
	}

	if (numOfEnemyMember < 3) {
		return OPONPENT_LACK_MEMBER;
	}

	// Handle send response to user request
	string sendBackData = SEND_TO_OPONENT_CHALLENGE;
	sendBackData = sendBackData + "|" + to_string(loginSession.userInfo.teamId);
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	cout << "(Debug) Send to opponent team invitation to challenge: " << dataSend << endl;
	int ret = Send(teams[enemyTeamId]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return REQUEST_FAIL;
	}
	return CHALLENGE_SUCCESS;
}

/*
19. Accept challenge
*/
string acceptChallenge(LoginSession &loginSession, int enemyTeamId) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return USER_IS_NOT_HOST;
	}
	else if (loginSession.userInfo.status > 3) {
		return USER_IN_GAME;
	}

	if (teams[enemyTeamId] == NULL) {
		return OPONENT_INVALID;
	}

	if (teams[enemyTeamId]->status == 1) {
		return OPONENT_IN_GAME;
	}

	int indexInArray = -1;
	for (int i = 0; i < MAX_TEAM; i++) {
		if (teams[enemyTeamId]->teamInviteToChallenge[i] == loginSession.userInfo.teamId) {
			indexInArray = i;
			break;
		}
	}

	if (indexInArray == -1) {
		return YOUR_TEAM_HAS_NOT_BEEN_CHALLENGE;
	}

	int teamIndex = loginSession.userInfo.teamId;
	int numOfMember = 0;
	for (int i = 0; i < 3; i++) {
		if (teams[teamIndex]->members[i] != NULL) {
			numOfMember++;
		}
	}

	if (numOfMember < 3) {
		return TEAM_LACK_MEMBER;
	}

	int numOfEnemyMember = 0;
	for (int i = 0; i < 3; i++) {
		if (teams[enemyTeamId]->members[i] != NULL) {
			numOfEnemyMember++;
		}
	}

	if (numOfEnemyMember < 3) {
		return OPONPENT_LACK_MEMBER;
	}

	// Handle send response to user request
	string sendBackData = SEND_TO_ALL_USERS_MATCHGAME;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	cout << "(Debug) Send to all member: " << dataSend << endl;
	for (int i = 0; i < 3; i++) {
		if (teams[enemyTeamId]->members[i] != NULL) {
			Send(teams[enemyTeamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
			teams[enemyTeamId]->members[i]->userInfo.status = 4;

		}
	}
	for (int i = 1; i < 3; i++) {
		if (teams[teamIndex]->members[i] != NULL) {
			teams[teamIndex]->members[i]->userInfo.status = 4;
			Send(teams[teamIndex]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
		}
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
	teams[enemyTeamId]->teamInviteToChallenge[indexInArray] = 0;
	for (int i = 0; i < MAX_TEAM; i++) {
		if (teams[teamIndex]->teamInviteToChallenge[i] != -1) {
			if (teams[teams[teamIndex]->teamInviteToChallenge[i]] != NULL) {
				if (teams[teams[teamIndex]->teamInviteToChallenge[i]]->status == 0) {
					string sendBackData = SEND_TO_HOST_OPONENT_REFUSE;
					sendBackData = sendBackData + "|" + loginSession.userInfo.username;
					char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
					strcpy(dataSend, sendBackData.c_str());
					cout << "(Debug) Send to lead of opponent team: " << dataSend << endl;
					Send(teams[teams[teamIndex]->teamInviteToChallenge[i]]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
				}

			}
			teams[teamIndex]->teamInviteToChallenge[i] = -1;
		}

	}
	for (int i = 0; i < MAX_TEAM; i++) {
		if (teams[enemyTeamId]->teamInviteToChallenge[i] != -1) {
			if (teams[teams[enemyTeamId]->teamInviteToChallenge[i]] != NULL) {
				if (teams[teams[enemyTeamId]->teamInviteToChallenge[i]]->status == 0) {
					string sendBackData = SEND_TO_HOST_OPONENT_REFUSE;
					sendBackData = sendBackData + "|" + teams[enemyTeamId]->members[0]->userInfo.username;
					char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
					strcpy(dataSend, sendBackData.c_str());
					cout << "(Debug) Send to lead of opponent team: " << dataSend << endl;
					Send(teams[teams[enemyTeamId]->teamInviteToChallenge[i]]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
				}
			}
			teams[enemyTeamId]->teamInviteToChallenge[i] = -1;
		}

	}
	string responseData = MATCHING_GAME_SUCCESS;
	responseData = responseData + "|" + to_string(enemyTeamId);
	return responseData;
}

/*
20. Decline challenge
*/
string declineChallenge(LoginSession &loginSession, int enemyTeamId) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status == 2) {
		return USER_IS_NOT_HOST;
	}
	else if (loginSession.userInfo.status > 3) {
		return USER_IN_GAME;
	}

	if (teams[enemyTeamId] == NULL) {
		return OPONENT_INVALID;
	}

	int indexInArray = -1;
	for (int i = 0; i < MAX_TEAM; i++) {
		if (teams[enemyTeamId]->teamInviteToChallenge[i] == loginSession.userInfo.teamId) {
			indexInArray = i;
			break;
		}
	}

	if (indexInArray == -1) {
		return YOUR_TEAM_HAS_NOT_BEEN_CHALLENGE;
	}

	if (teams[enemyTeamId]->status == 1) {
		return OPONENT_IN_GAME;
	}

	// Handle send response to user request
	string sendBackData = SEND_TO_HOST_OPONENT_REFUSE;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	cout << "(Debug) Send to lead of opponent team: " << dataSend << endl;
	int ret = Send(teams[enemyTeamId]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	if (ret == SOCKET_ERROR) {
		return REQUEST_FAIL;
	}

	teams[enemyTeamId]->teamInviteToChallenge[indexInArray] = 0;
	return REFUSE_CHALLENGE_SUCCESS;
}

// 21. Buy item
string buyItem(UserInfo* userInfo, string item) {
	switch (userInfo->status) {
	case 0: return RES_NOT_AUTHORIZE;
	case 1: return NOT_IN_A_TEAM;
	case 2: return MEMBER_NOT_INGAME;
	case 3: return MEMBER_NOT_INGAME;
	case 5: return MEMBER_IS_DIE_INGAME;
	case 4: {
		unsigned int* coin = &(userInfo->coin);
		if (*coin == 0) return LACK_MONEY;
		else
			switch (itemsMap.find(item)->second) {
			case 1:// HP 
			{
				int hp = userInfo->HP[0];

				if (hp == MAX_HP) return EXCEED_MAX_ITEM;
				else
				{
					int hp_need = MAX_HP - hp;
					if (hp_need <= *coin) {
						userInfo->HP[0] = MAX_HP;
						userInfo->coin -= MAX_HP - hp;
					}
					else {
						userInfo->HP[0] += *coin;
						userInfo->coin = 0;
					}
				}
				break;
			}
			case 2://bArmor
			{
				int bArmor = userInfo->HP[1];
				if (bArmor == MAX_B_ARMOR) return EXCEED_MAX_ITEM;
				else if (*coin >= Armor[1].price) {
					userInfo->HP[1] = MAX_B_ARMOR;
					userInfo->coin -= Armor[1].price;
				}
				else return LACK_MONEY;
				break;
			}
			case 3://aAmor 
			{
				int aArmor = userInfo->HP[2];
				if (aArmor == MAX_A_ARMOR) return EXCEED_MAX_ITEM;
				else if (*coin >= Armor[2].price) {
					userInfo->HP[2] = MAX_A_ARMOR;
					userInfo->coin -= Armor[2].price;
				}
				else return LACK_MONEY;
				break;
			}
			case 4://autogun
			{
				// full all 4 autoguns
				if (userInfo->autogun[0] == MAX_AUTO_GUN && userInfo->autogun[1] == MAX_AUTO_GUN && userInfo->autogun[2] == MAX_AUTO_GUN&& userInfo->autogun[3] == MAX_AUTO_GUN) return EXCEED_MAX_ITEM;
				// not enough money to buy bullet
				else if (*coin < Attack[0].b_price) return LACK_MONEY;

				else
					// find a new autogun or bullets can buy
					for (int i = 0; i < 4; i++) {
						int a_gun = userInfo->autogun[i];

						if (a_gun >= MAX_AUTO_GUN) continue; // already buy autogun and bullets is full
						else if (a_gun < 0) {	 // not buy autogun
							if (*coin >= Attack[0].price) { // enough money 
								userInfo->autogun[i] = MAX_AUTO_GUN;
								userInfo->coin -= Attack[0].price;
							}
						}
						else if (a_gun < MAX_AUTO_GUN && a_gun >= 0) { // already buy autogun but bulles not full
							if (*coin >= Attack[0].b_price) { // enough money
								userInfo->autogun[i] = MAX_AUTO_GUN;
								userInfo->coin -= Attack[0].b_price;
							}
							else	// when not even enough money to buy bullets -> cant buy anything -> break
								break;
						}
					}
				break;
			}
			case 5://laze
			{
				// 4 lazes full
				if (userInfo->laze[0] == MAX_LAZE && userInfo->laze[1] == MAX_LAZE &&userInfo->laze[2] == MAX_LAZE &&userInfo->laze[3] == MAX_LAZE) return EXCEED_MAX_ITEM;
				// not enough money to buy bullets
				else if (*coin < Attack[1].b_price) return LACK_MONEY;

				else {
					for (int i = 0; i < 4; i++) {
						int laze = userInfo->laze[i];
						if (laze >= MAX_LAZE) continue; // already buy laze and bullets is full
						else if (laze < 0) {	 // not buy laze
							if (*coin >= Attack[1].price) { // enough money 
								userInfo->laze[i] = MAX_LAZE;
								userInfo->coin -= Attack[1].price;
							}
						}
						else if (laze < MAX_LAZE && laze >= 0) { // already buy laze but bulles not full
							if (*coin >= Attack[1].b_price) { // enough money
								userInfo->laze[i] = MAX_LAZE;
								userInfo->coin -= Attack[1].b_price;
							}
							else	// when not even enough money to buy bullets -> cant buy anything -> break
								break;
						}
					}
				}

				break;
			}
			case 6:// rocket
			{
				// full 2 rockets
				if (userInfo->rocket == MAX_ROCKET) return EXCEED_MAX_ITEM;
				else if (*coin < Attack[2].b_price) return LACK_MONEY;
				else {
					unsigned int numCanBuy = *coin >= 2 * Attack[2].b_price ? 2 : 1;
					userInfo->rocket = numCanBuy;
					userInfo->coin -= numCanBuy* Attack[2].b_price;
				}
				break;
			}
			default:
				cout << "ERROR at buy item\n";
				return INVALID_ITEM;
				break;
			}
		return BUY_ITEM_SUCCESS;
		// switch case 4 end
	}
	default:
		cout << "Error at 21. Buy item function\n";
		break;
	}
}

// 22. Get info of all players in the game
string getAllPlayers(UserInfo* userInfo) {
	if (userInfo->status == 0) return RES_NOT_AUTHORIZE;
	if (userInfo->status == 1) return NOT_IN_A_TEAM;
	if (userInfo->status == 2 || userInfo->status == 3) return MEMBER_NOT_INGAME;

	// get two team info
	Room* room = rooms[teams[userInfo->teamId]->roomId];
	Team* team1 = room->team1;
	Team* team2 = room->team2;
	string response = GET_INFO_USERS_INGAME;
	// get member username and hp 
	//team1
	for (int i = 0; i < 3; i++) {
		response += "|";
		response += team1->members[i]->userInfo.username + " ";
		for (int j = 0; j < 3; j++) {
			response += to_string(team1->members[i]->userInfo.HP[j]) + " ";
		}
	}
	//team2;
	for (int i = 0; i < 3; i++) {
		response += "|";
		response += team2->members[i]->userInfo.username + " ";
		for (int j = 0; j < 3; j++) {
			response += to_string(team2->members[i]->userInfo.HP[j]) + " ";
		}
	}
	return response;
}

// 23. Get my ingame-info
string getMine(UserInfo* userInfo) {
	if (userInfo->status == 0) return RES_NOT_AUTHORIZE;
	if (userInfo->status == 1) return NOT_IN_A_TEAM;
	if (userInfo->status == 2 || userInfo->status == 3) return MEMBER_NOT_INGAME;

	string response = GET_PERSIONAL_INFO;
	response += "|";
	// get hp and armor
	for (int i = 0; i < 3; i++) {
		response += to_string(userInfo->HP[i]) + " ";
	}
	// get sungtudong
	for (int i = 0; i < 4; i++) {
		response += to_string(userInfo->autogun[i]) + " ";
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

/*
24. Attack enemy
*/
string attackEnemy(LoginSession &loginSession, string username) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status <= 3) {
		return MEMBER_NOT_INGAME;
	}

	if (loginSession.userInfo.status == 5) {
		return USER_IS_DEAD;
	}

	int teamId = loginSession.userInfo.teamId;
	int roomId = teams[teamId]->roomId;

	int enemyTeamId = rooms[roomId]->team1->id == teamId ? rooms[roomId]->team2->id : rooms[roomId]->team1->id;
	int enemyIndex = -1;
	for (int i = 0; i < 3; i++) {
		if (teams[enemyTeamId]->members[i] != NULL) {
			if (teams[enemyTeamId]->members[i]->userInfo.username == username) {
				enemyIndex = i;
				break;
			}

		}
	}
	if (enemyIndex == -1) {
		return INVALID_OPONENT;
	}

	if (teams[enemyTeamId]->members[enemyIndex]->userInfo.status == 5) {
		return OPONENT_IS_DEAD;
	}

	// get current time and check if last attack < 5s
	clock_t atkTime;
	atkTime = clock();
	if ((float)(atkTime - loginSession.userInfo.lastTimeATK) / CLOCKS_PER_SEC < 5) {
		return LOADING_BULLET;
	}
	loginSession.userInfo.lastTimeATK = atkTime;

	// Caculate damage of attack ??????
	int damage = 0;

	for (int i = 3; i >= 0; i--) {
		if (loginSession.userInfo.autogun[i] > 0) {
			damage += Attack[0].dameB;
			loginSession.userInfo.autogun[i]--;
			break;
		}
	}

	for (int i = 3; i >= 0; i--) {
		if (loginSession.userInfo.laze[i] > 0) {
			damage += Attack[1].dameB;
			loginSession.userInfo.laze[i]--;
			break;
		}
	}

	if (loginSession.userInfo.rocket > 0) {
		damage += Attack[2].dameB;
		loginSession.userInfo.rocket--;
	}

	int tempDamage = damage;

	// Caculate user info
	LoginSession* enemy = teams[enemyTeamId]->members[enemyIndex];
	if (enemy != NULL) {
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
			string sendBackData = SEND_TO_DEAD_USER_WHO_SHOT;
			sendBackData = sendBackData + "|" + loginSession.userInfo.username;
			char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
			strcpy(dataSend, sendBackData.c_str());
			cout << "(Debug) Send to member die: " << dataSend << endl;
			int ret = Send(enemy->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
			if (ret == SOCKET_ERROR) {
				return REQUEST_FAIL;
			}
			sendBackData = SEND_TO_ALL_USERS_WHO_DEADS;
			sendBackData = sendBackData + "|" + loginSession.userInfo.username + " " + enemy->userInfo.username;
			dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
			strcpy(dataSend, sendBackData.c_str());
			cout << "(Debug) Send to all member about member die: " << dataSend << endl;
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
				if (teams[enemyTeamId]->members[i] != NULL) {
					if (teams[enemyTeamId]->members[i]->userInfo.status == 5) {
						numOfDeathPerson++;
					}

				}
				else {
					numOfDeathPerson++;
				}
			}

			if (numOfDeathPerson == 3) {
				string sendBackData = SEND_TO_ALL_USER_WINTEAMS;
				sendBackData = sendBackData + "|" + to_string(teamId);
				dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
				strcpy(dataSend, sendBackData.c_str());
				cout << "(Debug) Send to all member in room about win: " << dataSend << endl;
				for (int i = 0; i < 3; i++) {
					if (teams[enemyTeamId]->members[i] != NULL) {
						Send(teams[enemyTeamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
					}
				}
				for (int i = 0; i < 3; i++) {
					if (teams[teamId]->members[i] != NULL) {
						if (teams[teamId]->members[i] != &loginSession) {
							Send(teams[teamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
						}

					}
				}
				string sendBackDataToLeader = SEND_TO_LEADER_WINTEAMS;
				sendBackDataToLeader = sendBackDataToLeader + "|" + to_string(teamId);
				dataSend = (char*)malloc(sendBackDataToLeader.length() * sizeof(char));
				strcpy(dataSend, sendBackDataToLeader.c_str());
				cout << "(Debug) Send to leader about win: " << dataSend << endl;
				if (teams[enemyTeamId]->members[0] != NULL) {
					Send(teams[enemyTeamId]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
				}
				if (teams[teamId]->members[0] != NULL&&teams[teamId]->members[0] != &loginSession) {
					Send(teams[teamId]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);

				}
				endGame(teams[teamId]);
				endGame(teams[enemyTeamId]);
				rooms[roomId] = NULL;
				EnterCriticalSection(&critical);
				numOfRoom--;
				LeaveCriticalSection(&critical);
				if (teams[teamId]->members[0] == &loginSession) {
					return sendBackDataToLeader;
				}
				else {
					return sendBackData;
				}
			}

		}
	}
	string sendBackData = SEND_TO_ALL_USERS_DAMAGE_OF_ATTACK;
	sendBackData = sendBackData + "|" + loginSession.userInfo.username + " " + teams[enemyTeamId]->members[enemyIndex]->userInfo.username + " " + to_string(tempDamage);
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	cout << "(Debug) Send to all member in room about attack: " << dataSend << endl;
	for (int i = 0; i < 3; i++) {
		if (teams[enemyTeamId]->members[i] != NULL) {
			Send(teams[enemyTeamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
		}
	}
	for (int i = 0; i < 3; i++) {
		if (teams[teamId]->members[i] != NULL) {
			if (teams[teamId]->members[i] != &loginSession) {
				Send(teams[teamId]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
			}
		}
	}


	return sendBackData;
}

/*
25. Create quiz
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
				int descriptionQuesionID = (std::rand() % (numOfQuestion));
				Question question;
				question.description = &questionDescriptions[descriptionQuesionID];
				for (int j = 0; j < MAX_QUESTION; j++) {
					if (rooms[i]->questions[j].id != -1) {
						question.id = j;
						rooms[i]->questions[j] = question;
						// Handle send response to user request
						string sendBackData = SEND_TO_ALL_USERS_QUIZ;
						sendBackData += "|" + question.description->question + "|";
						for (int k = 0; k < 4; k++) {
							if (question.description->answers[k] != "") {
								sendBackData += question.description->answers[k] + "|";
							}
						}
						char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
						strcpy(dataSend, sendBackData.c_str());
						cout << "(Debug) Send to all member in room about quiz: " << dataSend << endl;
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
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status <= 3) {
		return MEMBER_NOT_INGAME;
	}

	if (loginSession.userInfo.status == 5) {
		return USER_IS_DEAD;
	}

	int teamID = loginSession.userInfo.teamId;
	int roomID = teams[teamID]->roomId;

	if (rooms[roomID]->questions[quizId].id == -1) {
		return QUIZ_INVALID;
	}

	if (rooms[roomID]->questions[quizId].status == 1) {
		return NOT_FASTEST_ANSWER;
	}

	if (rooms[roomID]->questions[quizId].description->key == key) {
		loginSession.userInfo.coin += rooms[roomID]->questions[quizId].description->coin;
		rooms[roomID]->questions[quizId].status = 1;
		return ANSWER_TRUE_AND_FASTEST;
	}
	else {
		return WRONG_ANSWER;
	}
}

/*
27. Surrender match
*/
string surrender(LoginSession &loginSession) {
	// Pre handle
	if (loginSession.userInfo.status == 0) {
		return RES_NOT_AUTHORIZE;
	}
	else if (loginSession.userInfo.status == 1) {
		return NOT_IN_A_TEAM;
	}
	else if (loginSession.userInfo.status <= 3) {
		return MEMBER_NOT_INGAME;
	}

	if (loginSession.userInfo.username != teams[loginSession.userInfo.teamId]->members[0]->userInfo.username) {
		return USER_IS_NOT_HOST;
	}

	if (loginSession.userInfo.status == 5) {
		return USER_IS_DEAD;
	}

	int teamIndexUserSurr = loginSession.userInfo.teamId;

	// Handle send response to user request
	string sendBackData = SEND_TO_ALL_USERS_WINNER_TEAM_ID;
	int roomID = teams[teamIndexUserSurr]->roomId;
	int idTeamWin = rooms[teams[teamIndexUserSurr]->roomId]->team1->id == teamIndexUserSurr ? rooms[teams[teamIndexUserSurr]->roomId]->team2->id : rooms[teams[teamIndexUserSurr]->roomId]->team1->id;
	sendBackData = sendBackData + "|" + teams[idTeamWin]->name;
	char* dataSend = (char*)malloc(sendBackData.length() * sizeof(char));
	strcpy(dataSend, sendBackData.c_str());
	cout << "(Debug) Send to all member in room about surrered: " << dataSend << endl;

	for (int i = 1; i < 3; i++) {
		Send(teams[teamIndexUserSurr]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	}
	for (int i = 1; i < 3; i++) {
		Send(teams[idTeamWin]->members[i]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	}
	string sendBackDataToTeamLead = SEND_TO_TEAM_LEAD_WINNER_TEAM_ID;
	sendBackDataToTeamLead = sendBackDataToTeamLead + "|" + teams[idTeamWin]->name;
	dataSend = (char*)malloc(sendBackDataToTeamLead.length() * sizeof(char));
	strcpy(dataSend, sendBackDataToTeamLead.c_str());
	cout << "(Debug) Send to team lead about surrered: " << dataSend << endl;
	endGame(teams[teamIndexUserSurr]);
	endGame(teams[idTeamWin]);

	if (teams[idTeamWin]->members[0] != NULL) {
		Send(teams[idTeamWin]->members[0]->socketInfo.connSocket, dataSend, strlen(dataSend), 0);
	}

	rooms[roomID] = NULL;

	EnterCriticalSection(&critical);
	numOfRoom--;
	LeaveCriticalSection(&critical);
	return SURRENDER_SUCCESS;
}


#endif // !Handle user statement

#endif 
