#include "GameState.h"
using namespace std;
//void Player::updatePosition(int scalar){
//	//algorithm for converting 
//}
//void Player::updateRotation(int rotation){
//	rot=rotation;
//}
//void Player::setTranslation(int _speed,int _position){
//	maxSpeed=_speed;
//	targetPosition=_position;
//}
//void Player::setRotation(int _speed,int _angle){
//	maxRotSpeed=_speed;
//	targetRot=_angle;
//}
//void Player::updateMovement(){//feedback function
//	//translation
//	int proximityDistance=5;
//	int deltaPos=targetPosition-position;
//	if(abs(deltaPos)<proximityDistance){
//		speed=abs(maxSpeed)*(deltaPos)/proximityDistance;
//	}else{
//		speed=abs(maxSpeed)*(deltaPos>=0-deltaPos<=0);
//	}
//	//rotation
//	int proximityAngle=30;
//	int deltaRot=targetRot-rot-360*maxRotSpeed<0;
//	if(abs(deltaRot)<proximityAngle){
//		speed=abs(maxRotSpeed)*(deltaRot)/proximityDistance;
//	}else{
//		speed=maxRotSpeed;
//	}
//}