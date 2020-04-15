#include"snake.h"

//MAP
void Map::paint()
{
	Hide_Cursor();
	number = 0;
	if (Game::ms == 0)
	{
		for (int i = 0; i < face_y; i++)
			for (int j = 0; j < face_x; j++)
				if (i == 0 || i == face_y - 1 || j == 0 || j == face_x - 1)
				{
					gotoxy(j * 2, i);
					cout << "■";
				}
				else 
				{
					gotoxy(j * 2, i); cout << "  ";
				}
	}
	gotoxy(0, face_y);
	cout << "方向键移动    SPACE暂停    ESC退出";
	gotoxy(face_x, face_y);
	if (Game::ms == 0)cout << "传统";
	else if (Game::ms == 1)cout << "无墙";
	else if (Game::ms == 2)cout << "无尽";
	if (Game::degree == 0)cout << "  易";
	else if (Game::degree == 1)cout << "  中";
	else if (Game::degree == 2)cout << "  难";
	gotoxy((face_x - 9) * 2, face_y);
	cout << "当前分数:" << number;
}
void Map::update_grade()
{
	number += 10;
	gotoxy((face_x - 9) * 2, face_y);
	cout <<"当前分数:"<< number;
}


void Snake::initSnake()
{
	last = head = NULL;
	for (int i = 0; i < 3; i++)
	{
		S* p = new S;
		p->x = 1;
		p->y = face_y/2;
		p->next = NULL;
		if (head == NULL)
		{
			head = p;
			last = p;
		}
		else
		{
			last->next = p;
			last = p;
		}
	}
}
void Snake::move(int key)
{
	int x, y;
	x = head->x;y = head->y;
	switch (key)
	{
	case up:y--; break;
	case down:y++; break;
	case left:x--; break;
	case right:x++; break;
	}
	if (Game::ms == 1 || Game::ms == 2)//无墙、无尽模式建立边界循环
	{
		if (x == -1)x = face_x - 1;
		else if (x == face_x)x = 0;
		else if (y == -1)y = face_y - 1; 
		else if (y == face_y)y = 0; 
	}
	S* q;
	for (q = head; q->next != last; q = q->next);
	a[0] = last->x;
	a[1] = last->y;
	q->next = NULL;
	delete last;
	last = q;
	gotoxy(a[0] * 2, a[1]);
	cout << "  ";//删尾
	S* p = new S; 
	p->x = x; p->y = y; p->next = head; head = p;//添头

}
void Snake::add_snake()
{
	S* p = new S;
	p->x = a[0];
	p->y = a[1];
	p->next = NULL;
	last->next = p;
	last = p;
	size++;
}
bool Snake::is_die()
{
	S* p; bool is = false;
	if (Game::ms != 2)//除无尽模式外撞蛇身死
	{
		for (p = head->next; p != NULL; p = p->next)
			if (head->x == p->x && head->y == p->y)
				is = true;
	}
	if (Game::ms == 0)//传统模式
		if (head->x == 0 || head->x == face_x - 1 || head->y == 0 || head->y == face_y - 1)
			is = true;
	if(is)//删除动态内存空间
		for (S*q,*p = head; p != NULL;)
		{
			q = p;
			p = p->next;
			delete q;
		}
	return is;
}
void Snake::print()
{
	S* p;
	gotoxy(head->x * 2, head->y);
	cout << "■";
	for (p = head->next; p != NULL; p = p->next)
	{
		gotoxy(p->x * 2, p->y);
		cout << "■";
	}
}

void Food::setFood(S *&head)
{
	
	srand(time(NULL));
	S* p; bool isSnake=true;
	while (isSnake)
	{
		x = rand() % (face_x - 2) + 1;
		y = rand() % (face_y - 2) + 1;
		color = rand() % 9;
		for (p = head; p != NULL; p = p->next)
		{
			if (x == p->x && y == p->y)
				break;
		}
		if (p == NULL) isSnake = false;
		else  isSnake = true;
	}
	
	gotoxy(x * 2, y);
	cout << "■";
}
bool Food::isEat(S*& head)
{
	if (head->x == x && head->y == y)
		return true;
	else return false;
}

