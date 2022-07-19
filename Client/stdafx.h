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
#define BUFF_SIZE 2048
#define ENDING_DELIMITER "/"
#define SUCCESS_LOGIN 10
#define ACCOUNT_BLOCK 11
#define INVALID_ACCOUNT 12
#define LOGIN_IN_ANOTHER_CLIENT 13
#define ANOTHER_ACCOUNT_IN_SESSION 14
#define SUCCESS_POST 20
#define SUCCESS_LOGOUT 30
#define NOT_LOGIN 21
#define INVALID_COMMAND 99
#pragma comment(lib, "Ws2_32.lib")

using namespace std;


// TODO: reference additional headers your program requires here
