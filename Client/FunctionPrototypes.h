vector<string> splitData(string inlineData, string del);

int isNumber(string text);

vector<string> getPersionalInfo(string s);
vector<User> getAllUserIngame(string s);
vector<Team> handleSplitStr(string s);
void declineJoinTeam(char *res);
void handleResponse(char* res);
int showMenu();
unsigned __stdcall workerThread(void *param);
int Send(SOCKET s, char *buff, int size, int flags);

int Receive(SOCKET s, char *buff, int size, int flags);