int Game::button()
{
	int ukey = _getch();
	if (ukey == Esc || ukey == space || ukey == enter)
	{
		if (ukey == Esc)ukey = 0;
		else if (ukey == space)while (!_kbhit());
		return ukey;
	}
	else {
		ukey = _getch();
		if (ukey == right && key == left || ukey == up && key == down || ukey == down && key == up || ukey == left && key == right)
			ukey = key;
		return ukey;
	}
}
void Game::welcome()
{
	
	gotoxy(face_x - 16, face_y / 4);
	cout << "*****贪  吃  蛇  大  作  战*****";
	gotoxy(0, face_y);
	cout << "版本:1.0   修改时间:2020.2.23";
	set_ms();
	set_degree();
	choose_ms();
	choose_degree();
}
void Game::set_ms()
{
	gotoxy(face_x - 12, face_y / 2-2);
	cout << "**模式:  传统  无墙  无尽";	
}
void Game::choose_ms()
{
	int x = 60, i = 0;
	gotoxy(face_x - 4, face_y / 2 - 2);
	Look_Cursor();
	while (i != enter)
	{
		i = _getch();
		if (i == left)x--;
		else if (i == right)x++;
		ms = x % 3;
		if (ms == 0)
			gotoxy(face_x - 4, face_y / 2 - 2);
		else if (ms == 1)
			gotoxy(face_x + 2, face_y / 2 - 2);
		else if (ms == 2)
			gotoxy(face_x + 8, face_y / 2 - 2);
	}
	if (ms == 0)
	{
		cout << "     "; Sleep(ss);
		gotoxy(face_x - 4, face_y / 2 - 2);
		cout << " 传统"; Sleep(ss);
	}
	else if (ms == 1)
	{
		cout << "     "; Sleep(ss);
		gotoxy(face_x + 2, face_y / 2 - 2);
		cout << " 无墙"; Sleep(ss);
	}
	else if (ms == 2)
	{
		cout << "     "; Sleep(ss);
		gotoxy(face_x + 8, face_y / 2 - 2);
		cout << " 无尽"; Sleep(ss);
	}
}
void Game::set_degree()
{
	gotoxy(face_x - 12, face_y / 2);
	cout << "**难度:    易    中    难";
}
void Game::choose_degree()
{
	int x = 60, i = 0;
	gotoxy(face_x - 2, face_y / 2);
	Look_Cursor();
	while (i != enter)
	{
		i = _getch();
		if (i == left)x--;
		else if (i == right)x++;
		degree = x % 3;
		if (degree == 0)
			gotoxy(face_x - 2, face_y / 2 );
		else if (degree == 1)
			gotoxy(face_x + 4, face_y / 2 );
		else if (degree == 2)
			gotoxy(face_x + 10, face_y / 2 );
	}
	if (degree== 0)
	{
		cout << "     "; Sleep(ss);
		gotoxy(face_x - 2, face_y / 2 );
		cout << " 易"; Sleep(ss);
		ndtime = 150;
	}
	else if (degree == 1)
	{
		cout << "   "; Sleep(ss);
		gotoxy(face_x + 4, face_y / 2 );
		cout << " 中"; Sleep(ss);
		ndtime = 80;
	}
	else if (degree == 2)
	{
		cout << "   "; Sleep(ss);
		gotoxy(face_x + 10, face_y / 2);
		cout << " 难"; Sleep(ss);
		ndtime = 30;
	}
}
void Map::initdie()
{
	Sleep(500);
	system("cls");
	gotoxy(face_x -8, face_y / 4);
	cout << "***你  死  了***";
	gotoxy(face_x - 8, face_y / 2-2);
	cout << " 你的分数为;" << number;
	gotoxy(face_x - 8, face_y / 2);
	cout << "按任意键继续游戏...";
	Sleep(500);
	int o = _getch();
}
int Game::ms = 0;
int Game::degree = 0;

void game(Map& map, Snake& snake, Food& food, Game& g)
{
	system("cls");
	g.welcome();
	system("cls");
	map.paint();
	snake.initSnake();
	snake.print();
	food.setFood(snake.head);
	g.key = right;
	int x = 1;
	while (g.key!=0)
	{
		while (!_kbhit())
		{
			snake.move(g.key);
			snake.print();
			if (snake.is_die())
			{
				x = 0; break;
			}
			if (food.isEat(snake.head))
			{
				snake.add_snake();
				snake.print();
				food.setFood(snake.head);
				map.update_grade();
			}
			Sleep(g.ndtime);
		}if (x == 0)break;
		g.key = g.button();
	}
	map.initdie();
}

void gotoxy(int x, int y)
{
	COORD wz;
	wz.X = x;
	wz.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), wz);
}

void set_color(int x)
{
	int c;
	switch (x)
	{
	case 0:c = 7; break;//白
	case 1:c = 4; break;//红
	case 2:c = 14; break;//淡黄
	case 3:c = 6; break;//黄
	case 4:c = 2; break;//绿
	case 5:c = 1; break;//蓝
	case 6:c = 5; break;//紫
	case 7:c = 8; break;//灰
	case 8:c = 11; break;//淡浅绿色
	}
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, c);
}

void Hide_Cursor()    //隐藏光标
{
	//做一个门把手，打开冰箱门，拿出大象，把大象修改一下，再把大象塞回去
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); //HANDLE句柄指的是一个核心对象在某一个进程中的唯一索引，而不是指针。GetStdHandle是一个Windows API函数。它用于从一个特定的标准设备（标准输入、标准输出或标准错误）中取得一个句柄（用来标识不同设备的数值）。
	CONSOLE_CURSOR_INFO cci; //CONSOLE_CURSOR_INFO包含的控制台光标的信息
	GetConsoleCursorInfo(hOut, &cci); //检索有关指定的控制台屏幕缓冲区的光标的可见性和大小信息。(hConsoleOutput,控制台屏幕缓冲区的句柄。该句柄必须具有的 GENERIC_READ 的访问权限。; lpConsoleCursorInfo, 指向接收有关该控制台的光标的信息的CONSOLE_CURSOR_INFO结构的指针。)
	cci.bVisible = 0; //赋值0为隐藏，赋值1为可见
	SetConsoleCursorInfo(hOut, &cci);
}
void Look_Cursor()    //隐藏光标
{
	//做一个门把手，打开冰箱门，拿出大象，把大象修改一下，再把大象塞回去
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); //HANDLE句柄指的是一个核心对象在某一个进程中的唯一索引，而不是指针。GetStdHandle是一个Windows API函数。它用于从一个特定的标准设备（标准输入、标准输出或标准错误）中取得一个句柄（用来标识不同设备的数值）。
	CONSOLE_CURSOR_INFO cci; //CONSOLE_CURSOR_INFO包含的控制台光标的信息
	GetConsoleCursorInfo(hOut, &cci); //检索有关指定的控制台屏幕缓冲区的光标的可见性和大小信息。(hConsoleOutput,控制台屏幕缓冲区的句柄。该句柄必须具有的 GENERIC_READ 的访问权限。; lpConsoleCursorInfo, 指向接收有关该控制台的光标的信息的CONSOLE_CURSOR_INFO结构的指针。)
	cci.bVisible = 1; //赋值0为隐藏，赋值1为可见
	SetConsoleCursorInfo(hOut, &cci);
}
