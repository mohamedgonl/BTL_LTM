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

#define INVALID_COMMAND 99
#define RES_LOGIN_SUCCESS 110
#define RES_SIGNUP_SUCCESS 120

#endif // !RESPONSE_CODE

// TODO: reference additional headers your program requires here
