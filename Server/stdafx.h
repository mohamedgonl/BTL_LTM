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
#define MAX_NUM_ACCOUNT 900
#define MAX_CLIENT 900
#define MAX_TEAM 900
#define MAX_ROOM 150
#define MAX_QUESTION 100
#define BUFF_SIZE 2048
#define MAX_CLIENT_IN_A_THREAD 64
#define MAX_LENGTH_OF_CLIENT_REQUEST 2048
#define SERVER_ADDR "127.0.0.1"
#define ENDING_DELIMITER "/"
#define MAX_THREAD 50
#define MAX_HP 1000
#define MAX_B_ARMOR 500
#define MAX_A_AMOR 1500

#pragma comment (lib, "Ws2_32.lib")

#ifndef RESPONSE_CODE

#define INVALID_COMMAND "99"
#define RES_LOGIN_SUCCESS "110"
#define RES_SIGNUP_SUCCESS "120"

#endif // !RESPONSE_CODE

