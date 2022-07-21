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

void handleResponse(char* res) {
	switch (atoi(res)) {
	case RES_LOGIN_SUCCESS: {
		cout << "Login successful!" << endl;
		status = 1;
		break;
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
	}
	case RES_NOT_AUTHORIZE: {
		cout << "You are unauthorized" << endl;
	}
	case SEND_REQUEST_JOINTEAM_SUCCESS: {
		cout << "Send request to join team successfully" << endl;
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

