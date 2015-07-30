#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <ctime>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define ESCAPE 27

using namespace std;

void coutc(int, char*);
void borders();
void gameOver();
void food();
void exfood();
void gotoxy(int ,int);
void step(char, bool &);
COORD cOnv(int, int);
bool checkSnake(int, int);
void snakeLogo();
void menu(bool &);
void about(bool &);
void GameSpeed(bool &);
void GameMode(bool &);
int exscoreval(long long);
void drawBords();

const int width = 79, height = 20;
int xh, yh, xt, yt, gameMode;
int food_x, food_y, exfood_x, exfood_y, exfoodFlag, foodCount, score, prevdir, speed;
long long exscore=0;
vector<COORD> snake(3);
vector<COORD> bords(0);

int main()
{
	system("TITLE Snake - El E7san Team");
	char rep;
	do
	{
		system("CLS");
		prevdir = RIGHT;
		snake.resize(3);
		exfoodFlag = 0;
		foodCount=0;
		score=0;
		bool eXit = true;
		menu(eXit);

		if(!eXit) {
			return 0;
		}

		borders();
		drawBords();
		food();
		snake[0].X = 4;
		snake[0].Y = (height / 2);
		snake[1].X = 5;
		snake[1].Y = (height / 2);
		snake[2].X = 6;
		snake[2].Y = (height / 2);

		for(unsigned int i=0; i<snake.size(); i++) {
			gotoxy(snake[i].X, snake[i].Y);
			coutc(10, "*");
		}

		while(true) {
		if(kbhit()) {
			char c = getch();
			if(c == prevdir) {
				continue;
			}
			step(c, eXit);
		}else {
			_sleep(150-(speed*10));
			step(prevdir, eXit);
		}
		if(!eXit) {
			break;
		}
		}
		gotoxy(32, 12);
		cout << "Would You Like To Repeat Again ? (Y/N)";
		while((rep = getch())) {
			if(rep == 'Y' || rep == 'y') {
				break;
			}else if(rep == 'N' || rep == 'n') {
				break;
			}
		}
	}while(rep == 'Y' || rep == 'y');
	return 0;
}

COORD cOnv(int x, int y) {
	COORD temp;
	temp.X = x;
	temp.Y = y;

	return temp;
}

