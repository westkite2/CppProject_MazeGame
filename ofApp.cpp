#include <random>
#include "ofApp.h"

int numDoor = 6;	//���� ����(¦��)
int numItem = 3;	//�������� ����
int numEnemy = 4;	//���� ����
int numBomb = 3;	//��ź�� ����

int mazeHeight = 12;	//�̷��� ����(¦��)
int mazeWidth = 12;		//�̷��� �ʺ�(¦��)
char** mazeArr = new char* [mazeHeight * 2 + 1]; //�̷� 2���� �迭
Point* doorArr = new Point[numDoor]();			//���� ���� �����ϴ� �迭
Item* itemArr= new Item[numItem]();				//�������� ���� �����ϴ� �迭
Enemy* enemyArr= new Enemy[numEnemy]();			//���� ���� �����ϴ� �迭
Item* bombArr = new Item[numBomb]();			//��ź�� ���� �����ϴ� �迭
int* enemyDirArr = new int[4]();				//���� �̵������� ���� �����ϴ� �迭

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(20);
	ofBackground(0,0,0);
	
	font1.load("NanumBarunGothicBold.ttf", 40);	//ũ�� 40�� ��ü �ε�
	font2.load("NanumBarunGothicBold.ttf", 15);	//ũ�� 15�� ��ü �ε�
	font3.load("NanumBarunGothicBold.ttf", 20);	//ũ�� 20�� ��ü �ε�
	title = "MAZE GAME";
	text1 = "SUCCESS!";
	text2 = "FAILURE!";
	button = "START";
	subtext1 = "RETRY";
	subtext2 = "EXIT";

	//���� ���� �ε�
	sound_click.load("click.mp3");
	sound_move.load("move.mp3");
	sound_pickup.load("pickup.mp3");
	sound_boom.load("boom.mp3");
	sound_teleport.load("teleport.mp3");
	sound_gameover.load("gameover.mp3");
	sound_goal.load("goal.mp3");

	srand((unsigned int)time(NULL));
	
	for (int i = 0; i <= mazeHeight * 2; ++i) {		//�̷� �����Ҵ�
		mazeArr[i] = new char[mazeWidth * 2 + 1];
	}

	for (int i = 0; i < numDoor; ++i) {				//�� ���� �ʱ�ȭ
		doorArr[i].x = 0;
		doorArr[i].y = 0;
	}

	generateMaze(mazeHeight, mazeWidth);		//�̷� ����
	generateDoor();								//�� ����
	generateItem();								//������ ����
	generateBomb();								//��ź ����
	generateEnemy();							//�� ����
}

