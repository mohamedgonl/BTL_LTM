#include "stdafx.h"
#include "StructDefination.h"

Account accounts[MAX_NUM_ACCOUNT];
LoginSession* loginSessions[MAX_CLIENT];
Team* teams[MAX_TEAM];
Room* rooms[MAX_ROOM];
Question* questions[MAX_QUESTION];
LoginSession acc[MAX_NUM_ACCOUNT];
CRITICAL_SECTION critical;
list<char*> splitMsg(char* msg);
DataThread dataThread[MAX_THREAD];
int numOfAccount = 0;
int numOfThread = 0;
int numOfConn = 0;
int SERVER_PORT = 5500;
string fileDirectory;
map<string, int> action = {
	{ "LOGIN", 2 },
	{ "SIGNUP", 3 },
	{ "GETTEAMS", 4 },
	{ "JOIN", 5 },
	{ "CREATE", 6 },
	{ "SIGNOUT", 7 },
	{ "OUTTEAM", 8 },
	{ "GETMEMBERS", 9 },
	{ "GETUSERS", 10 },
	{ "MACCEPT", 11 },
	{ "M_DECLINE", 12 },
	{ "INVITED", 13 },
	{ "ACCEPT", 14 },
	{ "DECLINE", 15 },
	{ "KICK", 16 },
	{ "GETTEAM", 17 },
	{ "CHALLENGE", 18 },
	{ "ACCEPTCHALLENGE", 19 },
	{ "DECLINECHALLENGE", 20 },
	{ "BUY", 21 },
	{ "GETALL", 22 },
	{ "GETMINE", 23 },
	{ "ATK", 24 },
	{ "ANS", 26 },
	{ "SURR", 27 },
	{ "ENDGAME", 28 },
};
 