void step(char c, bool &eXit) {
	xh = snake[snake.size() - 1].X;
	yh = snake[snake.size() - 1].Y;
	xt = snake[0].X;
	yt = snake[0].Y;

	if(foodCount % 9 == 0 && exfoodFlag == 0 && foodCount != 0 && exscore == 0) {
		exfood();
		exscore = 2700000000000000000;
		exfoodFlag = 1;
	}
	if(exscore > 2) {
		exscore /= 2;
	}

	if(xh == exfood_x && yh == exfood_y) {
		score += exscoreval(exscore);
		gotoxy(17, height + 2);
		cout << score;
		exfoodFlag = 2;
		exscore = 0;
		exfood_x = 0;
		exfood_y = 0;
	}else if(exscore <= 2 && exfoodFlag == 1) {
		gotoxy(exfood_x, exfood_y);
		cout << " ";
		exfoodFlag = 2;
		exscore = 0;
		exfood_x = 0;
		exfood_y = 0;
	}


	if(xh == food_x && yh == food_y) {
		food();
		snake.push_back(cOnv(xh, yh));
		score += speed;
		foodCount++;
		gotoxy(17, height + 2);
		cout << score;
		if(exfoodFlag == 2) {
			exfoodFlag = 0;
		}
	}

	if(c == UP && prevdir != DOWN) {
		prevdir = UP;
		if(gameMode == 1) {
		gotoxy(xh, yh - 1);
		coutc(10, "*");
		snake.push_back(cOnv(xh, yh - 1));
		if(yh - 1 == 0 || xh == 0 || xh == width || checkSnake(xh, yh-1)) {
			gameOver();
			eXit = false;
		}
		}else if(gameMode == 0 || gameMode == 2 || gameMode == 3) {

			if(yh - 1 == 0) {
				gotoxy(xh, height - 1);
				coutc(10, "*");
				snake.push_back(cOnv(xh, height - 1));
				if(checkSnake(xh, height - 1)) {
					gameOver();
					eXit = false;
				}
			}else {
				gotoxy(xh, yh - 1);
				coutc(10, "*");
				snake.push_back(cOnv(xh, yh - 1));
				if(checkSnake(xh, yh-1)) {
				gameOver();
				eXit = false;
				}
			}
		}
		gotoxy(xt, yt);
		cout << " ";
		snake.erase(snake.begin());

	}else if(c == DOWN && prevdir != UP) {
		prevdir = DOWN;
		if(gameMode == 1) {
		gotoxy(xh, yh + 1);
		coutc(10, "*");
		snake.push_back(cOnv(xh, yh + 1));
		if(yh + 1 == height || xh == 0 || xh == width || checkSnake(xh, yh+1)) {
			gameOver();
			eXit = false;
		}
		}else if(gameMode == 0 || gameMode == 2 || gameMode == 3) {

			if(yh + 1 == height) {
				gotoxy(xh, 1);
				coutc(10, "*");
				snake.push_back(cOnv(xh, 1));
				if(checkSnake(xh, 1)) {
					gameOver();
					eXit = false;
				}
			}else {
				gotoxy(xh, yh + 1);
				coutc(10, "*");
				snake.push_back(cOnv(xh, yh + 1));
				if(checkSnake(xh, yh+1)) {
				gameOver();
				eXit = false;
				}
			}
		}
		gotoxy(xt, yt);
		cout << " ";
		snake.erase(snake.begin());
	}else if(c == LEFT && prevdir != RIGHT) {
		prevdir = LEFT;
		if(gameMode == 1) {
		gotoxy(xh - 1, yh);
		coutc(10, "*");
		snake.push_back(cOnv(xh - 1, yh));
		if(xh - 1 == 0 || yh == 0 || yh == height || checkSnake(xh-1, yh)) {
			gameOver();
			eXit = false;
		}
		}else if(gameMode == 0 || gameMode == 2 || gameMode == 3) {

			if(xh - 1 == 0) {
				gotoxy(width -1, yh);
				coutc(10, "*");
				snake.push_back(cOnv(width - 1, yh));
				if(checkSnake(width - 1, yh)) {
					gameOver();
					eXit = false;
				}
			}else {
				gotoxy(xh - 1, yh);
				coutc(10, "*");
				snake.push_back(cOnv(xh - 1, yh));
				if(checkSnake(xh-1, yh)) {
				gameOver();
				eXit = false;
				}
			}
		}
		gotoxy(xt, yt);
		cout << " ";
		snake.erase(snake.begin());
	}else if(c == RIGHT && prevdir != LEFT) {
		prevdir = RIGHT;
		if(gameMode == 1) {
		gotoxy(xh + 1, yh);
		coutc(10, "*");
		snake.push_back(cOnv(xh + 1, yh));
		if(xh + 1 == width || yh == 0 || yh == height || checkSnake(xh+1, yh)) {
			gameOver();
			eXit = false;
		}
		}else if(gameMode == 0 || gameMode == 2 || gameMode == 3) {

			if(xh + 1 == width) {
				gotoxy(1, yh);
				coutc(10, "*");
				snake.push_back(cOnv(1, yh));
				if(checkSnake(1, yh)) {
					gameOver();
					eXit = false;
				}
			}else {
				gotoxy(xh + 1, yh);
				coutc(10, "*");
				snake.push_back(cOnv(xh + 1, yh));
				if(checkSnake(xh+1, yh)) {
				gameOver();
				eXit = false;
				}
			}
		}
		gotoxy(xt, yt);
		cout << " ";
		snake.erase(snake.begin());
	}else if(c == ESCAPE) {
		gotoxy(width - 10, height + 2);
		system("PAUSE > NUL | echo Pause !!");
		gotoxy(width - 10, height + 2);
		cout << "\t\t";
	}

}

void gotoxy(int x, int y)
{
COORD cursorPlace;
cursorPlace.X = x;
cursorPlace.Y = y;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPlace);
}

