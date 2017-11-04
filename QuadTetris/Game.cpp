#include "Game.h"




void setColor(short bgcolor, short color) {
	color &= 0xf;
	bgcolor &= 0xf;

	if (color == 1) {
		color = BLACK;
		bgcolor = BLACK;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}
void gotoxy(short x, short y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

Game::Game()
{
	Init();
}
Game::~Game()
{
}

/////////////test
void mprintStatus(char * message,int i) {
	gotoxy(1, 18 +i);
	setColor(BLUE, WHITE);
	printf("%s", message);
}
///////////////

void Game::Init() {
	for (int i = 0; i < MAPSIZE; i++) {
		map[0][i] = 1;
		map[MAPSIZE - 1][i] = 1l;
		map[i][0] = 1;
		map[i][MAPSIZE - 1]=1;
	}
	for (int i = 1; i < MAPSIZE - 1; i++) {
		for (int j = 1; j < MAPSIZE - 1; j++) {
			map[i][j] = 0;
		}
	}

	keyEvent.DOWN = false;
	keyEvent.LEFT = false;
	keyEvent.RIGHT = false;
	keyEvent.UP = false;
	keyEvent.X = false;
	keyEvent.Z = false;
	
	gameContinue = true;
	directionUpdate = false;

	blockCount		= 0;
	gameLoopCount	= 0;
	nowDirection	= 2;
	directionUpdate = true;
	
	score = 0;
	level = 1;
	block = NULL;
	

}

void Game::start() {

	setColor(WHITE, WHITE);
	
	for (int i = 0; i < MAPSIZE; i++) {
		for (int j = 0; j < MAPSIZE; j++)
			printf(" 1");
		printf("\n");
	}

	setColor(BLACK, SKYBLUE);
	gotoxy(52, MAPSIZE - 3);
	printf("SCORE");



	newBlock();//첫번째 블록


	std::thread SUpdate(&Game::screenUpdate, this);
	std::thread KUpdate(&Game::keyUpdate, this);
	mainLoop();
	
	KUpdate.join();
	SUpdate.join();


	system("cls");
	
	gameOver();

}

void Game::screenUpdate() {
	brick*outputbricks; //출력을 위한 벽돌들
	int count=0;
	short screen[MAPSIZE][MAPSIZE] = { 0 };
	short newscreen[MAPSIZE][MAPSIZE] = { 0 };


	short arrowscreen[6][6] = { 0 };
	short tmparrow[6][6] = { 0 };
	short arrow[6][6] = {
		0,0,0,1,0,0,
		0,0,0,1,1,0,
		1,1,1,1,1,1,
		1,1,1,1,1,1,
		0,0,0,1,1,0,
		0,0,0,1,0,0
	};
	

	while (gameContinue) {
		for (int i = 0; i < MAPSIZE; i++)
			for (int j = 0; j < MAPSIZE; j++)
				newscreen[i][j] = 0; //화면 초기화


		mtx.lock(); //다른 메모리 참조전 락

		for (int i = 0; i < MAPSIZE; i++)
			for (int j = 0; j < MAPSIZE; j++)
				newscreen[i][j] = map[i][j]; //변경된 맵 그리기


		

		if (block != NULL) { //활성화 되어있는 블록 그리기
			
			setColor(BLACK, WHITE);
			outputbricks = block->getPos();
			for (int i = 0; i < block->length; i++) {
				newscreen[outputbricks[i].ipos][outputbricks[i].jpos] = outputbricks[i].color;

				/*
				char msg[100];
				sprintf_s(msg, "%d %d                    %p          %d ", outputbricks[i].ipos, outputbricks[i].jpos, this,count++);
				mprintStatus(msg, i);
				*/


			}

			delete outputbricks;
			
		}
		
		for (int i = 0; i < MAPSIZE; i++) {
			for (int j = 0; j < MAPSIZE; j++) {
				if (screen[i][j] != newscreen[i][j]) { //변경된 부분이 있을시에만
					
					if(newscreen[i][j]>1)
						setColor(BLACK, newscreen[i][j]);
					else
						setColor(WHITE, newscreen[i][j]);
					
					gotoxy(j * 2, i);
					if (newscreen[i][j])
						printf("■");
					else
						printf("  ");

					screen[i][j] = newscreen[i][j];
				}
			}
		}

		if (directionUpdate) {

			
			for (int i = 0; i < ARROWSIZE; i++) 
				for (int j = 0; j < ARROWSIZE; j++) 
					tmparrow[j][ARROWSIZE-1-i] = arrow[i][j];
			
			for (int i = 0; i < ARROWSIZE; i++)
				for (int j = 0; j < ARROWSIZE; j++)
					arrow[i][j] = tmparrow[i][j];
					
			
				
			setColor(BLACK, YELLOW);
			for (int i = 0; i < ARROWSIZE; i++) {
				for (int j = 0; j < ARROWSIZE; j++) {
					if (arrow[i][j] != arrowscreen[i][j]) {
						gotoxy(48 + (j * 2), i + 2);
						if (arrow[i][j])
							printf("■");
						else
							printf("  ");
					}
				}
			}

			for (int i = 0; i < ARROWSIZE; i++)
				for (int j = 0; j < ARROWSIZE; j++)
					arrowscreen[i][j] =arrow[i][j] ;

			directionUpdate = false;
		}
		
	
		setColor(BLACK,WHITE);
		gotoxy(50, MAPSIZE-2);
		printf("%09d", score);

		mtx.unlock();
		Sleep(15); //독과점을 막기위한 슬립

		
	}
}
	
void Game::keyUpdate() {
	bool rotatek=false;

	while (gameContinue) {
		mtx.lock();
		if (GetAsyncKeyState(VK_RIGHT) & 0x8001) {
			keyEvent.RIGHT = true;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8001) {
			keyEvent.LEFT = true;
		}
		if (GetAsyncKeyState(VK_UP) & 0x8001) {
			keyEvent.UP = true;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8001) {
			keyEvent.DOWN = true;
		}
		if (GetAsyncKeyState(90) & 0x8001) {
			keyEvent.Z = true;
			rotatek = true;
		}
		if (GetAsyncKeyState(88) & 0x8001) {
			keyEvent.X = true;
			rotatek = true;
		}
		mtx.unlock();
		if (rotatek)
			Sleep(100);
		Sleep(30);
	}

}

void Game::mainLoop() {
	
	while(gameContinue){

		mtx.lock();

		nowDirection = (blockCount + 2) % 4;

		inputProc(nowDirection);
		
		if (gameLoopCount % (MAXSLOWSPEED - level)==0) {
			if (!blockMove(nowDirection)) { //지정된 방향으로 움직일수 없다면
				putBlock();
				
			}
		}
		
		for (int i = 1; i < MAPSIZE-1; i++) {
			if (lineCheck(i, true)) {
				lineRemove(i, true);
				lineMove(i, true);
			}
			if (lineCheck(i, false)) {
				lineRemove(i, false);
				lineMove(i, false);
			}
		}
		
		gameLoopCount++;

		mtx.unlock();
		Sleep(100);	
	}
}

bool Game::blockMove(int direction) {
	brick * chkbricks;

	chkbricks = block->getPos(direction);

	if (isPutAble(chkbricks)) {
		block->addPos(direction);
		delete chkbricks;
		return true;
	}
	delete chkbricks;
	return false;
	
}

bool Game::blockRotate(int direction) {
	
	brick * chkbricks;
	chkbricks = block->getRotatePos(direction);

	if (isPutAble(chkbricks)) {
		block->rotate(direction);
		delete chkbricks;
		return true;
	}
	delete chkbricks;
	return false;


}

void Game::putBlock() {

	

	brick * chkbrickarr=block->getPos();

	for (int i = 0; i < block->length; i++) 
		map[chkbrickarr[i].ipos][chkbrickarr[i].jpos] = chkbrickarr[i].color;

	delete chkbrickarr;
	delete block;
	newBlock();
	blockCount++;


	directionUpdate = true;

}

void Game::newBlock() {
	
	short * tmpbricks = NULL;
	short color = 0;


	switch (rd() % 6) {
	case 0: color = BLUE; tmpbricks = new short[8]{ 1, 1, 1, 2, 2, 1, 2, 2 }; break;
	case 1: color = GREEN; tmpbricks = new short[8]{ 0, 2, 1, 2, 2, 2, 3, 2 }; break;
	case 2: color = YELLOW; tmpbricks = new short[8]{ 0, 2, 1, 1, 1, 2, 2, 1 }; break;
	case 3: color = RED; tmpbricks = new short[8]{ 0, 1, 1, 1, 1, 2, 2, 2 }; break;
	case 4: color = SKYBLUE; tmpbricks = new short[8]{ 1, 1, 1, 2, 2, 2, 3, 2 }; break;
	case 5: color = PINK; tmpbricks = new short[8]{ 1, 1, 1, 2, 2, 1, 3, 1 }; break;
	default: this->~Game();
	}


	block = new Block(color, 4, tmpbricks);

	delete tmpbricks;

	if (!isPutAble(block->getPos())) {
		gameContinue = false;
	}
}


bool Game::isPutAble(brick * chkbricks) {
	for (int i = 0; i < block->length; i++)
		if (map[chkbricks[i].ipos][chkbricks[i].jpos] != 0)
			return false;
		
	return true;
}

void Game::inputProc(int direction) {
	if (keyEvent.DOWN && direction!=UP) {
		if (blockMove(DOWN)) {
			score += level;
		}
		else
			putBlock();
		keyEvent.DOWN = false;
	}
	if (keyEvent.LEFT&& direction !=RIGHT) {
		if (blockMove(LEFT)) {
			score += level;
		}
		else
			putBlock();
		keyEvent.LEFT = false;
	}
	if (keyEvent.RIGHT&& direction != LEFT) {
		if (blockMove(RIGHT)) {
			score += level;
		}
		else
			putBlock();
		keyEvent.RIGHT = false;
	}
	if (keyEvent.UP&& direction != DOWN) {
		if (blockMove(UP)) {
			score += level;
		}
		else
			putBlock();
		keyEvent.UP = false;
	}
	if (keyEvent.Z) {

		blockRotate(LEFT);

		keyEvent.Z = false;
	}
	if (keyEvent.X) {
		blockRotate(RIGHT);
		keyEvent.X = false;
	}

}

bool Game::lineCheck(int pos, bool vertical) {

	std::function<bool()> func;
	int i = 1;

	if (vertical) {
		func = [&]()->bool {
			if (map[i][pos] == 0)
				return false;
			return true;
		};
	}
	else {
		func = [&]()->bool {
			if (map[pos][i] == 0)
				return false;
			return true;
		};
		
	}

	for (i = 1; i < MAPSIZE-1; i++) 
		if (!func())
			return false;

	return true;

}

void Game::lineRemove(int pos, bool vertical) {
	std::function<void()> func;
	int i = 1;
	if (vertical) {
		func = [&]()->void {
			map[i][pos] = 0;
		};
	}
	else {
		func = [&]()->void {
			map[pos][i] = 0;
		};
	}

	for(i=1;i<MAPSIZE-1;i++){
		func();
	}

	score += level * 10;
	level++;

}

void Game::lineMove(int pos, bool vertical) {
	std::function<void()> movefunc;
	int op;
	
	auto swap = [](short *a, short *b)->void {
		short c = *a;
		*a = *b;
		*b = c;
	};

	bool low=true;
	int i,j;
	i = j = 0;

	int endValue=MAPSIZE/2;

	low = pos < MAPSIZE / 2 ? true : false;
	
	if (!low)
		endValue++;

	op = low ? 1 : -1;

	if (vertical) movefunc = [&]()->void {
		swap(&map[j][i], &map[j][i+op]);
	};
	else movefunc = [&]()->void {
		swap(&map[i][j], &map[i+op][j]);
	};

	if (low) 
		for(j=1;j<MAPSIZE-1;j++)
			for (i = pos; i < endValue; i++) 
				movefunc();
			
	
	else
		for (j = 1; j<MAPSIZE-1; j++)
			for (i = pos; i > endValue; i--) 
				movefunc();
			



}

void Game::gameOver() {

	setColor(BLACK, SKYBLUE);
	printf("당신의 점수 : %09d\n", score);

	getchar();
}