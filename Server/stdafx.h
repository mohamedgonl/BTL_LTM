#include <conio.h>
#include <WinSock2.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <process.h>
#include <list>
#include <string>
#include <queue>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
using namespace std;
#define MAX_NUM_ACCOUNT 2500
#define MAX_CLIENT 3000
#define BUFF_SIZE 2048
#define MAX_CLIENT_IN_A_THREAD 64
#define MAX_LENGTH_OF_CLIENT_REQUEST 2048
#define SERVER_ADDR "127.0.0.1"
#define ENDING_DELIMITER "/"
#define SUCCESS_LOGIN "10"
#define ACCOUNT_BLOCK "11"
#define INVALID_ACCOUNT "12"
#define ANOTHER_ACCOUNT_IN_SESSION "14"
#define SUCCESS_POST "20"
#define SUCCESS_LOGOUT "30"
#define NOT_LOGIN "21"
#define INVALID_COMMAND "99"
#define MAX_THREAD 50
#include <fstream> 
#pragma comment (lib, "Ws2_32.lib")

// Define global variables
Account accounts[100];
LoginSession* loginSessions[100];
Team* teams[];
Room* rooms[];
Question* questions[];
string fileDirectory;

