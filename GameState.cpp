#include <vector>
#include <process.h>
#include "GameState.h"
#include "TeamConnection.h"
#include "CamCapture.h"
#include "ObjectTracker.h"
#include "cv.h"
#include "highgui.h"
#include "GameState.h"
#include "SerialConnection.h"
#include "Gametime.h"
#include <iostream>
#include <fstream>
#define DISTANCETOGOAL 300
using namespace std;
bool running=false;
bool paused=false;
SerialConnection* homeSerial;
SerialConnection* awaySerial;
CamCapture* capture;
ObjectTracker track_puck;
Puck puck;
int gameTime=0;
int gameStartTime=0;
bool trackingInitialized=false;
bool micConInitialized=false;
bool initializeTracking(){
	if(!trackingInitialized){
	
		capture = new CamCapture();
		
		if (!capture->initCam()){
			fprintf(stderr,"Could not initialize capturing...\n");
			return false;
		}
		try{
			/*skapar ett bildbehandlings objekt somletar efter grönt på spelplanen, se Objecttracker.cpp*/
			track_puck = ObjectTracker(cvLoadImage("playField.bmp",0),"green");
		}catch(exception e){
			cout<<"no mask or color information"<<endl;
			return false;
		}
		trackingInitialized=true;

		//tar en bild och använder den för att skapa en puckrepresentation
		IplImage* frame = cvCreateImage(IMAGESIZE,8,3);
		capture->myQueryFrame(frame);
		puck=Puck(frame);
	}
	return true;
}
bool initializeMicroControllers(){
	if(homeSerial==NULL){//skapar en seriell anslutning till en mikrokontroller om den inte redan finns
		homeSerial=new SerialConnection("COM3");
		//homeSerial->write(NULL,0,"a");//ber mikrokontrollern att kalibrera sig
	}
	
	if(awaySerial==NULL){
		awaySerial=new SerialConnection("COM4");//likadant för andra laget
		//awaySerial->write(NULL,0,"a");
	}
	
	if(!homeSerial->isAvailable()||!awaySerial->isAvailable()){//kollar om anslutning finns med mikrokontroller
		cout<<"error: could not connect to microprocessors"<<endl;
		cout<<"please connect them to COM ports 3 and 4 and shut down all other processes communicating with them"<<endl;
 		return false;
	}
	return true;
}
unsigned __stdcall cameraThread(void* param){
	CvPoint2D32f* puckPoint=new CvPoint2D32f();
	IplImage* frame = cvCreateImage(IMAGESIZE,8,3);//skapar en buffer för en bild
	int t0=0,t1=0,t2=0,t3=0;//DEBUGVERKTYG

	while(running){
		t0=clock();
		//fyller buffren med en ny bild
		capture->myQueryFrame(frame);//40ms
		t1=clock();
		//hanterar bilden
		track_puck.trackObject(frame,puckPoint);//20ms
		t2=clock();
		//uppdaterar puckens position med ny data
		puck.updatePosition(puckPoint);
		t3=clock();
		//float cycle=(GetTickCount()-t);
		//cout<<t0-t1<<"\t"<<t1-t2<<"\t"<<t3-t2<<endl;
		//

	}
	return NULL;
}

unsigned __stdcall senderThread(void* param){

	cout<<"senderthread started"<<endl;
	const int MESSAGELENGTH=27;
	char homeStatus[100];
	char awayStatus[100];
	int homeMessage[MESSAGELENGTH];
	int awayMessage[MESSAGELENGTH];
	ofstream myfile=ofstream();
	myfile.open ("measurements.txt");
	time_t msec = time(NULL) * 1000;
	
	int t=0;
	while(running){//bygger meddelanden och skickar dem till AI-modulerna
		//long t=clock();
		Sleep(50);
		//cout<<" cycletime: "<<clock()-t<<endl;
		//t =clock();
		int lengthHome=homeSerial->read(homeStatus);
		int lengthAway=awaySerial->read(awayStatus);
		
		int index=0;
		awayMessage[index]=puck.x;
		homeMessage[index++]=puck.x;

		awayMessage[index]=puck.y;
		homeMessage[index++]=puck.y;

		awayMessage[index]=getGametime();
		homeMessage[index++]=getGametime();
;//getGametime();

		myfile<<getGametime()<<"\t";
		for(int i=0;i<12;i++){
			awayMessage[index]=(awayStatus[i]&0xff);//awayStatus[i];
			homeMessage[index++]=(homeStatus[i]&0xff);
			
			myfile<<(int)homeStatus[i]<<"\t";
			
		}
		
		for(int i=0;i<12;i++){
			awayMessage[index]=(homeStatus[i]&0xff);
			homeMessage[index++]=(awayStatus[i]&0xff);//awayStatus[i];
			
			myfile<<(int)homeStatus[i]<<"\t";
		}
		
		myfile<<endl;
		homeTeam->send(awayMessage,MESSAGELENGTH);
		awayTeam->send(homeMessage,MESSAGELENGTH);
		//cout<<clock()-t<<endl;;
		
	}
	
	return NULL;
}
Puck::Puck(IplImage* frame){
		x=0;
		y=0;
		//ObjectTracker track_goal1 = ObjectTracker(cvLoadImage("goal1.bmp",0),"red");//object som letar efter ena målet
		//ObjectTracker track_goal2 = ObjectTracker(cvLoadImage("goal2.bmp",0),"red");//object som letar efter andra målet
		
		
		goal1=cvPoint2D32f(62,117);//skapar punkter med målens ungefärliga position
		goal2=cvPoint2D32f(324,116);
		
		

		//track_goal1.trackObject(frame,&goal1);//hittar målens position
		//track_goal2.trackObject(frame,&goal2);
	}
void Puck::updatePosition(CvPoint2D32f* ps){
	//gör linjär transformation pixelkordinatrer -> millimeter med hjälp utav målens position
 	x=ps->x-(goal1.x+goal2.x)/2;
	y=ps->y-(goal1.y+goal2.y)/2;
	//cout<<x<<", "<<y<<endl;
	float v1x=goal2.x-goal1.x;
	float v1y=goal2.y-goal1.y; 
	float v2x=v1y;
	float v2y=-v1x;
	float norm=sqrt(v1x*v1x+v1y*v1y);

	v1x=v1x/norm;
	v1y=v1y/norm;
	v2x=v2x/norm;
	v2y=v2y/norm;
	x=v1x*x+v1y*y;
	y=v2x*x+v2y*y;
	x*=2*DISTANCETOGOAL/norm;
	y*=2*DISTANCETOGOAL/norm;
	//cout<<goal1.x<<", "<<goal1.y<<endl;
	
}

