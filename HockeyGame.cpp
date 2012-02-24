#include "HockeyGame.h"
#include "PracticalSocket.h"
#include "TeamConnection.h"
#include <iostream>
#include "GameState.h"
#include "SerialConnection.h"
#include "Gametime.h"
using namespace std;

bool HockeyGame::setUpConnections(){//upprättar anslutning AI-moduler
	if(homeTeam==NULL||awayTeam==NULL){
		listeningSocket = new UDPSocket(PORT);//objekt för udp-kommunikation

		Connection homeSource=Connection();//objekt som innehåller information om anslutningar, se TeamConnection.cpp
		Connection awaySource=Connection();
		char* buf[BUFLENGTH];//skapar en buffert i minnet att lägga motagna meddelanden

		int handshake[]={1};//skapar ett handskaknings meddelande

		cout<<"waiting at homeplayer"<<endl;

		listeningSocket->recvFrom(buf,BUFLENGTH,homeSource.adress,homeSource.port);//Väntar på handskakning från AI-modul
		homeTeam = new Team(homeSource);//Sparar AImodulens plats
		homeTeam->send(handshake,1);//Skickar hanskakning

		cout<<"home aquired at port "<<homeSource.port<<endl;
		cout<<"waiting at awayplayer"<<endl;

		handshake[0]=2;//ny hanskakning för andra laget
	
		listeningSocket->recvFrom(buf,BUFLENGTH,awaySource.adress,awaySource.port);
		awayTeam = new Team(awaySource);
		awayTeam->send(handshake,4);
		cout<<"away aquired at port "<<awaySource.port<<endl;
		if(homeTeam->fromSource(awaySource)){//kollar att int samma AI har anslutit igen, om så ge fel
			cout<<"team already aquired, next team must be on another port"<<endl;
			return false;
		}
		recieverThreadHandle=(HANDLE)_beginthreadex(NULL,0,recieverThread,(void*)listeningSocket,CREATE_SUSPENDED,NULL);//startar tråden pausad, se teamConnections.cpp
		clientsAliveThreadHandle = (HANDLE)_beginthreadex(NULL, 0, checkClientsProc, NULL, CREATE_SUSPENDED, NULL);
	}
	return true;
}
bool HockeyGame::setUpGamestate(){
	if(!initializeMicroControllers()){
		return false;
	}
	senderThreadHandle=(HANDLE)_beginthreadex(NULL,0,senderThread,NULL,CREATE_SUSPENDED,NULL);//skapar tråden pausad
	
	return true;
}
bool HockeyGame::setUpCamera(){
	if(!initializeTracking()){ //startar bildhantering, se gamestate.cpp, definierar track_puck
		return false;
	}
	cameraThreadHandle=(HANDLE)_beginthreadex(NULL,0,cameraThread,NULL,CREATE_SUSPENDED,NULL);//skapar kameratråden pausad
	return true;
}
bool HockeyGame::initializeGame(){
	//creates the gamethreads suspended
	if(!running){
		running=true;
		if(!setUpCamera()){//avbryter starten om inte kameran kan startas
			running= false;
			return running;
		}
		if(!setUpGamestate()){
			running= false;
			return running;
		}
		if(!setUpConnections()){
			running= false;
			return running;
		}
	

	
		paused=true;
		
		cout<<"successfully initialized game: starting gamethreads"<<endl;
		startGametime();//definieras i Gametime.cpp
		//starts the threads
		resumeGame();
		
	}else{
		cout<<"can't initialize: game already running"<<endl;
	}
	return running;
}

void safeTerminateThread(HANDLE threadHandle) {
	if (threadHandle != NULL)
		TerminateThread(threadHandle, 0);
}

void HockeyGame::stopGame(){//avslutar spelet
	if(running){
		safeTerminateThread(senderThreadHandle);
		safeTerminateThread(recieverThreadHandle);
		safeTerminateThread(cameraThreadHandle);
		safeTerminateThread(clientsAliveThreadHandle);

		// TODO: Make/find delete and null-function
		if (homeTeam != NULL) {
			delete homeTeam;
			homeTeam = NULL;
		}
		if (awayTeam != NULL) {
			delete awayTeam;
			awayTeam = NULL;
		}
		if (listeningSocket != NULL) {
			delete listeningSocket;
			listeningSocket = NULL;
		}

		running=false;
		cout << "stopped" << endl;
	}else{
		cout<<"can't stop game: game isn't running"<<endl;
	}
}

void safeSuspendThread(HANDLE handle) {
	if (handle != NULL)
		SuspendThread(handle);
}

void HockeyGame::pauseGame(){//pausa spelet
	if(running&&!paused){
		paused=true;
		pauseGametime();

		safeSuspendThread(senderThreadHandle);
		safeSuspendThread(recieverThreadHandle);
		safeSuspendThread(cameraThreadHandle);
		safeSuspendThread(clientsAliveThreadHandle);

		cout << "paused" << endl;
	}else{
		cout<<"can't pause game"<<endl;
	}
}

void safeResumeThread(HANDLE handle) {
	if (handle != NULL)
		ResumeThread(handle);
}

void HockeyGame::resumeGame(){
	if(running&&paused){
		paused=false;
		resumeGametime();

		safeResumeThread(senderThreadHandle);
		safeResumeThread(recieverThreadHandle);
		safeResumeThread(cameraThreadHandle);
		safeResumeThread(clientsAliveThreadHandle);

		cout << "resumed" << endl;
	}else{
		cout<<"can't resume game"<<endl;
	}
}