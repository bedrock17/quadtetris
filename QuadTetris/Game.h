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
	������ ����,����,�ʱ�ȭ,����,���� �� ������ Ŭ����
	�׹ۿ� �������࿡ �ʿ��� �͵� ����
*/

#define MAPSIZE 18
#define MAXBLOCK 4
#define MAXSLOWSPEED 11
#define ARROWSIZE 6

void gotoxy(short x, short y); //�ܼ� Ŀ�� �̵�
void setColor(short color, short bgcolor); //�� ����

typedef enum _color { //����� ��
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

typedef struct _event { //�Է� ó����
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

	bool directionUpdate;//������� �ٲ�� ��� �����忡�˷���

	int blockCount; //��ϰ���
	int gameLoopCount; //���� ����� ������?
	int nowDirection; //���� �������

	int score; //����
	int level; //MAX 10

	bool gameContinue; //���� ���� �˻�

	short map[MAPSIZE][MAPSIZE];
	
	void Init(); //�⺻ ����
	
	void screenUpdate(); //ȭ���� ��� ������Ʈ ���� ������
	void keyUpdate(); //�Է��� �޾��� ������ 
	void mainLoop(); //���ӿ��� �����͸� ó���� ����

	void newBlock(); //���ο� ��� �Ҵ�
	void putBlock(); //����� �ʿ� ����

	void inputProc(int direction);//���� ����ǰ� �ִ� ������ �ݴ븦 ������ �ٸ� ������ ó��

	bool blockMove(int direction); //���ڷ� �Ѱ��� �������� ���� �� �� �ִٸ� true,���ٸ� false
	bool blockRotate(int direction);//���ڷ� �Ѱ��� �������� ȸ�� �� �� �ִٸ� true,���ٸ� false

	bool isPutAble(brick * chkbricks);//�ʿ� ��ϵ��� ������ �ִ��� �˻�

	
	
	void lineRemove(int pos,bool vertical);
	bool lineCheck(int pos, bool vertical);
	void lineMove(int pos, bool vertical);

	void gameOver();


public:
	Game();//������ �ʱ�ȭ
	~Game();
	
	void start(); //���� ����
	
};

