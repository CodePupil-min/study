/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include <string.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include "list.h"





float		g_fScreenLeft=0.f;    // 屏幕左边界值
float		g_fScreenRight=0.f;    // 右
float		g_fScreenTop=0.f;    // 上
float		g_fScreenBottom=0.f;    // 下

float		g_fSpeedUp = 0.f;
float		g_fSpeedDown = 0.f;
float		g_fSpeedLeft = 0.f;
float		 g_fSpeedRight = 0.f;


Weapon*		g_pMypeople		=	NULL;
Weapon*		g_pMypeople1	=	NULL;	// 我方
Weapon*		g_pBoss			=	NULL;	//boss

int			g_iLevel		=	1;		//关卡
int			g_iSoundID		=	0;		//音乐ID
int			g_iSoundID2		=	0;		//我方开枪
int			g_iSoundID3		=	0;		//boss开枪
bool		g_bStart		=	false;  // 控制一局游戏开始与结束
int			g_iScore		=	0;		// 一局游戏得分
float		g_fGameTime		=	100.f;    // 一局游戏的剩余时间
int			g_iWallCount	=	0;		// 记录墙的数量
int			g_iMap[11][13];				// 地图数组
int			g_iNum			=	0;		// 记录我方子弹、我方、敌方的数量
float		g_fCreateTime	=	2.f;	// 每批次生成的剩余时间
float		g_fSleepTime	=	3.f;	//关卡之间的间隔时间


const float	FIRE_TIME		=  2.f;		// 敌方发射炮弹的时间间隔
const float	BOSS_TIME		=  4.f;
const float CREATE_alien_TIME = 2.5f;		// 每批次生成的时间间隔
const float	GAME_TIME		=	100.f;	// 一局游戏的时间
const float	alien_SPEED		 = 5.f;     // 敌方速度
const float BULLET_SPEED	 = 20.f;		// 子弹速度
const float ROTATE_TIME		 = 2.f;     // 敌方调转方向的时间

//是否生成小怪
bool crene=true;
//玩家是否死亡
bool playerDead = false;
//玩家是否胜利
bool isWon=false;
//历史分数排名
int		rank[3]={0,0,0};

// 函数声明

void	MoveMyalien(int, bool);
void	LoadMap();
void	OnFire(char*, int, const float fMouseX, const float fMouseY);
void	CreateEnemyaliens();
void	CreateEnemy2aliens();
void	MoveEnemyalien(const char*);
bool	IsDead(const char*);
void	CreateBoss();
void	OnDeleteAllAlien();
void	AfterDead(const char* szName);
void	skill_1();
void	skill_2();
void	skill_3();
void	skill_4();

void	WriteScore(const int x);
void	ReadScore();
// 碰撞函数

void	OnMyalienColOther(const char*, const char*);
void	OnBulletColOther(const char*, const char*);
void	OnWallColOther(const char*, const char*);
void	OnalienColOther(const char*, const char*);
void	OnGoalColOther();

//UI绘制
void OnClickGo(const float fMouseX, const float fMouseY);
void paintUI(float fTime);
void CleanSprite(const char *szName);

//==============================================================


//绘制地图
#define	XBlock 32
#define YBlock 23
int mapitem[4];
int mapall[XBlock][YBlock];
char mapcontent[XBlock*YBlock][64];
int	iLoopX = 0, iLoopY = 0, iLoop = 0;
int ibasis;
int pIndex;
float sLeft=0;
float sTop=-0;
// 	按方块大小，在编辑器里摆放的第一块方块的起始坐标
const float	g_fBlockStartX	= sLeft+15.5;
const float	g_fBlockStartY	= sTop-4.6875;
// 	屏幕高度75 / 4块 = 18.75每块的大小.编辑器里预先摆放好的方块宽和高  //	必须与此值一致
const float	g_fBlockSize1 = 3.125;
const float	g_fBlockSize2 = 4.6875;
////////////////////////////////////////////////////////////////////////////////
//
//
int			g_iGameState		=	0;		// 游戏状态，0 -- 游戏结束等待开始状态；1 -- 按下空格键开始，初始化游戏；2 -- 游戏进行中
//
void		GameInit();
void		GameRun( float fTimeDelta );
void		GameEnd();
int XYToOneIndex( const int iIndexX, const int iIndexY );
void PaintMap(int decide1,int decide2,int decide3,int decide4);
//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。


//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态. 
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void GameMainLoop( float	fDeltaTime)
{
	paintUI(fDeltaTime);
	switch( g_iGameState )
	{
		// 初始化游戏，清空上一局相关数据
		
	case 1:
		{
			GameInit();
			g_iGameState	=	2; // 初始化之后，将游戏状态设置为进行中
		}
		break;

		// 游戏进行中，处理各种游戏逻辑
	case 2:
		{
			// TODO 修改此处游戏循环条件，完成正确游戏逻辑
			if( true )
			{
				GameRun( fDeltaTime );
			}
			else
			{
				// 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
				g_iGameState	=	0;
				GameEnd();
			}
		}
		break;

		// 游戏结束/等待按空格键开始
	case 0:
	default:
		break;
	}
	
}

