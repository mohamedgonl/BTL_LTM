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
vector<Team> handleSplitStr(string s)
{
	vector<Team> t;
	vector<string> splitTeam;
	stringstream stream(s);
	string word;
	while (getline(stream, word, '|')) {
		stringstream ss(word);
		string tmp;
		while (getline(ss, tmp, ' ')) {
			splitTeam.push_back(tmp);
		}
	}
	int numOfMember = -1;
	if (!splitTeam.empty()) {
		for (int i = 0; i < splitTeam.size() - 2; i += 3) {
			Team team;
			team.id_team = stoi(splitTeam[i]);
			team.team_name = splitTeam[i + 1];
			team.number = stoi(splitTeam[i + 2]);
			t.push_back(team);
		}
	}
	return t;
}

void declineJoinTeam(char *res) {
	string idTeam;
	idTeam = res + 4;
	string pre = "DECLINE ";
	int idTeamInInt = stoi(idTeam);
	for (int i = 0; i < NumberOfTeamInvite; i++) {
		if (listTeamInvite[i] != idTeamInInt) {
			string str;
			stringstream ss;
			ss << listTeamInvite[i];
			ss >> str;
			string sendData = pre + str + ENDING_DELIMITER;
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
			string sendData = pre + str + ENDING_DELIMITER;
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
		//2
	case INVALID_COMMAND: {
		cout << "Command is incorrect!" << endl;
		break;
	}
	case LOGIN_SUCCESS: {
		status = 1;
		cout << "Login successful!" << endl;
		break;
	}
	case ACC_INVALID: {
		cout << "This account does not exist!" << endl;
		break;
	}
	case WRONG_PASSWORD: {
		cout << "Wrong password!" << endl;
		break;
	}
	case ACC_IN_ANOTHER_SESSION: {
		cout << "This account was in another sesssion!" << endl;
		break;
	}
	case USER_LOGINNED: {
		cout << "You logged!" << endl;
		break;
	}
//3
	case SIGNUP_SUCCESS: {
		cout << "Sign up successful!" << endl;
		break;
	}
	case USERNAME_EXISTED: {
		cout << "This username does exist" << endl;
		break;
	}
	case SERVER_ERROR: {
		cout << "Server error!" << endl;
		break;
	}

//4
	case RES_GETTEAMS_SUCCESS: {
		cout << "Get teams successfully" << endl;
		vector<Team>  teams = handleSplitStr(res + 4);
		cout << setw(5) << left << "ID";
		cout << setw(30) << left << "Team'name";
		cout << setw(20) << right << "Team'members" << endl;
		cout << setfill('-');
		cout << setw(55) << "-" << endl;
		cout << setfill(' ');
		if (!teams.empty()) {
			for (int i = 0; i < teams.size(); i++) {
				cout << setw(5) << left << teams[i].id_team;
				cout << setw(30) << left << teams[i].team_name;
				cout << setw(20) << right << teams[i].number << endl;
			}
		}
		break;
	}
	case RES_NOT_AUTHORIZE: {
		cout << "You are unauthorized" << endl;
		break;
	}
	case USER_IN_GAME: {
		cout << "User in game!" << endl;
		break;
	}
//5
	case SEND_REQUEST_JOINTEAM_SUCCESS: {
		cout << "Send request to join team successfully" << endl;
		break;
	}
	case TEAM_IS_FULL: {
		cout << "Team is full" << endl;
		break;
	}
	case TEAM_INVALID: {
		cout << "This team does not exist!" << endl;
		break;
	}
	case USER_ALREADY_INTEAM: {
		cout << "This user already in team!" << endl;
		break;
	}
	case SEND_TO_INVITATION_JOIN_TEAM: {
		cout << "User " << res + 4 << " wants to join your team!" << endl;
		break;
	}
	case REQUEST_FAIL: {
		cout << "Send request fail!" << endl;
		break;
	}

//6
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
	case TEAM_NAME_ALREADY_EXISTS: {
		cout << "The team name already exists!" << endl;
		break;
	}

//7
	case LOGOUT_SUCCESS: {
		cout << "Sign out success!" << endl;
		status = 0;
		break;
	}

//8
	case LEAVE_TEAM_SUCCESS: {
		cout << "Leave team successfully" << endl;
		status = 1;
		break;
	}
	case NOT_IN_A_TEAM: {
		cout << "You are not in any team!" << endl;
		break;
	}
	case TEAM_WAS_DISSOLVED: {
		cout << endl <<  "Your team was dissolved!" << endl;
		break;
	}

//9
	case GET_TEAMMBER_SUCCESS: {
		cout << "Get members team successfully!" << endl;
		cout << "Team member:" << res + 4 << endl;
		break;
	}

//10
	case GETUSERS_IN_WAITINGROOM_SUCCESS: {
		cout << "List user in waitting room: " << res + 4 << endl;
		break;
	}
	case USER_IS_NOT_HOST: {
		cout << "You are not a leader!" << endl;
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
		cout << "This user is in team!" << endl;
		break;
	}
	case SEND_TO_JOIN_TEAM_SUCCESS: {
		cout << endl << "Join team id: " << res + 4 << " success" << endl;
		// Phải từ chối tất cả những team đã mời mình
		declineJoinTeam(res);
		status = 2;
		cout << "You just changed status, please press any key + Enter to continue!" << endl;
		break;
	}
	case USER_DONNOT_REQUEST_TO_JOIN: {
		cout << "This user do not request to join your team!" << endl;
		break;
	}

//12
	case DENY_JOIN_TEAM_SUCCESS: {
		cout << "Deny join team success!" << endl;
		break;
	}
	case SEND_TO_JOIN_TEAM_FAIL: {
		cout << endl << "You are refused to join by " << res + 4 << endl;
		break;
	}

//13
	case INVITE_USER_JOINTEAM_SUCCESS: {
		cout << "Send invitation success!" << endl;
		break;
	}

	case SEND_TO_RECEIVE_INVITATION_SUCCESS: {
		cout << endl << "Receive invitation fromm team id: " << (res + 4) << endl;
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
		idYourTeam = atoi(res + 4);
		declineJoinTeam(res);
		status = 2;
		break;
	}
	case TEAM_IN_GAME: {
		cout << "This team is in game!" << endl;
		break;
	}
	case SEND_TO_ACCEPT_INVITATION: {
		cout << res + 4 << " accepts your invitation!" << endl;
		break;
	}
	case UNINVITED: {
		cout << "You were uninvited" << endl;
		break;
	}

//15
	case USER_REFUSE_JOINTEAM_SUCCESS: {
		cout << "Refuse success!" << endl;
		break;
	}
	case SEND_TO_HOST_USER_REFUSE_INVITATION: {
		cout << res + 4 << " decline your invitation!" << endl;
		break;
	}

											  // 16
	case KICK_USER_SUCCESS: {
		cout << "Kick success!" << endl;
		break;
	}
	case USER_WANT_TOKICK_NOT_INTEAM: {
		cout << "This user you want to kick is not in team" << endl;
		break;
	}
	case SEND_TO_USER_KICKED: {
		cout << endl << "You were kicked" << endl;
		status = 1;
		cout << "You just changed status, please press any key + Enter to continue!" << endl;
		break;
	}

//17
	case GET_ALL_TEAMS: {
		cout << "Get all team successfully" << endl;
		vector<Team> teams = handleSplitStr(res + 4);
		cout << setw(5) << left << "ID";
		cout << setw(30) << left << "Team'name";
		cout << setw(20) << right << "Team'members" << endl;
		cout << setfill('-');
		cout << setw(55) << "-" << endl;
		cout << setfill(' ');
		if (!teams.empty()) {
			for (int i = 0; i < teams.size(); i++) {
				cout << setw(5) << left << teams[i].id_team;
				cout << setw(30) << left << teams[i].team_name;
				cout << setw(20) << right << teams[i].number << endl;
			}
		}
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
	case SEND_TO_OPONENT_CHALLENGE: {
		cout << endl << "You receive the challenge from team id: " << res + 4 << endl;
		string idTeam;
		idTeam = res + 4;
		if (isNumber(idTeam)) {
			listTeamChallenge[NumberOfTeamChallenge] = stoi(idTeam);
			NumberOfTeamChallenge++;
		}
		break;
	}

//19
	case MATCHING_GAME_SUCCESS: {
		cout << "Matching game success!" << endl;
		break;
	}
	case SEND_TO_ALL_USERS_MATCHGAME: {
		cout << "Start game!" << endl;
		declineChallenge(res + 4);
		status = 4;
		cout << "You just changed status, please press any key + Enter to continue!" << endl;
		break;
	}
	case UNCHALLENGED: {
		cout << "Your team was unchallenged by this team!" << endl;
		break;
	}

//20
	case REFUSE_CHALLENGE_SUCCESS: {
		cout << "Refuse challenge success!" << endl;
		break;
	}
	case SEND_TO_HOST_OPONENT_REFUSE: {
		cout << res + 4 << " refuse your challenge!" << endl;
		break;
	}
									  //21
									  //22
									  //23
	case GET_PERSIONAL_INFO: {
		int sung[4] = { 0, 0, 0, 0 };
		int dan[4] = { -1, -1, -1, -1 };
		int count = 0;
		char string[BUFF_SIZE];
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

			if (count != 4)token = strtok(NULL, " ");
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
	case LOADING_BULLET: {
		cout << "You cannot attack, please wait!" << endl;
		break;
	}
	case USER_IS_DEAD: {
		cout << "You are dead! :(" << endl;
		break;
	}
	case SEND_TO_ALL_USERS_DAMAGE_OF_ATTACK: {
		int count = 0;
		char string[BUFF_SIZE];
		strcpy(string, res);
		char * token = strtok(string + 4, " ");
		while (token != NULL) {
			count++;
			cout << token;
			switch (count) {
			case 1: {
				cout << " da tan cong ";
				break;
			}
			case 2: {
				cout << " voi luong dame ";
				break;
			}
			case 3: {
				cout << endl;
				break;
			}
			}
			token = strtok(NULL, " ");
		}
		break;
	}
	case SEND_TO_ALL_USERS_WHO_DEADS: {
		int count = 0;
		char string[BUFF_SIZE];
		strcpy(string, res);
		char * token = strtok(string + 4, " ");
		while (token != NULL) {
			count++;
			cout << token;
			switch (count) {
			case 1: {
				cout << " da ha guc ";
				break;
			}
			case 2: {
				cout << endl;
				break;
			}
			}

			token = strtok(NULL, " ");
		}
		break;
	}
	case SEND_TO_DEAD_USER_WHO_SHOT: {
		cout << "Ban da bi ban chet boi " << res + 4 << endl;
		break;
	}
	case SEND_TO_ALL_USER_WINTEAMS: {
		break;
	}
									// 25 
	case SEND_TO_ALL_USERS_QUIZ: {
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
	case QUIZ_INVALID: {
		cout << "Quiz does not exist!" << endl;
		break;
	}
	case WRONG_ANSWER: {
		cout << "Your answer is wrong!" << endl;
		break;
	}

					   // 27
	case SURRENDER_SUCCESS: {
		cout << "Your team surrender success!" << endl;
		break;
	}
	case SEND_TO_ALL_USERS_WINNER_TEAM_ID: {
		cout << "Team id " << res + 4 << " won!" << endl;

		break;
	}


										   // 28 
	case OUT_GAME: {
		cout << res + 4 << " just exited the game" << endl;
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