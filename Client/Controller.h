/**
* @function splitData: split a string by delimeter and push them to vector
* @param inlineDate : string input need to split
* @param del: delimeter
* @return vector<string>
**/
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

/**
* @funtion isNumber: confirm param text is number or not.
* @param text: a string.
* @return:	0 if text is not a number.
			1 if text is a number
**/
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

/**
* @function getPersionInfo: split input string and add to vector
* @param s : string input need to split
* @return vector<string>
**/
vector<string> getPersionalInfo(string s) {
	vector<string> splitPersionalInfo;
	stringstream stream(s);
	string word;
	while (getline(stream, word, ' ')) {
		splitPersionalInfo.push_back(word);
	}
	return splitPersionalInfo;
}

/**
* @function getAllUserIngame: split a string by delimeter and push them to  string vector and assign user data to string vector
* @param s : string input need to split
* @return vector<User>
**/
vector<User> getAllUserIngame(string s) {
	vector<User> users;
	vector<string> splitUser;
	stringstream stream(s);
	string word;
	while (getline(stream, word, '|')) {
		stringstream ss(word);
		string tmp;
		while (getline(ss, tmp, ' ')) {
			splitUser.push_back(tmp);
		}
	}
	if (!splitUser.empty()) {
		for (int i = 0; i < splitUser.size() - 3; i += 4) {
			User user;
			user.username = splitUser[i];
			user.hp = stoi(splitUser[i + 1]);
			user.armor = stoi(splitUser[i + 2]);
			user.coins = stoi(splitUser[i + 3]);
			users.push_back(user);
		}
	}
	return users;
}

/**
* @function handleSplitStr: split a string by delimeter and push them to  string vector and assign Team vector to string vector
* @param s : string input need to split
* @return vector<Team>
**/
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

/**
* @funtion declineJoinTeam: decline invitations to join team in some cases.
							Ex: when player creates a new room, accepts a invitation, was accepted to join team.
* @param res: a string pointer.
* @return:	nothing
**/
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


/**
* @funtion handleResponse: handle every message from server and print to console.
* @param res: a string pointer (message from server).
* @return: nothing
**/
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
		cout << "Login successfully!" << endl;
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
		cout << "This username is already existed" << endl;
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
		cout << "User is in game!" << endl;
		break;
	}

					   //5
	case SEND_REQUEST_JOINTEAM_SUCCESS: {
		cout << "Sending request to join team successfully" << endl;
		break;
	}
	case TEAM_IS_FULL: {
		cout << "Team is full of members" << endl;
		break;
	}
	case TEAM_INVALID: {
		cout << "This team does not exist!" << endl;
		break;
	}
	case USER_ALREADY_INTEAM: {
		cout << "This user is already in team!" << endl;
		break;
	}
	case SEND_TO_INVITATION_JOIN_TEAM: {
		cout << endl << "User " << res + 4 << " wants to join your team!" << endl;
		break;
	}
	case REQUEST_FAIL: {
		cout << "Send request fail!" << endl;
		break;
	}

					   //6
	case CREATE_TEAM_SUCCESS: {
		cout << "Create team successfully!" << endl;
		declineJoinTeam(res);
		status = 3;
		break;
	}
	case  NUMBER_OF_TEAM_LIMIT: {
		cout << "Number of team exceeds limit" << endl;
		break;
	}
	case TEAM_NAME_ALREADY_EXISTS: {
		cout << "The team name is already existed!" << endl;
		break;
	}

