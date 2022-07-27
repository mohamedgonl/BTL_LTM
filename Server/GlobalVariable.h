#include "stdafx.h"
#include "StructDefination.h"

#ifndef GlobalVariable
#define GlobalVariable
Account accounts[MAX_NUM_ACCOUNT];
LoginSession *loginSessions[MAX_CLIENT];
Team* teams[MAX_TEAM];
Room* rooms[MAX_ROOM];
QuestionDescription questionDescriptions[MAX_QUESTION];
CRITICAL_SECTION critical;
list<char*> splitMsg(char* msg);
DataThread dataThread[MAX_THREAD];
int numOfRoom = 0;
int numOfAccount = 0;
int numOfQuestion = 0;
int numOfThread = 0;
int numOfConn = 0;
int SERVER_PORT = 5500;
char* SERVER_ADDR = "127.0.0.1";
Question questions[MAX_QUESTION] = {};
string accountFileDirectory = "F:\\BTL\\BTL_LTM\\Server\\account.txt";
string questionFileDirectory = "F:\\BTL\\BTL_LTM\\Server\\question.txt";
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
	{ "GETALLTEAM", 17 },
	{ "CHALLENGE", 18 },
	{ "ACCEPTCHALLENGE", 19 },
	{ "DECLINECHALLENGE", 20 },
	{ "BUY", 21 },
	{ "GETALL", 22 },
	{ "GETMINE", 23 },
	{ "ATK", 24 },
	{ "ANS", 26 },
	{ "SURR", 27 },
};
// item
const Item_Armor Armor[3] = { {"HP",1,1}, {"bArmor", 1000, 500 },{"aArmor", 2000, 1500} };
const Item_Attack Attack[3] = { {"autogun", 500,100, 50, 10},{"laze",1000,100,10, 100},{"rocket", 0, 2000, 1 ,800} };

map<string, int> itemsMap = {
	{ "HP", 1 },
	{ "bArmor", 2 },
	{ "aArmor", 3 },
	{ "autogun", 4 },
	{ "laze", 5 },
	{ "rocket", 6 }
};
#endif // !GlobalVariable
