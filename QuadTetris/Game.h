#pragma once
#include <cstdio>
#include <windows.h>
#include <cstdlib>
#include <thread>
#include <random>
#include <mutex>
#include <iostream>
#include <cstdarg>
#include "Block.h"

//2016.10.27
/*
	게임의 점수,레벨,초기화,진행,판정 을 관리할 클래스
	그밖에 게임진행에 필요한 것들 포함
*/

#define MAPSIZE 18
#define MAXBLOCK 4
#define MAXSLOWSPEED 11
#define ARROWSIZE 6

void gotoxy(short x, short y); //콘솔 커서 이동
void setColor(short color, short bgcolor); //색 변경

typedef enum _color { //사용할 색
	BLACK = 0,
	GRAY = 8,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	PINK,
	YELLOW,
	WHITE
}COLOR;

typedef enum _direction {
	UP = 0,
	RIGHT,
	DOWN,
	LEFT
}DIRECTION;

typedef struct _event { //입력 처리용
	bool RIGHT;
	bool LEFT;
	bool UP;
	bool DOWN;
	bool Z;
	bool X;
	//bool SPACE;
}Event;

//void mprintStatus(char * message,int i);

class Game
{
private:
	
	Block *block;
	Event keyEvent;
	std::mutex mtx;
	std::random_device rd;

	bool directionUpdate;//진행방향 바뀔시 출력 스레드에알려줌

	int blockCount; //블록개수
	int gameLoopCount; //게임 진행된 프레임?
	int nowDirection; //현재 진행방향

	int score; //점수
	int level; //MAX 10

	bool gameContinue; //게임 오버 검사

	short map[MAPSIZE][MAPSIZE];
	
	void Init(); //기본 셋팅
	
	void screenUpdate(); //화면을 계속 업데이트 해줄 스레드
	void keyUpdate(); //입력을 받아줄 스레드 
	void mainLoop(); //게임에서 데이터를 처리할 루프

	void newBlock(); //새로운 블록 할당
	void putBlock(); //블록을 맵에 저장

	void inputProc(int direction);//현재 진행되고 있는 방향의 반대를 제외한 다른 움직임 처리

	bool blockMove(int direction); //인자로 넘겨준 방향으로 움직 일 수 있다면 true,없다면 false
	bool blockRotate(int direction);//인자로 넘겨준 방향으로 회전 할 수 있다면 true,없다면 false

	bool isPutAble(brick * chkbricks);//맵에 블록들을 놓을수 있는지 검사

	
	
	void lineRemove(int pos,bool vertical);
	bool lineCheck(int pos, bool vertical);
	void lineMove(int pos, bool vertical);

	void gameOver();


public:
	Game();//데이터 초기화
	~Game();
	
	void start(); //게임 시작
	
};