//==============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void GameInit()
{
	DeleteList();
	g_iLevel		=	1;
	g_iScore		=	0;
	g_pBoss			=	NULL;
	g_pMypeople		=	NULL;
	g_fGameTime		=	GAME_TIME;
	g_fCreateTime	=	0;
	g_iNum			=	0;	
	playerDead = false;
	crene=true;
	isWon=false;


	g_fScreenLeft=dGetScreenLeft();    // 屏幕左边界值
	g_fScreenRight=dGetScreenRight();    // 右
	g_fScreenTop=dGetScreenTop();    // 上
	g_fScreenBottom=dGetScreenBottom();    // 下
	PaintMap(1,2,3,4);
	
	g_pMypeople = (Weapon*)malloc(sizeof(Weapon));
	strcpy(g_pMypeople->szName, "player");
	dCloneSprite("playerA",g_pMypeople->szName);
	g_pMypeople->iHp = 20;
	dSetSpritePosition(g_pMypeople->szName,-3,23);//设置玩家位置
	dSetSpriteWorldLimit(g_pMypeople->szName, WORLD_LIMIT_NULL, g_fScreenLeft+3.125, g_fScreenTop+4.6875, g_fScreenRight-3.125, g_fScreenBottom-4.6875); // 设置玩家世界边界
	dSetSpriteCollisionSend(g_pMypeople->szName, true);

	dSetSpritePosition("alien1", g_fScreenRight+10, g_fScreenTop-10);
	dSetSpritePosition("alien2", g_fScreenRight+10, g_fScreenTop-10);
	dSetSpritePosition("alien3", g_fScreenRight+10, g_fScreenTop-10);
	dSetSpriteLinearVelocity("alien1", 0, 0);
	dSetSpriteLinearVelocity("alien2", 0, 0);
	dSetSpriteLinearVelocity("alien3", 0, 0);
	

}
//==============================================================================
//
// 每局游戏进行中
void GameRun( float fDeltaTime )
{
	
		if(g_bStart)
		{
			//每隔一段时间制造敌方目标
			g_fCreateTime-=fDeltaTime;
			if(g_fCreateTime<0.f&&crene)
			{
				for(int i=0;i<1;i++)
					CreateEnemyaliens();
				g_fCreateTime=CREATE_alien_TIME;
				g_fCreateTime-=(g_iLevel-1)/2;//刷怪间隔逐渐缩短，1 2.5秒；2 2秒；3 1.5秒。
			}

			//每关制造boss
			g_fGameTime-=fDeltaTime;
			if(g_fGameTime<0.f&&g_pBoss==NULL)
			{
				crene=false;
				CreateBoss();
				g_fGameTime=GAME_TIME;
			}

			Weapon* pNode	= g_pHead;
			int num = GetListSize();  // 当前链表的节点数量
			// 遍历当前链表，寻找敌方人物节点
			for(int i=0; i<num; i++)
			{
				if(pNode->iType == ENEMYalien||pNode->iType == BOSS)  // 敌方人物
				{
					if(pNode->iHp != 0)
					{
						float fPosX=dGetSpritePositionX(g_pMypeople->szName);
						float fPosY=dGetSpritePositionY(g_pMypeople->szName);
						
						pNode->fFireTime -= fDeltaTime;
						if(pNode->fFireTime < 0.f)
						{
							if(pNode->iType == ENEMYalien)
							{
								pNode->fFireTime = FIRE_TIME;
								OnFire(pNode->szName, ENEMYBULLET, fPosX, fPosY);
							}
							else if(pNode->iType == BOSS)
							{
								pNode->fFireTime = BOSS_TIME;
								int ibasis = dRandomRange(1,90);
								if(g_iLevel==1)
								{
									if(ibasis<=50)
										OnFire(pNode->szName, BOSSBULLET, fPosX, fPosY);									
									else if(ibasis<=90)
										skill_1();							
								}else if(g_iLevel==2)
								{
									if(ibasis<=50)
										OnFire(pNode->szName, BOSSBULLET, fPosX, fPosY);
									else if(ibasis<=70)									
										skill_2();
									else									
										skill_3();
								}else if(g_iLevel==3)
								{
									if(ibasis<=60)
										OnFire(pNode->szName, BOSSBULLET, fPosX, fPosY);
									else if(ibasis<=90)
										skill_4();
								}
							}
						}
					}
				}
				pNode = pNode->pNext;
			}
			
		}
}
//==============================================================================
//
// 本局游戏结束
void GameEnd()
{
}
//==========================================================================
//
// 鼠标移动
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseMove( const float fMouseX, const float fMouseY )
{
	
}
//==========================================================================
//
// 鼠标点击
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{

	if(iMouseType == MOUSE_LEFT)//检测到鼠标左键按下
	{
		//游戏进行时
		if(g_bStart)
			OnFire(g_pMypeople->szName, MYBULLET, fMouseX, fMouseY);//向开火函数传递玩家链表信息和鼠标点击位置，并且记录此子弹来自于玩家
		//未进行时执行其他操作
		else
			OnClickGo(fMouseX,fMouseY);
	}
}
//==========================================================================
//
// 鼠标弹起
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	
}
//==========================================================================
//
// 键盘按下
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{	
	if((playerDead||isWon)&&iKey==KEY_SPACE)
	{
		g_iGameState	=	0;
		WriteScore(g_iScore);
		DeleteList();
	}
	if(iKey == KEY_W || iKey == KEY_A || iKey == KEY_S || iKey == KEY_D )
	{
		switch(iKey)
		{
		case KEY_W:		
			g_fSpeedUp = -25.f;
			//g_pMypeople->iDir = UP;
			break;
		case KEY_A:
			g_fSpeedLeft = -25.f;
			//	g_pMypeople->iDir = LEFT;
			break;
		case KEY_S:	
			g_fSpeedDown = 25.f;
			//	g_pMypeople->iDir = DOWN;
			break;
		case KEY_D:
			g_fSpeedRight = 25.f;	
			//	g_pMypeople->iDir = RIGHT;
			break;
		default:
			break;
		}
		dSetSpriteLinearVelocity("player", g_fSpeedLeft + g_fSpeedRight, g_fSpeedUp + g_fSpeedDown);
		if((g_fSpeedLeft + g_fSpeedRight) > 0)	
			dSetSpriteFlipX("player", false);	// 向右，图片不需要翻转
		else if((g_fSpeedLeft + g_fSpeedRight) < 0)
			dSetSpriteFlipX("player",true);	// 向左，图片需要翻转
	}
}
//==========================================================================
//
// 键盘弹起
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
void OnKeyUp( const int iKey )
{
	switch(iKey)
	{
	case KEY_W:		
		g_fSpeedUp = 0.f;
		break;
	case KEY_A:
		g_fSpeedLeft = 0.f;
		break;
	case KEY_S:	
		g_fSpeedDown = 0.f;
		break;
	case KEY_D:
		g_fSpeedRight = 0.f;	
		break;
	default:
		break;
	}
	dSetSpriteLinearVelocity("player", g_fSpeedLeft + g_fSpeedRight, g_fSpeedUp + g_fSpeedDown);
	if((g_fSpeedLeft + g_fSpeedRight) > 0)	
		dSetSpriteFlipX("player", false);	// 向右游，图片不需要翻转
	else if((g_fSpeedLeft + g_fSpeedRight) < 0)
		dSetSpriteFlipX("player", true);	// 向左游，图片需要翻转

	
}
//===========================================================================
//
// 精灵与精灵碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	
	if(strcmp(szSrcName, g_pMypeople->szName) == 0)  // 我方
		OnMyalienColOther(szSrcName, szTarName);
	if(strstr(szSrcName, "bullet_") != NULL)           // 子弹击中敌方
		OnBulletColOther(szSrcName, szTarName);
	if(strstr(szSrcName, "alien_") != NULL)			  // 敌方人物
		OnalienColOther(szSrcName, szTarName);
	if(strstr(szSrcName, "boss_") != NULL)			  // 敌方boss
		OnalienColOther(szSrcName, szTarName);

	if(strstr(szTarName, "alien_") != NULL)			  // 敌方为接收碰撞体
		OnalienColOther(szTarName, szSrcName);
	if(strstr(szTarName, "boss_") != NULL)			  // boss为接收碰撞体
		OnalienColOther(szTarName, szSrcName);
	if(strstr(szTarName, "bullet_") != NULL)           // 子弹
		OnBulletColOther(szTarName, szSrcName);

	Weapon* p0 = FindNode(szSrcName);
	Weapon* p1 = FindNode(szTarName);

	if(p0 != NULL && IsDead(szSrcName))
	{
		dDeleteSprite(szSrcName);
		DeleteNode(szSrcName);
	}
	if(p1 != NULL && IsDead(szTarName))
	{
		dDeleteSprite(szTarName);
		DeleteNode(szTarName);
	}
}
//===========================================================================
//
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
	if(strstr(szName, "alien_") != NULL)   // 敌方触碰边界判断
	{
		Weapon* enemyalien = FindNode(szName);
		float fSpeedX = 0.f, fSpeedY = 0.f;
		int	  iDir = 0;

		// 调头
		switch(iColSide)
		{
		case 0:  // 碰到左边，调头朝右
			fSpeedX = alien_SPEED;
			iDir = RIGHT;
			break;
		case 1:  // 碰到右边，调头朝左
			fSpeedX = -alien_SPEED;
			iDir =	LEFT;
			break;
		case 2:  // 碰到上边，调头朝下
			fSpeedY = alien_SPEED;
			iDir = DOWN;
			break;
		case 3:  // 碰到下边，调头朝上
			fSpeedY = -alien_SPEED;
			iDir = UP;
			break;
		}
		dSetSpriteLinearVelocity(szName, fSpeedX, fSpeedY);
		dSetSpriteRotation(szName, iDir*90);

	}

	if((strcmp(szName, "player"))== 0)//玩家碰撞边界判断
	{//判断边界碰撞
	if(iColSide == 0)
	{
		dSetSpriteLinearVelocity(szName,0,0);
		dSetSpriteFlipX(szName, 0);
	}
	else if(iColSide == 1)
	{
		dSetSpriteLinearVelocity(szName,0,0);
		dSetSpriteFlipX(szName, 1);
	}
	else if(iColSide == 2)
	{
		dSetSpriteLinearVelocity(szName,0,0);
		dSetSpriteFlipY(szName, 0);
	}
	else if(iColSide == 3)
	{
		dSetSpriteLinearVelocity(szName,0,0);
		dSetSpriteFlipY(szName, 0);
	}
	}

}
//=========================================================================
int XYToOneIndex( const int iIndexX, const int iIndexY )
{
	return (iIndexY * XBlock + iIndexX);
}
void CleanMap()
{
	Weapon* pNode	= g_pHead;
	int num = GetListSize();  // 当前链表的节点数量
	for(int i=0; i<num; i++)
	{
		if(pNode->iType==MAP)
		{
			dDeleteSprite(pNode->szName);
		}
	}
}
void PaintMap(int decide1,int decide2,int decide3,int decide4)
{
	//绘制地图
	char szName[XBlock*YBlock];
	char copyName[XBlock*YBlock];
	Weapon* map = (Weapon*)malloc(sizeof(Weapon));
	int i=4;
	for(iLoopY = 0;iLoopY<YBlock;iLoopY++)
	{
		float y=iLoopY;
		for(iLoopX = 0;iLoopX<XBlock;iLoopX++)
		{
			float x=iLoopX;
			i++;
			pIndex=XYToOneIndex(iLoopX,iLoopY);
			if(iLoopY==0||iLoopY==22||iLoopX==0||iLoopX==31)
			{
				ibasis = dRandomRange(decide3,decide4);
				sprintf(szName, "map_%d", i);
				sprintf(copyName, "map%d", ibasis);
				dCloneSprite(copyName, szName);
				strcpy( mapcontent[pIndex],szName);
				dSetSpritePosition(mapcontent[pIndex], sLeft+g_fBlockSize1*(x-15.5), sTop+g_fBlockSize1*(y-12.5)+g_fBlockSize2);
				strcpy(map->szName,  szName);
				map->iType = MAP;

				AddToList(map);
			}else
			{
				ibasis = dRandomRange(decide1,decide2);
				sprintf(szName, "map_%d", i);
				sprintf(copyName, "map%d", ibasis);
				dCloneSprite(copyName, szName);
				strcpy( mapcontent[pIndex], szName);
				dSetSpritePosition(mapcontent[pIndex], sLeft+g_fBlockSize1*(x-15.5), sTop+g_fBlockSize1*(y-12.5)+g_fBlockSize2);
				strcpy(map->szName,  szName);
				map->iType = MAP;

				AddToList(map);
			}
			
		}
	}
}

