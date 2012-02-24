#include "TeamConnection.h"
#include <iostream>
#include <process.h>
#include "GameState.h"
#include <fstream>
#include "Gametime.h"

UDPSocket *listeningSocket = NULL;

Team::Team(Connection source) {
	connection = source;
	socket = new UDPSocket();
}

Team::~Team() {
	if (socket != NULL) {
		delete socket;
		socket = NULL;
	}
}

void Team::send(int* toSend,const int bufLength){//skickar speltillst�nd till spelare
	char msg[4*39];//antal chars som beskriver speltillst�ndet

	for (int i=0; i<bufLength; i++) {//dekonstruerar ett intf�lt till ett charf�lt
		int x = toSend[i];
		int j = i << 2;

		msg[j++] = (byte) ((x >> 0) & 0xff);  // l�ser en byte i taget i varje int

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
bool Team::fromSource(Connection source){
	return connection==source;
}

bool Connection::operator==(Connection b){//likamed operator f�r anslitning
	return b.adress==adress&&b.port==port;
}
Team* homeTeam=0;
Team* awayTeam=0;
unsigned __stdcall recieverThread(void* sock){
	if(homeTeam==NULL||awayTeam==NULL){
		cerr<<"teams not defined"<<endl;
		exit(1);
	}
	Connection source=Connection();
	UDPSocket* socket=(UDPSocket*) sock;
	char buf[BUFLENGTH];//l�st meddelande
	char msg[BUFLENGTH/4+1];//formaterat medelande

	ofstream myfile=ofstream();		//DEBUGVERKTYG
	myfile.open ("commands.txt");	//�pnnar fil d�r kommandon sparas
	homeSerial->write(NULL,0);//slutar kalibrara mikrokontrollerna n�r spelet b�rjar
	awaySerial->write(NULL,0);
	for(;;){// recieves commands and passes them on to correct microprocessor
		int rcvBytes=socket->recvFrom(buf,BUFLENGTH,source.adress,source.port);//tar emot meddelande
		int index=0;
		myfile<<getGametime()<<"\t";//sparar speltiden n�r kommandot mottogs
		for (int i = 0; i < rcvBytes; i = i + 4){//l�ser intf�lt som ett charf�lt av kommandon
			msg[index] = buf[i];
			myfile << (int)(index % 5 == 3 ? (signed char)msg[index] : (unsigned char)msg[index]) << "\t";
			index++;
		}
		myfile<<endl;
		if(homeTeam->fromSource(source)){//skickar vidare kommandot beroende p� vartifr�n meddelandet kom ifr�n
			homeSerial->write(msg,rcvBytes/4);//skriver till mikrokontroller
		}else if(awayTeam->fromSource(source)){
			awaySerial->write(msg,rcvBytes/4);
		}
		
	}
}
