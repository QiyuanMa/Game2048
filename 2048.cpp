// 2048.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include<vector>
#include<time.h>
#include<Windows.h>
#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c)&0x8000?1:0)

using namespace std;
int moveFlag = 0, score = 0 ,bestscore = 0;

class Square {
public:
	Square(int get_left = 0, int get_right = 0, int get_top = 0, int get_bottom = 0) {
		left = get_left;
		right = get_right;
		top = get_top;
		bottom = get_bottom;
	}

	void drawSquare() {
		if (num != 0) {
			setfillcolor(RGB(185 + 20 * (log(num) / log(2)), 110, 240 - 20 * (20 * (log(num) / log(2)))));
			setlinecolor(RGB(185 + 20 * (log(num) / log(2)), 110, 240 - 20 * (20 * (log(num) / log(2)))));
			fillrectangle(left, top, right, bottom);
		}
		else {
			setfillcolor(RGB(185, 110, 240));
			setlinecolor(RGB(185, 110, 240));
			fillrectangle(left, top, right, bottom);
		}
	}
	//画方块

	void drawNum() {
		if (num != 0) {
			_itow(num, Snum, 10);
			RECT r = { left, top, right, bottom };
			drawtext(Snum, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	//输出数字
	
	void drawText(TCHAR c) {
		RECT r = { left, top, right, bottom };
		drawtext(c, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	
	int inisetNum() {
		if (num == 0) {
			num = 2;
			return 0;
		}
		else {
			return 1;
		}
	}

	void setNum(int Get_num) {
		num = Get_num;
	}
	
	int getNum() {
		return num;
	}

	void setaddFlag(int Get_addFlag) {
		addFlag = Get_addFlag;
	}

	void moveSquare(Square* s) {
		if (num != 0) {
			if (s->num == 0) {
				s->setNum(num);
				num = 0;
				moveFlag++;
			}
			else if (s->num == num && addFlag == 0 && s->addFlag == 0) {
				s->setNum(2 * num); //能加，数字c*2
				s->setaddFlag(1); //置不可加
				score += 2 * num; //分数是合的数是几，加几
				num = 0;
				moveFlag++;
			}
		}
	}
	//移动

	int Test(Square* s) {
		if (s->num == num) {
			return 0;
		}
		else {
			return 1;
		}
	}

private:
	int left, right, top, bottom, num = 0, addFlag = 0;
	TCHAR Snum[10];
};

vector<Square> squareV;

inline void drawInterface() {
	for (int i = 0; i < 16; i++) {
		squareV.at(i).drawSquare();
		squareV.at(i).drawNum();
	}
}
//绘制界面

inline void drawTitle() {
	RECT r1 = { 18,8,226,142 };
	LOGFONT f1;
	gettextstyle(&f1);
	f1.lfHeight = 100;
	_tcscpy_s(f1.lfFaceName, _T("Consolas"));
	f1.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f1);
	drawtext(_T("2048"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	RECT r2 = { 224,28,324,75 };
	LOGFONT f2;
	gettextstyle(&f2);
	f2.lfHeight = 35;
	_tcscpy_s(f2.lfFaceName, _T("Consolas"));
	f2.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f2);
	drawtext(_T("SCORE"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r3 = { 332,28,432,75 };
	LOGFONT f3;
	gettextstyle(&f3);
	f3.lfHeight = 25;
	_tcscpy_s(f3.lfFaceName, _T("Consolas"));
	f3.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f3);
	drawtext(_T("BEST\nSCORE"), &r3, DT_CENTER | DT_VCENTER);
}
//绘制标题

inline void drawScore() {
	RECT r4 = { 224,75,324,138 };
	setfillcolor(RGB(90, 90, 90));
	setlinecolor(RGB(90, 90, 90));
	fillrectangle(224, 75, 324, 138);
	LOGFONT f4;
	gettextstyle(&f4);
	f4.lfHeight = 35;
	_tcscpy_s(f4.lfFaceName, _T("Consolas"));
	f4.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f4);
	TCHAR cache[10];
	_itow(score, cache, 10);
	drawtext(cache, &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r5 = { 332,75,432,138 };
	setfillcolor(RGB(90, 90, 90));
	setlinecolor(RGB(90, 90, 90));
	fillrectangle(332, 75, 432, 138);
	LOGFONT f5;
	gettextstyle(&f5);
	f5.lfHeight = 35;
	_tcscpy_s(f5.lfFaceName, _T("Consolas"));
	f5.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f5);
	_itow(bestscore, cache, 10);
	drawtext(cache, &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
//绘制分数

int getBestscore() {
	char cache[100] = {};
	char Bestscore[100] = {};
	FILE *fp = NULL;
	if ((fp = fopen("bestscore.txt", "r")) == NULL) {
		MessageBox(GetHWnd(), TEXT("File Error.\nPlease take care of the file 'bestscore.txt'."), TEXT("2048"), MB_OK);
		fclose(fp);
		return 0;
	}
	Bestscore[0] = 0;
	while (fgets(cache, 100, fp) != NULL) {
		strcat(Bestscore, cache);
	}
	fclose(fp);
	return atoi(Bestscore);
}

void setBestscore() {
	char Bestscore[10];
	_itoa(bestscore, Bestscore, 10);
	FILE *fp = NULL;
	if ((fp = fopen("bestscore.txt", "w")) == NULL) {
		MessageBox(GetHWnd(), TEXT("File Error.\nPlease take care of the file 'bestscore.txt'."), TEXT("2048"), MB_OK);
		fclose(fp);
		return;
	}
	_itoa(bestscore, Bestscore, 10);
	fputs(Bestscore, fp);
	fclose(fp);
}

BOOL WINAPI ConsoleHandler(DWORD Cevent) {		//ConsoleHandler函数用法详见msdn:
	if (Cevent == CTRL_CLOSE_EVENT) {			//https://msdn.microsoft.com/en-us/library/windows/desktop/ms686016(v=vs.85).aspx
		setBestscore();
		return TRUE;
	}
	else
		return FALSE;
}

int main()
{
	initgraph(440, 590);
	HWND hwnd = GetHWnd();
	MoveWindow(hwnd, 150, 20, 456, 629, TRUE);
	setbkcolor(RGB(90, 90, 90));
	setbkmode(TRANSPARENT);
	cleardevice();
	drawTitle();
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(f.lfFaceName, _T("Consolas"));    // 设置字体
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);                     // 设置字体样式
	//画布设定，字体设定 ↑
	
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			Square newSquare(8 + j * 108, 108 + j * 108, 158 + i * 108, 258 + i * 108);
			squareV.push_back(newSquare);
		}
	}
	// 将16个Square类对象存入vector中 ↑

Start:
	while (kbhit()) {
		_getch();
	}										//重要！！清除sleep期间_getch()读入的字符
	srand((unsigned int)time(NULL));
	squareV.at(rand() % 16).inisetNum();
	while (squareV.at(rand() % 16 + 1).inisetNum()) {}
	// 生成初始随机两个位置 ↑

	drawInterface();

	int turnedSequence[16] = { 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15 };
	score = 0;
	bestscore = getBestscore();
	drawScore();
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE)) {		//SetConsoleCtrlHandler函数用法详见msdn:
		while (TRUE) {															//https://msdn.microsoft.com/en-us/library/windows/desktop/ms686016(v=vs.85).aspx
			char get = _getch();
			if (get == 'q' || get == 'Q' || get == 27) {
				if (MessageBox(hwnd, TEXT("您确定要退出吗？"), TEXT("2048"), MB_YESNO) == IDYES) {
					goto End;
				}
			}
			//退出
			if (get == 72 || get == 'w') {
				for (int i = 4; i < 8; i++) {
					squareV.at(i).moveSquare(&(squareV.at(i - 4)));
				}
				for (i = 8; i < 12; i++) {
					squareV.at(i).moveSquare(&(squareV.at(i - 4)));
				}
				for (i = 4; i < 8; i++) {
					squareV.at(i).moveSquare(&(squareV.at(i - 4)));
				}
				for (i = 12; i < 16; i++) {
					squareV.at(i).moveSquare(&(squareV.at(i - 4)));
				}
				for (i = 8; i < 12; i++) {
					squareV.at(i).moveSquare(&(squareV.at(i - 4)));
				}
				for (i = 4; i < 8; i++) {
					squareV.at(i).moveSquare(&(squareV.at(i - 4)));
				}
				for (i = 0; i < 16; i++) {
					squareV.at(i).setaddFlag(0); //如果格子已经加过一次，不能再加第二次，每次操作完后变可加状态44空空不可加，置0变可加，再按一次，应
				}
				drawInterface();
				if (moveFlag != 0) {
					srand((unsigned int)time(NULL));
					while (squareV.at(rand() % 16).inisetNum()) {
					}
					moveFlag = 0;
				}
				drawInterface();
			}
			//上移
			else if (get == 80 || get == 's') {
				for (int i = 11; i > 7; i--) {
					squareV.at(i).moveSquare(&(squareV.at(i + 4)));
				}
				for (i = 7; i > 3; i--) {
					squareV.at(i).moveSquare(&(squareV.at(i + 4)));
				}
				for (i = 11; i > 7; i--) {
					squareV.at(i).moveSquare(&(squareV.at(i + 4)));
				}
				for (i = 3; i >= 0; i--) {
					squareV.at(i).moveSquare(&(squareV.at(i + 4)));
				}
				for (i = 7; i > 3; i--) {
					squareV.at(i).moveSquare(&(squareV.at(i + 4)));
				}
				for (i = 11; i > 7; i--) {
					squareV.at(i).moveSquare(&(squareV.at(i + 4)));
				}
				for (i = 0; i < 16; i++) {
					squareV.at(i).setaddFlag(0);
				}
				drawInterface();
				if (moveFlag != 0) {
					srand((unsigned int)time(NULL));
					while (squareV.at(rand() % 16).inisetNum()) {
					}
					moveFlag = 0;
				}
				drawInterface();
			}
			//下移
			else if (get == 75 || get == 'a') {
				for (int i = 4; i < 8; i++) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] - 1)));
				}
				for (i = 8; i < 12; i++) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] - 1)));
				}
				for (i = 4; i < 8; i++) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] - 1)));
				}
				for (i = 12; i < 16; i++) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] - 1)));
				}
				for (i = 8; i < 12; i++) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] - 1)));
				}
				for (i = 4; i < 8; i++) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] - 1)));
				}
				for (i = 0; i < 16; i++) {
					squareV.at(i).setaddFlag(0);
				}
				drawInterface();
				if (moveFlag != 0) {
					srand((unsigned int)time(NULL));
					while (squareV.at(rand() % 16).inisetNum()) {
					}
					moveFlag = 0;
				}
				drawInterface();
			}
			//左移
			else if (get == 77 || get == 'd') {
				for (int i = 11; i > 7; i--) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] + 1)));
				}
				for (i = 7; i > 3; i--) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] + 1)));
				}
				for (i = 11; i > 7; i--) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] + 1)));
				}
				for (i = 3; i >= 0; i--) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] + 1)));
				}
				for (i = 7; i > 3; i--) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] + 1)));
				}
				for (i = 11; i > 7; i--) {
					squareV.at(turnedSequence[i]).moveSquare(&(squareV.at(turnedSequence[i] + 1)));
				}
				for (i = 0; i < 16; i++) {
					squareV.at(i).setaddFlag(0);
				}
				drawInterface();
				if (moveFlag != 0) {
					srand((unsigned int)time(NULL));
					while (squareV.at(rand() % 16).inisetNum()) {
					}
					moveFlag = 0;
				}
				drawInterface();
			}
			//右移
			if (score > bestscore) {
				bestscore = score;
			}
			drawScore();
			settextstyle(&f);
			i = 0;
			int endFlagUp = 0, endFlagLeft = 0;
			while (squareV.at(i).getNum()) {
				i++;
				if (i == 16) {//挨着的，重复的数，（可上下左右操作）格子满了可及继续
					int j = 4;
					while (squareV.at(j).Test(&(squareV.at(j - 4)))) {
						j++;
						if (j == 16) {
							endFlagUp = 1;
							break;
						}
					}
					j = 4;
					while (squareV.at(turnedSequence[j]).Test(&(squareV.at(turnedSequence[j - 4])))) {
						j++;
						if (j == 16) {
							endFlagLeft = 1;
							break;
						}
					}
					break;
				}
			}
			if (endFlagUp == 1 && endFlagLeft == 1) {
				break;
			}
			//判断是否无法继续游戏
		}
		//主要操作部分

		for (i = 0; i < 16; i++) {
			squareV.at(i).setNum(0);
		}
		Sleep(2000);
		char gameOver[] = { 'G','A','M','E','O','V','E','R' };
		drawInterface();
		for (i = 4; i < 12; i++) {
			squareV.at(i).drawText(gameOver[i - 4]);
		}
		setBestscore();
		Sleep(3000);
		//输出GAME OVER

		if (MessageBox(hwnd, TEXT("是否继续游戏？"), TEXT("2048"), MB_YESNO) == IDYES) {
			goto Start;
		}
		//继续游戏弹窗
	}
	else {
		MessageBox(hwnd, TEXT("ERROR:could not set control handler."), TEXT("2048"), MB_OK);
	}
End:
	closegraph();
	setBestscore();
	return 0;
}