//=====================================================================
void CreateEnemyaliens()
{
	// 每次出现一个敌人，都出现在第一行，列数随机，但前面不能有障碍
	float	fPosX, fPosY;
	float	fSpeedX, fSpeedY;
	char	szName[128];
	char	dpName[128];
	int		iDir;
	static int posx=-45;
	//if(crene){
		Weapon* alien = (Weapon*)malloc(sizeof(Weapon));

		fPosX = dRandomRange(-30, 30);  // 统一出现在第一行
		posx+=8;
		iDir = 1;	  // 朝向统一向下

		// 如果前方有障碍，就重新找位置
		int j;
		do{
			j = dRandomRange(-10, 10);
		}while(g_iMap[1][j] == 1);


		fPosY = -35;
		sprintf(dpName, "alien%d", g_iLevel);
		sprintf(szName, "alien_%d", g_iNum++);
		
		dCloneSprite(dpName, szName);
		dSetSpritePosition(szName, fPosX, fPosY);
		dSetSpriteRotation(szName, iDir*90);
		dSetSpriteLinearVelocityY(szName, alien_SPEED);
		// 设置碰撞
		dSetSpriteCollisionSend(szName, true);
		dSetSpriteCollisionReceive(szName, true);
		// 设置世界边界
		dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);

			// 将指针节点加入到链表中
		strcpy(alien->szName,  szName);
		alien->iType = ENEMYalien;
		alien->iDir = iDir;
		alien->fRotateTime = ROTATE_TIME;
		alien->iHp = 1;
		alien->fFireTime = FIRE_TIME;
		alien->pNext=NULL;

		AddToList(alien);
	
}



