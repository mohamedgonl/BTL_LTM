int showMenu();
int isNumber(string text);
list<char*> splitMsg(char* msg);
void handleResponse(char* res);
string handleUserInput(string userInput);
int Send(SOCKET s, char *buff, int size, int flags);
int Receive(SOCKET s, char *buff, int size, int flags);

