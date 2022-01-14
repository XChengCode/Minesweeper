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
//ö�ٶ�������״̬
typedef enum GRIDSTATE {
	ncNULL,				//�յ�
	ncUNDOWN,		    //��������
	ncMINE,				//����
	ncONE,				//����1
	ncTWO,				//����2
	ncTHREE,			//����3
	ncFOUR,				//����4
	ncFIVE,				//����5
	ncSIX,				//����6
	ncSEVEN,			//����7
	ncEIGHT,			//����8
	ncFLAG,				//���
	ncQ,				//�ʺ�
	ncX,				//����
	ncBOMBING,			//��ը����
	ncUNFOUND			//δ����������
};
typedef enum GAMEOVERSTATE {
	ncNO,				//�յ�
	ncWIN,		    //��������
	ncLOSE,				//����
};
using namespace sf;			//SFML�е�ÿ���඼λ�ڸ������ռ�֮�£����趨sf�����ռ�Ļ�����Ӧ�ĺ���ǰ��Ҫ������������������� sf::VideoMode(width* GRIDSIZE, height* GRIDSIZE)

class LEI
{
public:
	int mState;//------->�׵�״̬
	int mStateBackUp;//------->����״̬
	bool isPress;//------->���Ƿ񱻰���
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
	LEI mGameData[LVL3_HEIGHT][LVL3_WIDTH];//------->20*20����
	bool isGameBegin;//------->��Ϸ�Ƿ�ʼ
	int isGameOverState;//------->��Ϸ������״̬
	Vector2i mCornPoint;//��Ϸ����λ��
	Vector2i RL_Point;
	Vector2i P1, P2;
	int Rightpressdown, Leftpressdown, LRPressed,Rightreleased;
	int gridSize;//���С��15��
	int imgBGno, imgSkinNo;
	Texture tBackground, tTiles, tButtons, tNum, tTimer, tCounter, tGameOver;		//�����������
	Sprite	sBackground, sTiles, sButtons, sNum, sTimer, sCounter, sGameOver;		//�����������
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
	void MineSet(int Py, int Px);//����

	void Input();
	void RButtonDown(Vector2i mPoint);//------->����һ�
	void LButtonDblClk(Vector2i mPoint);//------->������һ��
	void LButtonDown(Vector2i mPoint);//------->����������
	void RL_ButtonDown(Vector2i mPoint);
	void RL_ClkJudge();

	void NullClick(int j, int i);//���ҿտ�

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