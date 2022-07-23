#include "stdafx.h"
#include "StructDefination.h"

#ifndef GlobalVariable
#define GlobalVariable
Account accounts[MAX_NUM_ACCOUNT];
LoginSession *loginSessions[MAX_CLIENT];
Team* teams[MAX_TEAM];
Room* rooms[MAX_ROOM];
QuestionDescription questionDescriptions[MAX_QUESTION];
LoginSession acc[MAX_NUM_ACCOUNT];
CRITICAL_SECTION critical;
list<char*> splitMsg(char* msg);
DataThread dataThread[MAX_THREAD];
int numOfRoom = 0;
int numOfAccount = 0;
int numOfThread = 0;
int numOfConn = 0;
int SERVER_PORT = 5500;
string fileDirectory = "C:\\Users\\long\\Desktop\\account.txt";
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
	{ "M_ACCEPT", 11 },
	{ "M_DECLINE", 12 },
	{ "INVITE", 13 },
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
	{ "TEST", 28 },
	{ "TEST1", 29 },
};
// item
const Item_Armor Armor[3] = { {"HP",1,1}, {"bArmor", 1000, 500 },{"aArmor", 2000, 1500} };
const Item_Attack Attack[3] = { {"sungtudong", 500,100, 50, 10},{"laze",1000,100,10, 100},{"rocket", 0, 2000, 1 ,800} };

#endif // !GlobalVariable
