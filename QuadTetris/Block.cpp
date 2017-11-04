#include "Block.h"
#include "Game.h" 

Block::Block(short color,short length, short *posarr){
	
	this->length = length;
	
	length *= 2;
	bricks = new brick[length];

	for (int i = 0; i < length; i++) { 
		bricks[i].color = color;
		bricks[i].ipos = posarr[i*2];
		bricks[i].jpos = posarr[i*2+1];
	}
	blockipos=MAPSIZE / 2 - 2;
	blockjpos=MAPSIZE / 2 - 2;
}


Block::~Block()
{
	delete bricks;
}

void Block::rotate(int direction) {
	brick tmpbrick;
	std::function<void()> func;
	int i;

	if (direction == RIGHT) {
		func = [&]()->void{
			bricks[i].ipos = tmpbrick.jpos;
			bricks[i].jpos = MAX_BLOCK_SIZE - 1 - tmpbrick.ipos;
		};
	}
	else { //direction==LEFT
		func = [&]()->void {
			bricks[i].ipos = MAX_BLOCK_SIZE - 1 - tmpbrick.jpos;
			bricks[i].jpos = tmpbrick.ipos;
		};
	}

	for (i = 0; i < length; i++) {
		tmpbrick.ipos = bricks[i].ipos;
		tmpbrick.jpos = bricks[i].jpos;
		func(); 
	}


}

brick * Block::getTempBricks() {
	brick*tmpbricks = new brick[length];
	for (int i = 0; i < length; i++) {
		tmpbricks[i].color = bricks[i].color;
		tmpbricks[i].ipos = bricks[i].ipos;
		tmpbricks[i].jpos = bricks[i].jpos;
	}
	return tmpbricks;
}

brick * Block::getPos() {
	
	brick*tmpbricks = getTempBricks();
	for (int i = 0; i < length; i++) {
		tmpbricks[i].ipos+=blockipos;
		tmpbricks[i].jpos+=blockjpos;
	}
	return tmpbricks;
}

brick * Block::getPos(int direction) {
	brick*tmpbricks = getTempBricks();
	short addi, addj;
	switch (direction) {
	case UP: addi = -1; addj = 0; break;
	case RIGHT: addi = 0; addj = 1; break;
	case DOWN: addi = 1; addj = 0; break;
	case LEFT: addi = 0; addj = -1; break;
	default:system("pause");
	}

	for (int i = 0; i < length; i++) {
		tmpbricks[i].ipos += blockipos + addi;
		tmpbricks[i].jpos += blockjpos + addj;
	}

	return tmpbricks;
}

brick * Block::getRotatePos(int direction) {

	brick tmpbrick;
	brick * tmpbricks = getTempBricks();

	std::function<void()> func;
	int i;

	if (direction == RIGHT) {
		func = [&]()->void {
			tmpbricks[i].ipos = tmpbrick.jpos;
			tmpbricks[i].jpos = MAX_BLOCK_SIZE - 1 - tmpbrick.ipos;
		};
	}
	else { //direction==LEFT
		func = [&]()->void {
			tmpbricks[i].ipos = MAX_BLOCK_SIZE - 1 - tmpbrick.jpos;
			tmpbricks[i].jpos = tmpbrick.ipos;
		};
	}

	for (i = 0; i < length; i++) {
		tmpbrick.ipos = tmpbricks[i].ipos;
		tmpbrick.jpos = tmpbricks[i].jpos;
		func();
		tmpbricks[i].ipos += blockipos;
		tmpbricks[i].jpos += blockjpos;
	}


	return tmpbricks;
} 


void Block::setPos(short ipos,short jpos) {
	blockipos = ipos;
	blockjpos = jpos;
}

void Block::addPos(int direction) {
	short addi, addj;

	switch (direction) {
	case UP: addi = -1; addj = 0; break;
	case RIGHT: addi = 0; addj = 1; break;
	case DOWN: addi = 1; addj = 0; break;
	case LEFT: addi = 0; addj = -1; break;
	default:system("pause");
	}


	blockipos += addi;
	blockjpos += addj;
}

