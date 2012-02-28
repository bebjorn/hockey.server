#include "TeamConnection.h"
#include <iostream>
#include <process.h>
#include "GameState.h"
#include <fstream>
#include "Gametime.h"
#include "HockeyGame.h"
#include "Limits.h"

UDPSocket *listeningSocket = NULL;

Team::Team(Connection source) {
	connection = source;
	socket = new UDPSocket();
	lastAlive = 0;
}

Team::~Team() {
	if (socket != NULL) {
		delete socket;
		socket = NULL;
	}
}
void Team::send(int* toSend,const int bufLength){//skickar speltillstånd till spelare
	char msg[4*39];//antal chars som beskriver speltillståndet

	for (int i=0; i<bufLength; i++) {//dekonstruerar ett intfält till ett charfält
		int x = toSend[i];
		int j = i << 2;

		msg[j++] = (byte) ((x >> 0) & 0xff);  // läser en byte i taget i varje int

		msg[j++] = (byte) ((x >> 8) & 0xff);
		msg[j++] = (byte) ((x >> 16) & 0xff);
		msg[j++] = (byte) ((x >> 24) & 0xff);

	}
	/*for(int k=4*9;k<4*10;k++){
		int a=msg[k];
		cout<<a<<"\t";
	}
	cout<<endl;*/
	socket->sendTo(msg,4*bufLength,connection.adress,connection.port);//Skickar meddelandet till AI-modul

}

void Team::sendBytes(char *msg, const int bufLength) {
	socket->sendTo(msg,bufLength,connection.adress,connection.port);//Skickar meddelandet till AI-modul
}

bool Team::fromSource(Connection source){
	return connection==source;
}

void Team::reportAlive() {
	lastAlive = getGametime();
}

bool Team::isAlive() {
	int currentTime = getGametime();
	return (currentTime - lastAlive) < 10000;
}

bool Connection::operator==(Connection b){//likamed operator för anslitning
	return b.adress==adress&&b.port==port;
}

Team* homeTeam=0;
Team* awayTeam=0;

Team *teamFromConnection(Connection *pConnection) {
	if (homeTeam->fromSource(*pConnection)) 
		return homeTeam;
	else if(awayTeam->fromSource(*pConnection))
		return awayTeam;
	return NULL;
}

unsigned __stdcall recieverThread(void* sock){
	//###### Player comands are saved here
	char homeCmd[30];
	char awayCmd[30];
	//######
	if(homeTeam==NULL||awayTeam==NULL){
		cerr<<"teams not defined"<<endl;
		exit(1);
	}
	Connection source=Connection();
	UDPSocket* socket=(UDPSocket*) sock;
	char buf[BUFLENGTH];//läst meddelande
	char msg[BUFLENGTH/4+1];//formaterat medelande

	ofstream myfile=ofstream();		//DEBUGVERKTYG
	myfile.open ("commands.txt");	//öpnnar fil där kommandon sparas
	homeSerial->write(NULL,0);//slutar kalibrara mikrokontrollerna när spelet börjar
	awaySerial->write(NULL,0);
	for(;;){// recieves commands and passes them on to correct microprocessor
		int rcvBytes=socket->recvFrom(buf,BUFLENGTH,source.adress,source.port); //tar emot meddelande
		Team *team = teamFromConnection(&source);
		if (team != NULL) {
			if (rcvBytes == 1) {
				if (buf[0] == 'N')
					team->reportAlive();
			}
			else if ((rcvBytes % (5 * 4)) == 0) {
				int index = 0;
				int cmdIndex = 0;
				for (int cmdIndex = 0; cmdIndex < rcvBytes / 5 / 4; cmdIndex++) {
					myfile << getGametime() << "\t";			//sparar speltiden när kommandot mottogs
					char cmd[5];
					for (int i = 0; i < 5; i++) {	//läser intfält som ett charfält av kommandon
						cmd[i] = buf[cmdIndex * 5 * 4 + i * 4];
						myfile << (int)(i == 3 ? (signed char)msg[i] : (unsigned char)msg[i]) << "\t";
					}
					// one command should now be in msg
					if (isCommandOkay(team == homeTeam ? 0 : 1, cmd)) {
						memcpy(msg + index, cmd, 5);
						index += 5;
						myfile << "+";
					}
					else myfile << "-";
					myfile<<endl;
				}

				
				if (team == homeTeam)//skickar vidare kommandot beroende på vartifrån meddelandet kom ifrån
					homeSerial->write(msg, index);//skriver till mikrokontroller
				else if(team == awayTeam)
					awaySerial->write(msg, index);			
			}
		}
	}
}

bool checkClient(Team *pTeam) {
	char c = 'D';
	int i = 0;
	cout << "Checking team alive..." << endl;

	while (!pTeam->isAlive() && i < 5) {
		pTeam->sendBytes(&c, 1);
		i++;
		Sleep(1000);
	}
	if (!pTeam->isAlive()) {
		cout << "Team not alive!" << endl;
		return false;
	}
	cout << "Team alive :-)" << endl;
	return true;
}

unsigned __stdcall checkClientsProc(void *param) {
	while (true) {
		if (!checkClient(homeTeam))
			((HockeyGame *)param)->stopGame();
		if (!checkClient(awayTeam))
			((HockeyGame *)param)->stopGame();
		Sleep(10000);
	}
}
