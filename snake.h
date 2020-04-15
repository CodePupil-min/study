#pragma once
#include<iostream>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<windows.h>
#include<thread>
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
#define ss 200//ѡ��ģʽ���Ѷ�ʱ��˸ʱ��



void set_color(int x=0);//�����ַ���ɫ
void gotoxy(int x, int y);//���ù��λ��
void Hide_Cursor();   //���ع��
void Look_Cursor();



class Map
{
public:
	void paint();//������ʼ��ͼ����ʾ
	void update_grade();//ˢ�·���
	void initdie();//��������
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
	void initSnake();//��ʼ����
	void move(int key);//���ƶ�
	void add_snake();//��һ����β
	bool is_die();//���˷���1��û������0
	void print(); //����
	S* head;//ͷ�ڵ�
private:
	S* last;//β
	int size;
	int a[2];//�����ƶ�ʱ��ɾ�����һ���ڵ����꣬���Լӳ���ʱ
};
class Food 
{
public:
	void setFood(S *&head);//��ʳ��
	bool isEat(S*& head);//�Ե�����1��û�Ե�����0
private:
	int x;
	int y;//ʳ������
	int color;
};

class Game
{
public:
	int button();//�жϰ�������������¼����Լ���ͣ�˳�
	void welcome();//��ʼ����
	void set_degree();//�����Ѷȣ���0����1����2
	void choose_degree();
	void set_ms();//ģʽ����ͳ0����ǽ1���޾�2
	void choose_ms();//ѡ��ģʽ
	static int degree;//�Ѷ�
	int ndtime;//��ͬ�Ѷȶ�Ӧ��ʱ��
	static int ms;//ģʽ
	int key;//����
};

void game(Map& map, Snake& snake, Food& food, Game& g);