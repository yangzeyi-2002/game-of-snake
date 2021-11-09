#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

using namespace std;

void GameStart();
void Welcome();
void CreateMap();
void Gotoxy(int x, int y);
void InitSnake();
void RunGame();
void MoveSnake(int direct);

enum
{
    UP = 72,
    DOWN = 80,
    LEFT = 75,
    RIGHT = 77
};

//吃的
int FX = 5, FY = 2;

//蛇单个节点
typedef struct _snake
{
    int x;
    int y;
    struct _snake *next;
} Snake;

//蛇头
Snake *SHead;

//放置吃的
void SetFood(Snake *p)
{
    srand((unsigned)time(NULL));
    while (FX % 2 != 0)
    {
        int z = 0;
        FX = rand() % 56 + 2;
        FY = rand() % 28 + 1;
        while (p != NULL & p->next != NULL)
        {
            if (FX == p->x || FY == p->y)
            {
                z = -1;
            }
            p = p->next;
        }
        if (z == -1)
        {
            FX = 3;
        }
    }
    Gotoxy(FX, FY);
    printf("■");
}

//游戏开始
void GameStart()
{
    system("mode con cols=100 lines=30");
}

//欢迎
void Welcome()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\t\t\t\t\t欢迎游玩贪吃蛇");
    printf("\n\t\t\t\t\t↑,↓,←,→控制方向");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t");
    system("pause"); //暂停
    system("cls");   //清屏
}

//创建地图
void CreateMap()
{
    for (int i = 0; i < 60; i += 2)
    {
        Gotoxy(i, 0);
        printf("■");
        Gotoxy(i, 29);
        printf("■");
    }
    for (int i = 0; i < 30; i++)
    {
        Gotoxy(0, i);
        printf("■");
        Gotoxy(58, i);
        printf("■");
    }
}

//设置光标
void Gotoxy(int x, int y)
{
    COORD pos = {(short)x, (short)y};
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); //获取当前控制台id
    SetConsoleCursorPosition(hout, pos);
}

//初始化蛇
void InitSnake()
{
    Snake *temp = (Snake *)malloc(sizeof(Snake));
    temp->x = 20;
    temp->y = 15;
    temp->next = NULL;
    SHead = temp;
    for (int i = 1; i <= 3; i++)
    {
        Snake *p = (Snake *)malloc(sizeof(Snake));
        p->x = 20 + 2 * i;
        p->y = 15;
        p->next = NULL;
        temp->next = p;
        temp = p;
    }
    temp = SHead;
    while (temp != NULL)
    {
        Gotoxy(temp->x, temp->y);
        printf("■");
        temp = temp->next;
    }
}

//运行游戏
void RunGame()
{
    SetFood(SHead);
    int direct = RIGHT, exdirect, touch = 0,score=0;
    Gotoxy(75,14);
    printf("当前得分：");
    while (1)
    {
        Gotoxy(85,14);
        printf("%d",score);
        char ch = 77;
        //判断有没有输入
        if (_kbhit())
        {
            //获取输入
            ch = getch();
            switch (ch)
            {
            //↑
            case 72:
                direct = UP;
                break;
            //←
            case 75:
                direct = LEFT;
                break;
            //→
            case 77:
                direct = RIGHT;
                break;
            //↓
            case 80:
                direct = DOWN;
                break;
            default:
                break;
            }
        }
        //移动蛇
        //找尾巴
        Snake *p = SHead;
        while (p != NULL & p->next != NULL)
        {
            p = p->next;
        }

        //加节点
        Snake *temp = (Snake *)malloc(sizeof(Snake));
        temp->next = NULL;
        p->next = temp;
        if (direct == RIGHT && exdirect != LEFT)
        {
            temp->x = p->x + 2;
            temp->y = p->y;
            exdirect = RIGHT;
        }
        if (direct == RIGHT && exdirect == LEFT)
        {
            temp->x = p->x - 2;
            temp->y = p->y;
            exdirect = LEFT;
        }
        else if (direct == LEFT && exdirect != RIGHT)
        {
            temp->x = p->x - 2;
            temp->y = p->y;
            exdirect = LEFT;
        }
        else if (direct == LEFT && exdirect == RIGHT)
        {
            temp->x = p->x + 2;
            temp->y = p->y;
            exdirect = RIGHT;
        }
        else if (direct == UP && exdirect != DOWN)
        {
            temp->x = p->x;
            temp->y = p->y - 1;
            exdirect = UP;
        }
        else if (direct == UP && exdirect == DOWN)
        {
            temp->x = p->x;
            temp->y = p->y + 1;
            exdirect = DOWN;
        }
        else if (direct == DOWN && exdirect != UP)
        {
            temp->x = p->x;
            temp->y = p->y + 1;
            exdirect = DOWN;
        }
        else if (direct == DOWN && exdirect == UP)
        {
            temp->x = p->x;
            temp->y = p->y - 1;
            exdirect = UP;
        }
        //碰到自己
        Snake *q = SHead;
        while (q != NULL && q->next != NULL)
        {
            if (q->x == temp->x && q->y == temp->y)
            {
                touch = -1;
            }
            q = q->next;
        }

        // /吃到食物
        if (temp->x == FX && temp->y == FY)
        {
            FX = 3;
            SetFood(SHead);
            score++;
        }
        //边界
        else if (temp->x == 0 || temp->x == 59 || temp->y == 0 || temp->y == 29 || touch == -1)
        {
            system("cls");
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
            printf("\t\t\t\t\tWaste");
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t");
            break;
        }
        else
        {
            //打印
            Gotoxy(temp->x, temp->y);
            printf("■");
            Gotoxy(SHead->x, SHead->y);
            printf(" ");

            //删除头
            temp = SHead->next;
            delete SHead;
            SHead = temp;
        }
        Sleep(500);
    }
}

int main()
{
    GameStart();
    Welcome();
    CreateMap();
    InitSnake();
    RunGame();
    return 0;
}