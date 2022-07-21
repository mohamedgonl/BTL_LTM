// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>	
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <list>
using namespace std;
#define BUFF_SIZE 2048
#define ENDING_DELIMITER "/"
#pragma comment(lib, "Ws2_32.lib")


#ifndef RESPONSE_CODE

#define INVALID_COMMAND 999
#define RES_LOGIN_SUCCESS 110
#define RES_ACC_INVALID 111
#define RES_WRONG_PASSWORD 112
#define RES_ACC_IN_ANOTHER_SESSION 113

#define RES_SIGNUP_SUCCESS 120
#define RES_USERNAME_EXISTED 121

#define RES_GETTEAMS_SUCCESS 210
#define RES_NOT_AUTHORIZE 211
#define SEND_REQUEST_JOINTEAM_SUCCESS 220
#define TEAM_IS_FULL 221
#define REQUEST_JOINTEAM_IS_DENIED 229

#define CREATE_TEAM_SUCCESS 230
#define NUMBER_OF_TEAM_LIMIT 231

#define LOGOUT_SUCCESS 240
#define LEAVE_TEAM_SUCCESS 310
#define NOT_IN_A_TEAM 311

#define GETUSERS_IN_WAITINGROOM_SUCCESS 330
#define USER_IS_NOT_HOST 331
#define USER_INGAME 312



#endif // !RESPONSE_CODE

// TODO: reference additional headers your program requires here
