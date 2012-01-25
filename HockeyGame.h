#include <process.h>
#include <Windows.h>
class HockeyGame{//klass för hanterandet av flödet i spelet
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
