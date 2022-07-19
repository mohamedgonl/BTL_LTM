
#include "stdafx.h"
#include "StructDefination.h"
#ifndef Controllers_h
#define Controllers_h

<<<<<<< HEAD



// 2. Login
string loginAccount(string username, string password, UserInfo* userInfo) {

	// check if account is logged in
	for (int i = 0; i < sizeof (loginSessions); i++) {
		if (!strcmp((loginSessions[i]->userInfo.username).c_str, username.c_str)) return "113";
	}
	// if this account has not logged in
	for (int i = 0; i < sizeof(accounts); i++) {
		if (!strcmp(accounts[i].username.c_str, username.c_str)) {
			if ( !strcmp(accounts[i].password.c_str, password.c_str) ) { // if username and password matched
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

string registerAccount (string username, string password) {
	for (int i = 0; i < sizeof(accounts); i++) {
		if (!strcmp(accounts[i].username.c_str, username.c_str)) // if username is existed
			return "121";
	}

	// save account to data file
	
	try
	{
		ofstream file(fileDirectory);

	}
	catch (const std::exception&)
	{
		cout << "Error at function 3.Register: Save data error\n";
	}
	
	

	return "120";
}

// 4. Get list all teams
=======
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

#endif // !Handle user statement




>>>>>>> ae0cb19829a6d10e3c22c3777d210e1c6c6d7c2e


#endif 
