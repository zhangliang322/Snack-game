#include "stdio.h"
#include"windows.h"
#include"ctime"
#include "conio.h"
using namespace std;

#define getrand(a,b)(rand()%(b-a+1)+a)//get random number
//食物-2
//障碍-1
//空 0
int len = 10;
int food = 1;
int dirs[5][2] = { {0,0},{0,-1},{0,1} ,{-1,0},{1,0} };//direction
//对应键盘1234上下左右
int map_lenth = 20, map_width = 12;//len and width of map
int direction = 2;//current direction
COORD head;
int room[32][32]; //record map

void go(int x, int y)
{
	COORD coord; //自带的坐标结构
	coord.X = x * 2;   //这里将int类型值传给short,不过程序中涉及的坐标值均不会超过short范围//
	//为什么这里要乘2？
	coord.Y = y;
	HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);//获得std output 的handle
	SetConsoleCursorPosition(a, coord);//设置控制台光标坐标，在win窗内生成鼠标光标

}

void move()// move of snake
{
	head.X += dirs[direction][0];//x方向（左右方向）上的数值0或1
	head.Y += dirs[direction][1];//同理 y方向，像dfs一样用二维矩阵代表方向转向
	if (room[head.Y][head.X] != 0 && room[head.Y][head.X] != -2)
		return;//结束游戏

	if (room[head.Y][head.X] == -2)
	{
		len++;
		food = 0;
	}
	for (int i = 1;  i < map_lenth+1; i++)
	{
		for (int j = 0; j < map_width;j++)
		{
			if(room[j][i]>=1) //有蛇
				if (room[head.Y][head.X] != -2)//不是食物
				{
					//为什么是j在前面？
					room[j][i]--;//蛇经过的地方-1，例如蛇4321，经过后321，21，1，0，身体全部经过后变成0
					if (room[j][i] == 0)//如果蛇尾巴经过完毕，变成0，则做清空
					{
						go(i, j); printf("  ");//清空
					}
				}
		}
	}
	room[head.Y][head.X] = len;
	go(head.X, head.Y); printf("△");
}

void createFood()
{
	int randx = getrand(1, map_lenth);
	int randy = getrand(1, map_width);
	while (room[randy][randx] != 0)
	{
		randx = getrand(1, map_lenth);
		randy = getrand(1, map_width);
	}
	room[randy][randx] = -2;
	go(randx, randy);
	printf("♢");
	food = 1;

}

int main()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);//隐藏光标

    unsigned seed = time(0);
    srand(seed);//设置随机数种子

    for (int i = 0; i <= map_width + 1; i++)
        for (int j = 0; j <= map_lenth + 1; j++)
            if (i == 0 || i == map_width + 1 || j == 0 || j == map_lenth + 1)
            {
                room[i][j] = -1;
                go(j, i);
                printf("■");
            }//设置边界  打印地图

    createFood();//生成第一个食物

    while (!_kbhit())
        Sleep(10); //按下按键前不做出任何动作

    direction = -3;//初始化方向为负数使其不与其他方向冲突
    head.X = map_lenth / 2 + 1;
    head.Y = map_width / 2 + 1;//设置蛇头位置为中心
    room[head.Y][head.X] = len; //蛇头处为蛇长
    go(head.X, head.Y); printf("□");//打印蛇头

    while (true)
    {
        int dirBefore = direction;//储存未更改的方向
        int key = -1;
        for (int i = 1; i <= 25; i++)//分多个时间点检测方向,而不是只有一个时间点,这样可以提升操作手感
        {
            if (_kbhit() && key == -1)//如果有按键按下且未曾读取按键（使同一时间单位内按下多次时不覆盖前一次按键，而是等到下一时间单位读取并做出相应反馈，方便实现掉头等操作）
            {
                key = _getch();//读取
                if (key == 224)//读取方向键时需读取两次,第一次必定是224,所以判断读入的是方向键时则再读取一次
                {
                    key = _getch();

                    switch (key)
                    {
                    case 72: direction = 1; break;//上
                    case 80: direction = 2; break;//下
                    case 75: direction = 3; break;//左
                    case 77: direction = 4; break;//右
                    }
                }
            }
            Sleep(5);
        }
        if (dirBefore != direction && (dirBefore + direction == 3 || dirBefore + direction == 7))
            direction = dirBefore;//如果更改后的方向与原来的方向冲突(成反方向),不改变方向

        move();//移动蛇

        if (room[head.Y][head.X] != len)//如果蛇头位置的数不等于蛇的长度(说明移动函数中判定为撞到,未改变蛇头数值)
        {
            go(0, map_width + 3);//使结束反馈不遮挡画面
            Sleep(250);
            return 0;//结束程序
        }

        if (food == 0)
            createFood();//如果场上没有食物,则生成一个食物
    }
    return 0;//结束程序（虽然没啥用）
}