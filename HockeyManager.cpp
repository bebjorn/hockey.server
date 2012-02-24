#include <iostream>
#include "HockeyManager.h"
#include <conio.h>
#include "HockeyGame.h"
#include "CalibrationMode.h"
#include "calibrateMicroController.h"
using namespace std;
int main(){
	bool quit=false;
	
	greeter();//skriver ut välkomsttext
	showHelp();// skriver ut hjälp
	HockeyGame game=HockeyGame();
	while(!quit){
		switch(getch()){//lyssnar efter input från tangetbord
		case 'i':
				cout<<"initializing"<<endl;
				game.initializeGame();
			
			break;
		case 'p':
			
				cout<<"pausing"<<endl;
				game.pauseGame();
			
			break;
		case 'r':
			
				cout<<"resuming"<<endl;
				game.resumeGame();
			
			break;
		case 's':
			
				
				game.stopGame();
			

			break;
		case 'q':
			quit=true;//avslutar loopen
			cout<<"quiting"<<endl;
			break;
		case 'a':
			calibrate();// startar bildkalibrerings program
			break;
		case 'h':
			showHelp();
			break;
		case 'c':
			calibrateMicroControllers();
			break;
		default:
			;
		}
	}
}

void showHelp(){
	cout<<"i - initialize game"<<endl
		<<"p - pause game"<<endl
		<<"s - stop game"<<endl
		<<"q - quit game"<<endl
		<<"h - show this help"<<endl
		<<"c - calibrate microcontrollers"<<endl
		<<"a - calibrate image processing"<<endl;

}
void greeter(){
	cout<<"Welcome to the managing program for CIHC"<<endl;
}
