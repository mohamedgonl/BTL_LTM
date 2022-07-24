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

void declineJoinTeam(char *res) {
	string idTeam;
	idTeam = res + 4;
	string pre = "261|";
	int idTeamInInt = stoi(idTeam);
	for (int i = 0; i < NumberOfTeamInvite; i++) {
		if (listTeamInvite[i] != idTeamInInt) {
			string str;
			stringstream ss;
			ss << listTeamInvite[i];
			ss >> str;
			string sendData = pre + str;
			char* returnData = (char*)malloc(sendData.length() * sizeof(char));
			strcpy(returnData, sendData.c_str());
			Send(global, returnData, strlen(returnData), 0);
		}
	}
	NumberOfTeamInvite = 0;
}

void declineChallenge(char *res) {
	string idTeam;
	idTeam = res + 4;
	string pre = "DECLINECHALLENGE ";
	int idTeamInInt = stoi(idTeam);
	for (int i = 0; i < NumberOfTeamInvite; i++) {
		if (listTeamChallenge[i] != idTeamInInt) {
			string str;
			stringstream ss;
			ss << listTeamChallenge[i];
			ss >> str;
			string sendData = pre + str;
			char* returnData = (char*)malloc(sendData.length() * sizeof(char));
			strcpy(returnData, sendData.c_str());
			Send(global, returnData, strlen(returnData), 0);
		}
	}
	NumberOfTeamChallenge = 0;
}


