#pragma once
#include<iostream>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<windows.h>
using namespace std;

#define face_x 90
#define face_y 44
#define up 72
#define left  75
#define right 77
#define down 80
#define space 32
#define enter 13
#define Esc 27
#define ss 200//选择模式和难度时闪烁时间



void set_color(int x=0);//设置字符颜色
void gotoxy(int x, int y);//设置光标位置
void Hide_Cursor();   //隐藏光标
void Look_Cursor();



class Map
{
public:
	void paint();//画出初始地图和提示
	void update_grade();//刷新分数
	void initdie();//死亡界面
private:
	int number;
	int m[face_x][face_y+1];
};

struct S {
	int x;
	int y;
	S* next;
};
class Snake
{
public:
	void initSnake();//初始化蛇
	void move(int key);//蛇移动
	void add_snake();//加一节蛇尾
	bool is_die();//死了返回1，没死返回0
	void print(); //画蛇
	S* head;//头节点
private:
	S* last;//尾
	int size;
	int a[2];//保存移动时被删的最后一个节点坐标，用以加长度时
};
class Food 
{
public:
	void setFood(S *&head);//放食物
	bool isEat(S*& head);//吃到返回1，没吃到返回0
private:
	int x;
	int y;//食物坐标
	int color;
};

class Game
{
public:
	int button();//判断按键，处理相悖事件，以及暂停退出
	void welcome();//开始界面
	void set_degree();//设置难度，易0，中1，难2
	void choose_degree();
	void set_ms();//模式，传统0，无墙1，无尽2
	void choose_ms();//选择模式
	static int degree;//难度
	int ndtime;//不同难度对应的时间
	static int ms;//模式
	int key;//按键
};

void game(Map& map, Snake& snake, Food& food, Game& g);
