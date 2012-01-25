#include <vector>
#include "cv.h"
#include "highgui.h"
#include "SerialConnection.h"
#include "ObjectTracker.h"
#include "CamCapture.h"
#ifndef GAMESTATE
#define GAMESTATE
#define IMAGESIZE cvSize(744/2,480/2)
using namespace std;
struct Puck{
	int x;
	int y;
	CvPoint2D32f goal1;
	CvPoint2D32f goal2;
	Puck(){
		;
	};
	Puck(IplImage* frame);
	
	void updatePosition(CvPoint2D32f* point);
};
//struct Player{
//	int id;
//	//variables to send
//	int x;
//	int y;
//	int rot;
//
//	//variables for movement
//	int position;
//	int targetPosition;
//
//	int speed;
//	int maxSpeed;
//
//	int targetRot;
//
//	int rotSpeed;
//	int maxRotSpeed;
//
//	Player(int _id){
//		id=_id;
//		x=0;
//		y=0;
//		rot=0;
//		speed=0;
//		maxSpeed=0;
//
//	};
	//void updatePosition(int scalar);
	//void updateRotation(int rotation);
	///void setTranslation(int _speed,int _position);
	//void setRotation(int _speed,int _angle);
	//void updateMovement();//do feedback on player speed
//};
//extern vector<Player> players;
extern bool running;
extern bool paused;
extern Puck puck;
extern int gameTime;
extern int gameStartTime;
extern SerialConnection* homeSerial;
extern SerialConnection* awaySerial;
extern CamCapture* capture;
extern bool trackingInitialized;
bool initializeTracking();
bool initializeMicroControllers();
extern ObjectTracker track_puck;

unsigned __stdcall senderThread(void* param);
unsigned __stdcall cameraThread(void* param);

#endif