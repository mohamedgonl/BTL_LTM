int isNumber(string text) {
	int j;
	j = text.length();
	while (j--)
	{
		if (text[j] > 47 && text[j] < 58) {
			continue;
		}
		return 0;
	}
	return 1;
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


int showMenu() {
	string userInput;
	cout << "================ Menu ================" << endl;
	cout << "1. Login " << endl;
	cout << "2. Post article " << endl;
	cout << "3. Logout " << endl;
	cout << "Please select your options [1,2,3]: ";
	cin >> userInput;
	if (isNumber(userInput)) {
		return stoi(userInput);
	}
	else {
		return 0;
	}
}

int status = 0;
int idTeam = 0;
string listTeamInvite;
class Team {
public:
	string team_name;
	int id_team;
	int number;
};
vector<Team> t;
vector<string> splitTeam;
void handleSplitStr(string s)
{
	stringstream stream(s);
	string word;
	while (getline(stream, word, '|')) {
		stringstream ss(word);
		string tmp;
		while (getline(ss, tmp, ' ')) {
			splitTeam.push_back(tmp);
		}
	}
	for (int i = 0; i < splitTeam.size() - 2; i += 3) {
		Team team;
		team.id_team = stoi(splitTeam[i]);
		team.team_name = splitTeam[i + 1];
		team.number = stoi(splitTeam[i + 2]);
		t.push_back(team);
	}
	cout << "team:" << t.size() << endl;
}


SOCKET global;

void handleResponse(char* res) {
	char subbuff[4];
	memcpy(subbuff, &res[0], 3);
	subbuff[3] = '\0';
	switch (atoi(subbuff)) {
	case RES_LOGIN_SUCCESS: {
		if (status == 0) {
			status = 1;
			cout << "Login successful!" << endl;
		}
		break;
	}
	case RES_SIGNUP_SUCCESS: {
		cout << "Sign up successful!" << endl;
		break;
	}
	case LOGOUT_SUCCESS: {
		cout << "Sign out success!" << endl;
		status = 0;
		break;
	}
	case INVALID_COMMAND: {
		cout << "Command is incorrect!" << endl;
		break;
	}
	case RES_ACC_INVALID: {
		cout << "Acount is invalid" << endl;
		break;
	}
	case RES_WRONG_PASSWORD: {
		cout << "Password is incorrect" << endl;
		break;
	}
	case RES_ACC_IN_ANOTHER_SESSION: {
		cout << "Account is in another session" << endl;
		break;
	}
	case RES_USERNAME_EXISTED: {
		cout << "Username is existed!" << endl;
		break;
	}
	case RES_GETTEAMS_SUCCESS: {
		if (status == 1) {
			cout << "Get teams successfully" << endl;
			handleSplitStr(res + 4);
			cout << setw(5) << left << "ID";
			cout << setw(30) << left << "Team'name";
			cout << setw(20) << right << "Team'members" << endl;
			cout << setfill('-');
			cout << setw(55) << "-" << endl;
			cout << setfill(' ');
			for (Team team : t) {
				cout << setw(5) << left << team.id_team;
				cout << setw(30) << left << team.team_name;
				cout << setw(20) << right << team.number << endl;
			}
		}
		break;
	}
	case RES_NOT_AUTHORIZE: {
		cout << "You are unauthorized" << endl;
		break;
	}
	case NOT_IN_A_TEAM: {
		cout << "You are not in any team!" << endl;
		break;
	}
	case SEND_REQUEST_JOINTEAM_SUCCESS: {
		cout << "Send request to join team successfully" << endl;
		break;
	}
	case TEAM_IS_FULL: {
		cout << "Team is full" << endl;
		break;
	}
	case REQUEST_JOINTEAM_IS_DENIED: {
		cout << "Request to server fail!" << endl;
		break;
	}
	case CREATE_TEAM_SUCCESS: {
		cout << "Create team success" << endl;
		status = 3;
		break;
	}
	case  NUMBER_OF_TEAM_LIMIT: {
		cout << "Number of team exceeds limit" << endl;
		break;
	}
	case USER_IN_GAME_7: {
		cout << "User is in game!" << endl;
		break;
	}
	case LEAVE_TEAM_SUCCESS: {
		cout << "Leave team successfully" << endl;
		status = 0;
		break;
	}

	case USER_IN_GAME_8: {
		cout << "User in game!" << endl;
		break;
	}
	case GET_TEAMMBER_SUCCESS: {
		cout << "Get members team successfully!" << endl;
		if (status == 0) {
			cout << "Team member is:" << res + 4 << endl;
		}
	}
	case GETUSERS_IN_WAITINGROOM_SUCCESS: {
		if (status == 3) {
			cout << "List user in waitting room: " << res + 4 << endl;
		}
		break;
	}

//11
	case ACCEPT_USER_JOIN_TEAM_SUCCESS: {
		if (status == 3) {
			cout << "Send accept request success!" << endl;
		}
		break;
	}
	case USER_NOT_ONLINE: {
		if (status == 3) {
			cout << "This user is not online!" << endl;
		}
		break;
	}
	case USER_IN_ANOTHER_TEAM: {
		if (status == 3) {
			cout << "This user is on another team!" << endl;
		}
		break;
	}
	case USER_NOT_HOSTTEAM: {
		cout << "You are not a leader!" << endl;
		break;
	}
	case TEAM_IS_ALREADY_FULL: {
		cout << "Your team is full!" << endl;
		break;
	}
	case JOIN_TEAM_SUCCESS: {
		if (status == 1) {
			cout << "Join team id: " << res + 4 << endl;
			string pre = res + 4;
			if (isNumber(pre)) {
				status = 2;
				idTeam = stoi(pre);
			}
		}
		break;
	}
	case ACCEPT_USER_JOIN_TEAM_FAIL: {
		if (status == 3) {
			cout << "Send request accept fail!" << endl;
		}
		break;
	}

//12
	case DENY_JOIN_TEAM_SUCCESS: {
		if (status == 3) {
			cout << "Deny join team success" << endl;
		}
		break;
	}
	
	case USER_IN_ANOTHER_TEAM_12 : {
		if (status == 3) {
			cout << "This user is on another team!" << endl;
		}
		break;
	}
	case JOIN_TEAM_FAIL: {
		if (status == 1) {
			cout << "You are refused to join by team " << res + 4 << endl;
		}
		break;
	}
	case DENY_JOIN_TEAM_FAIL: {
		if (status == 3) {
			cout << "Send request deny fail!" << endl;
		}
		break;
	}

//13
	case INVITE_USER_JOINTEAM_SUCCESS: {
		if (status == 3) {
			cout << "Send invitation success!" << endl;
		}
		break;
	}
	case USER_IN_ANOTHER_TEAM_13: {
		if (status == 3) {
			cout << "This user is on another team!" << endl;
		}
		break;
	}

	case RECEIVE_INVITATION_SUCCESS: {
		if (status == 1) {
			cout << "Receive invitation form team id: " << (res + 4) << endl;
		}
		else {
			string pre = "261|";
			pre = pre + (res + 4);
			/*cout << pre;*/
			char* returnData = (char*)malloc(pre.length() * sizeof(char));
			strcpy(returnData, pre.c_str());

			Send(global, returnData, strlen(returnData), 0);
		}
		break;
	}

//14
	case USER_ACCEPT_JOINTEAM_SUCCESS: {
		if (status == 3) {
			cout << "Player " << res + 4 << " accept your invitation!" << endl;
		}
		break;
	}
	case TEAM_INVALID: {
		if (status == 1) {
			cout << "This team does not exist!" << endl;
		}
		break;
	}

//15
	case USER_REFUSE_JOINTEAM_SUCCESS: {
		if (status == 3) {
			cout << "Player " << res + 4 << " decline your invitation!";
		}
		break;
	}

	}
}

/* The send() wrapper function*/
int Send(SOCKET s, char *buff, int size, int flags) {
	int n;

	n = send(s, buff, size, flags);
	if (n == SOCKET_ERROR)
		printf("Error: %d\n", WSAGetLastError());

	return n;
}

/* The recv() wrapper function */
int Receive(SOCKET s, char *buff, int size, int flags) {
	int n;

	n = recv(s, buff, size, flags);
	if (n == SOCKET_ERROR)
		printf("Error: %d\n", WSAGetLastError());

	return n;
}