bool IsDead(const char* szName)//判断链表某一目标hp值决定生死
{
	Weapon* pSprite = FindNode(szName);

	if(pSprite != NULL && pSprite->iHp <= 0)
		return true;

	return false;
}

void	OnBulletColOther(const char* szBulletName, const char* szOtherName)//判断子弹碰撞时使子弹hp归零
{
	Weapon * pBullet = FindNode(szBulletName);  // 根据名称找出对应的子弹节点
	if(strcmp(szOtherName, "player")==0&&(pBullet->iType==ENEMYBULLET||pBullet->iType==BOSSBULLET))//击中玩家
	{	
		dDeleteSprite(szBulletName);
		DeleteNode(szBulletName);
		g_pMypeople->iHp-=g_iLevel;
		if(g_pMypeople->iHp<=0){
			g_bStart=false;
			playerDead=true;
			AfterDead(g_pMypeople->szName);
			dDeleteSprite("player");
		}
	}
	
	else if(pBullet->iType == MYBULLET)
	{	
		if(strstr(szOtherName, "alien_")!=NULL)//击中敌人
		{	
			dDeleteSprite(szBulletName);
			DeleteNode(szBulletName);
			Weapon * palien = FindNode(szOtherName);
			if(!palien)return;
			palien->iHp-=1;
			if(palien->iHp<=0)
			{
				AfterDead(szOtherName);
				dDeleteSprite(szOtherName);	
				g_iScore+=g_iLevel*10;
			}
		}
		if(strstr(szOtherName, "boss_")!=NULL)//击中boss
		{
			dDeleteSprite(szBulletName);
			DeleteNode(szBulletName);
			if(!g_pBoss)return;
			g_pBoss->iHp-=1;
			if(g_pBoss->iHp<=0)
			{
				AfterDead(g_pBoss->szName);
				OnDeleteAllAlien();//boss死亡删除所有小怪
				//删除boss
				dDeleteSprite(szOtherName);
				g_bStart=false;//进入过渡
				g_fGameTime=GAME_TIME;//游戏时间
				g_iScore+=g_iLevel*100;//加分
				g_pMypeople->iHp+=g_iLevel*3;//加血
				crene=true;//正常刷怪
				g_pBoss=NULL;//更新状态
				if(g_iLevel==3)isWon=true;//最终胜利
				g_iLevel++;//关卡+1
			}
		}
	}
		


}