//--------------------------------------------------------------
void ofApp::update() {
	for (int i = 0; i < numItem; i++) {							//�� �����ۿ� ���Ͽ�
		if (pY == itemArr[i].p.y && pX == itemArr[i].p.x) {		//�÷��̾ �������� ��ġ�� ������ ���
			if (itemArr[i].taken_flag == 0) {					//�������� ���� ȹ����� ���� ���(flag=0)
				sound_pickup.play();							//������ ȹ�� ȿ���� ���
				itemArr[i].taken_flag = 1;						//������ ȹ�� ���� ǥ��(flag=1)
			}
		}
	}

	for (int i = 0; i < numBomb; i++) {							//�� ��ź�� ���Ͽ�
		if (pY == bombArr[i].p.y && pX == bombArr[i].p.x		//�÷��̾ ��ź�� ��ġ�� ������ ���
			&& bombArr[i].taken_flag == 0) {					//��ź�� ���� ȹ����� ���� ���(flag=0)
			sound_pickup.play();								//������ ȹ�� ȿ���� ���
			bombArr[i].taken_flag = 1;							//��ź ȹ�� ���� ǥ��(flag=1)
			haveBomb++;											//�÷��̾ ������ ��ź �� ����
		}
	}

	if (pY == (mazeHeight - 1) && pX == (mazeWidth - 1)) {		//�÷��̾ ���������� ������ ���
		int cnt = 0;
		for (int i = 0; i < numItem; ++i) {						//�� �����ۿ� ���Ͽ�
			if (itemArr[i].taken_flag == 1) cnt++;				//�÷��̾��� ȹ�� ���� Ȯ��
		}
		if (cnt == numItem) {									//�÷��̾ �������� ��� ȹ���� ���
			gameover_flag = 1;									//�������� �÷��� ǥ��(flag=1�� SUCCESS)
		}
	}

	for (int i = 0; i < numEnemy; ++i) {						//�� ���� ���Ͽ�
		if (pY == enemyArr[i].p.y && pX == enemyArr[i].p.x) {	//�÷��̾�� ���� �浹�� ���
			gameover_flag = 2;									//�������� �÷��� ǥ��(flag=2�� FAIL)
		}
	}

	if (gamestart_flag == 1 &&gameover_flag==0) {				//������ �����Ͽ���, ����Ǳ� ���̸�
		if (ofGetFrameNum() % 10==0) moveEnemy();				//���� �̵�(������ �ӵ� ����)
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetLineWidth(10);
	
	if (gamestart_flag == 0) {											//���� ���� ��(�ʱ� ȭ��)
		//Ÿ��Ʋ
		ofSetColor(255, 255, 255);										//�Ͼ�
		font1.drawString(title, ofGetWidth() / 2						//����(MAZE GAME)
			- font1.stringWidth(title) / 2, ofGetHeight() / 2 - 100);

		//���۹�ư
		ofSetColor(0, 255, 0);											//�ʷ�
		ofDrawRectangle(ofGetWidth() / 2 - 100,							//����(�׵θ�)
			ofGetHeight() / 2 - 40, 200, 70);
		ofSetColor(0, 0, 0);											//����
		ofDrawRectangle(ofGetWidth() / 2 - 90,							//����(����)
			ofGetHeight() / 2 - 30, 180, 50);
		ofSetColor(0, 255, 0);											//�ʷ�
		font3.drawString(button, ofGetWidth() / 2 -						//����(START)
			font3.stringWidth(button) / 2, ofGetHeight() / 2 + 5);

		//���� ����
		ofSetColor(240, 240, 240);										//�Ͼ�
		font2.drawString("Up, Down, Left, Right : Move",				//����Ű ����
			ofGetWidth() / 2 - 133, ofGetHeight() / 2 + 100);
		font2.drawString("W, S, A, D : Use Bomb",						//��ź ��� ����
			ofGetWidth() / 2 - 98, ofGetHeight() / 2 + 130);
		font2.drawString("SPACE BAR : Enter Door",						//�� ��� ����
			ofGetWidth() / 2 - 112, ofGetHeight() / 2 + 160);
	}

	if (gamestart_flag == 1) {				//���� ����
		ofSetColor(0, 0, 255);				//�Ķ�
		drawMaze(mazeHeight, mazeWidth);	//�̷� �׸���
		drawDoor();							//�� �׸���
		ofSetColor(255, 255, 0);			//���
		drawItem();							//������ �׸���
		ofSetColor(0, 255, 0);				//�ʷ�
		drawGoal();							//�������� �׸���
		ofSetColor(20, 190, 255);			//�ϴû�
		drawBomb();							//��ź �׸���
		drawBombInfo();						//��ź ���� �׸���
		drawEnemy();						//�� �׸���
		ofSetColor(255, 255, 255);			//�Ͼ�
		drawPlayer();						//�÷��̾� �׸���
	}
	
	ofSetLineWidth(10);
	if (gameover_flag == 1) {					//���� ����(����)
		while (tmp==0) {
			sound_goal.play();					//���� ȿ���� ���
			tmp++;								//1ȸ�� ���
		}
		ofSetColor(0, 255, 0);												//�ʷ�
		ofDrawRectangle(ofGetWidth() / 2 - 160,								//����(�׵θ�)
			ofGetHeight() / 2 - 85, 320, 170);
		ofSetColor(0, 0, 0);												//����
		ofDrawRectangle(ofGetWidth() / 2 - 150,								//����(����)
			ofGetHeight() / 2 - 75, 300, 150);
		ofSetColor(255, 0, 0);												//����
		font1.drawString(text1, ofGetWidth() / 2 -							//����(SUCCESS!)
			font1.stringWidth(text1) / 2, ofGetHeight() / 2 - 5);
		ofSetColor(255, 255, 0);											//���
		font2.drawString(subtext1, ofGetWidth() / 2 -						//����(RETRY)
			font1.stringWidth(subtext1) / 2 - 5, ofGetHeight() / 2 + 45);
		font2.drawString(subtext2, ofGetWidth() / 2 -						//����(EXIT)
			font1.stringWidth(subtext2) / 2 + 100, ofGetHeight() / 2 + 45);
	}
	
	if (gameover_flag == 2) {					//���� ����(����)
		while (tmp==0) {
			sound_gameover.play();				//���ӿ��� ȿ���� ���
			tmp++;								//1ȸ�� ���
		}
		ofSetColor(0, 255, 0);												//�ʷ�
		ofDrawRectangle(ofGetWidth() / 2 - 160,								//����(�׵θ�)
			ofGetHeight() / 2 - 85, 320, 170);
		ofSetColor(0, 0, 0);												//����
		ofDrawRectangle(ofGetWidth() / 2 - 150,								//����(����)
			ofGetHeight()/ 2 - 75, 300, 150);
		ofSetColor(255, 0, 0);												//����
		font1.drawString(text2, ofGetWidth()/ 2 -							//����(FAILURE!)
			font1.stringWidth(text2) / 2, ofGetHeight() / 2 - 5);
		ofSetColor(255, 255, 0);											//���
		font2.drawString(subtext1, ofGetWidth() / 2 -						//����(RETRY)
			font1.stringWidth(subtext1) / 2 - 5, ofGetHeight() / 2 + 45);
		font2.drawString(subtext2, ofGetWidth() / 2 -						//����(EXIT)
			font1.stringWidth(subtext2) / 2 + 100, ofGetHeight() / 2 + 45);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (gamestart_flag == 1  && gameover_flag==0) {					//���� ��
		if (key == OF_KEY_RIGHT) movePlayer(dirY[0], dirX[0]);		//������ ����Ű ������ �÷��̾� ���������� �̵�
		if (key == OF_KEY_LEFT) movePlayer(dirY[1], dirX[1]);		//���� ����Ű ������ �÷��̾� �������� �̵�
		if (key == OF_KEY_DOWN) movePlayer(dirY[2], dirX[2]);		//�Ʒ��� ����Ű ������ �÷��̾� �Ʒ������� �̵�
		if (key == OF_KEY_UP) movePlayer(dirY[3], dirX[3]);			//���� ����Ű ������ �÷��̾� �������� �̵�
		
		if (key == ' ') {											//�����̽��� ������
			for (int i = 0; i < numDoor; ++i) {						//�� ���� ���Ͽ�
				if (pY == doorArr[i].y && pX == doorArr[i].x) {		//�÷��̾ ���� ��ġ �ִ� ���
					enterDoor(pY, pX, i);							//�� ����
					break;
				}
			}
		}

		if (haveBomb>0) {											//�÷��̾�� ��ź�� �ִ� ���
			if (key == 'D' || key == 'd')							//DŰ ������
				bombExplode(dirY[0], dirX[0]);						//�÷��̾��� ������ �� ����
			else if (key == 'A' || key == 'a')						//AŰ ������
				bombExplode(dirY[1], dirX[1]);						//�÷��̾��� ���� �� ����
			else if (key == 'S' || key == 's')						//SŰ ������
				bombExplode(dirY[2], dirX[2]);						//�÷��̾��� �Ʒ��� �� ����
			else if (key == 'W' || key == 'w')						//WŰ ������
				bombExplode(dirY[3], dirX[3]);						//�÷��̾��� ���� �� ����
		}
	}

	if (key == 'Q' || key == 'q') {				//QŰ ������
		freeMemory();							//�޸� ����
		_Exit(0);								//���α׷� ����
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if ((ofGetWidth() / 2 - 100) <= x
		&& x <= (ofGetWidth() / 2 + 100)
		&& (ofGetHeight() / 2 - 40) <= y
		&& y <= (ofGetHeight() / 2 + 30)) {			//START��ư Ŭ�� ��
		sound_click.play();							//��ư Ŭ�� ȿ���� ���
		gamestart_flag = 1;							//���ӽ��� �÷��� ��
	}

	if (gameover_flag != 0) {
		//RETRY Ŭ�� ��
		if ((ofGetWidth() / 2 - font1.stringWidth(subtext1) / 2 - 7) <= x
			&& x <= ((ofGetWidth() / 2 - font1.stringWidth(subtext1) / 2 - 7) + (font1.stringWidth(subtext1) / 2 - 12))
			&& (ofGetHeight() / 2 + 26) <= y
			&& y <= ((ofGetHeight() / 2 + 26) + font1.stringHeight(subtext1) / 2 + 3)) {

			//���� �ʱ�ȭ
			gamestart_flag = 1;		//���ӽ��� �÷��� ��
			gameover_flag = 0;		//�������� �÷��� ������
			pY = 0;					//�÷��̾� y��ǥ �ʱ�ȭ
			pX = 0;					//�÷��̾� x��ǥ �ʱ�ȭ
			haveBomb = 0;			//�÷��̾� ���� ��ź ���� �ʱ�ȭ
			tmp = 0;				//���� ��� ���� ���� �ʱ�ȭ

			for (int i = 0; i < numDoor; ++i) {		//�� ���� �ʱ�ȭ
				doorArr[i].x = 0;
				doorArr[i].y = 0;
			}

			generateMaze(mazeHeight, mazeWidth);	//�̷� ����
			generateDoor();							//�� ����
			generateItem();							//������ ����
			generateBomb();							//��ź ����
			generateEnemy();						//�� ����

			sound_click.play();						//��ư Ŭ�� ȿ���� ���
		}
	
		//EXIT Ŭ�� ��
		else if ((ofGetWidth() / 2 - font1.stringWidth(subtext2) / 2 + 98) <= x
			&& x <= ((ofGetWidth() / 2 - font1.stringWidth(subtext2) / 2 + 98) + (font1.stringWidth(subtext2) / 2 - 7))
			&& (ofGetHeight() / 2 + 26) <= y
			&& y <= ((ofGetHeight() / 2 + 26) + font1.stringHeight(subtext2) / 2 + 3)) {

			freeMemory();		//�޸� ����
			_Exit(0);			//���α׷� ����
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

/* ************************ USER DEFINED FUNCTIONS *********************** */
//--------------------------------------------------------------
inline int mazeArrV(int i) {							//���� �̷��� ������ �°� ��ǥ�� �����ϴ� �Լ�
	return 2 * i + 1;
}

//--------------------------------------------------------------
void ofApp::generateMaze(int H, int W) {				//�̷� ���� �Լ�
	for (int i = 0; i <= H * 2; i += 2) {				//���� ��� ����
		for (int j = 0; j < W * 2; j += 2) {
			mazeArr[i][j] = '+';
			mazeArr[i][j + 1] = '-';
		}
		mazeArr[i][W * 2] = '+';
	}
	for (int i = 1; i < H * 2; i += 2) {				//���� ��� ����
		for (int j = 0; j < W * 2; j += 2) {
			mazeArr[i][j] = '|';
			mazeArr[i][j + 1] = ' ';
		}
		mazeArr[i][W * 2] = '|';
	}
	for (int i = 3; i <= H * 2 - 1; i += 2) {			//�� ���� ���Ͽ� �����̳� �������� �� ����
		for (int j = 3; j <= W * 2 - 1; j += 2) {
			int way = rand() % 2;						//�����̳� ���� �� ���� ����
			if (way == 0) mazeArr[i - 1][j] = ' ';		//�������� �� ����
			else mazeArr[i][j - 1] = ' ';				//�������� �� ����
		}
	}
	for (int i = 3; i <= H * 2 - 1; i += 2)				//ù ��° ���� ���Ͽ� �������θ� �� ����(��踦 ���� �� �����Ƿ�)
		mazeArr[i - 1][1] = ' ';
	for (int j = 3; j <= W * 2 - 1; j += 2)				//ù ��° �࿡ ���Ͽ� �������θ� �� ����(��踦 ���� �� �����Ƿ�)
		mazeArr[1][j - 1] = ' ';
}

//--------------------------------------------------------------
void ofApp::generateDoor() {							//���� ��ġ ���� ���� �Լ�
	int tmpY, tmpX;
	for (int i = 0; i < numDoor / 2; ++i) {				//���� ������ �̷��� ���ʿ��� ����
		while (1) {										//ù��° ��� ��, �̷��� �߰� ���� ȸ���Ͽ� ����
			int wrong = 0;
			tmpY = rand() % (mazeHeight - 1);			//����: 0~(mazeHeight-2)
			tmpY += 1;									//����: 1~(mazeHeight-1)
			tmpX = rand() % (mazeWidth / 2 - 2);		//����: 0~mazeWidth/2-3
			tmpX += 1;									//����: 1~(mazeWidth/2-2)
			for (int j = 0; j < (numDoor / 2); ++j) {	//�ٸ� ���� ��ǥ �ߺ� ����
				if (tmpY == doorArr[j].y && tmpX == doorArr[j].x) wrong = 1;
			}
			if (wrong == 0) break;
		}
		doorArr[i].y = tmpY;							//������ ���� y��ǥ ����
		doorArr[i].x = tmpX;							//������ ���� x��ǥ ����
	}
	for (int i = numDoor / 2; i < numDoor; ++i) {		//���� ������ �̷��� �����ʿ��� ����
		while (1) {
			int wrong = 0;
			tmpY = rand() % (mazeHeight - 1);			//����: 0~(mazeHeight-2)
			tmpY += 1;									//����: 1~(mazeHeight-1)
			tmpX = rand() % (mazeWidth / 2 - 1);		//����: 0~(mazeWidth/2-2)
			tmpX += mazeWidth / 2 + 1;					//����: 7~(mazeWidth/2+5)
			if ((tmpY == mazeHeight - 1)
				&& (tmpX == mazeWidth - 1)) continue;	//���� ���� ȸ��
			for (int j = 0; j < numDoor; ++j) {			//�ٸ� ���� ��ǥ �ߺ� ����
				if (tmpY == doorArr[j].y && tmpX == doorArr[j].x) wrong = 1;
			}
			if (wrong == 0) break;
		}
		doorArr[i].y = tmpY;							//������ ���� y��ǥ ����
		doorArr[i].x = tmpX;							//������ ���� x��ǥ ����
	}
}

//--------------------------------------------------------------
void ofApp::generateItem() {							//�������� ��ġ ���� ���� �Լ�
	int tmpY, tmpX;
	for (int i = 0; i < numItem; ++i) {
		while (1) {										//�̷��� ù��° ��� ���� ȸ���Ͽ� ����
			int wrong = 0;
			tmpY = rand() % (mazeHeight - 1);			//����: 0~(mazeHeight-2)
			tmpY += 1;									//����: 1~(mazeHeight-1)
			tmpX = rand() % (mazeWidth - 1);
			tmpX += 1;
			if ((tmpY == mazeHeight - 1)
				&& (tmpX == mazeWidth - 1)) continue;	//�������� ȸ��
			for (int j = 0; j < numDoor; ++j) {			//���� ��ǥ �ߺ� ����
				if (tmpY == doorArr[j].y && tmpX == doorArr[j].x) wrong = 1;
			}
			for (int j = 0; j < numItem; ++j) {			//�ٸ� �����۰� ��ǥ �ߺ� ����
				if (tmpY == itemArr[j].p.y && tmpX == itemArr[j].p.x) wrong = 1;
			}
			if (wrong == 0) break;
		}
		itemArr[i].p.y = tmpY;							//������ �������� y��ǥ ����
		itemArr[i].p.x = tmpX;							//������ �������� x��ǥ ����
		itemArr[i].taken_flag = 0;						//������ ��ȹ�� ǥ��
	}
}

//--------------------------------------------------------------
void ofApp::generateBomb() {							//��ź�� ��ġ ���� ���� �Լ�
	int tmpY, tmpX, j;
	for (int i = 0; i < numBomb; ++i) {
		while (1) {										//�̷��� ù��° ��� ���� ȸ���Ͽ� ����
			int wrong = 0;
			tmpY = rand() % (mazeHeight - 1);			//����: 0~(mazeHeight-2)
			tmpY += 1;									//����: 1~(mazeHeight-1)
			tmpX = rand() % (mazeWidth - 1);
			tmpX += 1;
			if ((tmpY == mazeHeight - 1)
				&& (tmpX == mazeWidth - 1)) continue;	//�������� ȸ��
			for (j = 0; j < numDoor; ++j) {				//���� ��ǥ �ߺ� ����
				if (tmpY == doorArr[j].y && tmpX == doorArr[j].x) wrong = 1;
			}
			for (j = 0; j < numItem; ++j) {				//�����۰� ��ǥ �ߺ� ����
				if (tmpY == itemArr[j].p.y && tmpX == itemArr[j].p.x) wrong = 1;
			}
			for (j = 0; j < numBomb; ++j) {				//�ٸ� ��ź�� ��ǥ �ߺ� ����
				if (tmpY == bombArr[j].p.y && tmpX == bombArr[j].p.x) wrong = 1;
			}
			if (wrong == 0) break;
		}
		bombArr[i].p.y = tmpY;							//������ ��ź�� y��ǥ ����
		bombArr[i].p.x = tmpX;							//������ ��ź�� y��ǥ ����
		bombArr[i].taken_flag = 0;						//��ź ��ȹ�� ǥ��
	}
}

//--------------------------------------------------------------
void ofApp::generateEnemy() {							//���� ��ġ ���� ���� �Լ�
	int tmpY, tmpX;
	for (int i = 0; i < numEnemy; ++i) {
		while (1) {
			int wrong = 0;
			tmpY = rand() % mazeHeight;					//����: 0~mazeHeight-1
			tmpX = rand() % mazeWidth;
			if ((tmpY == 0) && (tmpX == 0)) continue;	//�̷� �������� ȸ��
			for (int j = 0; j < numEnemy; ++j) {		//�ٸ� ���� ��ǥ �ߺ� ����
				if (tmpY == enemyArr[j].p.y && tmpX == enemyArr[j].p.x) wrong = 1;
			}
			if (wrong == 0) break;
		}
		enemyArr[i].p.y = tmpY;							//������ ���� y��ǥ ����
		enemyArr[i].p.x = tmpX;							//������ ���� x��ǥ ����
		enemyArr[i].pre_dir = 0;						//���� ���� ���� �ʱ�ȭ
	}
}

//--------------------------------------------------------------
void ofApp::drawMaze(int H, int W) {					//�̷� �׸��� �Լ�
	for (int i = 0; i <= H * 2 - 1; i += 2) {
		for (int j = 0; j <= W * 2 - 1; j += 2) {
			if (mazeArr[i][j + 1] == '-') {
				ofDrawLine(j * sf + offset, i * sf + offset, (j + 1) * sf + 30 + offset, i * sf + offset);
			}
			if (mazeArr[i + 1][j] == '|') {
				ofDrawLine(j * sf + offset, i * sf + offset, j * sf + offset, (i + 1) * sf + 30 + offset);
			}
		}
	}
	for (int j = 0; j <= W * 2 - 1; j += 2) {			//�Ʒ� ��� �׸���
		ofDrawLine(j * sf + offset, 2 * H * sf + offset, (j + 1) * sf + 30 + offset, 2 * H * sf + offset);
	}
	for (int i = 0; i <= H * 2 - 1; i += 2) {			//������ ��� �׸���
		ofDrawLine(W * 2 * sf + offset, i * sf + offset, W * 2 * sf + offset, (i + 1) * sf + 30 + offset);
	}
}

//--------------------------------------------------------------
void ofApp::drawGoal() {								//�������� �׸��� �Լ�
	ofSetLineWidth(2);
	ofDrawLine(576, 584, 576, 602);						//���
	ofSetLineWidth(10);
	ofDrawTriangle(575, 570, 575, 584, 600, 577);		//���
}

//--------------------------------------------------------------
void ofApp::drawPlayer() {								//�÷��̾� �׸��� �Լ�
	ofDrawCircle(mazeArrV(pX) * sf + offset, mazeArrV(pY) * sf + offset, 15);
}

//--------------------------------------------------------------
void ofApp::drawDoor(void) {							//�� �׸��� �Լ�

	for (int i = 0; i < numDoor; ++i) {
		ofSetColor(150, 150, 150);						//ȸ��(��)
		ofDrawRectangle(mazeArrV(doorArr[i].x) * sf + offset - 15,
			mazeArrV(doorArr[i].y) * sf + offset - 15, 30, 30);
		ofSetColor(0, 0, 0);							//����(����)
		if (i < numDoor / 2)							//�� ���� ����
			ofDrawBitmapString(i + 1, mazeArrV(doorArr[i].x) * sf + 7, mazeArrV(doorArr[i].y) * sf + 14);
		else											//�� ������ ����
			ofDrawBitmapString(i - numDoor / 2 + 1, mazeArrV(doorArr[i].x) * sf + 7, mazeArrV(doorArr[i].y) * sf + 14);
	}
}

//--------------------------------------------------------------
void ofApp::drawItem() {								//������ �׸��� �Լ�
	int x = 5;											//ũ�� ���� ����
	for (int i = 0; i < numItem; ++i) {					//�� �����ۿ� ���Ͽ�
		if (itemArr[i].taken_flag == 1) continue;		//�̹� ȹ���� �������� ����
		ofDrawTriangle(									//���� ��� �׸���
			mazeArrV(itemArr[i].p.x) * sf + offset,
			mazeArrV(itemArr[i].p.y) * sf + offset - 2 * x,
			mazeArrV(itemArr[i].p.x) * sf + offset - sqrt(3) * x,
			mazeArrV(itemArr[i].p.y) * sf + offset + 1 * x,
			mazeArrV(itemArr[i].p.x) * sf + offset + sqrt(3) * x,
			mazeArrV(itemArr[i].p.y) * sf + offset + 1 * x);
	}
}
//--------------------------------------------------------------
void ofApp::drawBomb() {								//��ź �׸��� �Լ�
	int x = 7;											//ũ�� ���� ����
	for (int i = 0; i < numBomb; ++i) {					//�� ��ź�� ���Ͽ�
		if (bombArr[i].taken_flag == 1) continue;		//�̹� ȹ���� ��ź�� ����
		ofFill();
		ofBeginShape();									//�� ��� �׸���
		ofDrawTriangle(
			mazeArrV(bombArr[i].p.x) * sf + offset,
			mazeArrV(bombArr[i].p.y) * sf + offset - 2 * x,
			mazeArrV(bombArr[i].p.x) * sf + offset - sqrt(3) * x,
			mazeArrV(bombArr[i].p.y) * sf + offset + 1 * x,
			mazeArrV(bombArr[i].p.x) * sf + offset + sqrt(3) * x,
			mazeArrV(bombArr[i].p.y) * sf + offset + 1 * x);
		ofDrawTriangle(
			mazeArrV(bombArr[i].p.x) * sf + offset,
			mazeArrV(bombArr[i].p.y) * sf + offset + 2 * x,
			mazeArrV(bombArr[i].p.x) * sf + offset - sqrt(3) * x,
			mazeArrV(bombArr[i].p.y) * sf + offset - 1 * x,
			mazeArrV(bombArr[i].p.x) * sf + offset + sqrt(3) * x,
			mazeArrV(bombArr[i].p.y) * sf + offset - 1 * x);
		ofEndShape();
	}
}

//--------------------------------------------------------------
void ofApp::drawBombInfo() {							//�÷��̾ ������ ��ź �׸��� �Լ�
	int x = 4;											//ũ�� ���� ����
	for (int i = 0; i < haveBomb; ++i) {				//������ ��ź�� ���Ͽ�
		ofFill();
		ofBeginShape();									//�� ��� �׸���
		ofDrawTriangle(
			20 + 20 * i, 630 - 2 * x,
			(20 + 20 * i) - sqrt(3) * x, 630 + 1 * x,
			(20 + 20 * i) + sqrt(3) * x, 630 + 1 * x);
		ofDrawTriangle(
			20 + 20 * i, 630 + 2 * x,
			(20 + 20 * i) - sqrt(3) * x, 630 - 1 * x,
			(20 + 20 * i) + sqrt(3) * x, 630 - 1 * x);
		ofEndShape();
	}
}

//--------------------------------------------------------------
void ofApp::drawEnemy(void) {							//�� �׸��� �Լ�
	int i;
	for (i = 0; i < numEnemy - 1; ++i) {				//���� ���� �� Ȯ��
		for (int j = i + 1; j < numEnemy; ++j) {
			if (enemyArr[i].p.y == enemyArr[j].p.y && enemyArr[i].p.x == enemyArr[j].p.x) {
				enemyArr[i].meet_flag = 1;
				enemyArr[j].meet_flag = 1;
			}
		}
	}
	for (i = 0; i < numEnemy; ++i) {
		if (enemyArr[i].meet_flag == 1)					//���� ���� ���� ��ο� �������� ǥ��
			ofSetColor(150, 0, 0);
		else ofSetColor(255, 0, 0);						//ȥ�� �ִ� ���� �������� ǥ��
		ofDrawCircle(mazeArrV(enemyArr[i].p.x) * sf + offset,
			mazeArrV(enemyArr[i].p.y) * sf + offset, 15);
	}
	for (i = 0; i < numEnemy; ++i)						//���� ���� �ʱ�ȭ
		enemyArr[i].meet_flag = 0;
}

//--------------------------------------------------------------
bool ofApp::checkRoad(int Y, int X, int dirY, int dirX) {	//(Y+dirY, X+dirX)�� ���� ���� Ȯ��
															//(Y, X): ���� ��ġ, (dirY, dirX): �̵��� ����
	if (mazeArr[2 * Y + 1 + dirY][2 * X + 1 + dirX] == ' ')	//���� ����
		return true;
	else false;												//���� ����(��谡 ����)
}

//--------------------------------------------------------------
void ofApp::movePlayer(int dirY, int dirX) {				//�÷��̾ (dirY, dirX)�������� �̵�
	sound_move.play();										//�÷��̾� �̵� ȿ���� ���
	if (checkRoad(pY, pX, dirY, dirX) == 1) {				//���� �ִ� ���
		pY = pY + dirY;										//�÷��̾� y��ǥ �̵�
		pX = pX + dirX;										//�÷��̾� x��ǥ �̵�
	}
}

//--------------------------------------------------------------
int ofApp::opposDir(int dir) {								//dir�� �ݴ� ������ ��ȯ�ϴ� �Լ�
	if (dir == 0) return 1;									//�����̸� ���� ��ȯ
	else if (dir == 1) return 0;							//�����̸� ���� ��ȯ
	else if (dir == 2) return 3;							//�����̸� ���� ��ȯ
	else if (dir == 3) return 2;							//�����̸� ���� ��ȯ
}

//--------------------------------------------------------------
void ofApp::moveEnemy() {									//���� ���� �������� �̵��ϴ� �Լ�
	int j;
	for (int i = 0; i < numEnemy; ++i) {					//�� ���� ���Ͽ�
		int cnt = 0;										//�̵� ������ ������ �� ����
		for (j = 0; j < 4; ++j) {							//�������� 4���� ���⿡ ���Ͽ�
			if (checkRoad(enemyArr[i].p.y, enemyArr[i].p.x, dirY[j], dirX[j]) == true) { //���� �ִ� ���
				cnt++;										//�̵� ������ ������ �� ���� ����
				enemyDirArr[cnt - 1] = j;					//�̵������� ���� ����
			}
		}
		if (cnt == 1) {										//�̵� ������ ������ �ϳ��� ���
			enemyArr[i].p.y += dirY[enemyDirArr[0]];		//���� ��ǥ�� �ش� �������� �̵�
			enemyArr[i].p.x += dirX[enemyDirArr[0]];
			enemyArr[i].pre_dir = enemyDirArr[0];			//�̵��� ������ ���� ���� �������� ����
		}
		else if (cnt == 2) {								//�̵� ������ ������ 2���� ���
			for (j = 0; j < cnt; ++j) {
				if (enemyDirArr[j] != opposDir(enemyArr[i].pre_dir)) {	//�ڷ� �ǵ��ư��� �� ����
					enemyArr[i].p.y += dirY[enemyDirArr[j]];			//���� ��ǥ�� �ش� �������� �̵�
					enemyArr[i].p.x += dirX[enemyDirArr[j]];
					enemyArr[i].pre_dir = enemyDirArr[j];				//�̵��� ������ ���� ���� �������� ����
					break;
				}
			}
		}
		else {															//�̵� ������ ������ 3�� �̻��� ���
			int way;
			while (1) {
				way = rand() % cnt;										//���� ������ ���Ƿ� ����
				if (enemyDirArr[way] != opposDir(enemyArr[i].pre_dir))	//�ڷ� �ǵ��ư��� �� ����
					break;
			}
			enemyArr[i].p.y += dirY[enemyDirArr[way]];					//���� ��ǥ�� �ش� �������� �̵�
			enemyArr[i].p.x += dirX[enemyDirArr[way]];
			enemyArr[i].pre_dir = enemyDirArr[way];						//�̵��� ������ ���� ���� �������� ����
		}
	}
}

//--------------------------------------------------------------
void ofApp::enterDoor(int& Y, int& X, int door) {			//���� ���� �÷��̾ �̵���Ű�� �Լ�
	sound_teleport.play();
	if (door < numDoor / 2) {								//�̷� ���� ���� ���Ͽ�
		Y = doorArr[door + numDoor / 2].y;					//�̷� �����ʿ��� ���� ��ȣ�� ���� ������ �÷��̾� ��ǥ�� �̵�
		X = doorArr[door + numDoor / 2].x;
	}
	else {													//�̷� ������ ���� ���Ͽ�
		Y = doorArr[door - numDoor / 2].y;					//�̷� ���ʿ��� ���� ��ȣ�� ���� ������ �÷��̾� ��ǥ�� �̵�
		X = doorArr[door - numDoor / 2].x;
	}
}

//--------------------------------------------------------------
void ofApp::bombExplode(int dirY, int dirX) {							//��ź���� ���� ���ʶ߸��� �Լ�
	if (mazeArr[mazeArrV(pY) + dirY][mazeArrV(pX) + dirX] != ' ') {		//�ش� ���⿡ ���� �ִ��� Ȯ��
		if (0 < (mazeArrV(pY) + dirY)									//�ش� ���� �̷� ��谡 �ƴ��� Ȯ��
			&& (mazeArrV(pY) + dirY) < mazeHeight * 2
			&& 0 < (mazeArrV(pX) + dirX)
			&& (mazeArrV(pX) + dirX) < mazeWidth * 2) {
			sound_boom.play();											//���� ȿ���� ���
			mazeArr[mazeArrV(pY) + dirY][mazeArrV(pX) + dirX] = ' ';	//�� ����
			haveBomb--;													//�÷��̾ ������ ��ź �� ����
		}
	}
}

//--------------------------------------------------------------
void ofApp::freeMemory() {						//�޸� ���� �Լ�
	for (int i = 0; i < mazeHeight; ++i) {		//�̷� ���� 2���� �迭 ����
		delete[] mazeArr[i];
	}
	delete[] mazeArr;

	delete[] doorArr;					//�� ���� �迭 ����
	delete[] itemArr;					//������ ���� �迭 ����
	delete[] enemyArr;					//�� ���� �迭 ����
	delete[] bombArr;					//��ź ���� �迭 ����
	delete[] enemyDirArr;				//���� �̵������� ���� ���� �迭 ����
}

/* ************************ USER DEFINED FUNCTIONS *********************** */