void drawBords() {

	if(gameMode == 2) {
		bords.resize(92);
		for(unsigned int i=0; i<bords.size(); i++) {

			if(i < (bords.size() - 32) / 2) {
				gotoxy(25+i, 6);
				coutc(15,"#");
				bords[i] = cOnv(25+i, 6);
			}else if(i < (bords.size() - 32)) {
				gotoxy(25+i-30, 14);
				coutc(15,"#");
				bords[i] = cOnv(25+i-30, 14);
			}else {
				if(i < bords.size() - 27) {
					gotoxy(1 + i - 60, 1);
					coutc(15,"#");
					bords[i] = cOnv(1 + i - 60, 1);
				}else if(i < bords.size() - 22) {
					gotoxy(75 + i - 66, 1);
					coutc(15,"#");
					bords[i] = cOnv(75 + i - 66, 1);
				}else if(i < bords.size() - 19) {
					gotoxy(1, 2 + i - 70);
					coutc(15,"#");
					bords[i] = cOnv(1, 2 + i - 70);
				}else if(i < bords.size() - 16) {
					gotoxy(78, 2 + i - 73);
					coutc(15,"#");
					bords[i] = cOnv(78, 2 + i - 73);
				}else if(i < bords.size() - 13) {
					gotoxy(1, 16 + i - 76);
					coutc(15,"#");
					bords[i] = cOnv(1, 16 + i - 76);
				}else if(i < bords.size() - 10) {
					gotoxy(78, 16 + i - 79);
					coutc(15,"#");
					bords[i] = cOnv(78, 16 + i - 79);
				}else if(i < bords.size() - 5) {
					gotoxy(2 + i - 83, 19);
					coutc(15,"#");
					bords[i] = cOnv(2 + i - 83, 19);
				}else {
					gotoxy(75 + i - 88, 19);
					coutc(15,"#");
					bords[i] = cOnv(75 + i - 88, 19);
				}
			}
		}
	}else if(gameMode == 3) {
		bords.resize(88);
		for(unsigned int i=0; i<bords.size(); i++) {

			if(i < bords.size() - 76) {
				gotoxy(25+i, 6);
				coutc(15,"#");
				bords[i] = cOnv(25+i, 6);
			}else if(i < bords.size() - 64) {
				gotoxy(43+i - 12, 6);
				coutc(15,"#");
				bords[i] = cOnv(43+i - 12, 6);
			}else if(i < bords.size() - 62) {
				gotoxy(25, 7+i-24);
				coutc(15,"#");
				bords[i] = cOnv(25, 7+i-24);
			}else if(i < bords.size() - 60) {
				gotoxy(54, 7+i-26);
				coutc(15,"#");
				bords[i] = cOnv(54, 7+i-26);
			}else if(i < bords.size() - 58) {
				gotoxy(25, 12+i-28);
				coutc(15,"#");
				bords[i] = cOnv(25, 12+i-28);
			}else if(i < bords.size() - 56) {
				gotoxy(54, 12+i-30);
				coutc(15,"#");
				bords[i] = cOnv(54, 12+i-30);
			}else if(i < bords.size() - 44) {
				gotoxy(25 + i - 32, 14);
				coutc(15,"#");
				bords[i] = cOnv(25 + i - 32, 14);
			}else if(i < bords.size() - 32) {
				gotoxy(43 + i - 44, 14);
				coutc(15,"#");
				bords[i] = cOnv(43 + i - 44, 14);
			}else {
				if(i < bords.size() - 27) {
					gotoxy(1 + i - 56, 1);
					coutc(15,"#");
					bords[i] = cOnv(1 + i - 56, 1);
				}else if(i < bords.size() - 22) {
					gotoxy(75 + i - 62, 1);
					coutc(15,"#");
					bords[i] = cOnv(75 + i - 62, 1);
				}else if(i < bords.size() - 19) {
					gotoxy(1, 2 + i - 66);
					coutc(15,"#");
					bords[i] = cOnv(1, 2 + i - 66);
				}else if(i < bords.size() - 16) {
					gotoxy(78, 2 + i - 69);
					coutc(15,"#");
					bords[i] = cOnv(78, 2 + i - 69);
				}else if(i < bords.size() - 13) {
					gotoxy(1, 16 + i - 72);
					coutc(15,"#");
					bords[i] = cOnv(1, 16 + i - 72);
				}else if(i < bords.size() - 10) {
					gotoxy(78, 16 + i - 75);
					coutc(15,"#");
					bords[i] = cOnv(78, 16 + i - 75);
				}else if(i < bords.size() - 5) {
					gotoxy(2 + i - 79, 19);
					coutc(15,"#");
					bords[i] = cOnv(2 + i - 79, 19);
				}else {
					gotoxy(75 + i - 84, 19);
					coutc(15,"#");
					bords[i] = cOnv(75 + i - 84, 19);
				}
			}
		}
	}

}