void	OnalienColOther(const char* szalienName, const char* szOtherName)//判断敌人各种碰撞时对链表的操作
{
	Weapon * aim = FindNode(szalienName);
	if(aim->iType!=BOSS)
	{
		// 如果碰到玩家，停下来
		if(strcmp(szOtherName, "player") == 0)
			dSetSpriteLinearVelocity(szalienName, 0.f, 0.f);

	
		// 如果碰到也是敌方，敌方停下来
		if(strstr(szOtherName, "alien_") != NULL)
		{
			float fSpeedX0 = dGetSpriteLinearVelocityX(szalienName);
			float fSpeedY0 = dGetSpriteLinearVelocityY(szalienName);
			float fSpeedX1 = dGetSpriteLinearVelocityX(szOtherName);
			float fSpeedY1 = dGetSpriteLinearVelocityY(szOtherName);

			// 如果该目标原来有速度，调头前行

			if(fSpeedX0 > 0.0001)
			{
				dSetSpriteLinearVelocity(szalienName, -alien_SPEED, 0);
				dSetSpriteRotation(szalienName, 270);
			}
			else
			{
				dSetSpriteLinearVelocity(szalienName, alien_SPEED, 0);
				dSetSpriteRotation(szalienName, 90);
			}


			if(fSpeedY0 > 0.0001)
			{
				dSetSpriteLinearVelocity(szalienName, 0, -alien_SPEED);
				dSetSpriteRotation(szalienName, 0);
			}
			else
			{
				dSetSpriteLinearVelocity(szalienName, 0, alien_SPEED);
				dSetSpriteRotation(szalienName, 180);
			}

		}
	}
}


void	OnFire(char* szName, int iType, const float fMouseX, const float fMouseY)//开火函数（玩家和敌人通用）
{
	char dpBullet[128];
	char dpsBullet[128];
	char myBullet[128];
	char szBullet[128];	
	float fPosX, fPosY;
	int iDir;
	float fSpeedX,fSpeedY;

	//获取每一关的小怪子弹模板
	sprintf(dpBullet, "bullet%d", g_iLevel);
	//获取每一关的boss子弹模板
	sprintf(dpsBullet, "sbullet%d", g_iLevel);
	//玩家子弹
	sprintf(myBullet, "mybullet");

	//给所有子弹命名
	sprintf(szBullet, "bullet_%d",g_iNum++);

	if(iType==BOSSBULLET)
	{
		dCloneSprite(dpsBullet,szBullet);
		g_iSoundID3 = dPlaySound("eneshot.ogg",0,1);
	}
	else if(iType==MYBULLET)
	{
		dCloneSprite(myBullet,szBullet);
		g_iSoundID2 = dPlaySound("shot.ogg",0,1);
	}
	else if(iType==ENEMYBULLET)
	{
		dCloneSprite(dpBullet,szBullet);
	}
	dSetSpriteWorldLimit(szBullet,WORLD_LIMIT_NULL,g_fScreenLeft+3.125f, g_fScreenTop+4.685f, g_fScreenRight-3.125f, g_fScreenBottom-4.685f);  // 设置子弹世界边界
	// 设置碰撞模式
	dSetSpriteCollisionSend(szBullet,true);
	dSetSpriteCollisionReceive(szBullet,true);

	// 获取当前玩家坐标值
	fPosX = dGetSpritePositionX(szName);
	fPosY = dGetSpritePositionY(szName);
	float mPosX = dGetSpritePositionX("player");
	float mPosY = dGetSpritePositionY("player");
	

	//当开火的是敌方
	if(iType==ENEMYBULLET)
	{
		int x = mPosX-fPosX;
		int y = mPosY-fPosY;
		if(x>0)
		{
			fPosX += dGetSpriteWidth(szName)/2 + 1;
		}else
		{
			fPosX -= dGetSpriteWidth(szName)/2 + 1;
		}
		if(y>0)
		{
			fPosY += dGetSpriteHeight(szName)/2 + 1;
		}else
		{
			fPosY -= dGetSpriteHeight(szName)/2 + 1;
		}
	}
	//boss
	else if(iType==BOSSBULLET)
	{
		fPosY += dGetSpriteHeight(szName)/2 + 2;
	}
	//me
	else if(iType==MYBULLET)
	{
		int x = fMouseX-fPosX;
		int y = fMouseY-fPosY;
		if(x>0)
		{
			fPosX += dGetSpriteWidth(szName)/2 + 1;
		}else
		{
			fPosX -= dGetSpriteWidth(szName)/2 + 1;
		}
	}
	dSetSpritePosition(szBullet, fPosX, fPosY);

	dSpriteMoveTo(szBullet,fMouseX, fMouseY, 15, false);

	// 创建子弹的指针变量，并加入到链表中
	Weapon *pBullet = (Weapon*)malloc(sizeof(Weapon));
	pBullet->iDir = iDir;   // 子弹朝向跟玩家朝向一致
	sprintf(pBullet->szName, szBullet);
	pBullet->fPosX = fPosX;
	pBullet->fPosY = fPosY;
	pBullet->iType = iType;
	pBullet->iHp  = 1;
	pBullet->pNext=NULL;

	AddToList(pBullet);
	


}

