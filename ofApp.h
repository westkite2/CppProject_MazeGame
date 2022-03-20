#pragma once
#include "ofMain.h"

struct Point {			// (x,y)��ǥ ������ ����ü
	int x;				// x��ǥ
	int y;				// y��ǥ
};

struct Item {			//�������� ������ �����ϴ� ����ü
	Point p;			//�������� ��ǥ
	int taken_flag;		//������ ȹ�濩�� ǥ��
};

struct Enemy {			//���� ������ �����ϴ� ����ü
	Point p;			//���� ��ǥ
	int pre_dir;		//���� ������ ������ ���� ����
	int meet_flag;		//���� ���� �������� ���� ǥ��
};


class ofApp : public ofBaseApp{
	private:
		int gamestart_flag = 0;				//���ӽ��� �÷���
		int gameover_flag = 0;				//�������� �÷���
		int pY = 0;							//�÷��̾��� y��ǥ(�ʱⰪ: ������)
		int pX = 0;							//�÷��̾��� x��ǥ(�ʱⰪ: ������)
		int dirY[4] = { 0, 0, 1, -1 };		//y��ǥ ���� �̵��� ����(��, ��, ��, ��)
		int dirX[4] = { 1, -1, 0, 0 };		//x��ǥ ���� �̵��� ����(��, ��, ��, ��)
		int haveBomb = 0;					//�÷��̾ ���� ��ź�� ��
		int sf = 25;						//���� Ȯ���ϱ� ���� ����(scale factor)
		int offset = 10;					//���� �����ϱ� ���� ����
		int tmp = 0;						//���� ��� ������ ���� ����

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
		
		void generateMaze(int H, int W);						//�̷� ���� �Լ�
		void generateDoor();									//�� ���� �Լ�
		void generateItem();									//������ ���� �Լ�
		void generateBomb();									//��ź ���� �Լ�
		void generateEnemy();									//�� ���� �Լ�
		void drawMaze(int H, int W);							//�̷� �׸��� �Լ�
		void drawGoal();										//�������� �׸��� �Լ�
		void drawPlayer();										//�÷��̾� �׸��� �Լ�
		void drawDoor();										//�� �׸��� �Լ�
		void drawItem();										//������ �׸��� �Լ�
		void drawBomb();										//��ź �׸��� �Լ�
		void drawBombInfo();									//ȹ���� ��ź ���� �׸��� �Լ�
		void drawEnemy();										//�� �׸��� �Լ�
		bool checkRoad(int Y, int X, int dirY, int dirX);		//���� ���� Ȯ���ϴ� �Լ�
		void movePlayer(int dirY, int dirX);					//�÷��̾� �̵� �Լ�
		int opposDir(int dir);									//dir�� �ݴ� ������ ��ȯ�ϴ� �Լ�
		void moveEnemy();										//�� �̵� �Լ�
		void enterDoor(int& Y, int& X, int door);				//���� ���� �÷��̾ �̵���Ű�� �Լ�
		void bombExplode(int dirY, int dirX);					//��ź���� ���� ���ʶ߸��� �Լ�
		void freeMemory();										//�޸𸮸� �����ϴ� �Լ�

		ofTrueTypeFont font1;			//��ü1
		string title;					//���� Ÿ��Ʋ
		string text1;					//SUCCESS!
		string text2;					//FAILURE!
		ofTrueTypeFont font2;			//��ü2
		string subtext1;				//RETRY
		string subtext2;				//EXIT
		ofTrueTypeFont font3;			//��ü3
		string button;					//START

		ofSoundPlayer sound_click;		//��ư Ŭ�� ȿ����
		ofSoundPlayer sound_move;		//�÷��̾� �̵� ȿ����
		ofSoundPlayer sound_pickup;		//������ ȹ�� ȿ����
		ofSoundPlayer sound_boom;		//���� ȿ����
		ofSoundPlayer sound_teleport;	//�� �̵� ȿ����
		ofSoundPlayer sound_gameover;	//���ӿ��� ȿ����
		ofSoundPlayer sound_goal;		//���� ȿ����

};