void food() {
	// 3ayzeen nehandle beta3et el te3ban (food)
	while(true) {

		srand (time(NULL));
		food_x = rand() % (width - 1) + 1;
		food_y = rand() % (height - 1) + 1;
		for(int i=0; i<snake.size(); i++) {
			if((snake[i].X == food_x && snake[i].Y == food_y) || (exfood_x == food_x && exfood_y == food_y)) {
				food_x = rand() % (width - 1) + 1;
				food_y = rand() % (height - 1) + 1;
			}
		}
		if(bords.size() > 0) {
			for(int i=0; i<bords.size(); i++) {
				if((bords[i].X == food_x && bords[i].Y == food_y)) {
					food_x = rand() % (width - 1) + 1;
					food_y = rand() % (height - 1) + 1;
				}
			}
		}
		gotoxy(food_x, food_y);
		coutc(12, "\3");
		break;
	}
}

void exfood() {
	// 3ayzeen nehandle beta3et el te3ban (food)
	char* arr[5] = {"\1", "\2", "\4", "\5", "\6"};
	int x;
	while(true) {
		srand (time(NULL) + time(NULL));
		exfood_x = rand() % (width - 1) + 1;
		exfood_y = rand() % (height - 1) + 1;
		for(int i=0; i<snake.size(); i++) {
				if(snake[i].X == exfood_x && snake[i].Y == exfood_y) {
					exfood_x = rand() % (width - 1) + 1;
					exfood_y = rand() % (height - 1) + 1;
				}
		}
		if(bords.size() > 0) {
			for(int i=0; i<bords.size(); i++) {
				if((bords[i].X == exfood_x && bords[i].Y == exfood_y)) {
					exfood_x = rand() % (width - 1) + 1;
					exfood_y = rand() % (height - 1) + 1;
				}
			}
		}
		gotoxy(exfood_x, exfood_y);
		x = (exfood_x + exfood_y) % 5;
		coutc(10, arr[x]);
		break;
	}
}

void gameOver()
{
	system("CLS");
cout << "     *********             *****         ******       ******   ***************\n";
_sleep(100);
cout << "   *           *          *     *        *     *     *     *   *             *\n";
_sleep(100);
cout << "   *     ***    *        *   *   *       *      *   *      *   *    **********\n";
_sleep(100);
cout << "   *   *     *  *       *   * *   *      *       * *       *   *    *\n";
_sleep(100);

cout << "   *   *     ****      *   *   *   *     *        *        *   *    **********\n";
_sleep(100);
cout << "   *   *              *   *******   *    *                 *   *             *\n";
_sleep(100);
cout << "   *   *             *               *   *    *       *    *   *             *\n";
_sleep(100);

cout << "   *   *   *******   *    *******    *   *    * *   * *    *   *    **********\n";
_sleep(100);
cout << "   *   *    *   *    *    *     *    *   *    *   *   *    *   *    *\n";
_sleep(100);
cout << "   *     **     *    *    *     *    *   *    *       *    *   *    **********\n";
_sleep(100);
cout << "   *           *     *    *     *    *   *    *       *    *   *             *\n";
_sleep(100);
cout << "     * ********      ******     ******   ******       ******   ***************\n";
_sleep(100);
cout << "\n";
_sleep(100);
cout << "       ****       *****         *****   ***************      ********* \n";
_sleep(100);
cout << "     *      *     *   *         *   *   *             *    *           *\n";
_sleep(100);
cout << "    *  ****  *    *   *         *   *   *    **********   *    *****    *\n";
_sleep(100);
cout << "   *  *    *  *   *   *         *   *   *    *            *   *     *   *\n";
_sleep(100);
cout << "   *  *    *  *   *   *         *   *   *    **********   *    *****   *\n";
_sleep(100);
cout << "   *  *    *  *   *   *         *   *   *             *   *           *\n";
_sleep(100);
cout << "   *  *    *  *   *    *       *    *   *             *   *   * *   *\n";
_sleep(100);
cout << "   *  *    *  *    *    *     *    *    *    **********   *   *   *   *\n";
_sleep(100);
cout << "   *  *    *  *     *    *****    *     *    *            *   *    *   *\n";
_sleep(100);
cout << "    *  ****  *       *           *      *    **********   *   *     *   *\n";
_sleep(100);
cout << "     *      *         *         *       *             *   *   *      *   *\n";
_sleep(100);
cout << "       ****             *******         ***************   *****       *****";
	gotoxy(0, 12);
	coutc(12, "\tYour score is : "); cout << score;
    _sleep(1000);
}

void borders() {
	char* bord[2] = {".", "#"};
	int gm;

	if(gameMode > 1) {
		gm = 0;
	}else {
		gm = gameMode;
	}
	for (int i=0; i<=width; i++) coutc(15,bord[gm]);
	for(int i=1; i<height; i++) {
		gotoxy(0,i);
		coutc(15,bord[gm]);
		gotoxy(width,i);
		coutc(15,bord[gm]);
	}
	gotoxy(0,height);
	for (int i=0; i<=width; i++) coutc(15,bord[gm]);
	gotoxy(1, height + 2);
	coutc(9,"Your Score Is : 0");
}

