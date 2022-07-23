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
#include "process.h"
#include <vector>
#include <sstream>
#include <iomanip>
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

//3
#define RES_SIGNUP_SUCCESS 120
#define RES_USERNAME_EXISTED 121

//4
#define RES_GETTEAMS_SUCCESS 210
#define RES_NOT_AUTHORIZE 211

//5
#define SEND_REQUEST_JOINTEAM_SUCCESS 220
#define TEAM_IS_FULL 222
#define REQUEST_JOINTEAM_IS_DENIED 229


//6
#define CREATE_TEAM_SUCCESS 230
#define NUMBER_OF_TEAM_LIMIT 231

//7
#define LOGOUT_SUCCESS 240
#define USER_IN_GAME_7 241

//8
#define LEAVE_TEAM_SUCCESS 310
#define NOT_IN_A_TEAM 311
#define USER_IN_GAME_8 312

//9
#define GET_TEAMMBER_SUCCESS 320

//10
#define GETUSERS_IN_WAITINGROOM_SUCCESS 330
#define USER_INGAME 312
#define USER_IS_NOT_HOST 331

// 11
#define ACCEPT_USER_JOIN_TEAM_SUCCESS 340
#define USER_NOT_ONLINE 341
#define USER_IN_ANOTHER_TEAM 342
#define USER_NOT_HOSTTEAM 331
#define TEAM_IS_ALREADY_FULL 343
#define ACCEPT_USER_JOIN_TEAM_FAIL 349

//12
#define DENY_JOIN_TEAM_SUCCESS 350
#define USER_NOT_ONLINE_12 351
#define USER_IN_ANOTHER_TEAM_12 352
#define DENY_JOIN_TEAM_FAIL 359


//13
#define INVITE_USER_JOINTEAM_SUCCESS 360
#define RECEIVE_INVITATION_SUCCESS 363
#define USER_IN_ANOTHER_TEAM_13 361

//14
#define USER_ACCEPT_JOINTEAM_SUCCESS 250
#define TEAM_INVALID 251

#define JOIN_TEAM_SUCCESS 344
#define JOIN_TEAM_FAIL 353

//15
#define USER_REFUSE_JOINTEAM_SUCCESS 260
#define USER_REFUSE_JOINTEAM_FAIL 259

//16
#define KICK_USER_SUCCESS 370
#define USER_WANT_TOKICK_NOT_INTEAM 371

//17
#define GET_ALL_TEAMS 380


//18
#define CHALLENGE_SUCCESS 390
#define OPONPENT_LACK_MEMBER 391
#define OPONENT_IN_GAME 392
#define OPONENT_INVALID 393
#define TEAM_LACK_MEMBER 394
#define RECVEICE_REQUEST_CHALLENGE 395

//19
#define MATCHING_GAME_SUCCESS 590


//20
#define REFUSE_CHALLENGE_SUCCESS 520


//21
#define BUY_ITEM_SUCCESS 410
#define LACK_MONEY 411
#define INVALID_ITEM 412
#define MEMBER_NOT_INGAME 413
#define MEMBER_IS_DIE_INGAME 414
#define EXCEED_MAX_ITEM 415
#define FAIL_TRANSACTION 499

//22
#define GET_INFO_USERS_INGAME 420

//23
#define GET_PERSIONAL_INFO 430

//24
#define ATTACK_SUCCESS 440
#define OPONENT_IS_DEAD 441
#define INVALID_OPONENT 442
#define INTIME_DO_NONG 443
#define USER_IS_DEAD 444


//26
#define ANSWER_TRUE_AND_FASTEST 450
#define NOT_FASTEST_ANSWER 451


//27
#define SURRENDER_SUCCESS 461


#endif // !RESPONSE_CODE

// TODO: reference additional headers your program requires here
