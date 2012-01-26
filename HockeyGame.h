#include <process.h>
#include <Windows.h>
class HockeyGame{//klass f�r hanterandet av fl�det i spelet
	HANDLE senderThreadHandle;
	HANDLE recieverThreadHandle;
	HANDLE cameraThreadHandle;
public:
	bool initializeGame();
	void stopGame();
	void pauseGame();
	void resumeGame();
private:
	bool setUpConnections();
	bool setUpGamestate();
	bool setUpCamera();	
	
};
