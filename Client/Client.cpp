#include "stdafx.h"
#include "StructDefination.h"
#include "GlobalVariable.h"
#include "FunctionPrototypes.h"
#include "Controller.h"


int showMenu(int *status) {
	bool crash = false; //Check if status just change when showMenu's running, crash will cancel command
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
		cout << "1. Get list team " << endl;
		cout << "2. Join team " << endl;
		cout << "3. Create team " << endl;
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
		cout << "3. Get list member" << endl;
		cout << "4. Invite user to join team" << endl;
		cout << "5. Kick user" << endl;
		cout << "6. Get all teams" << endl;
		cout << "7. Challenge team" << endl;
		cout << "8. Accept challenge" << endl;
		cout << "9. Out team" << endl;
		cout << "Please select your options [1,2,3,4,5,6,7,8,9]: ";
		cin >> userInput;
		string x;
		getline(cin, x);
		if (*status != 3) crash = true;
		break;
	}

	}
	if (isNumber(userInput) && !crash && (userInput.length() < 3)) {
		// if userInput.length < 3 it will a big number -> INVALID 
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
	// Not logged in yet
	case 0: {
		string username;
		string password;
		cout << "Please input your username: ";
		getline(cin, username);
		cout << "Please input your password: ";
		getline(cin, password);
		// Login function
		if (option == 1) {
			userInput = "LOGIN " + username + " " + password; 
		}
		// SignUp function
		if (option == 2) {
			userInput = "SIGNUP " + username + " " + password;
		}
		break;
	}
	// You are being in waiting room
	case 1: {
		// Get information about existing teams
		if (option == 1) {
			userInput = "GETTEAMS";
		}
		// Want to join team
		if (option == 2) {
			string id;
			cout << "Please input ID team: ";
			cin >> id;
			string x;
			getline(cin, x);
			userInput = "JOIN " + id;
		}
		// Create team
		if (option == 3) {
			string teamName;
			cout << "Please input your name team: ";
			cin >> teamName;
			string x;
			getline(cin, x);
			userInput = "CREATE " + teamName;
		}
		// Sign out
		if (option == 4) {
			userInput = "SIGNOUT";
		}
		// Accept invitation
		if (option == 5) {
			string id;
			cout << "Accept invitation to join team with id: " ;
			cin >> id;
			string x;
			getline(cin, x);
			userInput = "ACCEPT " + id;

		}
		break;
	}
	// You are a member
	case 2: {
		// Get information about team members
		if (option == 1) {
			userInput = "GETMEMBERS";
		}
		// Out team
		if (option == 2) {
			userInput = "OUTTEAM";
		}
		break;
	}
	// You are a leader
	case 3: {
		// Get information user in waiting room
		if (option == 1) {
			userInput = "GETUSERS";
		}
		// Accept user
		if (option == 2) {
			string username;
			cout << "Username you want to accecpt :";
			cin >> username;
			string x;
			getline(cin, x);
			userInput = "M_ACCEPT " + username;
		}
		// Get information about team members
		if (option == 3) {
			userInput = "GETMEMBERS";
		}
		// Invite user to join team
		if (option == 4) {
			string username;
			cout << "Username you want to invite: ";
			cin >> username;
			string x;
			getline(cin, x);
			userInput = "INVITE " + username;
		}
		// Kick member
		if (option == 5) {
			string username;
			cout << "Username you want to kick: ";
			cin >> username;
			string x;
			getline(cin, x);
			userInput = "KICK " + username;
		}
		// Get list existing teams
		if (option == 6) {
			userInput = "GETTEAMS";
		}
		// send challenge
		if (option == 7) {
			string teamId;
			cout << "Team id you want to challenge: ";
			cin >> teamId;
			string x;
			getline(cin, x);
			userInput = "CHALLENGE " + teamId;
		}
		// accept challenge 
		if (option == 8) {
			string teamId;
			cout << "Team id you want to accept challenge: ";
			cin >> teamId;
			string x;
			getline(cin, x);
			userInput = "ACCEPTCHALLENGE " + teamId;
		}
		// Out team
		if (option == 9) {
			userInput = "OUTTEAM";
		}
		break;
	}
	case 4: {
		cout << "=====Usage:Type commands as below to buy items=====" << endl;
		cout << "1. Command to buy items : BUY ITEM_NAME" << endl;
		cout << "Item: HP, bArmor(Basic Armor), aArmor(advanced)," << endl;
		cout << "autogun, laze, rocket" << endl;
		cout << "2. Get all players'infomation in game : GETALL" << endl;
		cout << "3. Get your personal information : GETMINE" << endl;
		cout << "4. Attack oponent : ATK OPONENT_NAME" << endl;
		cout << "5. Answer quiz : ANS ID KEY" << endl;
		cout << "6. Surrender oponent(leader) : SURR" << endl;
		cout << "Input your command : ";
		getline(cin, userInput);
		break;
	}
	}
	// Handle user input here
	return userInput;
}

