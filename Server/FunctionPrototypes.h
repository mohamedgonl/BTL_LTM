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


