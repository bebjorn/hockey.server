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
	int teamPlayerShift;
	
	public:
		void send(int* buf,const int bufLength);
		bool fromSource(Connection source);
		void command(int com[BUFLENGTH],int length);
		Team(UDPSocket* sock,Connection source,bool homeTeam=true);
	
};
extern Team* homeTeam;
extern Team* awayTeam;


unsigned __stdcall recieverThread(void* param);

#endif