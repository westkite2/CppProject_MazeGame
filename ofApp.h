#pragma once
#include "ofMain.h"

struct Point {			// (x,y)좌표 형식의 구조체
	int x;				// x좌표
	int y;				// y좌표
};

struct Item {			//아이템의 정보를 저장하는 구조체
	Point p;			//아이템의 좌표
	int taken_flag;		//아이템 획득여부 표시
};

struct Enemy {			//적의 정보를 저장하는 구조체
	Point p;			//적의 좌표
	int pre_dir;		//적이 직전에 움직인 방향 저장
	int meet_flag;		//적이 서로 만났는지 여부 표시
};


class ofApp : public ofBaseApp{
	private:
		int gamestart_flag = 0;				//게임시작 플래그
		int gameover_flag = 0;				//게임종료 플래그
		int pY = 0;							//플레이어의 y좌표(초기값: 시작점)
		int pX = 0;							//플레이어의 x좌표(초기값: 시작점)
		int dirY[4] = { 0, 0, 1, -1 };		//y좌표 기준 이동의 방향(동, 서, 남, 북)
		int dirX[4] = { 1, -1, 0, 0 };		//x좌표 기준 이동의 방향(동, 서, 남, 북)
		int haveBomb = 0;					//플레이어가 가진 폭탄의 수
		int sf = 25;						//값을 확대하기 위한 변수(scale factor)
		int offset = 10;					//값을 조정하기 위한 변수
		int tmp = 0;						//사운드 재생 조절을 위한 변수

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void generateMaze(int H, int W);						//미로 생성 함수
		void generateDoor();									//문 생성 함수
		void generateItem();									//아이템 생성 함수
		void generateBomb();									//폭탄 생성 함수
		void generateEnemy();									//적 생성 함수
		void drawMaze(int H, int W);							//미로 그리기 함수
		void drawGoal();										//골인지점 그리기 함수
		void drawPlayer();										//플레이어 그리기 함수
		void drawDoor();										//문 그리기 함수
		void drawItem();										//아이템 그리기 함수
		void drawBomb();										//폭탄 그리기 함수
		void drawBombInfo();									//획득한 폭탄 정보 그리기 함수
		void drawEnemy();										//적 그리기 함수
		bool checkRoad(int Y, int X, int dirY, int dirX);		//길의 여부 확인하는 함수
		void movePlayer(int dirY, int dirX);					//플레이어 이동 함수
		int opposDir(int dir);									//dir의 반대 방향을 반환하는 함수
		void moveEnemy();										//적 이동 함수
		void enterDoor(int& Y, int& X, int door);				//문을 통해 플레이어를 이동시키는 함수
		void bombExplode(int dirY, int dirX);					//폭탄으로 벽을 무너뜨리는 함수
		void freeMemory();										//메모리를 해제하는 함수

		ofTrueTypeFont font1;			//서체1
		string title;					//게임 타이틀
		string text1;					//SUCCESS!
		string text2;					//FAILURE!
		ofTrueTypeFont font2;			//서체2
		string subtext1;				//RETRY
		string subtext2;				//EXIT
		ofTrueTypeFont font3;			//서체3
		string button;					//START

		ofSoundPlayer sound_click;		//버튼 클릭 효과음
		ofSoundPlayer sound_move;		//플레이어 이동 효과음
		ofSoundPlayer sound_pickup;		//아이템 획득 효과음
		ofSoundPlayer sound_boom;		//폭파 효과음
		ofSoundPlayer sound_teleport;	//문 이동 효과음
		ofSoundPlayer sound_gameover;	//게임오버 효과음
		ofSoundPlayer sound_goal;		//골인 효과음

};
