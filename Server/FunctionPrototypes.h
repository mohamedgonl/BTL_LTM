#ifndef FunctionPrototypes
#define FuntionPrototypes
void freeSockInfo(LoginSession* siArray, int n);
queue<char*> recvStreamProcessing(LoginSession &loginSession, char buff[BUFF_SIZE]);
void closeEventInArray(WSAEVENT* eventArr, int n);
vector<string> splitData(string inlineData, string del);
int Send(SOCKET s, char *buff, int size, int flags);
int Receive(SOCKET s, char *buff, int size, int flags);
unsigned __stdcall workingThread(void* params);
void interactWithClient(LoginSession &loginSession, char buff[BUFF_SIZE]);
list<char*> splitMsg(char* msg);
int isNumber(char* text);
char* handleResponse(char* it, LoginSession &loginSession);
string acceptRequestJoinTeam(LoginSession &loginSession, string nameOfRequestUser);
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
string attackEnemy(LoginSession &loginSession, string username);
string answerQuiz(LoginSession &loginSession, int quizId, string key);
string surrender(LoginSession &loginSession);
unsigned __stdcall sendQuestionThread(void* params);
void createQuestion();
#endif // !FunctionPrototypes
