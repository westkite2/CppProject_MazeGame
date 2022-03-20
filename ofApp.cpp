#include <random>
#include "ofApp.h"

int numDoor = 6;	//문의 개수(짝수)
int numItem = 3;	//아이템의 개수
int numEnemy = 4;	//적의 개수
int numBomb = 3;	//폭탄의 개수

int mazeHeight = 12;	//미로의 높이(짝수)
int mazeWidth = 12;		//미로의 너비(짝수)
char** mazeArr = new char* [mazeHeight * 2 + 1]; //미로 2차원 배열
Point* doorArr = new Point[numDoor]();			//문의 정보 저장하는 배열
Item* itemArr= new Item[numItem]();				//아이템의 정보 저장하는 배열
Enemy* enemyArr= new Enemy[numEnemy]();			//적의 정보 저장하는 배열
Item* bombArr = new Item[numBomb]();			//폭탄의 정보 저장하는 배열
int* enemyDirArr = new int[4]();				//적의 이동가능한 방향 저장하는 배열

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(20);
	ofBackground(0,0,0);
	
	font1.load("NanumBarunGothicBold.ttf", 40);	//크기 40인 서체 로드
	font2.load("NanumBarunGothicBold.ttf", 15);	//크기 15인 서체 로드
	font3.load("NanumBarunGothicBold.ttf", 20);	//크기 20인 서체 로드
	title = "MAZE GAME";
	text1 = "SUCCESS!";
	text2 = "FAILURE!";
	button = "START";
	subtext1 = "RETRY";
	subtext2 = "EXIT";

	//사운드 파일 로드
	sound_click.load("click.mp3");
	sound_move.load("move.mp3");
	sound_pickup.load("pickup.mp3");
	sound_boom.load("boom.mp3");
	sound_teleport.load("teleport.mp3");
	sound_gameover.load("gameover.mp3");
	sound_goal.load("goal.mp3");

	srand((unsigned int)time(NULL));
	
	for (int i = 0; i <= mazeHeight * 2; ++i) {		//미로 동적할당
		mazeArr[i] = new char[mazeWidth * 2 + 1];
	}

	for (int i = 0; i < numDoor; ++i) {				//문 정보 초기화
		doorArr[i].x = 0;
		doorArr[i].y = 0;
	}

	generateMaze(mazeHeight, mazeWidth);		//미로 생성
	generateDoor();								//문 생성
	generateItem();								//아이템 생성
	generateBomb();								//폭탄 생성
	generateEnemy();							//적 생성
}