bool checkSnake(int x, int y) {

for(int i=0; i<snake.size()-1; i++) {
	if (snake[i].X==x && snake[i].Y==y) {
		return true;
	}
}
if(bords.size() > 0) {
	for(int i=0; i<bords.size()-1; i++) {
		if (bords[i].X==snake[snake.size()-1].X && bords[i].Y==snake[snake.size()-1].Y) {
			return true;
		}
	}
}
	return false;
}

void coutc(int color, char* output)
{
   HANDLE handle= GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute( handle, color);
   cout<< output;
   SetConsoleTextAttribute( handle, color);
}

void snakeLogo() {
	coutc(15, " ##############################################################################\n");
	coutc(15,"                                                                              \n");
	coutc(15,"         ****      *         *           *           *     *     *********    \n");
	coutc(15,"       *      *    * *       *          * *          *    *      *            \n");
	coutc(15,"       *      *    *  *      *         *   *         *   *       *            \n");
	coutc(15,"        *          *   *     *        *     *        *  *        *            \n");
	coutc(15,"          *        *    *    *       *********       *  *        ******       \n");
	coutc(15,"            *      *     *   *      *         *      *   *       *            \n");
	coutc(15,"      *      *     *      *  *     *           *     *    *      *            \n");
	coutc(15,"      *      *     *       * *    *             *    *     *     *            \n");
	coutc(15,"        ****       *         *   *               *   *      *    *********    \n");
	coutc(15,"                                                                              \n");
	coutc(15," ##############################################################################\n");

	for(int i=0; i<13; i++) {
		gotoxy(0,i);
		coutc(15,"#");
		gotoxy(width,i);
		coutc(15,"#");
	}
}

void menu(bool &eXit) {

	snakeLogo();
	gotoxy(37, 9);
	coutc(15, "Main Menu");
	gotoxy(33, 14);
	coutc(15, "Play Game");
	gotoxy(33, 15);
	coutc(15, "About");
	gotoxy(33, 16);
	coutc(15, "Exit");

	char ch;
	int menop=1;
	gotoxy(31, 13+menop);
	cout << "\4";
	gotoxy(0, 0);
	while((ch = getch())) {
			if(ch == DOWN) {
				gotoxy(31, 13+menop);
				cout << " ";
				if(menop == 3) {
					menop = 1;
				}else {
					menop++;
				}
				gotoxy(31, 13+menop);
				cout << "\4";
				gotoxy(0, 0);
			}else if(ch == UP) {
				gotoxy(31, 13+menop);
				cout << " ";
				if(menop == 1) {
					menop = 3;
				}else {
					menop--;
				}
				gotoxy(31, 13+menop);
				cout << "\4";
				gotoxy(0, 0);
			}else if(ch == ENTER) {

				if(menop == 1) {
					system("CLS");
					GameMode(eXit);
					break;
				}else if(menop == 2) {
					system("CLS");
					about(eXit);
					break;
				}else if(menop == 3) {
					eXit = false;
					break;
				}

			}else if(ch == ESCAPE) {
				eXit = false;
				break;
			}

	}
}

void GameMode(bool &eXit) {

	snakeLogo();

	gotoxy(37, 9);
	coutc(15, "Game Mode");
	gotoxy(33, 14);
	cout << "Maze";
	gotoxy(33, 15);
	cout << "Borders 1";
	gotoxy(33, 16);
	cout << "Borders 2";
	gotoxy(33, 17);
	cout << "Borders 3";

	char ch;
	gameMode=0;
	gotoxy(31, 14+gameMode);
	cout << "\4";
	gotoxy(0, 0);
	while((ch = getch())) {
			if(ch == DOWN) {
				gotoxy(31, 14+gameMode);
				cout << " ";
				if(gameMode == 3) {
					gameMode = 0;
				}else {
					gameMode++;
				}
				gotoxy(31, 14+gameMode);
				cout << "\4";
				gotoxy(0, 0);
			}else if(ch == UP) {
				gotoxy(31, 14+gameMode);
				cout << " ";
				if(gameMode == 0) {
					gameMode = 3;
				}else {
					gameMode--;
				}
				gotoxy(31, 14+gameMode);
				cout << "\4";
				gotoxy(0, 0);
			}else if(ch == ENTER) {
				system("CLS");
				GameSpeed(eXit);
				break;
			}else if(ch == ESCAPE) {
				system("CLS");
				menu(eXit);
				break;
			}

	}

}

