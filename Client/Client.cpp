#include "stdafx.h"
#include "StructDefination.h"
#include "GlobalVariable.h"
#include "FunctionPrototypes.h"
#include "Controller.h"

int showMenu(int status) {
	string userInput;
	switch (status)
	{
// not login
	case 0: {
		cout << "================ Menu ================" << endl;
		cout << "1. Login " << endl;
		cout << "2. Sign up " << endl;
		cout << "Please select your options [1,2]: ";
		cin >> userInput;
		break;
	}
// in waiting room
	case 1: {
		cout << "================ Waitting ================" << endl;
		cout << "1. Get list room " << endl;
		cout << "2. Join room " << endl;
		cout << "3. Create room " << endl;
		cout << "4. Sign out " << endl;
		cout << "5. Accept invitation to join team" << endl;
		cout << "Please select your options [1,2,3,4,5]: ";
		cin >> userInput;
		break;
	}
// in a team
	case 2: {
		cout << "================ In team ================" << endl;
		cout << "1. Get list member " << endl;
		cout << "2. Leave team " << endl;
		cout << "Please select your options [1,2]: ";
	}
// host'room
	case 3: {
		cout << "================ Host ================" << endl;
		cout << "1. Get users in waiting room" << endl;
		cout << "2. Verify user want to join team" << endl;
		cout << "3. Refuse user to join team" << endl;
		cout << "4. Invite user to join team" << endl;
		cout << "5. Kick user" << endl;
		cout << "6. Get all teams" << endl;
		cout << "7. Challenge team" << endl; 
		cout << "8. Accept challenge" << endl;
		cout << "9. Decline challenge" << endl;
		cout << "10. Surrender" << endl;
		//cout << "5. 
	}
	case 4: {
		cout << "1. BUY item" << endl;
		cout << "2. Get information in game" << endl;
		cout << "3. Get own information" << endl;
		cout << "4. Attack oponent" << endl;
		cout << "5. Answer quiz" << endl;
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
		if (option == 5) {
			string id;
			cout << "Accept invitation to join team with id:" << endl;
			cin >> id;
			userInput = "Accept " + id;

		}
		if (option == 6) {
			string id;
			cout << "Decline invitation to join team with id:" << endl;
			cin >> id;
			userInput = "Decline " + id;
		}
	}

	case 2: {
		if (option == 1) {
			userInput = "GETMEMBERS";
		}
		if (option == 2) {
			userInput = "OUTTEAM";
		}

	}
	case 3: {
		if (option == 1) {
			userInput = "GETUSERS";
		}
		if (option == 2) {
			userInput = "M_ACCEPT username";
		}
		if (option == 3) {
			userInput = "M_DECLINE username";
		}
		if (option == 4) {
			string username;
			cout << "Username you want to invite:";
			cin >> username;
			userInput = "INVITE "+username;
		}
		if (option == 5) {
			string username;
			cout << "Username you want to kick:";
			cin >> username;
			userInput = "KICK " + username;
		}
		if (option == 6) {
			userInput = "GETTEAMS";
		}
		if (option == 7) {
			string teamId;
			cout << "Teamid you want to challenge:";
			cin >> teamId;
			userInput = "CHALLENGE " + teamId;
		}
		if (option == 8) {
			userInput = "ACCEPTCHALLENGE";
		}
		if (option == 9) {
			userInput = "DECLINECHALLENGE";
		}
		if (option == 10) {
			userInput = "SURR";
		}
	}
	case 4: {
		if (option == 1) {
			string item_name;
			cout << "Item name you want to buy:";
			cin >> item_name;
			userInput = "BUY " + item_name;
		}
		if (option == 2) {
			userInput = "GETALL";
		}
		if (option == 3) {
			userInput = "GETMINE";
		}
		if (option == 4) {
			string oponent_name;
			cout << "Oponent name you want to attack:";
			cin >> oponent_name;
			userInput = "ATK " + oponent_name;
		}
		if (option == 5) {
			string key;
			cout << "Input your key" << endl;
			cin >> key;
			userInput = "ANS " + key;
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