// 我方人物与其他精灵碰撞
void	OnMyalienColOther(const char* szalienName, const char* szOtherName)
{
	 if(strstr(szOtherName, "boss_") != NULL)
		 dSetSpriteLinearVelocity(g_pMypeople->szName, 0, 0);

	/*if(strstr(szOtherName, "wall") != NULL)
		dSetSpriteLinearVelocity(g_pMypeople1->szName, 0, 0);
    if(strstr(szOtherName, "stock") != NULL)
		dSetSpriteLinearVelocity(g_pMypeople1->szName, 0, 0);
    if(strstr(szOtherName, "sea") != NULL)
		dSetSpriteLinearVelocity(g_pMypeople1->szName, 0, 0);*/
	 if(strstr(szOtherName, "boss_") != NULL)
		 dSetSpriteLinearVelocity(g_pMypeople1->szName, 0, 0);

}


//出现boss
void  CreateBoss()
{
	// 每次出现一个敌人，都出现在第一行，列数随机，但前面不能有障碍
	float	fPosX, fPosY;
	char	szName[128];
	char	dpName[128];
	int		iDir;

	Weapon* boss = (Weapon*)malloc(sizeof(Weapon));

	//统一出现在地图上方正中间
	fPosY = g_fScreenTop+5.f;
	fPosX = (g_fScreenLeft + g_fScreenRight) / 2;
	iDir = 0;	  // 朝向统一向下

	sprintf(dpName, "boss%d", g_iLevel);
	sprintf(szName, "boss_%d_%d",g_iLevel, g_iNum++);
	dCloneSprite(dpName, szName);
	
	dSetSpritePosition(szName, fPosX, fPosY);
	
	// 设置碰撞
	dSetSpriteCollisionSend(szName, true);
	dSetSpriteCollisionReceive(szName, true);
	// 设置世界边界
	dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);

	// 将指针节点加入到链表中
	strcpy(boss->szName, szName);
	boss->iType = BOSS;
	boss->iDir = iDir;
	boss->fRotateTime = ROTATE_TIME;
	boss->iHp = g_iLevel*10;
	boss->fFireTime =FIRE_TIME;
	boss->pNext=NULL;
	g_pBoss=boss;

	AddToList(boss);
}

//清除当前屏幕所有小怪
void OnDeleteAllAlien()
{
	Weapon* pNode	= g_pHead; 
	for(;pNode!=NULL;pNode=pNode->pNext)
	{
		if(pNode->iType==ENEMYalien)
		{
			dDeleteSprite(pNode->szName);
			AfterDead(pNode->szName);
		}
		
	}
}

//==========================================================

char help_map[]="UI_helpmap";

char cover_box[]="UI_cover";

char close_box[]="UI_close";
char begin_box[] = "UI_begin";
char help_box[] = "UI_help";
char message_box[]="UI_message";
char over_box[] = "UI_over";

char score_box[] = "UI_score";

char my_score[]="UI_myscore";
char score_first[]="UI_Fscore";
char score_second[]="UI_Sscore";
char score_third[]="UI_Tscore";


