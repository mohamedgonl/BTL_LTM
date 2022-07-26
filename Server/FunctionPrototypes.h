#ifndef FunctionPrototypes
#define FuntionPrototypes
#ifndef HandleWSAEventPrototype
/*
@function getAccountFromTxtFile: push accounts to accounts array
@params:
-pathname: link of file
@return:
-array of string data file
*/
int getAccountFromTxtFile(string pathname);

/*
@function getQuestionFromTxtFile: push questions to questions array
@params:
-pathname: link of file
@return:
-array of string data file
*/
int getQuestionFromTxtFile(string pathname);
void freeSockInfo(LoginSession* siArray, int n);
void closeEventInArray(WSAEVENT* eventArr, int n);
vector<string> splitData(string inlineData, string del);
/*
@function: splitMsg: split message
@params:
- msg: message splitted
@return:
- list of messages splitted
*/
list<char*> splitMsg(char* msg);
/*
@function: Receive data by socket
@params:
- s: socket connect
- buff: buff receive data
- size: size of buff
- flags: options
@return:
- size if buff received
*/
int Receive(SOCKET s, char *buff, int size, int flags);
/*
@function: Send data by socket
@params:
- s: socket connect
- buff: buff send data
- size: size of buff
- flags: options
@return:
- size if buff send success
*/
int Send(SOCKET s, char *buff, int size, int flags);
/*
@function: isNumber: check if number
@params:
- text: string want to check number
@return:
- 1 if is number else 0
*/
int isNumber(char* text);

/*
@function: workingThread: create a working thread
@params: info pass to thread
*/
unsigned __stdcall workingThread(void* params);
void interactWithClient(LoginSession &loginSession, char buff[BUFF_SIZE]);
char* handleResponse(char* it, LoginSession &loginSession);
unsigned __stdcall sendQuestionThread(void* params);

#endif // !HandleWSAEventPrototype

#ifndef HandleUserInput
/*
1. Handle stream data
@param:
- loginSession: a pointer of struct LoginSession poin to info of client
- buff: buff receive from client
@return:
- a queue of statement
*/
queue<char*> recvStreamProcessing(LoginSession &loginSession, char buff[BUFF_SIZE]);

// 2. Login
/*
@params:
-userInfo: a pointer of struct UserInfo point to info of user
-username: username of account recv from client
-password: password of account recv from client
@return:
-RESPONSE_CODE
*/
string loginAccount(UserInfo* userInfo, string username, string password);

// 3. Register
/*
@params:
-userInfo: a pointer of struct UserInfo point to info of user
-username: username of account recv from client
-password: password of account recv from client
@return:
-RESPONSE_CODE
*/
string registerAccount(UserInfo* userInfo, string username, string password);

// 4. Get list all teams
/*
@params:
-userInfo: a pointer of struct UserInfo point to info of user
@return:
-RESPONSE_CODE
*/
string getAllTeams(UserInfo* userInfo);

// 5. Join team
/*
@params:
-userInfo: a pointer of struct UserInfo point to info of user
-teamId: teamId player want to join
@return:
-RESPONSE_CODE
*/
string joinTeam(UserInfo* userInfo, unsigned int teamId);

//6. create team
/*
@params:
-loginSession: a pointer of struct LoginSession point to info of client
-teamName: name of team want to create
@return:
-RESPONSE_CODE
*/
string createTeam(LoginSession* userInfo, string teamName);

//7. Sign out
/*
@params:
-userInfo: a pointer of struct UserInfo point to info of user
@return:
-RESPONSE_CODE
*/
string accountSignOut(UserInfo* userInfo);

// 8. Get out of team
/*
@params:
-userInfo: a pointer of struct UserInfo point to info of user
@return:
-RESPONSE_CODE
*/
string getOutTeam(UserInfo* userInfo);

// 9. Get team members
/*
@params:
-userInfo: a pointer of struct UserInfo point to info of user
@return:
-RESPONSE_CODE
*/
string getTeamMembers(UserInfo* userInfo);

/*
10. Get list user in waiting room
@params:
-loginSession: a pointer of struct LoginSession point to info of client
@return:
-RESPONSE_CODE
*/
string getListUserInWaitingRoom(LoginSession &loginSession);

