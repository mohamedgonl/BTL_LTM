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

//2
#define INVALID_COMMAND 999
#define LOGIN_SUCCESS 110
#define ACC_INVALID 111
#define WRONG_PASSWORD 112
#define ACC_IN_ANOTHER_SESSION 113
#define USER_LOGINNED 114

//3
#define SIGNUP_SUCCESS 120
#define USERNAME_EXISTED 121
#define SERVER_ERROR 122

//4
#define RES_GETTEAMS_SUCCESS 210
#define RES_NOT_AUTHORIZE 211
#define USER_IN_GAME 212
//5
#define SEND_REQUEST_JOINTEAM_SUCCESS 220
#define TEAM_IS_FULL 221
#define TEAM_INVALID 222
#define USER_ALREADY_INTEAM 223
#define SEND_TO_INVITATION_JOIN_TEAM 224
#define REQUEST_FAIL 229


//6
#define CREATE_TEAM_SUCCESS 230
#define NUMBER_OF_TEAM_LIMIT 231
#define TEAM_NAME_ALREADY_EXISTS 232

//7
#define LOGOUT_SUCCESS 240


//8
#define LEAVE_TEAM_SUCCESS 310
#define NOT_IN_A_TEAM 311
#define TEAM_WAS_DISSOLVED 312


//9
#define GET_TEAMMBER_SUCCESS 320

//10
#define GETUSERS_IN_WAITINGROOM_SUCCESS 330
#define USER_IS_NOT_HOST 331

// 11
#define ACCEPT_USER_JOIN_TEAM_SUCCESS 340 
#define USER_NOT_ONLINE 341
#define USER_IN_ANOTHER_TEAM 342
#define SEND_TO_JOIN_TEAM_SUCCESS 343

//12
#define DENY_JOIN_TEAM_SUCCESS 350
#define SEND_TO_JOIN_TEAM_FAIL 351

//13
#define INVITE_USER_JOINTEAM_SUCCESS 360
#define SEND_TO_RECEIVE_INVITATION_SUCCESS 361 

//14
#define USER_ACCEPT_JOINTEAM_SUCCESS 250
#define TEAM_IN_GAME 251
#define SEND_TO_ACCEPT_INVITATION 252 

//15
#define USER_REFUSE_JOINTEAM_SUCCESS 260
#define SEND_TO_HOST_USER_REFUSE_INVITATION 261

//16
#define KICK_USER_SUCCESS 370
#define USER_WANT_TOKICK_NOT_INTEAM 371
#define SEND_TO_USER_KICKED 372

//17
#define GET_ALL_TEAMS 380

//18
#define CHALLENGE_SUCCESS 390
#define OPONPENT_LACK_MEMBER 391
#define OPONENT_IN_GAME 392
#define OPONENT_INVALID 393
#define TEAM_LACK_MEMBER 394
#define SEND_TO_OPONENT_CHALLENGE 395

//19
#define MATCHING_GAME_SUCCESS 510
#define SEND_TO_ALL_USERS_MATCHGAME 511

//20
#define REFUSE_CHALLENGE_SUCCESS 520 
#define SEND_TO_HOST_OPONENT_REFUSE 521


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
#define LOADING_BULLET 443
#define USER_IS_DEAD 444
#define SEND_TO_ALL_USERS_DAMAGE_OF_ATTACK 445
#define SEND_TO_ALL_USERS_WHO_DEADS 446
#define SEND_TO_DEAD_USER_WHO_SHOT 447
#define SEND_TO_ALL_USER_WINTEAMS 448

//25
#define SEND_TO_ALL_USERS_QUIZ 400

//26
#define ANSWER_TRUE_AND_FASTEST 450
#define NOT_FASTEST_ANSWER 451
#define QUIZ_INVALID 452
#define WRONG_ANSWER 453

//27
#define SURRENDER_SUCCESS 460
#define SEND_TO_ALL_USERS_WINNER_TEAM_ID 461 

//28
#define OUT_GAME 470

#endif // !RESPONSE_CODE

// TODO: reference additional headers your program requires here
