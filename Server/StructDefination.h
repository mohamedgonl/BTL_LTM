
#include "stdafx.h"
#ifndef StructDefination_h
#define StructDefination_h
typedef struct DataThread {
	SocketInfo clientInfo[MAX_CLIENT_IN_A_THREAD + 1];
	WSAEVENT events[MAX_CLIENT_IN_A_THREAD + 1];
	DWORD nEvents = 0;
	int hasThread = 0;
};


// define account info struct

struct Account {
	string username;
	string password;
};


// define user session struct info
typedef struct SocketInfo {
	SOCKET connSocket;
	char clientIP[INET_ADDRSTRLEN];
	int clientPort;
};
struct UserInfo {
	string username;
	unsigned int status; // unloggedin = 0  , 1 = waitingroom, 2 = room_member, 3 = room_master, 4 = ingame&live, 5 = ingame_dead
	unsigned int coin = 0;
	unsigned int HP[3] = { 1000, 0, 0 };
	int suntudong[4] = { 50, -200, -200, -200 };
	int laze[4] = { -90, -90, -90, -90 };
	unsigned int rocket = 0;
	int teamId = -1; // -1 if haven't join any team
};
struct LogginSession {
	SocketInfo socketInfo;
	UserInfo userInfo;
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
	unsigned int dame;
};

// define question struct
struct Question {
	int status = 0;
	string description;
	unsigned int key; // key = 1 || 2 || 3 || 4
	unsigned int coin;
};

// define team struct
struct Team {
	unsigned int id;
	string name;
	LogginSession** members;
	int status = 0; // 0 if not ingame, 1 if ingame
	int roomId = -1;
};

// define room struct

struct Room {
	int id;
	int status;
	Team* team1;
	Team* team2;
};


#endif 