//7
	case LOGOUT_SUCCESS: {
		cout << "Sign out successfully!" << endl;
		status = 0;
		break;
	}

						 //8
	case LEAVE_TEAM_SUCCESS: {
		cout << "Leave team successfully!" << endl;
		status = 1;
		break;
	}
	case NOT_IN_A_TEAM: {
		cout << "You are not belong to any team!" << endl;
		break;
	}
	case TEAM_WAS_DISSOLVED: {
		cout << endl << "Your team was dissolved!" << endl;
		cout << "You just changed status, please press any key + Enter to continue!" << endl;
		status = 1;
		break;
	}
	case MEMBER_LEAVE: {
		cout << endl << res + 4 << " leave your team!" << endl;
		break;
	}

					   //9
	case GET_TEAMMBER_SUCCESS: {
		cout << "==========Members in team==========" << endl;
		stringstream ss(res + 4);
		string word;
		while (getline(ss, word, ' ')) {
			cout << word << endl;
		}
		break;
	}

							   //10
	case GETUSERS_IN_WAITINGROOM_SUCCESS: {
		cout << "==========List users in waitting room==========" << endl;
		stringstream ss(res + 4);
		string word;
		while (getline(ss, word, ' ')) {
			cout << word << endl;
		}
		break;
	}
	case USER_IS_NOT_HOST: {
		cout << "You are not a leader!" << endl;
		break;
	}

						   //11
	case ACCEPT_USER_JOIN_TEAM_SUCCESS: {
		cout << "Send accept request successfully!" << endl;
		break;
	}
	case USER_NOT_ONLINE: {
		cout << "This user is not online!" << endl;
		break;
	}
	case USER_IN_ANOTHER_TEAM: {
		cout << "This user is belong to team!" << endl;
		break;
	}
	case SEND_TO_JOIN_TEAM_SUCCESS: {
		cout << endl << "Join team #" << res + 4 << " successfully!" << endl;
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
		cout << "Deny join team successfully!" << endl;
		break;
	}
	case SEND_TO_JOIN_TEAM_FAIL: {
		cout << endl << "You are refused to join by " << res + 4 << endl;
		break;
	}

								 //13
	case INVITE_USER_JOINTEAM_SUCCESS: {
		cout << "Send invitation successfully!" << endl;
		break;
	}
	case SEND_TO_RECEIVE_INVITATION_SUCCESS: {
		cout << endl << "Receive invitation from team id: " << (res + 4) << endl;
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
		cout << "Join team successfully!" << endl;
		idYourTeam = atoi(res + 4);
		declineJoinTeam(res);
		status = 2;
		break;
	}
	case TEAM_IN_GAME: {
		cout << "This team is being in game!" << endl;
		break;
	}
	case SEND_TO_ACCEPT_INVITATION: {
		cout << endl << res + 4 << " accepts your invitation!" << endl;
		break;
	}
	case UNINVITED: {
		cout << "You do not receive invitation from this team!" << endl;
		break;
	}

					//15
	case USER_REFUSE_JOINTEAM_SUCCESS: {
		cout << "Refuse successfully!" << endl;
		break;
	}
	case SEND_TO_HOST_USER_REFUSE_INVITATION: {
		cout << endl << res + 4 << " declines your invitation!" << endl;
		break;
	}

											  // 16
	case KICK_USER_SUCCESS: {
		cout << "Kick member successfully!" << endl;
		break;
	}
	case USER_WANT_TOKICK_NOT_INTEAM: {
		cout << "This user is not belong to your team!" << endl;
		break;
	}
	case SEND_TO_USER_KICKED: {
		cout << endl << "You were kicked" << endl;
		status = 1;
		cout << "You just changed status, please press any key + Enter to continue!" << endl;
		break;
	}
	case KICK_LEADER: {
		cout << "You cannot kick yourself!" << endl;
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
		cout << "Send challenge successfully!" << endl;
		break;
	}
	case CHALLENGE_YOURSELF: {
		cout << "You cannot challenge yourself!" << endl;
		break;
	}
	case OPONPENT_LACK_MEMBER: {
		cout << "This team is not enough members!" << endl;
		break;
	}
	case OPONENT_IN_GAME: {
		cout << "This team is being in game!" << endl;
		break;
	}
	case OPONENT_INVALID: {
		cout << "This team does not exist!" << endl;
		break;
	}
	case TEAM_LACK_MEMBER: {
		cout << "Your team is not enough members!" << endl;
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
		cout << "Matching game successfully!" << endl;
		status = 4;
		break;
	}
	case SEND_TO_ALL_USERS_MATCHGAME: {
		cout << endl << "Start game!" << endl;
		status = 4;
		cout << "You just changed status, please press any key + Enter to continue!" << endl;
		break;
	}
	case UNCHALLENGED: {
		cout << "Do not receive challenge from this team!" << endl;
		break;
	}
	case CHALLLENGED_YOURSELF: {
		cout << "You cannot challenge yourself!" << endl;
		break;
	}
							   //20
	case REFUSE_CHALLENGE_SUCCESS: {
		cout << "Refuse challenge successfully!" << endl;
		break;
	}
	case SEND_TO_HOST_OPONENT_REFUSE: {
		cout << res + 4 << " refuses your challenge!" << endl;
		break;
	}
									  //21
	case BUY_ITEM_SUCCESS: {
		cout << "Buy item successfully!" << endl;
		break;
	}
	case LACK_MONEY: {
		cout << "Not enough money!" << endl;
		break;
	}
	case INVALID_ITEM: {
		cout << "Item does not exist!" << endl;
		break;
	}
	case MEMBER_NOT_INGAME: {
		cout << "You are not being in game!" << endl;
		break;
	}
	case MEMBER_IS_DIE_INGAME: {
		cout << "You were dead!" << endl;
		break;
	}
	case EXCEED_MAX_ITEM: {
		cout << "Cannot buy this item anymore!" << endl;
		break;
	}
	case FAIL_TRANSACTION: {
		cout << "Buy item fail!" << endl;
		break;
	}
						   //22
	case GET_INFO_USERS_INGAME: {
		cout << "Get users in game successfully" << endl;
		vector<string> splitTeam;
		vector<string> listString1;
		vector<string> listString2;
		splitTeam = splitData(res + 4, "\n");
		listString1 = splitData(splitTeam[0], "|");
		listString2 = splitData(splitTeam[1].substr(1), "|");
		cout << "===========Team 1==========" << endl;
		cout << setw(20) << left << "Username";
		cout << setw(10) << left << "HP";
		cout << setw(10) << left << "Armor";
		cout << setw(10) << left << "Coins" << endl;
		cout << setfill('-');
		cout << setw(50) << "-" << endl;
		cout << setfill(' ');
		if (!listString1.empty()) {
			for (int i = 0; i < listString1.size(); i++) {
				cout << setw(20) << left << splitData(listString1[i], " ")[0];
				cout << setw(10) << left << splitData(listString1[i], " ")[1];
				cout << setw(10) << left << splitData(listString1[i], " ")[2];
				cout << setw(10) << left << splitData(listString1[i], " ")[3] << endl;
			}

		}
		cout << "===========Team 2==========" << endl;
		cout << setw(20) << left << "Username";
		cout << setw(10) << left << "HP";
		cout << setw(10) << left << "Armor";
		cout << setw(10) << left << "Coins" << endl;
		cout << setfill('-');
		cout << setw(50) << "-" << endl;
		cout << setfill(' ');
		if (!listString2.empty()) {
			for (int i = 0; i < listString2.size(); i++) {
				cout << setw(20) << left << splitData(listString2[i], " ")[0];
				cout << setw(10) << left << splitData(listString2[i], " ")[1];
				cout << setw(10) << left << splitData(listString2[i], " ")[2];
				cout << setw(10) << left << splitData(listString2[i], " ")[3] << endl;
			}

		}
		break;
	}//23
	case GET_PERSONAL_INFO: {
		vector<string> userInfo = getPersionalInfo(res + 4);
		cout << "===========Personal info==========" << endl;
		if (!userInfo.empty()) {
			cout << "HP: " << userInfo[0] << endl;
			cout << "Basic armor: " << userInfo[1] << endl;
			cout << "Advanced armor: " << userInfo[2] << endl;
			for (int i = 3; i <= 6; i++) {
				if (stoi(userInfo[i]) < 0) {
					cout << "Gun barrel " << (i - 2) << ": No ammo available" << endl;
				}
				else cout << "Gun barrel " << (i - 2) << ": " << userInfo[i] << endl;
			}
			for (int i = 7; i <= 10; i++) {
				if (stoi(userInfo[i]) < 0) {
					cout << "Laser beam " << (i - 6) << ": " << "Unavailable" << endl;
				}
				else cout << "Laser beam " << (i - 6) << ": " << userInfo[i] << endl;
			}
			cout << "Rocket: " << userInfo[11] << endl;
			cout << "Coins: " << userInfo[12] << endl;
		}
		break;
	}

							//24 
	case ATTACK_SUCCESS: {
		cout << "Attack successfully!" << endl;
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
		cout << "You cannot attack, please waiting " << res + 4 << "s to reload bullet!" << endl;
		break;
	}
	case SEND_TO_ALL_USERS_DAMAGE_OF_ATTACK: {
		cout << endl;
		int count = 0;
		char string[BUFF_SIZE];
		strcpy(string, res);
		char * token = strtok(string + 4, "|");
		while (token != NULL) {
			count++;
			cout << token;
			switch (count) {
			case 1: {
				cout << " attacks ";
				break;
			}
			case 2: {
				cout << " with ";
				break;
			}
			case 3: {
				cout << " damage" << endl;
				break;
			}
			}
			token = strtok(NULL, "|");
		}
		break;
	}
	case SEND_TO_ALL_USERS_WHO_DEADS: {
		cout << endl;
		int count = 0;
		char string[BUFF_SIZE];
		strcpy(string, res);
		char * token = strtok(string + 4, "|");
		while (token != NULL) {
			count++;
			cout << token;
			switch (count) {
			case 1: {
				cout << " kills ";
				break;
			}
			case 2: {
				cout << endl;
				break;
			}
			}
			token = strtok(NULL, "|");
		}
		break;
	}
	case SEND_TO_DEAD_USER_WHO_SHOT: {
		cout << endl << "You are killed by " << res + 4 << endl;
		break;
	}
	case SEND_TO_ALL_MEMBER_AT_END: {
		cout << endl << "Team " << res + 4 << " won!" << endl;
		cout << "You just changed status, please press any key + Enter to continue!" << endl;
		status = 2;
		break;
	}
	case SEND_TO_ALL_LEADER_AT_END: {
		cout << endl << "Team " << res + 4 << " won!" << endl;
		cout << "You just changed status, please press any key + Enter to continue!" << endl;
		status = 3;
		break;
	}

									// 25 
	case SEND_TO_ALL_USERS_QUIZ: {
		cout << endl << "Do a quiz to get coin!" << endl;
		char base[BUFF_SIZE];
		int count = 1;
		strcpy(base, res);
		char * token = strtok(base + 4, "|");
		while (token != NULL) {
			switch (count) {
			case 1: {
				cout << "ID: ";
				break;
			}
			case 2: {
				cout << "Coins: ";
				break;
			}
			case 3: {
				cout << "Question: ";
				break;
			}
			default: {
				cout << "Answer " << count - 3 << ": ";
				break;
			}
			}
			cout << token << endl;
			token = strtok(NULL, "|");
			count++;
		}
		break;
	}

								 // 26
	case ANSWER_TRUE_AND_FASTEST: {
		cout << "Your answer is true and fastest!" << endl;
		break;
	}
	case NOT_FASTEST_ANSWER: {
		cout << "Somebody else has true and faster answer! :(" << endl;
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
		cout << "Your team surrenders successfully!" << endl;
		status = 3;
		break;
	}
	case SEND_TO_ALL_USERS_WINNER_TEAM_ID: {
		cout << "Team " << res + 4 << " won!" << endl;
		cout << "You just changed status, please press any key + Enter to continue!" << endl;
		status = 2;
		break;
	}
	case SEND_TO_LEADER_WINNER_TEAM_ID: {
		cout << "Team " << res + 4 << " won!" << endl;
		cout << "You just changed status, please press any key + Enter to continue!" << endl;
		status = 3;
		break;
	}

										// 28 
	case OUT_GAME: {
		cout << endl << res + 4 << " exited the game" << endl;
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