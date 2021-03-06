#include "PracticalSocket.h"
#include <string> 
#define BUFLENGTH 255
#define PORT 60040
#ifndef CONNECTION
#define CONNECTION
struct Connection{//struct f�r att h�lla reda p� anslutnng
	std::string adress;
	unsigned short port;
	bool operator==(Connection b);
	Connection(){adress="";port=0;};
};
class Team{// klass f�r att sk�ta kommunikation med AI-modul
	Connection connection;
	UDPSocket* socket;
	int lastAlive;
	
	public:
		void send(int* buf,const int bufLength);
		void sendBytes(char *buf, const int bufLength);
		bool fromSource(Connection source);
		void command(int com[BUFLENGTH],int length);
		void reportAlive();
		bool isAlive();
		Team(Connection source);
		~Team();
};
extern Team* homeTeam;
extern Team* awayTeam;

extern UDPSocket *listeningSocket;

unsigned __stdcall recieverThread(void* param);
unsigned __stdcall checkClientsProc(void *param);

#endif