int SERVER_PORT = 5500;
char* SERVER_ADDR = "127.0.0.1";
int status = 0;
int idYourTeam = -1;
SOCKET global;
int listTeamInvite[1000];
int NumberOfTeamInvite = 0;
int listTeamChallenge[1000];
int NumberOfTeamChallenge = 0;
class User {
public:
	string username;
	int hp;
	int armor;
	int coins;
};
class Team {
public:
	string team_name;
	int id_team;
	int number;
};