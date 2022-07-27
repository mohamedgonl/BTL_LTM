#ifndef FunctionPrototypes
#define FuntionPrototypes
#ifndef HandleWSAEventPrototype

/**
* @function getAccountFromTxtFile: push accounts to accounts array
* @params pathname: link of file
* @return: array of string data file
**/
int getAccountFromTxtFile(string pathname);

/**
* @function getQuestionFromTxtFile: push questions to questions array
* @params: pathname link of file
* @return: array of string data file
*/
int getQuestionFromTxtFile(string pathname);

/**
* @funtion freeSockInfo: close socket, free it in loginSessions and teams.
* @param arr: a LoginSession pointer.
* @param n: location of element want to free.
**/
void freeSockInfo(LoginSession* siArray, int n);

/**
* @funtion closeEventInArray: close event.
* @param eventArr: a WSAEVENT pointer.
* @param n: location of element want to close event.
**/
void closeEventInArray(WSAEVENT* eventArr, int n);

/**
* @funtion splitData: split Data into messages which is separated by Separator character.
* @param inlineData: Data.
* @param del: separator character
* @return: vetor string of message.
**/
vector<string> splitData(string inlineData, string del);

/**
* @funtion splitMsg: split conjoined message into separate messages.
* @param msg: a char pointer.
* @return: list char* of separate messages.
**/
list<char*> splitMsg(char* msg);

/**
* @function: Receive data by socket
* @params s: socket connect
* @params buff: buff receive data
* @params size: size of buff
* @params flags: options
* @return: size if buff received
**/
int Receive(SOCKET s, char *buff, int size, int flags);

/**
* @function: Send data by socket
* @params s: socket connect
* @params buff: buff send data
* @params size: size of buff
* @params flags: options
* @return: size if buff send success
**/
int Send(SOCKET s, char *buff, int size, int flags);

/**
* @funtion isNumber: confirm param text is number or not.
* @param text: a string.
* @return:	0 if text is not a number.
			1 if text is a number
**/
int isNumber(char* text);

/**
* @function: workingThread: create a working thread
* @params: info pass to thread
**/
unsigned __stdcall workingThread(void* params);

/**
* @funtion interactWithClient: receive and send data to client.
* @param loginSession: a LoginSession address.
* @param buff: a char array to receive from client
**/
void interactWithClient(LoginSession &loginSession, char buff[BUFF_SIZE]);

/**
* @funtion handleResponse: handle every message from client and build responses.
* @param it: a separate message from client.
* @param loginSession: a LoginSession address.
**/
char* handleResponse(char* it, LoginSession &loginSession);

/**
* @funtion sendQuestionThread: thread send question to clients.
* @param it: a separate message from client.
* @param loginSession: a LoginSession address.
**/
unsigned __stdcall sendQuestionThread(void* params);

#endif // !HandleWSAEventPrototype

#ifndef HandleUserInput
/**
1. Handle stream data
* @param loginSession: a pointer of struct LoginSession poin to info of client
* @param buff: buff receive from client
* @return: a queue of statement
**/
queue<char*> recvStreamProcessing(LoginSession &loginSession, char buff[BUFF_SIZE]);

// 2. Login
/**
* @params userInfo: a pointer of struct UserInfo point to info of user
* @params username: username of account recv from client
* @params password: password of account recv from client
* @return: RESPONSE_CODE
**/
string loginAccount(UserInfo* userInfo, string username, string password);

// 3. Register
/**
* @params userInfo: a pointer of struct UserInfo point to info of user
* @params username: username of account recv from client
* @params password: password of account recv from client
* @return: RESPONSE_CODE
**/
string registerAccount(UserInfo* userInfo, string username, string password);

// 4. Get list all teams
/**
* @params userInfo: a pointer of struct UserInfo point to info of user
* @return: RESPONSE_CODE
**/
string getAllTeams(UserInfo* userInfo);

// 5. Join team
/**
* @params userInfo: a pointer of struct UserInfo point to info of user
* @params teamId: teamId player want to join
* @return: RESPONSE_CODE
**/
string joinTeam(UserInfo* userInfo, unsigned int teamId);

//6. create team
/**
* @params loginSession: a pointer of struct LoginSession point to info of client
* @params teamName: name of team want to create
* @return: RESPONSE_CODE
**/
string createTeam(LoginSession* userInfo, string teamName);

//7. Sign out
/**
* @params: userInfo: a pointer of struct UserInfo point to info of user
* @return: RESPONSE_CODE
**/
string accountSignOut(UserInfo* userInfo);

