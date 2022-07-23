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
SOCKET global;

void handleResponse(char* res) {
	char subbuff[4];
	memcpy( subbuff, &res[0], 3 );
	subbuff[3] = '\0';
	string pre;
	bool Res = false;
	switch (atoi(subbuff)) {
	case RES_LOGIN_SUCCESS: {
		if (status == 0) {
			status = 1;
			cout << "Login successful!" << endl;
		}
		return;
	}
	case RES_SIGNUP_SUCCESS: {
		cout << "Sign up successful!" << endl;
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
		break;
	}
	case CREATE_TEAM_SUCCESS: {
		cout << "Create team successfully" << endl;
		break;
	}
	case RES_NOT_AUTHORIZE: {
		cout << "You are unauthorized" << endl;
		break;
	}
	case SEND_REQUEST_JOINTEAM_SUCCESS: {
		cout << "Send request to join team successfully" << endl;
		break;
	}
	case GETUSERS_IN_WAITINGROOM_SUCCESS: {
		if (status == 3) {
			cout << "List user in waitting room: " << res + 4 << endl;
		}
		break;
	}
	case ACCEPT_USER_JOIN_TEAM_SUCCESS: {
		if (status == 1) {
			cout << "Join team id: " << res + 4 << endl;
			pre = res + 4;
			if (isNumber(pre)) {
				status = 2;
				idTeam = stoi(pre);
			}
		}
	}
	case ACCEPT_USER_JOIN_TEAM_FAIL: {
		if (status == 1) {
			cout << "Team id: " << res + 4 << " decline you join!" << endl;
		}
	}
	
	case RECEIVE_INVITATION_SUCCESS: {
		if (status == 1) {
			cout << "Receive invitation form team id: " << (res + 4) << endl;
			//listTeamInvite = listTeamInvite + (res + 4) + " ";
		}
		else {
			string pre = "261|";
			pre = pre + (res + 4);
			
		}
		break;
	}
	case USER_ACCEPT_JOINTEAM_SUCCESS: {
		if (status == 3) {
			cout << "Player " << res + 4 << " accept your invitation!";
		}
		break;
	}
	case USER_REFUSE_JOINTEAM_SUCCESS: {
		if (status == 3) {
			cout << "Player " << res + 4 << " decline your invitation!";
		}
		break;
	}
	case GET_ALL_TEAMS: {
		if (status == 3) {
			cout << "Player " << res + 4 << " decline your invitation!";
		}
		break;
	}
	case CHALLENGE_SUCCESS: {
		if (status == 3) {
			cout << "Your request chanllenge to team " << res + 4 << "is sent successfully!"  << endl;
		}
		break;
	}
	case RECVEICE_REQUEST_CHALLENGE: {
		if (status == 3) {
			cout << "Team id: " << res + 4 << " sent request challenge!" << endl;
			pre = "390";
		}
		if (status == 2) {

		}
		break;
	}
	case MATCHING_GAME_SUCCESS: {
		if (status == 2 || status == 3) {

		}
	}

	if (Res) {
		char* returnData = (char*)malloc(pre.length() * sizeof(char));
		strcpy(returnData, pre.c_str());
		Send(global, returnData, strlen(returnData), 0);
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

