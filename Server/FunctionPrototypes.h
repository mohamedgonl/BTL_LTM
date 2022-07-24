#ifndef FunctionPrototypes
#define FuntionPrototypes
#ifndef HandleWSAEventPrototype
int getAccountFromTxtFile(string pathname);
int getQuestionFromTxtFile(string pathname);
void freeSockInfo(LoginSession* siArray, int n);
void closeEventInArray(WSAEVENT* eventArr, int n);
vector<string> splitData(string inlineData, string del);
list<char*> splitMsg(char* msg);
int Receive(SOCKET s, char *buff, int size, int flags);
int Send(SOCKET s, char *buff, int size, int flags);
int isNumber(char* text);
unsigned __stdcall workingThread(void* params);
void interactWithClient(LoginSession &loginSession, char buff[BUFF_SIZE]);
char* handleResponse(char* it, LoginSession &loginSession);
unsigned __stdcall sendQuestionThread(void* params);

#endif // !HandleWSAEventPrototype

#ifndef HandleUserInput
queue<char*> recvStreamProcessing(LoginSession &loginSession, char buff[BUFF_SIZE]);
string loginAccount(UserInfo* userInfo, string username, string password);
string registerAccount(UserInfo* userInfo, string username, string password);
string getAllTeams(UserInfo* userInfo);
string joinTeam(UserInfo* userInfo, unsigned int teamId);
string createTeam(LoginSession* userInfo, string teamName);
string accountSignOut(UserInfo* userInfo);
string getOutTeam(UserInfo* userInfo);
string getTeamMembers(UserInfo* userInfo);
string getListUserInWaitingRoom(LoginSession &loginSession);
string acceptRequestJoinTeam(LoginSession &loginSession, string nameOfRequestUser);
string declineRequestJoinTeam(LoginSession &loginSession, string nameOfRequestUser);
string inviteJoinTeam(LoginSession &loginSession, string usernameUser);
string acceptInvitedToJoinTeam(LoginSession &loginSession, int teamID);
string declineInvitedToJoinTeam(LoginSession &loginSession, int teamID);
string kickUserOutRoom(LoginSession &loginSession, string username);
string getAllTeams(LoginSession &loginSession);
string challenge(LoginSession &loginSession, int enemyTeamId);
string acceptChallenge(LoginSession &loginSession, int enemyTeamId);
string declineChallenge(LoginSession &loginSession, int enemyTeamId);
string buyItem(UserInfo* userInfo, string item);
string getAllPlayers(UserInfo* userInfo);
string getMine(UserInfo* userInfo);
string attackEnemy(LoginSession &loginSession, string username);
string answerQuiz(LoginSession &loginSession, int quizId, string key);
string surrender(LoginSession &loginSession);
#endif // !HandleUserInput

#endif // !FunctionPrototypes
