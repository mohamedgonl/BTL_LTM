#include "stdafx.h"
#include "StructDefination.h"
#include "GlobalVariable.h"
#include "FunctionPrototypes.h"
#include "Controller.h"

int status = 0;

int showMenu(int status) {
	string userInput;
	switch (status)
	{
	case 0: {
		cout << "================ Menu ================" << endl;
		cout << "1. Login " << endl;
		cout << "2. Sign up " << endl;
		cout << "Please select your options [1,2]: ";
		cin >> userInput;
		break;
	}
	case 1: {
		cout << "================ Waitting ================" << endl;
		cout << "1. Get list room " << endl;
		cout << "2. Join room " << endl;
		cout << "3. Create room " << endl;
		cout << "4. Sign out " << endl;
		cout << "Please select your options [1,2,3,4]: ";
		cin >> userInput;
		break;
	}
	default: {

	}
	}

	if (isNumber(userInput)) {
		return stoi(userInput);
	}
	else {
		return 0;
	}
}


string handleUserInput(int option) {
	string userInput ;
	switch (status)
	{
	case 0: {
		string username;
		string password;
		cout << "Please input your username: ";
		cin >> username;
		cout << "Please input your password: ";
		cin >> password;
		if (option == 1) {
			userInput = "USER " + username + " " + password;
		}
		if (option == 2) {
			userInput = "SIGNUP " + username + " " + password;
		}
		break;
	}
	case 1: {
		if (option == 1) {
			userInput = "GETTEAMS";
		}
		if (option == 2) {
			string id;
			cout << "Please input ID team: ";
			cin >> id;
			userInput = "JOIN " + id;
		}
		if (option == 3) {
			string teamName;
			cout << "Please input your name team: ";
			cin >> teamName;
			userInput = "CREATE " + teamName;
		}
		if (option == 4) {
			userInput = "SIGNOUT";
		}
	}

	}
	
	// Handle user input here
	return userInput;
}

int main(int argc, char* argv[]) {
	// Handle command line parameter
	if (argc == 3) {
		if (isNumber(argv[2])) {
			if (atoi(argv[2]) >= 0 && atoi(argv[2]) <= 65535) {
				SERVER_PORT = atoi(argv[2]);
			}
			else {
				printf("Port is in range 0 to 65535, use 5500 as default port!\n");
			}
		}
		else {
			printf("Port must be number, use 5500 as default port!\n");
		}
		SERVER_ADDR = argv[1];
	}
	else {
		printf("Command line parameter is not valid, use 5500 as default port!\n");
	}

	// Inittiate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		printf("Winsock 2.2 is not supported\n");
		return 0;
	}

	// Construct socket
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET) {
		printf("Error %d: Cannot create server socket.\n", WSAGetLastError());
		return 0;
	}

	// Set time-out for receiving
	int tv = 10000; //Time-out interval: 10000ms
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(int));

	// Specify server address
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);

	// Request to connect server
	if (connect(client, (sockaddr *)&serverAddr, sizeof(serverAddr))) {
		printf("Error %d: Cannot connect server.\n", WSAGetLastError());
		return 0;
	}
	printf("Connected server!\n");

	// Communicate with server
	char buff[BUFF_SIZE], temp[BUFF_SIZE];
	string userInput;
	int ret, messageLen, num;

	while (1) {

		int option;
		while (true) {
			option = showMenu(status);
			
			while (status == 0 && option <= 0 || option >= 3) {
				cout << "Invalid options. Please try again!" << endl;
				option = showMenu(status);
			}

			while (status == 1 && option <= 0 || option >= 5) {
				cout << "Invalid options. Please try again!" << endl;
				option = showMenu(status);
			}
			
			//cout << "Input your statement: " << endl;
			//getline(cin, inputData);
			string userInput = handleUserInput(option);
			while (userInput.length() >= BUFF_SIZE - strlen(ENDING_DELIMITER)) {
				cout << "Message too long (more than 2044 character). Try again: ";
				getline(cin, userInput);
			}
			userInput = userInput + ENDING_DELIMITER;
			strcpy(buff, userInput.c_str());
			Send(client, buff, strlen(buff), 0);
			ret = Receive(client, buff, BUFF_SIZE, 0);
			if (ret > 0) {
				buff[ret] = 0;
				handleResponse(buff);
			}
		}
	}
	// Close socket
	closesocket(client);

	// Terminate Winsock
	WSACleanup();

	return 0;
}


void handleResponse(char* res) {
	switch (atoi(res)) {
	case SUCCESS_LOGIN: {
		cout << "Login successful!" << endl;
		status = 1;
		break;
	}
	case ACCOUNT_BLOCK: {
		cout << "Your account is block!" << endl;
		break;
	}
	case INVALID_ACCOUNT: {
		cout << "Your account is not available!" << endl;
		break;
	}
	case ANOTHER_ACCOUNT_IN_SESSION: {
		cout << "Have another account is login in this device!" << endl;
		break;
	}
	case NOT_LOGIN: {
		cout << "You are not login. Please login to operate that function!" << endl;
		break;
	}
	case SUCCESS_POST: {
		cout << "Post articles successful!" << endl;
		break;
	}
	case SUCCESS_LOGOUT: {
		cout << "Logout successful!" << endl;
		break;
	}
	case INVALID_COMMAND: {
		cout << "Command is incorrect!" << endl;
		break;
	}
	}
}

