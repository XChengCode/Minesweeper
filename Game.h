#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <iostream>
#include <sstream>
#define  GRIDSIZE 25
#define  LVL1_WIDTH 9
#define  LVL1_HEIGHT 9
#define  LVL1_NUM 10
#define  LVL2_WIDTH 16
#define  LVL2_HEIGHT 16
#define  LVL2_NUM 40
#define  LVL3_WIDTH 30
#define  LVL3_HEIGHT 16
#define  LVL3_NUM 99
//枚举定义网格状态
typedef enum GRIDSTATE {
	ncNULL,				//空地
	ncUNDOWN,		    //背景方块
	ncMINE,				//地雷
	ncONE,				//数字1
	ncTWO,				//数字2
	ncTHREE,			//数字3
	ncFOUR,				//数字4
	ncFIVE,				//数字5
	ncSIX,				//数字6
	ncSEVEN,			//数字7
	ncEIGHT,			//数字8
	ncFLAG,				//标记
	ncQ,				//问号
	ncX,				//备用
	ncBOMBING,			//爆炸的雷
	ncUNFOUND			//未检测出来的雷
};
typedef enum GAMEOVERSTATE {
	ncNO,				//空地
	ncWIN,		    //背景方块
	ncLOSE,				//地雷
};
using namespace sf;			//SFML中的每个类都位于该命名空间之下，不设定sf命名空间的话，相应的函数前需要用作用域解析符，例如 sf::VideoMode(width* GRIDSIZE, height* GRIDSIZE)

class LEI
{
public:
	int mState;//------->雷的状态
	int mStateBackUp;//------->备份状态
	bool isPress;//------->雷是否被按下
	bool isPressBackUp;
	bool before;
	int mReal;
};

class Game
{
public:
	sf::RenderWindow window;
	sf::Event event;
	Game();
	~Game();
	bool testMode;
	bool gameOver, gameQuit;
	bool RL_ClkJudge_flag;
	bool RClk, LClk;
	bool Choose[100];
	int mouse_RL;
	int Window_width, Window_height, stageWidth, stageHeight, mMineNum, mFlagCalc;
	int gameLvL, mTime;
	LEI mGameData[LVL3_HEIGHT][LVL3_WIDTH];//------->20*20的雷
	bool isGameBegin;//------->游戏是否开始
	int isGameOverState;//------->游戏结束的状态
	Vector2i mCornPoint;//游戏区域位置
	Vector2i RL_Point;
	Vector2i P1, P2;
	int Rightpressdown, Leftpressdown, LRPressed,Rightreleased;
	int gridSize;//块大小（15）
	int imgBGno, imgSkinNo;
	Texture tBackground, tTiles, tButtons, tNum, tTimer, tCounter, tGameOver;		//创建纹理对象
	Sprite	sBackground, sTiles, sButtons, sNum, sTimer, sCounter, sGameOver;		//创建精灵对象
	sf::IntRect ButtonRectEasy, ButtonRectNormal, ButtonRectHard, ButtonRectBG, ButtonRectSkin, ButtonRectRestart, ButtonRectQuit;
	//Font font;
	//Text text;
	SoundBuffer sbWin, sbBoom;
	Sound soundWin, soundBoom;
	Music bkMusic;
	// A Clock starts counting as soon as it's created
	sf::Clock gameClock, mouseClickTimer;
	//void Initial();
	//void Input();
	//void Logic();
	//void Draw();
	void Run();

	void Initial();
	void IniData();
	void LoadMediaData();
	void MineSet(int Py, int Px);//布雷

	void Input();
	void RButtonDown(Vector2i mPoint);//------->鼠标右击
	void LButtonDblClk(Vector2i mPoint);//------->鼠标左击一下
	void LButtonDown(Vector2i mPoint);//------->鼠标左击两下
	void RL_ButtonDown(Vector2i mPoint);
	void RL_ClkJudge();

	void NullClick(int j, int i);//查找空块

	void Logic();
	void isWin();
	void unCover();

	void Draw();
	void DrawGrid();
	void DrawButton();
	void DrawScore();
	void DrawTimer();
	void DrawGameEnd();
};