// 8. Get out of team
/**
* @params userInfo: a pointer of struct UserInfo point to info of user
* @return: RESPONSE_CODE
**/
string getOutTeam(UserInfo* userInfo);

// 9. Get team members
/**
* @params userInfo: a pointer of struct UserInfo point to info of user
* @return: RESPONSE_CODE
**/
string getTeamMembers(UserInfo* userInfo);

/**
10. Get list user in waiting room
* @params loginSession: a pointer of struct LoginSession point to info of client
* @return: RESPONSE_CODE
**/
string getListUserInWaitingRoom(LoginSession &loginSession);

/**
11. Accept user request to go to our team
* @params loginSession: a pointer of struct LoginSession point to info of client
* @params nameOfRequestUser: name of user who invited
* @return: RESPONSE_CODE
**/
string acceptRequestJoinTeam(LoginSession &loginSession, string nameOfRequestUser);

/**
12. Decline user request to go to our team
* @params loginSession: a pointer of struct LoginSession point to info of client
* @params nameOfRequestUser: name of user who invited
* @return: RESPONSE_CODE
**/
string declineRequestJoinTeam(LoginSession &loginSession, string nameOfRequestUser);

/**
13. Invite user to join room
* @params loginSession: a pointer of struct LoginSession point to info of client
* @params usernameUser: name of user who want to invite
* @return: RESPONSE_CODE
**/
string inviteJoinTeam(LoginSession &loginSession, string usernameUser);

/**
14. Accept invited to join room
* @params loginSession: a pointer of struct LoginSession point to info of client
* @params teamID: id of team accept invite
* @return: RESPONSE_CODE
**/
string acceptInvitedToJoinTeam(LoginSession &loginSession, int teamID);

/**
15. Decline invited to join room
* @params loginSession: a pointer of struct LoginSession point to info of client
* @params teamID: id of team cecline invite
* @return: RESPONSE_CODE
**/
string declineInvitedToJoinTeam(LoginSession &loginSession, int teamID);

/**
16. Kick user out room
* @params loginSession: a pointer of struct LoginSession point to info of client
* @params username: username of player want to kick
* @return: RESPONSE_CODE
**/
string kickUserOutRoom(LoginSession &loginSession, string username);

/**
17. Get all teams
* @params loginSession: a pointer of struct LoginSession point to info of client
* @return: RESPONSE_CODE+(|data|)
**/
string getAllTeams(LoginSession &loginSession);

/**
18. Challenge
* @params loginSession: a pointer of struct LoginSession point to info of client
* @params enemyTeamId: id of team want to challenge
* @return: RESPONSE_CODE
**/
string challenge(LoginSession &loginSession, int enemyTeamId);

/**
19. Accept challenge
* @params loginSession: a pointer of struct LoginSession point to info of client
* @params enemyTeamId: id of team want to accept challenge
* @return: RESPONSE_CODE
**/
string acceptChallenge(LoginSession &loginSession, int enemyTeamId);

/**
20. Decline challenge
* @params loginSession: a pointer of struct LoginSession point to info of client
* @params enemyTeamId: id of team want to decline challenge
* @return: RESPONSE_CODE
**/
string declineChallenge(LoginSession &loginSession, int enemyTeamId);

// 21. Buy item
/**
* @params userInfo: a pointer of struct UserInfo point to info of user
* @params item: name of item want to buy
* @return: RESPONSE_CODE
**/
string buyItem(UserInfo* userInfo, string item);

// 22. Get info of all players in the game
/**
* @params userInfo: a pointer of struct UserInfo point to info of user
* @return: RESPONSE_CODE
**/
string getAllPlayers(UserInfo* userInfo);

// 23. Get my ingame-info
/**
* @params userInfo: a pointer of struct UserInfo point to info of user
* @return: RESPONSE_CODE
**/
string getMine(UserInfo* userInfo);

/**
24. Attack enemy
* @params loginSession: a pointer of struct LoginSession point to info of user
* @params username: name of enemy want to attack
* @return: RESPONSE_CODE
**/
string attackEnemy(LoginSession &loginSession, string username);

/**
26. Answer quiz
* @params loginSession: a pointer of struct LoginSession point to info of user
* @params quizId: id of quiz
* @params key: player's answer of quiz
* @return: RESPONSE_CODE
**/
string answerQuiz(LoginSession &loginSession, int quizId, string key);

/**
27. Surrender match
* @params loginSession: a pointer of struct LoginSession point to info of user
* @return: RESPONSE_CODE
**/
string surrender(LoginSession &loginSession);
#endif // !HandleUserInput

#endif // !FunctionPrototypes