//--------------------------------------------------------------
void ofApp::update() {
	for (int i = 0; i < numItem; i++) {							//각 아이템에 대하여
		if (pY == itemArr[i].p.y && pX == itemArr[i].p.x) {		//플레이어가 아이템의 위치에 도달한 경우
			if (itemArr[i].taken_flag == 0) {					//아이템이 아직 획득되지 않은 경우(flag=0)
				sound_pickup.play();							//아이템 획득 효과음 재생
				itemArr[i].taken_flag = 1;						//아이템 획득 여부 표시(flag=1)
			}
		}
	}

	for (int i = 0; i < numBomb; i++) {							//각 폭탄에 대하여
		if (pY == bombArr[i].p.y && pX == bombArr[i].p.x		//플레이어가 폭탄의 위치에 도달한 경우
			&& bombArr[i].taken_flag == 0) {					//폭탄이 아직 획득되지 않은 경우(flag=0)
			sound_pickup.play();								//아이템 획득 효과음 재생
			bombArr[i].taken_flag = 1;							//폭탄 획득 여부 표시(flag=1)
			haveBomb++;											//플레이어가 보유한 폭탄 수 증가
		}
	}

	if (pY == (mazeHeight - 1) && pX == (mazeWidth - 1)) {		//플레이어가 골인지점에 도달한 경우
		int cnt = 0;
		for (int i = 0; i < numItem; ++i) {						//각 아이템에 대하여
			if (itemArr[i].taken_flag == 1) cnt++;				//플레이어의 획득 여부 확인
		}
		if (cnt == numItem) {									//플레이어가 아이템을 모두 획득한 경우
			gameover_flag = 1;									//게임종료 플래그 표시(flag=1은 SUCCESS)
		}
	}

	for (int i = 0; i < numEnemy; ++i) {						//각 적에 대하여
		if (pY == enemyArr[i].p.y && pX == enemyArr[i].p.x) {	//플레이어와 적이 충돌한 경우
			gameover_flag = 2;									//게임종료 플래그 표시(flag=2는 FAIL)
		}
	}

	if (gamestart_flag == 1 &&gameover_flag==0) {				//게임이 시작하였고, 종료되기 전이면
		if (ofGetFrameNum() % 10==0) moveEnemy();				//적을 이동(프레임 속도 조절)
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetLineWidth(10);
	
	if (gamestart_flag == 0) {											//게임 시작 전(초기 화면)
		//타이틀
		ofSetColor(255, 255, 255);										//하양
		font1.drawString(title, ofGetWidth() / 2						//글자(MAZE GAME)
			- font1.stringWidth(title) / 2, ofGetHeight() / 2 - 100);

		//시작버튼
		ofSetColor(0, 255, 0);											//초록
		ofDrawRectangle(ofGetWidth() / 2 - 100,							//상자(테두리)
			ofGetHeight() / 2 - 40, 200, 70);
		ofSetColor(0, 0, 0);											//검정
		ofDrawRectangle(ofGetWidth() / 2 - 90,							//상자(내부)
			ofGetHeight() / 2 - 30, 180, 50);
		ofSetColor(0, 255, 0);											//초록
		font3.drawString(button, ofGetWidth() / 2 -						//글자(START)
			font3.stringWidth(button) / 2, ofGetHeight() / 2 + 5);

		//게임 설명
		ofSetColor(240, 240, 240);										//하양
		font2.drawString("Up, Down, Left, Right : Move",				//방향키 설명
			ofGetWidth() / 2 - 133, ofGetHeight() / 2 + 100);
		font2.drawString("W, S, A, D : Use Bomb",						//폭탄 사용 설명
			ofGetWidth() / 2 - 98, ofGetHeight() / 2 + 130);
		font2.drawString("SPACE BAR : Enter Door",						//문 사용 설명
			ofGetWidth() / 2 - 112, ofGetHeight() / 2 + 160);
	}

	if (gamestart_flag == 1) {				//게임 시작
		ofSetColor(0, 0, 255);				//파랑
		drawMaze(mazeHeight, mazeWidth);	//미로 그리기
		drawDoor();							//문 그리기
		ofSetColor(255, 255, 0);			//노랑
		drawItem();							//아이템 그리기
		ofSetColor(0, 255, 0);				//초록
		drawGoal();							//골인지점 그리기
		ofSetColor(20, 190, 255);			//하늘색
		drawBomb();							//폭탄 그리기
		drawBombInfo();						//폭탄 정보 그리기
		drawEnemy();						//적 그리기
		ofSetColor(255, 255, 255);			//하양
		drawPlayer();						//플레이어 그리기
	}
	
	ofSetLineWidth(10);
	if (gameover_flag == 1) {					//게임 종료(성공)
		while (tmp==0) {
			sound_goal.play();					//골인 효과음 재생
			tmp++;								//1회만 재생
		}
		ofSetColor(0, 255, 0);												//초록
		ofDrawRectangle(ofGetWidth() / 2 - 160,								//상자(테두리)
			ofGetHeight() / 2 - 85, 320, 170);
		ofSetColor(0, 0, 0);												//검정
		ofDrawRectangle(ofGetWidth() / 2 - 150,								//상자(내부)
			ofGetHeight() / 2 - 75, 300, 150);
		ofSetColor(255, 0, 0);												//빨강
		font1.drawString(text1, ofGetWidth() / 2 -							//글자(SUCCESS!)
			font1.stringWidth(text1) / 2, ofGetHeight() / 2 - 5);
		ofSetColor(255, 255, 0);											//노랑
		font2.drawString(subtext1, ofGetWidth() / 2 -						//글자(RETRY)
			font1.stringWidth(subtext1) / 2 - 5, ofGetHeight() / 2 + 45);
		font2.drawString(subtext2, ofGetWidth() / 2 -						//글자(EXIT)
			font1.stringWidth(subtext2) / 2 + 100, ofGetHeight() / 2 + 45);
	}
	
	if (gameover_flag == 2) {					//게임 종료(실패)
		while (tmp==0) {
			sound_gameover.play();				//게임오버 효과음 재생
			tmp++;								//1회만 재생
		}
		ofSetColor(0, 255, 0);												//초록
		ofDrawRectangle(ofGetWidth() / 2 - 160,								//상자(테두리)
			ofGetHeight() / 2 - 85, 320, 170);
		ofSetColor(0, 0, 0);												//검정
		ofDrawRectangle(ofGetWidth() / 2 - 150,								//상자(내부)
			ofGetHeight()/ 2 - 75, 300, 150);
		ofSetColor(255, 0, 0);												//빨강
		font1.drawString(text2, ofGetWidth()/ 2 -							//글자(FAILURE!)
			font1.stringWidth(text2) / 2, ofGetHeight() / 2 - 5);
		ofSetColor(255, 255, 0);											//노랑
		font2.drawString(subtext1, ofGetWidth() / 2 -						//글자(RETRY)
			font1.stringWidth(subtext1) / 2 - 5, ofGetHeight() / 2 + 45);
		font2.drawString(subtext2, ofGetWidth() / 2 -						//글자(EXIT)
			font1.stringWidth(subtext2) / 2 + 100, ofGetHeight() / 2 + 45);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (gamestart_flag == 1  && gameover_flag==0) {					//게임 중
		if (key == OF_KEY_RIGHT) movePlayer(dirY[0], dirX[0]);		//오른쪽 방향키 누르면 플레이어 오른쪽으로 이동
		if (key == OF_KEY_LEFT) movePlayer(dirY[1], dirX[1]);		//왼쪽 방향키 누르면 플레이어 왼쪽으로 이동
		if (key == OF_KEY_DOWN) movePlayer(dirY[2], dirX[2]);		//아래쪽 방향키 누르면 플레이어 아래쪽으로 이동
		if (key == OF_KEY_UP) movePlayer(dirY[3], dirX[3]);			//위쪽 방향키 누르면 플레이어 위쪽으로 이동
		
		if (key == ' ') {											//스페이스바 누르면
			for (int i = 0; i < numDoor; ++i) {						//각 문에 대하여
				if (pY == doorArr[i].y && pX == doorArr[i].x) {		//플레이어가 문의 위치 있는 경우
					enterDoor(pY, pX, i);							//문 입장
					break;
				}
			}
		}

		if (haveBomb>0) {											//플레이어에게 폭탄이 있는 경우
			if (key == 'D' || key == 'd')							//D키 누르면
				bombExplode(dirY[0], dirX[0]);						//플레이어의 오른쪽 벽 폭파
			else if (key == 'A' || key == 'a')						//A키 누르면
				bombExplode(dirY[1], dirX[1]);						//플레이어의 왼쪽 벽 폭파
			else if (key == 'S' || key == 's')						//S키 누르면
				bombExplode(dirY[2], dirX[2]);						//플레이어의 아래쪽 벽 폭파
			else if (key == 'W' || key == 'w')						//W키 누르면
				bombExplode(dirY[3], dirX[3]);						//플레이어의 위쪽 벽 폭파
		}
	}

	if (key == 'Q' || key == 'q') {				//Q키 누르면
		freeMemory();							//메모리 해제
		_Exit(0);								//프로그램 종료
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
		&& y <= (ofGetHeight() / 2 + 30)) {			//START버튼 클릭 시
		sound_click.play();							//버튼 클릭 효과음 재생
		gamestart_flag = 1;							//게임시작 플래그 업
	}

	if (gameover_flag != 0) {
		//RETRY 클릭 시
		if ((ofGetWidth() / 2 - font1.stringWidth(subtext1) / 2 - 7) <= x
			&& x <= ((ofGetWidth() / 2 - font1.stringWidth(subtext1) / 2 - 7) + (font1.stringWidth(subtext1) / 2 - 12))
			&& (ofGetHeight() / 2 + 26) <= y
			&& y <= ((ofGetHeight() / 2 + 26) + font1.stringHeight(subtext1) / 2 + 3)) {

			//변수 초기화
			gamestart_flag = 1;		//게임시작 플래그 업
			gameover_flag = 0;		//게임종료 플래그 내리기
			pY = 0;					//플레이어 y좌표 초기화
			pX = 0;					//플레이어 x좌표 초기화
			haveBomb = 0;			//플레이어 보유 폭탄 개수 초기화
			tmp = 0;				//사운드 재생 관련 변수 초기화

			for (int i = 0; i < numDoor; ++i) {		//문 정보 초기화
				doorArr[i].x = 0;
				doorArr[i].y = 0;
			}

			generateMaze(mazeHeight, mazeWidth);	//미로 생성
			generateDoor();							//문 생성
			generateItem();							//아이템 생성
			generateBomb();							//폭탄 생성
			generateEnemy();						//적 생성

			sound_click.play();						//버튼 클릭 효과음 재생
		}
	
		//EXIT 클릭 시
		else if ((ofGetWidth() / 2 - font1.stringWidth(subtext2) / 2 + 98) <= x
			&& x <= ((ofGetWidth() / 2 - font1.stringWidth(subtext2) / 2 + 98) + (font1.stringWidth(subtext2) / 2 - 7))
			&& (ofGetHeight() / 2 + 26) <= y
			&& y <= ((ofGetHeight() / 2 + 26) + font1.stringHeight(subtext2) / 2 + 3)) {

			freeMemory();		//메모리 해제
			_Exit(0);			//프로그램 종료
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
inline int mazeArrV(int i) {							//실제 미로의 정보에 맞게 좌표를 조정하는 함수
	return 2 * i + 1;
}

//--------------------------------------------------------------
void ofApp::generateMaze(int H, int W) {				//미로 생성 함수
	for (int i = 0; i <= H * 2; i += 2) {				//가로 경계 생성
		for (int j = 0; j < W * 2; j += 2) {
			mazeArr[i][j] = '+';
			mazeArr[i][j + 1] = '-';
		}
		mazeArr[i][W * 2] = '+';
	}
	for (int i = 1; i < H * 2; i += 2) {				//세로 경계 생성
		for (int j = 0; j < W * 2; j += 2) {
			mazeArr[i][j] = '|';
			mazeArr[i][j + 1] = ' ';
		}
		mazeArr[i][W * 2] = '|';
	}
	for (int i = 3; i <= H * 2 - 1; i += 2) {			//각 셀에 대하여 북쪽이나 서쪽으로 길 생성
		for (int j = 3; j <= W * 2 - 1; j += 2) {
			int way = rand() % 2;						//북쪽이나 서쪽 중 랜덤 선택
			if (way == 0) mazeArr[i - 1][j] = ' ';		//북쪽으로 길 생성
			else mazeArr[i][j - 1] = ' ';				//서쪽으로 길 생성
		}
	}
	for (int i = 3; i <= H * 2 - 1; i += 2)				//첫 번째 열에 대하여 북쪽으로만 길 생성(경계를 없앨 수 없으므로)
		mazeArr[i - 1][1] = ' ';
	for (int j = 3; j <= W * 2 - 1; j += 2)				//첫 번째 행에 대하여 서쪽으로만 길 생성(경계를 없앨 수 없으므로)
		mazeArr[1][j - 1] = ' ';
}

//--------------------------------------------------------------
void ofApp::generateDoor() {							//문의 위치 랜덤 생성 함수
	int tmpY, tmpX;
	for (int i = 0; i < numDoor / 2; ++i) {				//문의 절반은 미로의 왼쪽에서 생성
		while (1) {										//첫번째 행과 열, 미로의 중간 열은 회피하여 생성
			int wrong = 0;
			tmpY = rand() % (mazeHeight - 1);			//범위: 0~(mazeHeight-2)
			tmpY += 1;									//범위: 1~(mazeHeight-1)
			tmpX = rand() % (mazeWidth / 2 - 2);		//범위: 0~mazeWidth/2-3
			tmpX += 1;									//범위: 1~(mazeWidth/2-2)
			for (int j = 0; j < (numDoor / 2); ++j) {	//다른 문과 좌표 중복 방지
				if (tmpY == doorArr[j].y && tmpX == doorArr[j].x) wrong = 1;
			}
			if (wrong == 0) break;
		}
		doorArr[i].y = tmpY;							//생성된 문의 y좌표 저장
		doorArr[i].x = tmpX;							//생성된 문의 x좌표 저장
	}
	for (int i = numDoor / 2; i < numDoor; ++i) {		//문의 절반은 미로의 오른쪽에서 생성
		while (1) {
			int wrong = 0;
			tmpY = rand() % (mazeHeight - 1);			//범위: 0~(mazeHeight-2)
			tmpY += 1;									//범위: 1~(mazeHeight-1)
			tmpX = rand() % (mazeWidth / 2 - 1);		//범위: 0~(mazeWidth/2-2)
			tmpX += mazeWidth / 2 + 1;					//범위: 7~(mazeWidth/2+5)
			if ((tmpY == mazeHeight - 1)
				&& (tmpX == mazeWidth - 1)) continue;	//골인 지점 회피
			for (int j = 0; j < numDoor; ++j) {			//다른 문과 좌표 중복 방지
				if (tmpY == doorArr[j].y && tmpX == doorArr[j].x) wrong = 1;
			}
			if (wrong == 0) break;
		}
		doorArr[i].y = tmpY;							//생성된 문의 y좌표 저장
		doorArr[i].x = tmpX;							//생성된 문의 x좌표 저장
	}
}

//--------------------------------------------------------------
void ofApp::generateItem() {							//아이템의 위치 랜점 생성 함수
	int tmpY, tmpX;
	for (int i = 0; i < numItem; ++i) {
		while (1) {										//미로의 첫번째 행과 열은 회피하여 생성
			int wrong = 0;
			tmpY = rand() % (mazeHeight - 1);			//범위: 0~(mazeHeight-2)
			tmpY += 1;									//범위: 1~(mazeHeight-1)
			tmpX = rand() % (mazeWidth - 1);
			tmpX += 1;
			if ((tmpY == mazeHeight - 1)
				&& (tmpX == mazeWidth - 1)) continue;	//골인지점 회피
			for (int j = 0; j < numDoor; ++j) {			//문과 좌표 중복 방지
				if (tmpY == doorArr[j].y && tmpX == doorArr[j].x) wrong = 1;
			}
			for (int j = 0; j < numItem; ++j) {			//다른 아이템과 좌표 중복 방지
				if (tmpY == itemArr[j].p.y && tmpX == itemArr[j].p.x) wrong = 1;
			}
			if (wrong == 0) break;
		}
		itemArr[i].p.y = tmpY;							//생성된 아이템의 y좌표 저장
		itemArr[i].p.x = tmpX;							//생성된 아이템의 x좌표 저장
		itemArr[i].taken_flag = 0;						//아이템 미획득 표시
	}
}

//--------------------------------------------------------------
void ofApp::generateBomb() {							//폭탄의 위치 랜덤 생성 함수
	int tmpY, tmpX, j;
	for (int i = 0; i < numBomb; ++i) {
		while (1) {										//미로의 첫번째 행과 열은 회피하여 생성
			int wrong = 0;
			tmpY = rand() % (mazeHeight - 1);			//범위: 0~(mazeHeight-2)
			tmpY += 1;									//범위: 1~(mazeHeight-1)
			tmpX = rand() % (mazeWidth - 1);
			tmpX += 1;
			if ((tmpY == mazeHeight - 1)
				&& (tmpX == mazeWidth - 1)) continue;	//골인지점 회피
			for (j = 0; j < numDoor; ++j) {				//문과 좌표 중복 방지
				if (tmpY == doorArr[j].y && tmpX == doorArr[j].x) wrong = 1;
			}
			for (j = 0; j < numItem; ++j) {				//아이템과 좌표 중복 방지
				if (tmpY == itemArr[j].p.y && tmpX == itemArr[j].p.x) wrong = 1;
			}
			for (j = 0; j < numBomb; ++j) {				//다른 폭탄과 좌표 중복 방지
				if (tmpY == bombArr[j].p.y && tmpX == bombArr[j].p.x) wrong = 1;
			}
			if (wrong == 0) break;
		}
		bombArr[i].p.y = tmpY;							//생성된 폭탄의 y좌표 저장
		bombArr[i].p.x = tmpX;							//생성된 폭탄의 y좌표 저장
		bombArr[i].taken_flag = 0;						//폭탄 미획득 표시
	}
}

//--------------------------------------------------------------
void ofApp::generateEnemy() {							//적의 위치 랜덤 생성 함수
	int tmpY, tmpX;
	for (int i = 0; i < numEnemy; ++i) {
		while (1) {
			int wrong = 0;
			tmpY = rand() % mazeHeight;					//범위: 0~mazeHeight-1
			tmpX = rand() % mazeWidth;
			if ((tmpY == 0) && (tmpX == 0)) continue;	//미로 시작지점 회피
			for (int j = 0; j < numEnemy; ++j) {		//다른 적과 좌표 중복 방지
				if (tmpY == enemyArr[j].p.y && tmpX == enemyArr[j].p.x) wrong = 1;
			}
			if (wrong == 0) break;
		}
		enemyArr[i].p.y = tmpY;							//생성된 적의 y좌표 저장
		enemyArr[i].p.x = tmpX;							//생성된 적의 x좌표 저장
		enemyArr[i].pre_dir = 0;						//적의 이전 방향 초기화
	}
}

//--------------------------------------------------------------
void ofApp::drawMaze(int H, int W) {					//미로 그리기 함수
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
	for (int j = 0; j <= W * 2 - 1; j += 2) {			//아래 경계 그리기
		ofDrawLine(j * sf + offset, 2 * H * sf + offset, (j + 1) * sf + 30 + offset, 2 * H * sf + offset);
	}
	for (int i = 0; i <= H * 2 - 1; i += 2) {			//오른쪽 경계 그리기
		ofDrawLine(W * 2 * sf + offset, i * sf + offset, W * 2 * sf + offset, (i + 1) * sf + 30 + offset);
	}
}

//--------------------------------------------------------------
void ofApp::drawGoal() {								//골인지점 그리기 함수
	ofSetLineWidth(2);
	ofDrawLine(576, 584, 576, 602);						//깃대
	ofSetLineWidth(10);
	ofDrawTriangle(575, 570, 575, 584, 600, 577);		//깃면
}

//--------------------------------------------------------------
void ofApp::drawPlayer() {								//플레이어 그리기 함수
	ofDrawCircle(mazeArrV(pX) * sf + offset, mazeArrV(pY) * sf + offset, 15);
}

//--------------------------------------------------------------
void ofApp::drawDoor(void) {							//문 그리기 함수

	for (int i = 0; i < numDoor; ++i) {
		ofSetColor(150, 150, 150);						//회색(문)
		ofDrawRectangle(mazeArrV(doorArr[i].x) * sf + offset - 15,
			mazeArrV(doorArr[i].y) * sf + offset - 15, 30, 30);
		ofSetColor(0, 0, 0);							//검정(글자)
		if (i < numDoor / 2)							//문 왼쪽 절반
			ofDrawBitmapString(i + 1, mazeArrV(doorArr[i].x) * sf + 7, mazeArrV(doorArr[i].y) * sf + 14);
		else											//문 오른쪽 절반
			ofDrawBitmapString(i - numDoor / 2 + 1, mazeArrV(doorArr[i].x) * sf + 7, mazeArrV(doorArr[i].y) * sf + 14);
	}
}

//--------------------------------------------------------------
void ofApp::drawItem() {								//아이템 그리기 함수
	int x = 5;											//크기 조절 변수
	for (int i = 0; i < numItem; ++i) {					//각 아이템에 대하여
		if (itemArr[i].taken_flag == 1) continue;		//이미 획득한 아이템은 생략
		ofDrawTriangle(									//세모 모양 그리기
			mazeArrV(itemArr[i].p.x) * sf + offset,
			mazeArrV(itemArr[i].p.y) * sf + offset - 2 * x,
			mazeArrV(itemArr[i].p.x) * sf + offset - sqrt(3) * x,
			mazeArrV(itemArr[i].p.y) * sf + offset + 1 * x,
			mazeArrV(itemArr[i].p.x) * sf + offset + sqrt(3) * x,
			mazeArrV(itemArr[i].p.y) * sf + offset + 1 * x);
	}
}
//--------------------------------------------------------------
void ofApp::drawBomb() {								//폭탄 그리기 함수
	int x = 7;											//크기 조절 변수
	for (int i = 0; i < numBomb; ++i) {					//각 폭탄에 대하여
		if (bombArr[i].taken_flag == 1) continue;		//이미 획득한 폭탄은 생략
		ofFill();
		ofBeginShape();									//별 모양 그리기
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
void ofApp::drawBombInfo() {							//플레이어가 보유한 폭탄 그리기 함수
	int x = 4;											//크기 조절 변수
	for (int i = 0; i < haveBomb; ++i) {				//보유한 폭탄에 대하여
		ofFill();
		ofBeginShape();									//별 모양 그리기
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
void ofApp::drawEnemy(void) {							//적 그리기 함수
	int i;
	for (i = 0; i < numEnemy - 1; ++i) {				//서로 만난 적 확인
		for (int j = i + 1; j < numEnemy; ++j) {
			if (enemyArr[i].p.y == enemyArr[j].p.y && enemyArr[i].p.x == enemyArr[j].p.x) {
				enemyArr[i].meet_flag = 1;
				enemyArr[j].meet_flag = 1;
			}
		}
	}
	for (i = 0; i < numEnemy; ++i) {
		if (enemyArr[i].meet_flag == 1)					//서로 만난 적은 어두운 빨강으로 표시
			ofSetColor(150, 0, 0);
		else ofSetColor(255, 0, 0);						//혼자 있는 적은 빨강으로 표시
		ofDrawCircle(mazeArrV(enemyArr[i].p.x) * sf + offset,
			mazeArrV(enemyArr[i].p.y) * sf + offset, 15);
	}
	for (i = 0; i < numEnemy; ++i)						//만남 여부 초기화
		enemyArr[i].meet_flag = 0;
}

//--------------------------------------------------------------
bool ofApp::checkRoad(int Y, int X, int dirY, int dirX) {	//(Y+dirY, X+dirX)에 길의 유무 확인
															//(Y, X): 현재 위치, (dirY, dirX): 이동할 방향
	if (mazeArr[2 * Y + 1 + dirY][2 * X + 1 + dirX] == ' ')	//길이 있음
		return true;
	else false;												//길이 없음(경계가 있음)
}

//--------------------------------------------------------------
void ofApp::movePlayer(int dirY, int dirX) {				//플레이어를 (dirY, dirX)방향으로 이동
	sound_move.play();										//플레이어 이동 효과음 재생
	if (checkRoad(pY, pX, dirY, dirX) == 1) {				//길이 있는 경우
		pY = pY + dirY;										//플레이어 y좌표 이동
		pX = pX + dirX;										//플레이어 x좌표 이동
	}
}

//--------------------------------------------------------------
int ofApp::opposDir(int dir) {								//dir의 반대 방향을 반환하는 함수
	if (dir == 0) return 1;									//동쪽이면 서쪽 반환
	else if (dir == 1) return 0;							//서쪽이면 동쪽 반환
	else if (dir == 2) return 3;							//남쪽이면 북쪽 반환
	else if (dir == 3) return 2;							//북쪽이면 남쪽 반환
}

//--------------------------------------------------------------
void ofApp::moveEnemy() {									//적을 랜덤 방향으로 이동하는 함수
	int j;
	for (int i = 0; i < numEnemy; ++i) {					//각 적에 대하여
		int cnt = 0;										//이동 가능한 방향의 수 변수
		for (j = 0; j < 4; ++j) {							//동서남북 4가지 방향에 대하여
			if (checkRoad(enemyArr[i].p.y, enemyArr[i].p.x, dirY[j], dirX[j]) == true) { //길이 있는 경우
				cnt++;										//이동 가능한 방향의 수 변수 증가
				enemyDirArr[cnt - 1] = j;					//이동가능한 방향 저장
			}
		}
		if (cnt == 1) {										//이동 가능한 방향이 하나인 경우
			enemyArr[i].p.y += dirY[enemyDirArr[0]];		//적의 좌표를 해당 방향으로 이동
			enemyArr[i].p.x += dirX[enemyDirArr[0]];
			enemyArr[i].pre_dir = enemyDirArr[0];			//이동한 방향을 적의 이전 방향으로 저장
		}
		else if (cnt == 2) {								//이동 가능한 방향이 2개인 경우
			for (j = 0; j < cnt; ++j) {
				if (enemyDirArr[j] != opposDir(enemyArr[i].pre_dir)) {	//뒤로 되돌아가는 것 방지
					enemyArr[i].p.y += dirY[enemyDirArr[j]];			//적의 좌표를 해당 방향으로 이동
					enemyArr[i].p.x += dirX[enemyDirArr[j]];
					enemyArr[i].pre_dir = enemyDirArr[j];				//이동한 방향을 적의 이전 방향으로 저장
					break;
				}
			}
		}
		else {															//이동 가능한 방향이 3개 이상인 경우
			int way;
			while (1) {
				way = rand() % cnt;										//한쪽 방향을 임의로 선택
				if (enemyDirArr[way] != opposDir(enemyArr[i].pre_dir))	//뒤로 되돌아가는 것 방지
					break;
			}
			enemyArr[i].p.y += dirY[enemyDirArr[way]];					//적의 좌표를 해당 방향으로 이동
			enemyArr[i].p.x += dirX[enemyDirArr[way]];
			enemyArr[i].pre_dir = enemyDirArr[way];						//이동한 방향을 적의 이전 방향으로 저장
		}
	}
}

//--------------------------------------------------------------
void ofApp::enterDoor(int& Y, int& X, int door) {			//문을 통해 플레이어를 이동시키는 함수
	sound_teleport.play();
	if (door < numDoor / 2) {								//미로 왼쪽 문에 대하여
		Y = doorArr[door + numDoor / 2].y;					//미로 오른쪽에서 같은 번호를 가진 문으로 플레이어 좌표를 이동
		X = doorArr[door + numDoor / 2].x;
	}
	else {													//미로 오른쪽 문에 대하여
		Y = doorArr[door - numDoor / 2].y;					//미로 왼쪽에서 같은 번호를 가진 문으로 플레이어 좌표를 이동
		X = doorArr[door - numDoor / 2].x;
	}
}

//--------------------------------------------------------------
void ofApp::bombExplode(int dirY, int dirX) {							//폭탄으로 벽을 무너뜨리는 함수
	if (mazeArr[mazeArrV(pY) + dirY][mazeArrV(pX) + dirX] != ' ') {		//해당 방향에 벽이 있는지 확인
		if (0 < (mazeArrV(pY) + dirY)									//해당 벽이 미로 경계가 아닌지 확인
			&& (mazeArrV(pY) + dirY) < mazeHeight * 2
			&& 0 < (mazeArrV(pX) + dirX)
			&& (mazeArrV(pX) + dirX) < mazeWidth * 2) {
			sound_boom.play();											//폭파 효과음 재생
			mazeArr[mazeArrV(pY) + dirY][mazeArrV(pX) + dirX] = ' ';	//벽 제거
			haveBomb--;													//플레이어가 보유한 폭탄 수 감소
		}
	}
}

//--------------------------------------------------------------
void ofApp::freeMemory() {						//메모리 해제 함수
	for (int i = 0; i < mazeHeight; ++i) {		//미로 정보 2차원 배열 해제
		delete[] mazeArr[i];
	}
	delete[] mazeArr;

	delete[] doorArr;					//문 정보 배열 해제
	delete[] itemArr;					//아이템 정보 배열 해제
	delete[] enemyArr;					//적 정보 배열 해제
	delete[] bombArr;					//폭탄 정보 배열 해제
	delete[] enemyDirArr;				//적의 이동가능한 방향 정보 배열 해제
}

/* ************************ USER DEFINED FUNCTIONS *********************** */