char level_box[] = "UI_level";
char myhp_box[]="UI_myhp";
char bosshp_box[]="UI_bosshp";
char time_box[]="UI_time";
void OnClickGo(const float fMouseX, const float fMouseY)
{
	//游戏未进行时
	if (g_iGameState==0)
	{	//点击开始游戏
		if(dIsPointInSprite(begin_box, fMouseX, fMouseY))
		{
			CleanSprite(help_map);
			CleanSprite(help_box);
			CleanSprite(close_box);
			CleanSprite(begin_box);
			CleanSprite(cover_box);

			dSetTextString(score_first,"");
			dSetTextString(score_second,"");
			dSetTextString(score_third,"");
			g_bStart=true;
			g_iGameState=1;
			g_iSoundID = dPlaySound("bgmusic1.ogg",1,1);
		}
		//将帮助界面和关闭按钮放上去
		else if (dIsPointInSprite(help_box, fMouseX, fMouseY))
		{
			dSetSpritePosition(help_map,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2);
			dSetSpritePosition(close_box,40,30);
		}
		//点击关闭将画面清空
		if(dIsPointInSprite(close_box, fMouseX, fMouseY))
		{
			CleanSprite(help_map);
			CleanSprite(close_box);
		}
	}	
}
//这里指移出屏幕外
void CleanSprite(const char *szName)
{
	dSetSpritePosition(szName,g_fScreenLeft-400.f,g_fScreenTop-400.f);
}
void paintUI(float fTime)
{
	if(g_iGameState==0)
	{
		ReadScore();//读取分数

		dSetTextString(message_box,"");
		dSetTextString(over_box,"");
		dSetTextString(my_score,"");

		char* first=dMakeSpriteName("1st : ", rank[0]);
		char* second=dMakeSpriteName("2nd : ", rank[1]);
		char* third=dMakeSpriteName("3th : ", rank[2]);
		dSetTextString(score_first,first);
		dSetTextString(score_second,second);
		dSetTextString(score_third,third);

		//将开始游戏和帮助按钮放上去
		dSetSpritePosition(begin_box,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2+10.f);
		dSetSpritePosition(help_box,(g_fScreenLeft+g_fScreenRight)/2+36.f,(g_fScreenTop+g_fScreenBottom)/2+32.f);
		dSetSpritePosition(cover_box,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2);

		dSetSpritePosition(score_first,(g_fScreenLeft+g_fScreenRight)/2+36.f,(g_fScreenTop+g_fScreenBottom)/2+15.f);
		dSetSpritePosition(score_second,(g_fScreenLeft+g_fScreenRight)/2+36.f,(g_fScreenTop+g_fScreenBottom)/2+20.f);
		dSetSpritePosition(score_third,(g_fScreenLeft+g_fScreenRight)/2+36.f,(g_fScreenTop+g_fScreenBottom)/2+25.f);
	}
	else if(g_iGameState==2)
	{
		if(!g_bStart)
		{
			CleanSprite("TURRET");
			dSetTextString(bosshp_box,"");
			//玩家死亡
			if(playerDead)
			{
				 char *szScore=dMakeSpriteName("SCORE:", g_iScore);
				dSetTextString(myhp_box,"");
				dSetTextString(score_box,"");
				dSetTextString(level_box,"");

				dSetTextString(message_box,"GAME OVER");
				dSetTextString(over_box,"Press space to start again");
				dSetTextString(my_score,szScore);
			}
			else if(isWon)
			{
				 char *szScore=dMakeSpriteName("SCORE:", g_iScore);
				dSetTextString(myhp_box,"");
				dSetTextString(score_box,"");
				dSetTextString(level_box,"");

				dSetTextString(message_box,"CONGRATULATIONS");
				dSetTextString(over_box,"Press space to start again");
				dSetTextString(my_score,szScore);
			}
			else if(g_iLevel==2)
			{
				dSetTextString(message_box,"第二关");
				dStopSound(g_iSoundID);
				dSetSpritePosition("TURRET",(g_fScreenLeft+g_fScreenRight),(g_fScreenTop+g_fScreenBottom)/2);
				g_iSoundID = dPlaySound("bgmusic2.ogg",1,1);
				g_fSleepTime-=fTime;
				if(g_fSleepTime<0.f)
				{
					crene=true;
					dSetTextString(message_box," ");
					PaintMap(5,6,7,8);
					g_fSleepTime=3.f;
					g_bStart=true;
				}
			}
			else if(g_iLevel==3)
			{
				dSetTextString(message_box,"第三关");
				dStopSound(g_iSoundID);
				dSetSpritePosition("TURRET",(g_fScreenLeft+g_fScreenRight),(g_fScreenTop+g_fScreenBottom)/2);
				g_iSoundID = dPlaySound("bgmusic3.ogg",1,1);
				g_fSleepTime-=fTime;
				if(g_fSleepTime<0.f)
				{
					crene=true;
					dSetTextString(message_box," ");
					PaintMap(9,10,11,12);
					g_fSleepTime=3.f;
					g_bStart=true;
				}
			}
			dSetSpritePosition(message_box,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2-10.f);
			dSetSpritePosition(my_score,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2);
			dSetSpritePosition(over_box,(g_fScreenLeft+g_fScreenRight)/2,(g_fScreenTop+g_fScreenBottom)/2+20.f);
		}
		//游戏进行中
		else if(g_bStart)
		{
			char *szScore=dMakeSpriteName("score:", g_iScore);
			char *szLevel=dMakeSpriteName("level:", g_iLevel);
			char *szHp	 =dMakeSpriteName("lives:", g_pMypeople->iHp);
			char *szBshp=" ";
			char *szTime=dMakeSpriteName(" ",g_fGameTime);
			dSetTextString(score_box,szScore);
			dSetTextString(level_box,szLevel);
			dSetTextString(myhp_box,szHp);
			if(g_pBoss==NULL)
			{
				dSetTextString(bosshp_box,"");
				dSetTextString(time_box,szTime);
			}
			else
			{
				szBshp = dMakeSpriteName("lives:", g_pBoss->iHp);
				dSetTextString(bosshp_box,szBshp);
				dSetTextString(time_box,"");
			}
		}
	}
}


