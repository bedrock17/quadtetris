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
	const short MAX_BLOCK_SIZE=4; //����� ���μ��� �ִ� ���� 4�� ����
	short blockipos, blockjpos; //��ǥ ������
	brick *bricks;



public:
	int length; //����� ����

	Block::Block(short color, short length, short *posarr);//���� ��� ����� ����Ұ��� �׸��� �� ����� ��ǥ �Է��ϸ� ��� ����
	//Block(brick[]); //����Ǿ� �ִ� ���� �ҷ��� ����� ���� ��� ��� �߰�����
	
	~Block(); //�����Ҵ�� ������ ����

	void rotate(int direction); //RIGHT�ð���� LEFT�ݽð����

	void setPos(short ipos,short jpos);//��� ��ǥ ����
	void addPos(int direction);//���ڷ� �Ѱ��� ����ŭ �̵�

	brick * getTempBricks();//bricks���� ����

	brick * getPos(); //��밡���� ��� ��ǥ ��ȯ
	brick * getPos(int direction); //���ڷ� �Ѱ��� �������� ������ ��밡���� ��ǥ ��ȯ
	brick * getRotatePos(int direction);//���ڷ� �Ѱ��� �������� ȸ���� ��밡���� ��ǥ ��ȯ
};

