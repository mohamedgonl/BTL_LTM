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
		
		while (cin >> userInput) {
			if (*status != 0) break;
		};
		string x;
		getline(cin, x);
		if (*status != 0) {
			crash++;
		}
		break;
	}
// in waiting room
	case 1: {
		cout << "================ Waitting ================" << endl;
		cout << "1. Get list room " << endl;
		cout << "2. Join room " << endl;
		cout << "3. Create room " << endl;
		cout << "4. Sign out " << endl;
		cout << "Please select your options [1,2,3,4]: ";
		cin >> userInput;
		string x;
		getline(cin, x);
		cout << "Status = 1" << endl;
		break;
	}
// in a team
	case 2: {
		cout << "================ In team ================" << endl;
		cout << "1. Get list member " << endl;
		cout << "2. Leave team " << endl;
		cout << "Please select your options [1,2]: ";
		break;
	}
// host'room
	case 3: {
		cout << "================ Host ================" << endl;
		cout << "1. Get users in waiting room" << endl;
		cout << "2. Verify user want to join team" << endl;
		cout << "3. Refuse user to join team" << endl;
		cout << "4. Invite user to join team" << endl;
		break;
		//cout << "5. 
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
			cout << userInput;
		}
		if (option == 2) {
			userInput = "SIGNUP " + username + " " + password;
			cout << userInput;
		}
		if (status != 0) break;
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
			userInput = "M_ACCEPT username";
		}
		break;
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
			printf("Receive from server %s\n", buff);
			handleResponse(buff);
		}
	}
	printf("Out ra roi \n");
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
	_beginthreadex(0, 0, echoThread, (void *)&param, 0, 0);

	while (1) {

		int option;
		while (true) {
			option = showMenu(&status);
			
			while (status == 0 && option <= 0 || option >= 3) {
				cout << "Invalid options. Please try again!" << endl;
				option = showMenu(&status);
			}

			while (status == 1 && option <= 0 || option >= 5) {
				cout << "Invalid options. Please try again!" << endl;
				option = showMenu(&status);
			}
			
			//cout << "Input your statement: " << endl;
			//getline(cin, inputData);
			cout << "option : " << option << endl;
			cout << "Da den tan day" << endl;
			string userInput = handleUserInput(option);
			while (userInput.length() >= BUFF_SIZE - strlen(ENDING_DELIMITER)) {
				cout << "Message too long (more than 2044 character). Try again: ";
				getline(cin, userInput);
			}
			userInput = userInput + ENDING_DELIMITER;
			strcpy(buff, userInput.c_str());
			Send(client, buff, strlen(buff), 0);
			//Sleep(100);
			cout << buff;
			//ret = Receive(client, buff, BUFF_SIZE, 0);
			/*if (ret > 0) {
				buff[ret] = 0;
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



