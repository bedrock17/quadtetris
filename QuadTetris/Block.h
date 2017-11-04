#pragma once

#include <functional>

typedef struct _brick {
	short ipos;
	short jpos;
	short color;
}brick;

class Block
{
private:
	const short MAX_BLOCK_SIZE=4; //블록의 가로세로 최대 길이 4로 고정
	short blockipos, blockjpos; //좌표 기준점
	brick *bricks;



public:
	int length; //블록의 개수

	Block::Block(short color, short length, short *posarr);//색과 몇개의 블록을 사용할건지 그리고 각 블록의 좌표 입력하면 블록 생성
	//Block(brick[]); //저장되어 있는 값을 불러와 사용자 지정 블록 사용 추가예정
	
	~Block(); //동적할당된 벽돌들 해제

	void rotate(int direction); //RIGHT시계방향 LEFT반시계방향

	void setPos(short ipos,short jpos);//블록 좌표 설정
	void addPos(int direction);//인자로 넘겨준 값만큼 이동

	brick * getTempBricks();//bricks깊은 복사

	brick * getPos(); //사용가능한 블록 좌표 반환
	brick * getPos(int direction); //인자로 넘겨준 방향으로 움직인 사용가능한 좌표 반환
	brick * getRotatePos(int direction);//인자로 넘겨준 방향으로 회전한 사용가능한 좌표 반환
};

