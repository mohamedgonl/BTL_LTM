#include "stdafx.h"
#include "StructDefination.h"
#include "GlobalVariable.h"
#include "FunctionPrototypes.h"
#include "Controller.h"

struct paramThread {
	SOCKET connectedSocket;
	sockaddr_in serverAddr;
};

int showMenu(int *status) {
	bool crash = false;
	string userInput;
	switch (*status)
	{
// not login
	case 0: {
		cout << "================ Menu ================" << endl;
		cout << "1. Login " << endl;
		cout << "2. Sign up " << endl;
		cout << "Please select your options [1,2]: ";
		cin >> userInput;
		string x;
		getline(cin, x);
		if (*status != 0) crash = true;
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
		string x;
		getline(cin, x);
		if (*status != 1) crash = true;
		break;
	}
// in a team
	case 2: {
		cout << "================ In team ================" << endl;
		cout << "1. Get list member " << endl;
		cout << "2. Leave team " << endl;
		cout << "Please select your options [1,2]: ";
		cin >> userInput;
		string x;
		getline(cin, x);
		if (*status != 2) crash = true;
		break;
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
		cin >> userInput;
		string x;
		getline(cin, x);
		if (*status != 3) crash = true;
		break;
	}
	case 4: {
		cout << "================ In game ================" << endl;
		cout << "1. BUY item" << endl;
		cout << "2. Get information in game" << endl;
		cout << "3. Get own information" << endl;
		cout << "4. Attack oponent" << endl;
		cout << "5. Answer quiz" << endl;
		cin >> userInput;
		string x;
		getline(cin, x);
		if (*status != 4) crash = true;
		break;
	}
	default: {

	}
	}
	if (isNumber(userInput) && !crash) {
		return stoi(userInput);
	}
	else {
		return 0;
	}
}


string handleUserInput(int option) {
	string userInput;
	switch (status)
	{
	case 0: {
		string username;
		string password;
		cout << "Please input your username: ";
		getline(cin, username);
		cout << "Please input your password: ";
		getline(cin, password);
		if (option == 1) {
			userInput = "LOGIN " + username + " " + password;
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
			string x;
			getline(cin, x);
			userInput = "JOIN " + id;
		}
		if (option == 3) {
			string teamName;
			cout << "Please input your name team: ";
			cin >> teamName;
			string x;
			getline(cin, x);
			userInput = "CREATE " + teamName;
		}
		if (option == 4) {
			userInput = "SIGNOUT";
		}

		if (option == 5) {
			string id;
			cout << "Accept invitation to join team with id: " << endl;
			cin >> id;
			string x;
			getline(cin, x);
			userInput = "ACCEPT " + id;

		}
		if (option == 6) {
			string id;
			cout << "Decline invitation to join team with id: " << endl;
			cin >> id;
			string x;
			getline(cin, x);
			userInput = "DECLINE " + id;
		}
		break;
	}

	case 2: {
		if (option == 1) {
			userInput = "GETMEMBERS";
		}
		if (option == 2) {
			userInput = "OUTTEAM";
		}
		break;


	}
	case 3: {
		if (option == 1) {
			userInput = "GETUSERS";
		}
		if (option == 2) {
			string username;
			cout << "Username you want to accecpt :";
			cin >> username;
			string x;
			getline(cin, x);
			userInput = "M_ACCEPT " + username;
		}
		break;
		if (option == 3) {
			string username;
			cout << "Username you want to decline :";
			cin >> username;
			string x;
			getline(cin, x);
			userInput = "M_DECLINE " + username;
		}
		if (option == 4) {
			string username;
			cout << "Username you want to invite:";
			cin >> username;
			string x;
			getline(cin, x);
			userInput = "INVITE " + username;
		}
		if (option == 5) {
			string username;
			cout << "Username you want to kick:";
			cin >> username;
			string x;
			getline(cin, x);
			userInput = "KICK " + username;
		}
		if (option == 6) {
			userInput = "GETTEAMS";
		}
		if (option == 7) {
			string teamId;
			cout << "Teamid you want to challenge:";
			cin >> teamId;
			string x;
			getline(cin, x);
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
			string x;
			getline(cin, x);
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
			string x;
			getline(cin, x);
			userInput = "ATK " + oponent_name;
		}
		if (option == 5) {
			string key;
			cout << "Input your key" << endl;
			cin >> key;
			string x;
			getline(cin, x);
			userInput = "ANS " + key;
		}
	}
	}
	
	// Handle user input here
	return userInput;
}

unsigned __stdcall echoThread(void *paramUndefined) {
	paramThread param = *((paramThread*)paramUndefined);
	SOCKET connectedSocket = param.connectedSocket;
	sockaddr_in serverAddr = param.serverAddr;
	int serverAddrLen = sizeof(serverAddr);
	char buff[BUFF_SIZE], buffer[BUFF_SIZE] = "", temp[BUFF_SIZE] = "";
	int ret;
	char *p;
	// 1024
	bool logging = false;
	char userLogged[50];
	while (1) {
		ret = recv(connectedSocket, buff, BUFF_SIZE, 0);
		if (ret == SOCKET_ERROR) {
			printf("Error %d: Cannot receive data.\n", WSAGetLastError());
			// Disconnect client
			break;
		}
		else if (ret == 0) {
			printf("Client disconnects.\n");
			// Disconnect client
			break;
		}
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			cout << buff << endl;
			handleResponse(buff);
		}
	}
	closesocket(connectedSocket);
	return 0;
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
	int tv = 100000; //Time-out interval: 10000ms
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
	paramThread param;
	param.connectedSocket = client;
	param.serverAddr = serverAddr;
	int ret, messageLen, num;
	global = client;
	_beginthreadex(0, 0, echoThread, (void *)&param, 0, 0);

	while (1) {

		int option;
		while (true) {
			option = showMenu(&status);
			
			while (status == 0 && (option <= 0 || option >= 3)) {
				cout << "Invalid options. Please try again!" << endl;
				option = showMenu(&status);
			}

			while (status == 1 && (option <= 0 || option >= 6)) {
				cout << "Invalid options. Please try again!" << endl;
				option = showMenu(&status);
			}

			while (status == 2 && (option <= 0 || option >= 3)) {
				cout << "Invalid options. Please try again!" << endl;
				option = showMenu(&status);
			}

			while (status == 3 && (option <= 0 || option >= 11)) {
				cout << "Invalid options. Please try again!" << endl;
				option = showMenu(&status);
			}

			while (status == 4 && (option <= 0 || option >= 6)) {
				cout << "Invalid options. Please try again!" << endl;
				option = showMenu(&status);
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
			Sleep(100);
			//cout << buff;
			//ret = Receive(client, buff, BUFF_SIZE, 0);
			/*if (ret > 0) {
				buff[ret] = 0;
				cout << "Receive from server: " << buff << endl;
				handleResponse(buff);
			}*/
		}
	}
	// Close socket
	closesocket(client);

	// Terminate Winsock
	WSACleanup();

	return 0;
}

string handleUserInput(string userInput) {
	// Handle user input here
	return userInput;
}
