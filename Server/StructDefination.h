#include "stdafx.h"
#ifndef StructDefination_h
#define StructDefination_h

struct UserInfo {
	string username = "";
	unsigned int status = 0; // unloggedin = 0  , 1 = waitingroom, 2 = room_member, 3 = room_master, 4 = ingame&live, 5 = ingame_dead
	unsigned int coin = 0;
	unsigned int HP[3] = { 1000, 0, 0 };
	int sungtudong[4] = { 50, -200, -200, -200 };
	int laze[4] = { -90, -90, -90, -90 };
	unsigned int rocket = 0;
	clock_t lastTimeATK = 0;
	int teamId = -1; // -1 if haven't join any team
};

// define user session struct info
struct SocketInfo {
	SOCKET connSocket;
	char clientIP[INET_ADDRSTRLEN];
	int clientPort;
};

struct LoginSession {
	SocketInfo socketInfo;
	UserInfo userInfo;
	char buff[2048];
};

struct DataThread {
	LoginSession loginSession[MAX_CLIENT_IN_A_THREAD + 1];
	WSAEVENT events[MAX_CLIENT_IN_A_THREAD + 1];
	DWORD nEvents = 0;
	int hasThread = 0;
};

// define account info struct
struct Account {
	string username;
	string password;
};

// item struct define
struct Item_Armor {
	string name;
	unsigned int price;
	unsigned int armor;
};

struct Item_Attack {
	string name;
	unsigned int price;
	unsigned int b_price; // price of bullets
	unsigned int numBullets;
	unsigned int dameB; // dame of a bullet
};

struct QuestionDescription {
	string question = "";
	string answers[4] = { "","","","" };
	string key = "";
	unsigned int coin = 0;
};

// define question struct
struct Question {
	int id = -1;
	int status = 0; // 0 has answered, 1: answered
	QuestionDescription* description;
};

// define team struct
struct Team {
	int id = -1;
	string name = "";
	LoginSession* members[3] = { {},{},{} };
	int status = 0; // 0 if not ingame, 1 if ingame
	int roomId = -1;
};

// define room struct

struct Room {
	int id = -1;
	int status = 0;
	Team* team1;
	Team* team2;
	Question questions[MAX_QUESTION];
};

#endif 