void GameSpeed(bool &eXit) {

	snakeLogo();

	gotoxy(37, 9);
	coutc(15, "Game Speed");
	gotoxy(33, 14);
	cout << "Speed 1";
	gotoxy(33, 15);
	cout << "Speed 2";
	gotoxy(33, 16);
	cout << "Speed 3";
	gotoxy(33, 17);
	cout << "Speed 4";
	gotoxy(33, 18);
	cout << "Speed 5";
	gotoxy(33, 19);
	cout << "Speed 6";
	gotoxy(33, 20);
	cout << "Speed 7";
	gotoxy(33, 21);
	cout << "Speed 8";
	gotoxy(33, 22);
	cout << "Speed 9";
	gotoxy(33, 23);
	cout << "Speed 10";

	char ch;
	speed=1;
	gotoxy(31, 13+speed);
	cout << "\4";
	gotoxy(0, 0);
	while((ch = getch())) {
			if(ch == DOWN) {
				gotoxy(31, 13+speed);
				cout << " ";
				if(speed == 10) {
					speed = 1;
				}else {
					speed++;
				}
				gotoxy(31, 13+speed);
				cout << "\4";
				gotoxy(0, 0);
			}else if(ch == UP) {
				gotoxy(31, 13+speed);
				cout << " ";
				if(speed == 1) {
					speed = 10;
				}else {
					speed--;
				}
				gotoxy(31, 13+speed);
				cout << "\4";
				gotoxy(0, 0);
			}else if(ch == ENTER) {
				system("CLS");
				break;
			}else if(ch == ESCAPE) {
				system("CLS");
				GameMode(eXit);
				break;
			}

	}

}

void about(bool &eXit) {

	char ch;
	coutc(15, "###############################################################################\n");
	coutc(15, "\n");
	coutc(15, "   *******  *        *******  *********    ****           *        *         * \n");
	coutc(15, "   *        *        *                *  *      *        * *       * *       * \n");
	coutc(15, "   *        *        *               *   *      *       *   *      *  *      * \n");
	coutc(15, "   *        *        *          ******    *            *     *     *   *     * \n");
	coutc(15, "   *****    *  Team  *****         *        *         *********    *    *    * \n");
	coutc(15, "   *        *        *            *           *      *         *   *     *   * \n");
	coutc(15, "   *        *        *           *       *      *   *           *  *      *  * \n");
	coutc(15, "   *        *        *          *        *      *   *           *  *       * * \n");
	coutc(15, "   *******  *******  *******   *           ****     *           *  *         * \n");
	coutc(15, "\n");
	coutc(15, "   Lots Of Thanks\t\t\t\tTeam : (alphabetically)\n");
	coutc(15, "   To Our\t\t\t\t\t1 - Hossam El-Dein (Goodname)\n");
	coutc(15, "   TA Eng. To-Be :\t\t\t\t2 - Mohamed Ahmed (Eagle)\n");
	coutc(15, "   Mohamed Shaker \3\t\t\t\t3 - Mohamed Osama (Mosa)\n");
	coutc(15, "                  \4\t\t\t\t4 - Mohamed Said (Kogo)\n");
	coutc(15, "\n");
	coutc(15, "###############################################################################\n");
	coutc(15, "\n");
	coutc(15, "     This One Goes For All Them TAs And Them Members Of The Students Union\n");
	coutc(15, " \t\t\t\tThank You All Guyz\n");

	for(int i=0; i<18; i++) {
		gotoxy(0,i);
		coutc(15,"#");
		gotoxy(width,i);
		coutc(15,"#");
	}

	gotoxy(37, 22);
	cout << "\4 Back";
	gotoxy(0, 0);
	while((ch = getch())) {
		if(ch == ENTER || ch == ESCAPE) {
			system("CLS");
			menu(eXit);
			break;
		}
	}
}

int exscoreval(long long time) {

	if(time >= 1 && time <= 392) {
		return speed * 2;
	}else if(time >= 785 && time <= 201165) {
		return speed * 3;
	}else if(time >= 402331 && time <= 102996826) {
		return speed * 4;
	}else if(time >= 205993652 && time <= 5273437500000) {
		return speed * 5;
	}else if(time >= 10546875000000 && time <= 2700000000000000000) {
		return speed * 6;
	}else {
		return 0;
	}

}
