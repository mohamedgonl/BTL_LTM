#include "stdafx.h"
#include "StructDefination.h"

Account accounts[];
LoginSession* loginSessions[];
Team* teams[];
Room* rooms[];
Question* questions[];



LoginSession acc[MAX_NUM_ACCOUNT];
CRITICAL_SECTION critical;
list<char*> splitMsg(char* msg);
DataThread dataThread[MAX_THREAD];
int numOfAccount = 0;
int numOfThread = 0;
int numOfConn = 0;
int SERVER_PORT = 5500;