void handleResponse(char* res) {
	char subbuff[4];
	memcpy(subbuff, &res[0], 3);
	subbuff[3] = '\0';
	string pre;
	switch (atoi(subbuff)) {
	case RES_LOGIN_SUCCESS: {
		status = 1;
		cout << "Login successful!" << endl;
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
		declineJoinTeam(res);
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
		cout << "Team member is:" << res + 4 << endl;
	}
	case GETUSERS_IN_WAITINGROOM_SUCCESS: {
		cout << "List user in waitting room: " << res + 4 << endl;
		break;
	}

//11
	case ACCEPT_USER_JOIN_TEAM_SUCCESS: {
		cout << "Send accept request success!" << endl;
		break;
	}
	case USER_NOT_ONLINE: {
		cout << "This user is not online!" << endl;
		break;
	}
	case USER_IN_ANOTHER_TEAM: {
		cout << "This user is on another team!" << endl;
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
		cout << "Join team id: " << res + 4 << " success" << endl;
		// Phải từ chối tất cả những team đã mời mình
		declineJoinTeam(res);
		status = 2;
		break;
	}
	case ACCEPT_USER_JOIN_TEAM_FAIL: {
		cout << "Send request accept fail!" << endl;
		break;
	}

//12
	case DENY_JOIN_TEAM_SUCCESS: {
		cout << "Deny join team success!" << endl;
		break;
	}
	case JOIN_TEAM_FAIL: {
		cout << "You are refused to join by team " << res + 4 << endl;
		break;
	}
	case DENY_JOIN_TEAM_FAIL: {
		cout << "Send request deny fail!" << endl;
		break;
	}

//13
	case INVITE_USER_JOINTEAM_SUCCESS: {
		cout << "Send invitation success!" << endl;
		break;
	}

	case RECEIVE_INVITATION_SUCCESS: {
		cout << "Receive invitation fromm team id: " << (res + 4) << endl;
		string idTeam;
		idTeam = res + 4;
		if (isNumber(idTeam)) {
			listTeamInvite[NumberOfTeamInvite] = stoi(idTeam);
			NumberOfTeamInvite++;
		}
		break;
	}

//14
	case USER_ACCEPT_JOINTEAM_SUCCESS: {
		cout << "Join team success!" << endl;
		declineJoinTeam(res);
		status = 2;
		break;
	}
	case TEAM_INVALID: {
		cout << "This team does not exist!" << endl;
		break;
	}
	

//15
	case USER_REFUSE_JOINTEAM_SUCCESS: {
		cout << "Player " << res + 4 << " decline your invitation!";
		break;
	}

//18
	case CHALLENGE_SUCCESS: {
		cout << "Send challenge success!" << endl;
		break;
	}
	case OPONPENT_LACK_MEMBER: {
		cout << "This team is not enough member!" << endl;
		break;
	}
	case OPONENT_IN_GAME: {
		cout << "This team is in game!" << endl;
		break;
	}
	case OPONENT_INVALID: {
		cout << "This team does not exist!" << endl;
		break;
	}
	case TEAM_LACK_MEMBER: {
		cout << "Your team is not enough member!" << endl;
		break;
	}
	case RECVEICE_REQUEST_CHALLENGE: {
		cout << "You receive the challenge from team id: " << res + 4 << endl;
		string idTeam;
		idTeam = res + 4;
		if (isNumber(idTeam)) {
			listTeamChallenge[NumberOfTeamChallenge] = stoi(idTeam);
			NumberOfTeamChallenge++;
		}
		break;
	}

//19
	case  MATCHING_GAME_SUCCESS: {
		cout << "Matching game success!" << endl;
		break;
	}
	case START_GAME: {
		cout << "Start game!" << endl;
		declineChallenge(res + 4);
		status = 4;
		break;
		// Từ chối các lời thách đấu trước đó
	}

//20
	case REFUSE_CHALLENGE_SUCCESS: {
		cout << "Refuse challenge success!" << endl;
		break;
	}
	case REFUSE_CHALLENGE: {
		cout << "This player refuse your challenge!" << endl;
		break;
	}
//23
	case GET_PERSIONAL_INFO: {
		int sung[4] = { 0, 0, 0, 0 };
		int dan[4] = { -1, -1, -1, -1 };
		int count = 0;
		char string[200];
		strcpy(string, res);
		char * token = strtok(string + 4, " ");
		while (token != NULL) {
			count++;
			switch (count) {
			case 1: {
				cout << "HP: " << token << endl;
				break;
			}
			case 2: {
				cout << "ARMOR 1: " << token << endl;
				break;
			}
			case 3: {
				cout << "ARMOR 2: " << token << endl;
				break;
			}
			case 4: {
				for (int i = 0; i < 4; i++) {
					if (atoi(token) > 0) {
						sung[i] = 1;
						dan[i] = atoi(token);
					}
					token = strtok(NULL, " ");
				}
				for (int i = 0; i < 4; i++) {
					if (atoi(token) > 0) {
						sung[i] = 2;
						dan[i] = atoi(token);
					}
					token = strtok(NULL, " ");
				}
				for (int i = 0; i < 4; i++) {
					if (sung[i] == 0) {
						cout << "Vi tri " << i << ": chua co u sung" << endl;
					}
					else if (sung[i] == 1) {
						cout << "Vi tri " << i << ": sung tu dong " << dan[i] << " vien" << endl;
					}
					else if (sung[i] == 2) {
						cout << "Vi tri " << i << ": sung laze " << dan[i] << " vien" << endl;
					}
				}
				break;
			}
			case 5: {
				cout << "Rocket: " << token << " qua" << endl;
				break;
			}
			case 6: {
				cout << "Tien: " << token << " coin" << endl;
				break;
			}

			}

			if (count !=  4)token = strtok(NULL, " ");
		}
		break;
	}

//24 
	case ATTACK_SUCCESS: {
		cout << "Attack success!" << endl;
		break;
	}
	case OPONENT_IS_DEAD: {
		cout << "This player is dead!" << endl;
		break;
	}
	case INVALID_OPONENT: {
		cout << "This player is invaild!" << endl;
		break;
	}
	case INTIME_DO_NONG: {
		cout << "You cannot attack, please wait!" << endl;
		break;
	}
	case USER_IS_DEAD: {
		cout << "You are dead! :(" << endl;
		break;
	}

// 25 
	case HAVE_QUIZ: {
		cout << "You have quiz!" << endl;
		char base[BUFF_SIZE];
		strcpy(base, res);
		char * token = strtok(base + 4, "|");
		while (token != NULL) {
			cout << token << endl;
			token = strtok(NULL, "|");
		}
		break;
	}

// 26
	case ANSWER_TRUE_AND_FASTEST: {
		cout << "Your answer is true and fastest!" << endl;
		break;
	}
	case NOT_FASTEST_ANSWER: {
		cout << "Somebody else has answer true and faster than you! :(" << endl;
		break;
	}

// 27
	case SURRENDER_SUCCESS: {
		cout << "Your team surrender success!" << endl;
		break;
	}
	case TEAM_WON: {
		cout << "Team id " << res + 4 << " won!" << endl;
		
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