/*
11. Accept user request to go to our team
@params:
-loginSession: a pointer of struct LoginSession point to info of client
-nameOfRequestUser: name of user who invited
@return:
-RESPONSE_CODE
*/
string acceptRequestJoinTeam(LoginSession &loginSession, string nameOfRequestUser);

/*
12. Decline user request to go to our team
@params:
-loginSession: a pointer of struct LoginSession point to info of client
-nameOfRequestUser: name of user who invited
@return:
-RESPONSE_CODE
*/
string declineRequestJoinTeam(LoginSession &loginSession, string nameOfRequestUser);

/*
13. Invite user to join room
@params:
-loginSession: a pointer of struct LoginSession point to info of client
-usernameUser: name of user who want to invite
@return:
-RESPONSE_CODE
*/
string inviteJoinTeam(LoginSession &loginSession, string usernameUser);

/*
14. Accept invited to join room
@params:
-loginSession: a pointer of struct LoginSession point to info of client
-teamID: id of team accept invite
@return:
-RESPONSE_CODE
*/
string acceptInvitedToJoinTeam(LoginSession &loginSession, int teamID);

/*
15. Decline invited to join room
@params:
-loginSession: a pointer of struct LoginSession point to info of client
-teamID: id of team cecline invite
@return:
-RESPONSE_CODE
*/
string declineInvitedToJoinTeam(LoginSession &loginSession, int teamID);

/*
16. Kick user out room
@params:
-loginSession: a pointer of struct LoginSession point to info of client
-username: username of player want to kick
@return:
-RESPONSE_CODE
*/
string kickUserOutRoom(LoginSession &loginSession, string username);


/*
17. Get all teams
@params:
-loginSession: a pointer of struct LoginSession point to info of client
@return:
-RESPONSE_CODE+(|data|)
*/
string getAllTeams(LoginSession &loginSession);

/*
18. Challenge
@params:
-loginSession: a pointer of struct LoginSession point to info of client
-enemyTeamId: id of team want to challenge
@return:
-RESPONSE_CODE
*/
string challenge(LoginSession &loginSession, int enemyTeamId);

/*
19. Accept challenge
@params:
-loginSession: a pointer of struct LoginSession point to info of client
-enemyTeamId: id of team want to accept challenge
@return:
-RESPONSE_CODE
*/
string acceptChallenge(LoginSession &loginSession, int enemyTeamId);

/*
20. Decline challenge
@params:
-loginSession: a pointer of struct LoginSession point to info of client
-enemyTeamId: id of team want to decline challenge
@return:
-RESPONSE_CODE
*/
string declineChallenge(LoginSession &loginSession, int enemyTeamId);

// 21. Buy item
/*
@params:
-userInfo: a pointer of struct UserInfo point to info of user
-item: name of item want to buy
@return:
-RESPONSE_CODE
*/
string buyItem(UserInfo* userInfo, string item);


// 22. Get info of all players in the game
/*
@params:
-userInfo: a pointer of struct UserInfo point to info of user
@return:
-RESPONSE_CODE
*/
string getAllPlayers(UserInfo* userInfo);

// 23. Get my ingame-info
/*
@params:
-userInfo: a pointer of struct UserInfo point to info of user
@return:
-RESPONSE_CODE
*/
string getMine(UserInfo* userInfo);

/*
24. Attack enemy
@params:
-loginSession: a pointer of struct LoginSession point to info of user
-username: name of enemy want to attack
@return:
-RESPONSE_CODE
*/
string attackEnemy(LoginSession &loginSession, string username);

/*
26. Answer quiz
@params:
-loginSession: a pointer of struct LoginSession point to info of user
-quizId: id of quiz
-key: player's answer of quiz
@return:
-RESPONSE_CODE
*/
string answerQuiz(LoginSession &loginSession, int quizId, string key);

/*
27. Surrender match
@params:
-loginSession: a pointer of struct LoginSession point to info of user
@return:
-RESPONSE_CODE
*/
string surrender(LoginSession &loginSession);
#endif // !HandleUserInput

#endif // !FunctionPrototypes
