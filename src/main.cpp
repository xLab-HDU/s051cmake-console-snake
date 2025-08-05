// 局部更新版本
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define STAGE_WIDTH 20
#define STAGE_HEIGHT 20
#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 25
#define CORNER_X 1
#define CORNER_Y 1
#define THICHKNESS 1
#define MAXLENGTH 100
#define COLOR_WALL 0X06
#define COLOR_TEXT 0X0F
#define COLOR_TEXT4END 0XEC
#define COLOR_SCORE 0X0C
#define COLOR_FRUIT 0X0C
#define COLOR_SNAKE_HEAD 0X09
#define COLOR_SNKAE_BODY 0X0A
#define DIFFICULTY_FACTOR  50
using namespace std;

bool isGameOver, isFlash;
bool isFullWidth, isUpdateMap, isPause;
const int mWidth = STAGE_WIDTH;
const int mHeight = STAGE_HEIGHT;
int headX, headY, fruitX, fruitY, mScore;
int tailX[MAXLENGTH], tailY[MAXLENGTH];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
void Prompt_info(int _x, int _y);

void Initial()
{
    SetConsoleTitleW(L"Console_贪吃蛇*双字节版*");
    COORD dSize = { WINDOW_WIDTH, WINDOW_HEIGHT };
    SetConsoleScreenBufferSize(h, dSize); //设置窗口缓冲区大小
    CONSOLE_CURSOR_INFO _cursor = { 1, false }; //设置光标大小，隐藏光标
    SetConsoleCursorInfo(h, &_cursor);

    // SMALL_RECT rc = { 0, 0, 84, 26 }; //要设置的窗口显示的大小和位置
    // SetConsoleWindowInfo(h, TRUE, &rc);

    isGameOver = false;
    isFlash = false;
    isFullWidth = true;
    isUpdateMap = false;
    isPause = false;
    dir = STOP;
    headX = mWidth / 2;
    headY = mHeight / 2;
    srand((unsigned)time(NULL));
    fruitX = rand() % (mWidth - 2) + 1;
    fruitY = rand() % (mHeight - 2) + 1;
    mScore = 0;

    nTail = 1;
    tailX[0] = headX;
    tailY[0] = headY;
    for (int i = 1; i < MAXLENGTH; i++)
    {
        tailX[i] = 0;
        tailY[i] = 0;
    }
}
//光标位置
void setPos(int X, int Y)
{
    COORD pos;
    if (isFullWidth)
        pos.X = CORNER_X + 2 * X + THICHKNESS;	//全角字符占用两个半角字符的位置，故步进是原先的两倍
    else
        pos.X = CORNER_X + X + THICHKNESS;
    pos.Y = CORNER_Y + Y + THICHKNESS;
    SetConsoleCursorPosition(h, pos);
}
void DrawMap()
{
    system("cls");

    int textColor = COLOR_WALL;
    SetConsoleTextAttribute(h, textColor);

    setPos(-THICHKNESS, -THICHKNESS);//绘制顶上的墙
    for (int i = 0; i < mWidth + THICHKNESS * 2; i++)
        if (isFullWidth)
            cout << "□";//输出双字节符号
        else
            cout << "#";

    for (int i = 0; i < mHeight; i++)
    {
        setPos(-THICHKNESS, i);//绘制左右的墙
        for (int j = 0; j < mWidth + THICHKNESS * 2; j++)
        {
            if (j == 0)
                if (isFullWidth)
                    cout << "□";//输出双字节符号
                else
                    cout << "#";
            else if (j == mWidth + THICHKNESS)
                if (isFullWidth)
                    cout << "□";//输出双字节符号
                else
                    cout << "#";
            else
                if (isFullWidth)
                    cout << "  ";////此处是两个空格
                else
                    cout << " ";
        }
        cout << endl;
    }
    setPos(-THICHKNESS, STAGE_HEIGHT);//绘制下方的墙
    for (int i = 0; i < mWidth + THICHKNESS * 2; i++)
        if (isFullWidth)
            cout << "□";//输出双字节符号
        else
            cout << "#";
}
void eraseSnake()
{
    for (int i = 0; i < nTail; i++)
    {
        setPos(tailX[i], tailY[i]);
        if (isFullWidth)
            cout << "  ";////此处是两个空格
        else
            cout << " ";
    }
}
void DrawLocally()
{
    if (isUpdateMap)
    {
        isUpdateMap = false;
        DrawMap();
        Prompt_info(5, 1);
    }

    if (!isFlash)
    {
        setPos(fruitX, fruitY);
        SetConsoleTextAttribute(h, COLOR_FRUIT);
        if (isFullWidth)
            cout << "★";////输出双字节符号
        else
            cout << "F";
        isFlash = true;
    }
    else
    {
        setPos(fruitX, fruitY);
        SetConsoleTextAttribute(h, COLOR_FRUIT);
        if (isFullWidth)
            cout << "  ";////此处是两个空格
        else
            cout << " ";
        isFlash = false;
    }

    for (int i = 0; i < nTail; i++)
    {
        setPos(tailX[i], tailY[i]);
        if (i == 0)
        {
            SetConsoleTextAttribute(h, COLOR_SNAKE_HEAD);
            if (isFullWidth)
                cout << "●";////输出双字节符号
            else
                cout << "O";
        }
        else
        {
            SetConsoleTextAttribute(h, COLOR_SNKAE_BODY);
            if (isFullWidth)
                cout << "ｏ";////输出双字节符号
            else
                cout << "o";
        }
    }

    setPos(0, STAGE_HEIGHT + THICHKNESS * 2);
    SetConsoleTextAttribute(h, COLOR_TEXT);
    cout << "游戏得分" << mScore;
}
void Input()
{
    if (_kbhit())
    {
        bool spaceReadyFlag = true;
        if (isPause == true)
            spaceReadyFlag = false;
        isPause = false;
        switch (_getch())
        {
        case 'a':
        case 'A':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'd':
        case 'D':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w':
        case 'W':
            if (dir != DOWN)
                dir = UP;
            break;
        case 's':
        case 'S':
            if (dir != UP)
                dir = DOWN;
            break;
        case ' ':
            if (spaceReadyFlag == true)
                isPause = true;
            break;
        case 'x':
        case 'X':
            isGameOver = true;
            break;
        case 13:		//回车键ASCII码对应的十六进制为0x0D
            if (isFullWidth)
                isFullWidth = false;
            else
                isFullWidth = true;
            isUpdateMap = true;
            break;
        case 224:                    //方向键区的ASCII码
            switch (_getch()) {
            case 72:				//上
                if (dir != DOWN)
                    dir = UP;
                break;
            case 80:				//下
                if (dir != UP)
                    dir = DOWN;
                break;
            case 75:				//左
                if (dir != RIGHT)
                    dir = LEFT;
                break;
            case 77:				//右
                if (dir != LEFT)
                    dir = RIGHT;
                break;
            }
        default:
            break;
        }
    }
}
void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = headX;
    tailY[0] = headY;

    switch (dir)
    {
    case LEFT:
        headX--;
        break;
    case RIGHT:
        headX++;
        break;
    case UP:
        headY--;
        break;
    case DOWN:
        headY++;
        break;
    default:
        break;
    }

    for (int i = 1; i < nTail; i++)
        if (tailX[i] == headX && tailY[i] == headY)
            isGameOver = true;

    if (headX >= mWidth) headX = 0;	else if (headX < 0)	headX = mWidth - 1;
    if (headY >= mHeight) headY = 0; else if (headY < 0) headY = mHeight - 1;

    //if (x > mWidth || x < 0 || y > mHeight || y < 0)
    //	isGameOver = true;
    if (headX == fruitX && headY == fruitY)
    {
        mScore += 10;
        fruitX = rand() % (mWidth - 2) + 1;
        fruitY = rand() % (mHeight - 2) + 1;
        nTail++;
    }
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
}
void Prompt_info(int _x, int _y)
{
    int initialX = 20, initialY = 0;

    SetConsoleTextAttribute(h, COLOR_TEXT);
    setPos(_x + initialX, _y + initialY);
    cout << "■ 游戏说明：";
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "    A.蛇身自撞，游戏结束";
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "    B.蛇可穿墙";
    initialY++;
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "■ 操作说明：";
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "    □ 舞台切换：回车键";
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "    □ 向左移动：←A";
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "    □ 向右移动：→D";
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "    □ 向下移动：↓S";
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "    □ 向上移动：↑W";
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "    □ 开始游戏：任意方向键";
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "    □ 暂停游戏：Space键";
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "    □ 退出游戏: x键退出";
    initialY++;
    initialY++;
    setPos(_x + initialX, _y + initialY);
    cout << "■ 作者：杭电数媒 李仕";
}
void showScore(int _x, int _y)
{
    setPos(_x + 20, _y + 15);
    SetConsoleTextAttribute(h, COLOR_TEXT);
    int s = mScore / DIFFICULTY_FACTOR;
    cout << "☆ 当前难度:     级";
    if (isFullWidth) {
        setPos(_x + 27, _y + 15);//分数的位置可根据具体的文字数量做适当调整
    }
    else {
        setPos(_x + 34, _y + 15);
    }
    SetConsoleTextAttribute(h, COLOR_SCORE);
    cout << s;

    setPos(_x + 20, _y + 17);
    SetConsoleTextAttribute(h, COLOR_TEXT);
    cout << "● 当前积分:  ";
    SetConsoleTextAttribute(h, COLOR_SCORE);
    cout << mScore;
}
void gameOver_info()
{
    setPos(5, 8);
    SetConsoleTextAttribute(h, COLOR_TEXT4END);
    cout << "游戏结束!!";
    setPos(3, 9);
    cout << "Y重新开始/N退出";
    SetConsoleTextAttribute(h, COLOR_TEXT);
}
int main()
{
    // 设置控制台为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    bool gameQuit = false;

    do {
        Initial();
        DrawMap();
        Prompt_info(5, 1);

        while (!isGameOver)
        {
            //Draw();
            Input();
            eraseSnake();
            if (!isPause)
                Logic();
            DrawLocally();

            showScore(5, 1);
            int dwMilliseconds = 200 / (mScore / DIFFICULTY_FACTOR + 1);
            Sleep(dwMilliseconds);
        }
        gameOver_info();

        while (isGameOver)
        {
            if (_kbhit())
            {
                switch (_getch())
                {
                case 'y':
                case 'Y':
                    isGameOver = false;
                    system("cls");
                    break;
                case 'n':
                case 'N':
                    isGameOver = false;
                    gameQuit = true;
                    break;
                default:
                    break;
                }
            }
            Sleep(50);
        }
    } while (!gameQuit);


    setPos(0, STAGE_HEIGHT + THICHKNESS * 3);
    //system("pause");
    return 0;
}