//特效
void AfterDead(const char* szName)
{
	static int i=0;//记录第几个死亡的
	//各个角色死亡特效模板
	char dpAlien[]="deadA";
	char dpBoss[]="deadB";
	char dpBoss2[]="deadC";
	char dpPlayer[]="deadP";
	//死亡特效
	char dead[128];
	sprintf(dead,"dead_%d",i++);
	float fPosX=dGetSpritePositionX(szName);
	float fPosY=dGetSpritePositionY(szName);
	if(strstr(szName,"alien_"))
	{
		dCloneSprite(dpAlien, dead);
		dSetSpriteLifeTime(dead,0.5);
	}
	else if(strstr(szName,"boss_2"))
	{
		dCloneSprite(dpBoss2, dead);
		dSetSpriteLifeTime(dead,1);
	}
	else if(strstr(szName,"boss_"))
	{
		dCloneSprite(dpBoss, dead);
		dSetSpriteLifeTime(dead,1);
	}
	else if(strstr(szName,"player"))
	{
		dCloneSprite(dpPlayer, dead);
		dSetSpriteLifeTime(dead,1);
	}
	dSetSpritePosition(dead,fPosX,fPosY);
	
}
//技能
void skill_1()
{
	char dpName[128]="skill1";
	char szName[128];
	Weapon* skill = (Weapon*)malloc(sizeof(Weapon));
	int numy[5]={-1,0,1,2,3};
	float numx[2]={-5.f,5.f};
	for(int j=0;j<2;j++)
		for(int i=0;i<5;i++)
		{
			sprintf(szName, "bullet_%d", g_iNum++);
			dCloneSprite(dpName, szName);
			strcpy(skill->szName, szName);
			skill->iType = BOSSBULLET;
			skill->iHp=1;
			AddToList(skill);
			dSetSpriteCollisionSend(szName, true);
			dSetSpriteCollisionReceive(szName, true);
			dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);
			dSetSpritePosition(skill->szName,numx[j],numy[i]*10.f);
			dSetSpriteLinearVelocity(skill->szName,numx[j],0.f);
		}

}

void skill_2()
{
	char dpName[128]="skill2";
	char szName[128];
	int  num[9]={-4,-3,-2,-1,0,1,2,3,4};
	Weapon* skill = (Weapon*)malloc(sizeof(Weapon));
	int ibasis = dRandomRange(1,11);
	for(int i=1;i<12;i++)
	{
		if(i!=ibasis)
		{
			sprintf(szName, "bullet_%d", g_iNum++);
			dCloneSprite(dpName, szName);
			strcpy(skill->szName, szName);
			skill->iType = BOSSBULLET;
			skill->iHp=1;
			AddToList(skill);
			dSetSpriteCollisionSend(szName, true);
			dSetSpriteCollisionReceive(szName, true);
			dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);
			dSetSpritePosition(skill->szName,num[i]*11.1f-5.55f,-30.f);
			dSetSpriteLinearVelocity(skill->szName,0.f,5.f);
		}
	}
	
}
void skill_3()
{
	char dpName[128]="skill3";
	char szName[128];
	int num[7]={-3,-2,-1,0,1,2,3};
	Weapon* skill = (Weapon*)malloc(sizeof(Weapon));
	float bpX = dGetSpritePositionX("boss2");
	float bpY = dGetSpritePositionY("boss2")+20.f;
	for(int i = 1;i<7;i++)
	{
		sprintf(szName, "bullet_%d", g_iNum++);
		dCloneSprite(dpName, szName);
		strcpy(skill->szName, szName);
		skill->iType = BOSSBULLET;
		skill->iHp=1;
		AddToList(skill);
		dSetSpriteCollisionSend(szName, true);
		dSetSpriteCollisionReceive(szName, true);
		dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);
		dSetSpritePosition(skill->szName,bpX,bpY);
		dSetSpriteLinearVelocityPolar(skill->szName,30.f,num[i]*30);
	}

}

void skill_4()
{
	char dpName[128]="skill4";
	char szName[128];
	Weapon* skill = (Weapon*)malloc(sizeof(Weapon));
	int numx[5]={-2,-1,0,1,2};
	int numy[4]={-1,0,1,2};
	for(int i=0;i<5;i++)
		for(int j=0;j<4;j++)
		{
			sprintf(szName, "bullet_%d", g_iNum++);
			dCloneSprite(dpName, szName);
			strcpy(skill->szName, szName);
			skill->iType = BOSSBULLET;
			skill->iHp=1;
			AddToList(skill);
			dSetSpriteCollisionSend(szName, true);
			dSetSpriteCollisionReceive(szName, true);
			dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, g_fScreenLeft, g_fScreenTop, g_fScreenRight, g_fScreenBottom);
			dSetSpritePosition(skill->szName,numx[i]*20.f,numy[j]*15.f);
			dSetSpriteLifeTime(skill->szName,1.5f);
		}

}

//
void  WriteScore(const int score) {
	for(int i=0;i<3;i++)
	{
		if(score>rank[i])
		{
			for(int j=i+1;j<3;j++)
				rank[j]=rank[j-1];
			rank[i]=score;
			break;
		}
	}

	FILE* file = fopen("排行榜.txt", "w");
	if(file==NULL)return;
	fwrite(rank,sizeof(int),3,file);
	fclose(file);
}

void ReadScore() {
	int s[3]={0,0,0};
	FILE* file = fopen("排行榜.txt", "r");
	if(file==NULL)return;
	fread(s,sizeof(int),3,file);
	fclose(file);
	rank[0]=s[0];
	rank[1]=s[1];
	rank[2]=s[2];
}