// Reveice Thread
unsigned __stdcall workerThread(void *param) {
	SOCKET connectedSocket = *((SOCKET*)param);
	char buff[BUFF_SIZE];
	int ret;
	char *p;
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
		else if (ret > 0) {
			buff[ret] = 0;
			handleResponse(buff);
		}
	}
	cout << "Receive thread off" << endl;
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

	char buff[BUFF_SIZE];
	int ret;
	global = client;
	// Begin workerThread
	_beginthreadex(0, 0, workerThread, (void *)&client, 0, 0);
	while (1) {

		int option;
		while (true) {
			string userInput;
			bool prepareToSendRequest = true;// True -> prepare; False -> ready to send 
			while (prepareToSendRequest) {
				switch (status)
				{
				case 0: {
					option = showMenu(&status); // Press number
					if (status != 0) break;
					if (option <= 0 || option >= 3) {
						cout << "Invalid options. Please try again!" << endl;
						break;
					}
					userInput = handleUserInput(option); // Enter command
					if (status == 0) prepareToSendRequest = false;
					break;
				}
				case 1: {
					option = showMenu(&status); // Press number
					if (status != 1) break;
					if (option <= 0 || option >= 6) {
						cout << "Invalid options. Please try again!" << endl;
						break;
					}
					userInput = handleUserInput(option); // Enter command
					if (status == 1) prepareToSendRequest = false; 
					break;
				}
				case 2: {
					option = showMenu(&status); // Press number
					if (status != 2) break;
					if (option <= 0 || option >= 3) {
						cout << "Invalid options. Please try again!" << endl;
						break;
					}
					userInput = handleUserInput(option); // Enter command
					if (status == 2) prepareToSendRequest = false;
					break;
				}
				case 3: {
					option = showMenu(&status); // Press number
					if (status != 3) break;
					if (option <= 0 || option >= 11) {
						cout << "Invalid options. Please try again!" << endl;
						break;
					}
					userInput = handleUserInput(option); // Enter command
					if (status == 3) prepareToSendRequest = false;
					break;
				}
				case 4: {
					userInput = handleUserInput(0); // Enter command
					if (status == 4) prepareToSendRequest = false; 
					break;
				}

				}
			}

			while (userInput.length() >= BUFF_SIZE - strlen(ENDING_DELIMITER)) {
				cout << "Message too long (more than 2044 character). Try again: ";
				getline(cin, userInput);
			}
			userInput = userInput + ENDING_DELIMITER;
			strcpy(buff, userInput.c_str());
			Send(client, buff, strlen(buff), 0);
			Sleep(200);

		}
	}
	// Close socket
	closesocket(client);

	// Terminate Winsock
	WSACleanup();

	return 0;
}

//string handleUserInput(string userInput) {
//	// Handle user input here
//	return userInput;
//}
