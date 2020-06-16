#include"snake.h"
using namespace std;
int main()
{
	system("title Ì°³ÔÉß´ó×÷Õ½");
	system("mode con lines=45 cols=180");
	system("color ae");
	Map map;
	Snake snake;
	Food food;
	Game g;
	while(1)
	game(map, snake, food, g);
}
