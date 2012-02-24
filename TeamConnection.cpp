#include "TeamConnection.h"
#include <iostream>
#include <process.h>
#include "GameState.h"
#include <fstream>
#include "Gametime.h"
Team::Team(UDPSocket* sock,Connection source,bool home){
	connection=source;
	socket=sock;
	teamPlayerShift=0;
	if(!home){
		teamPlayerShift=6;
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
bool Team::fromSource(Connection source){
	return connection==source;
}

bool Connection::operator==(Connection b){//likamed operator för anslutning
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
	char buf[BUFLENGTH];//läst meddelande
	char msg[BUFLENGTH/4+1];//formaterat medelande

	ofstream myfile=ofstream();		//DEBUGVERKTYG
	myfile.open ("commands.txt");	//öpnnar fil där kommandon sparas
	homeSerial->write(NULL,0);//slutar kalibrara mikrokontrollerna när spelet börjar
	awaySerial->write(NULL,0);
	for(;;){// recieves commands and passes them on to correct microprocessor
		int rcvBytes=socket->recvFrom(buf,BUFLENGTH,source.adress,source.port); //tar emot meddelande
		int index=0;
		myfile<<getGametime()<<"\t";//sparar speltiden när kommandot mottogs
		for (int i=0;i<rcvBytes;i=i+4){//läser intfält som ett charfält av kommandon
			msg[index++]=buf[i];
			myfile<<(int)(unsigned char)msg<<"\t";
		}
		myfile<<endl;
		if(homeTeam->fromSource(source)){//skickar vidare kommandot beroende på vartifrån meddelandet kom ifrån

			//här borde spelbegränsningar hamna
			char homeStatus[100];
			int lengthHome=homeSerial->read(homeStatus);
			// kontrollera om det finns splare som inte kommit fram dit de ska ännu
			// Måste spara undan alla komandon så att det går att kolla vilka som inte är färdiga ännu.
			// slut på spelbegränsningar
			homeSerial->write(msg,rcvBytes/4);//skriver till mikrokontroller
		}else if(awayTeam->fromSource(source)){
			awaySerial->write(msg,rcvBytes/4);
		}
		
	}
}
