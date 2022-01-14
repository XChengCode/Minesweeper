#include "Game.h"
Game::Game()
{
	
	Window_width = 860;
	Window_height = 600;
	RL_ClkJudge_flag = false;
	gameLvL = 2;
	imgBGno = 1;
	imgSkinNo = 5;
	window.create(sf::VideoMode(Window_width, Window_height), L"程博源的扫雷",sf::Style::Close|sf::Style::Titlebar);
}


Game::~Game()
{
}

void Game::Initial()
{
	window.setFramerateLimit(60);	//每秒设置目标帧数

	gridSize = GRIDSIZE;//---->点击的位置的块的大小

	switch (gameLvL)
	{
	case 1:
		stageWidth = LVL1_WIDTH;
		stageHeight = LVL1_HEIGHT;
		mMineNum = LVL1_NUM;//初始化雷的个数
		break;
	case 2:
		stageWidth = LVL2_WIDTH;
		stageHeight = LVL2_HEIGHT;
		mMineNum = LVL2_NUM;//初始化雷的个数
		break;
	case 3:
		stageWidth = LVL3_WIDTH;
		stageHeight = LVL3_HEIGHT;
		mMineNum = LVL3_NUM;//初始化雷的个数
		break;
	default:
		break;
	}

	gameOver = false;
	gameQuit = false;
	isGameOverState = ncNO;
	mFlagCalc = 0;//初始化旗子的数量
	isGameBegin = false;//初始化游戏是否开始
	mTime = 0;
	mouse_RL = 0;
	Rightpressdown = 0;
	Leftpressdown = false;
	LRPressed = false;
	Rightreleased = 0;
	RClk = false;
	LClk = false;

	for (int i = 0;i < 99;i++)
	{
		Choose[i] = false;
	}

	mCornPoint.x = (Window_width - stageWidth * GRIDSIZE) / 2;
	mCornPoint.y = (Window_height - stageHeight * GRIDSIZE) / 2;
	IniData();//初始化数据
	LoadMediaData();//加载素材
}
void Game::LoadMediaData()
{
	/*	if (!font.loadFromFile("data/fonts/simsun.ttc"))//选择字体，SFML不能直接访问系统的字体，特殊的字体，需要自己加载
		{
			std::cout << "字体没有找到" << std::endl;
		}
		text.setFont(font);*/

	std::stringstream ss;
	ss << "data/images/BK0" << imgBGno << ".jpg";//"data/images/BK01.jpg"

	if (!tBackground.loadFromFile(ss.str()))//加载纹理图片
	{
		std::cout << "BK image 没有找到" << std::endl;
	}

	ss.str("");//清空字符串
	ss << "data/images/Game" << imgSkinNo << ".jpg";//"data/images/Game1.jpg"
	if (!tTiles.loadFromFile(ss.str()))
	{
		std::cout << "Game Skin image 没有找到" << std::endl;
	}

	if (!tNum.loadFromFile("data/images/num.jpg"))
	{
		std::cout << "num.jpg 没有找到" << std::endl;
	}
	if (!tTimer.loadFromFile("data/images/jishiqi.jpg"))
	{
		std::cout << "jishiqi.jpg 没有找到" << std::endl;
	}
	if (!tCounter.loadFromFile("data/images/jishuqi.jpg"))
	{
		std::cout << "jishuqi.jpg 没有找到" << std::endl;
	}
	if (!tButtons.loadFromFile("data/images/button.jpg"))
	{
		std::cout << "button.jpg 没有找到" << std::endl;
	}
	if (!tGameOver.loadFromFile("data/images/gameover.jpg"))
	{
		std::cout << "gameover.jpg 没有找到" << std::endl;
	}
	sBackground.setTexture(tBackground);					//设置精灵对象的纹理
	sTiles.setTexture(tTiles);
	sNum.setTexture(tNum);
	sTimer.setTexture(tTimer);
	sCounter.setTexture(tCounter);
	sButtons.setTexture(tButtons);
	sGameOver.setTexture(tGameOver);
}
void Game::IniData()
{
	int i, j;

	for (j = 0; j < stageHeight; j++)//所有块置为空且未点击
		for (i = 0; i < stageWidth; i++)
		{
			mGameData[j][i].mState = ncUNDOWN;
			mGameData[j][i].isPress = false;
			mGameData[j][i].before = false;
		}
}
void Game::MineSet(int Py, int Px)//布雷
{
	int mCount, i, j, k, l;
	mCount = 0;

	srand(time(NULL));               //用当前系统时间作为随机数生成器的种子

	//随机布雷
	do {
		k = rand() % stageHeight;//生成随机数
		l = rand() % stageWidth;
		if (k == Py && l == Px)
			continue;//如果随机左边为当前第一次点击的位置，则重新再来

		if (mGameData[k][l].mState == ncUNDOWN)
		{
			mGameData[k][l].mState = ncMINE;
			mGameData[k][l].mStateBackUp = ncMINE;//备份状态
			mCount++;
		}
	} while (mCount != mMineNum);

	//方格赋值	
	for (i = 0; i < stageHeight; i++)
		for (j = 0; j < stageWidth; j++)
		{
			if (mGameData[i][j].mState != ncMINE)
			{
				mCount = 0;
				for (k = i - 1; k < i + 2; k++)
					for (l = j - 1; l < j + 2; l++)
						if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
						{
							if (mGameData[k][l].mState == ncMINE)
								mCount++;
						}//计算(i,j)周围雷的数目

				switch (mCount)//保存状态
				{
				case 0:
					mGameData[i][j].mState = ncNULL;
					break;
				case 1:
					mGameData[i][j].mState = ncONE;
					break;
				case 2:
					mGameData[i][j].mState = ncTWO;
					break;
				case 3:
					mGameData[i][j].mState = ncTHREE;
					break;
				case 4:
					mGameData[i][j].mState = ncFOUR;
					break;
				case 5:
					mGameData[i][j].mState = ncFIVE;
					break;
				case 6:
					mGameData[i][j].mState = ncSIX;
					break;
				case 7:
					mGameData[i][j].mState = ncSEVEN;
					break;
				case 8:
					mGameData[i][j].mState = ncEIGHT;
					break;
				}


				if (mGameData[i][j].before == true)
				{
					mGameData[i][j].mStateBackUp = mGameData[i][j].mState;
					mGameData[i][j].mState = ncFLAG;
					if(i==Py&&j==Px)
					{
						mGameData[i][j].mState = mGameData[i][j].mStateBackUp;
					}
				}
				
			}
			mGameData[i][j].mReal = mGameData[i][j].mState;
		}
}

void Game::Input()
{
	
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			Choose[0] = true;
		}
		if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Escape)
		{
			Choose[1] = true;
		}
		if (event.type == sf::Event::MouseButtonPressed && RL_ClkJudge_flag == false)
		{
			Choose[2] = true;
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			Choose[3] = true;
		}
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			Choose[4] = true;
		}
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			Choose[5] = true;
		}
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
		{
			Choose[6] = true;
		}
		if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::T)
		{
			Choose[7] = true;
		}
		if (event.type == sf::Event::Resized)//如果发生拉动缩放事件
		{
			Choose[8] = true;
		}
	}
}

void Game::unCover()
{
	int i, j;

	for (j = 0; j < stageHeight; j++)//所有块置为空且未点击
		for (i = 0; i < stageWidth; i++)
		{
			if (mGameData[j][i].isPress == false)
			{
				if (mGameData[j][i].mState == ncMINE)
				{
					mGameData[j][i].isPress = true;
					mGameData[j][i].mState = ncUNFOUND;
					mGameData[j][i].mReal = ncUNFOUND;
				}
				mGameData[j][i].isPress = true;
			}
		}
}


void Game::RButtonDown(Vector2i mPoint)//------->鼠标右击
{
	int i, j;

	i = (mPoint.x - mCornPoint.x) / gridSize;
	j = (mPoint.y - mCornPoint.y) / gridSize;

	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)//如果点击是在范围内
	{
		if (isGameBegin == false)
		{
			isGameBegin = true;
			gameClock.restart();
			RClk = true;
			mGameData[j][i].before = true;
			//MineSet(j, i);
		}

		if (isGameBegin == true && LClk == false)
		{
			mGameData[j][i].before = true;
		}
		
		if (mGameData[j][i].isPress == false)
		{
			mGameData[j][i].isPress = true;
			mGameData[j][i].mStateBackUp = mGameData[j][i].mState;
			mGameData[j][i].mState = ncFLAG;
			mFlagCalc++;
		}
		else
		{
			if (mGameData[j][i].mState == ncFLAG)
			{
				mGameData[j][i].isPress = true;
				mGameData[j][i].before = false;
				mGameData[j][i].mState = ncQ;
				mFlagCalc--;
			}
			else if (mGameData[j][i].mState == ncQ)
			{
				mGameData[j][i].isPress = false;
				mGameData[j][i].mState = mGameData[j][i].mStateBackUp;
			}
		}
	}
}
void Game::LButtonDblClk(Vector2i mPoint)//------->鼠标左击2下
{
	int i, j, k, l, lvl;

	i = (mPoint.x - mCornPoint.x) / gridSize;
	j = (mPoint.y - mCornPoint.y) / gridSize;

	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)//如果点击是在范围内
	{
		if (mGameData[j][i].isPress == true)//如果已被点击
		{
			if (mGameData[j][i].mState != ncFLAG)//如果当前块不是旗子
				for (k = j - 1; k < j + 2; k++)
					for (l = i - 1; l < i + 2; l++)//遍历周围8个格子
						if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
						{
							if (mGameData[k][l].mState == ncFLAG)//如果状态是旗子
							{
								if (mGameData[k][l].mStateBackUp != ncMINE) //如果原先状态不是雷
								{
									//gameOver = true;
									//MessageBox(NULL, TEXT("Game Over!"), NULL, MB_ICONINFORMATION | MB_YESNO);
									isGameOverState = ncLOSE;
									isGameBegin = false;
									//mGameData[j][i].mState = ncBOMBING;
									unCover();
								}

							}
							else {//如果状态不是旗子
								if (mGameData[k][l].isPress == false)
								{
									mGameData[k][l].isPress = true;
									if (mGameData[k][l].mState == ncMINE)//如果为雷
									{
										//gameOver = true;
										//MessageBox(NULL, TEXT("Game Over!"), NULL, MB_ICONWARNING | MB_YESNO);
										isGameBegin = false;
										isGameOverState = ncLOSE;
										mGameData[k][l].mState = ncBOMBING;
										unCover();
									}
									if (mGameData[k][l].mState == ncNULL)//如果为空继续查找空块						
										NullClick(k, l);
								}
							}
						}
		}
	}
}
void Game::LButtonDown(Vector2i mPoint)//------->鼠标左击1下
{
	int i, j;



	i = (mPoint.x - mCornPoint.x) / gridSize;//获取鼠标当前点击的块的位置
	j = (mPoint.y - mCornPoint.y) / gridSize;

	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)//如果点击是在范围内
	{
		if (isGameBegin == false)//如果游戏未开始
		{
			isGameBegin = true;//游戏开始
			gameClock.restart();
			LClk = true;
			MineSet(j, i);//点击之后再随机布雷
		}

		if (isGameBegin == true && RClk == true)
		{
			MineSet(j, i);
			LClk = true;
			RClk = false;

		}
		
		if (mGameData[j][i].mState != ncFLAG)//如果状态不是旗子
			if (mGameData[j][i].isPress == false)
			{
				mGameData[j][i].isPress = true;//当前块被点击
				if (mGameData[j][i].mState == ncMINE)//如果为雷
				{
					//gameOver = true;
					//MessageBox(NULL, TEXT("Game Over!"), NULL, MB_ICONWARNING | MB_YESNO);
					isGameBegin = false;
					isGameOverState = ncLOSE;
					mGameData[j][i].mState = ncBOMBING;
					mGameData[j][i].mReal = ncBOMBING;
					
				}
			}


		if (mGameData[j][i].mState == ncNULL)//如果当前点击的块的状态为无
			NullClick(j, i);//查找未被点击的空块
	}
}
void Game::RL_ButtonDown(Vector2i mPoint)//左右键同时点击
{
	int i, j, k, l;
	i = (mPoint.x - mCornPoint.x) / gridSize;
	j = (mPoint.y - mCornPoint.y) / gridSize;
	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)
	{
		if (mGameData[j][i].isPress == true)//如果已被点击
		{
			if (mGameData[j][i].mState != ncFLAG)//如果当前块不是棋子
			{
				for (k = j - 1; k < j + 2; k++)//遍历一下8格子
				{
					for (l = i - 1; l < i + 2; l++)
					{
						if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
						{
							if (mGameData[k][l].isPress == false)
							{
								mGameData[k][l].isPress = true;
								mGameData[k][l].mStateBackUp = mGameData[k][l].mState;
								mGameData[k][l].mState = ncX;
							}
						}
					}
				}
			}
		}
	}
	RL_Point = mPoint;
	RL_ClkJudge_flag = true;
}
void Game::RL_ClkJudge()//左右键双击的判定
{
	int i, j, k, l;
	i = (RL_Point.x - mCornPoint.x) / gridSize;
	j = (RL_Point.y - mCornPoint.y) / gridSize;
	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)
	{
		if (mGameData[j][i].isPress == true)//如果已经被点击了
		{
			if (mGameData[j][i].mState != ncFLAG)//如果当前块不是旗子
			{
				for (k = j - 1; k < j + 2; k++)
				{
					for (l = i - 1; l < i + 2; l++)
					{

						if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
						{
							if (mGameData[k][l].mState == ncX)
							{
								mGameData[k][l].isPress = false;
								mGameData[k][l].mState = mGameData[k][l].mStateBackUp;
							}

						}
					}
				}
			}
		}
	
	}
	RL_ClkJudge_flag = false;
}
void Game::NullClick(int j, int i)//查找空块
{
	int k, l;
	for (k = j - 1; k < j + 2; k++)
		for (l = i - 1; l < i + 2; l++)
			if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
			{
				if (mGameData[k][l].isPress == false)
				{
					mGameData[k][l].isPress = true; //显示为已点击
					if (mGameData[k][l].mState == ncNULL) {//如果状态为空
						NullClick(k, l);//则继续查找空块
					}
				}
			}
}

void Game::Logic()
{
	
	for (int i = 0;i < 100;i++)
	{
		if (Choose[0])
		{
			window.close();
			gameQuit = true;
			Choose[0] = false;
		}
		if (Choose[1])
		{
			window.close();
			gameQuit = true;
			Choose[1] = false;
		}
		if (Choose[2])
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Mouse::isButtonPressed(sf::Mouse::Right))//左右键同时
			{
				RL_ButtonDown(Mouse::getPosition(window));
			}
			Choose[2] = false;
		}
		if (Choose[3])
		{
			if (RL_ClkJudge_flag == true)
			{
				RL_ClkJudge();

				LButtonDblClk(Mouse::getPosition(window));

			}
			Choose[3] = false;
		}
		if (Choose[4])
		{
			if (isGameOverState == ncNO)
			{
				if (mouseClickTimer.getElapsedTime().asMilliseconds() <= 300)
				{

					//LButtonDblClk(Mouse::getPosition(window));
					mouseClickTimer.restart();
					P2 = Mouse::getPosition(window);
				}
				else
				{
					LButtonDown(Mouse::getPosition(window));
				}
			}
			Choose[4] = false;
		}
		if (Choose[5])
		{
			if (isGameOverState == ncNO)
			{
				mouseClickTimer.restart();
				P1 = Mouse::getPosition(window);//SFML的clock类就两个函数getElapsedTime()和restart()，超简单！

				if (mouseClickTimer.getElapsedTime().asMilliseconds() <= 300 && P2.x - P1.x < GRIDSIZE / 4 && P2.y - P1.y < GRIDSIZE / 4)
				{
					LButtonDblClk(P2);
				}

				//按钮判断
				if (isGameBegin == false)
				{
					if (ButtonRectEasy.contains(event.mouseButton.x, event.mouseButton.y))
					{
						gameLvL = 1;
						Initial();
					}
					if (ButtonRectNormal.contains(event.mouseButton.x, event.mouseButton.y))
					{
						gameLvL = 2;
						Initial();
					}
					if (ButtonRectHard.contains(event.mouseButton.x, event.mouseButton.y))
					{
						gameLvL = 3;
						Initial();
					}

				}

			}
			if (ButtonRectBG.contains(event.mouseButton.x, event.mouseButton.y))
			{
				imgBGno++;
				if (imgBGno > 7)//背景图的数量
					imgBGno = 1;
				LoadMediaData();
			}
			if (ButtonRectSkin.contains(event.mouseButton.x, event.mouseButton.y))
			{
				imgSkinNo++;
				if (imgSkinNo > 6)//皮肤的数量
					imgSkinNo = 1;
				LoadMediaData();
			}
			if (ButtonRectRestart.contains(event.mouseButton.x, event.mouseButton.y))
			{
				Initial();
			}
			if (ButtonRectQuit.contains(event.mouseButton.x, event.mouseButton.y))
			{
				window.close();
				gameOver = false;
				gameQuit = true;
			}
			Choose[5] = false;
		}
		if (Choose[6])
		{
			if (isGameOverState == ncNO)
			{
				RButtonDown(Mouse::getPosition(window));
			}
			Choose[6] = false;
		}
		if (Choose[7])
		{
			if (testMode == false)
			{
				testMode = true;
				for (int i = 0;i < stageHeight;i++)
				{
					for (int j = 0;j < stageWidth;j++)
					{
						mGameData[i][j].isPressBackUp = mGameData[i][j].isPress;
						mGameData[i][j].isPress = true;
					}
				}
			}
			else
			{
				testMode = false;
				for (int i = 0;i < stageHeight;i++)
				{
					for (int j = 0;j < stageWidth;j++)
					{
						mGameData[i][j].isPressBackUp = mGameData[i][j].isPress;
						mGameData[i][j].isPress = false;
					}
				}
			}
			Choose[7] = false;
		}
		if (Choose[8])
		{
			Vector2u size;
			size.x = Window_width;
			size.y = Window_height;
			window.setSize(size);
			Choose[8] = false;
		}
	}
	isWin();
	if (isGameOverState == ncLOSE)
	{
		unCover();
	}
}
void Game::isWin()
{
	int i, j, c = 0,p=0;
	if (mFlagCalc == mMineNum)//判断插的所有旗是不是都是雷
	{
		for (i = 0; i < stageWidth; i++)
			for (j = 0; j < stageHeight; j++)
			{
				if (mGameData[j][i].mState == ncFLAG)
					if (mGameData[j][i].mStateBackUp == ncMINE)
						c++;
			}
	}
	else {//判断插的剩下的块是不是都是雷
		for (i = 0; i < stageWidth; i++)
			for (j = 0; j < stageHeight; j++)
			{
				if (mGameData[j][i].isPress == false || mGameData[j][i].mState == ncFLAG) {
					c++;
				}
			}

	}

	for (i = 0; i < stageWidth; i++)
		for (j = 0; j < stageHeight; j++)
		{
			if (mGameData[j][i].isPress == true)
			{
					p++;
				
			}
		}

	if (c == mMineNum&&p==i*j)//如果所有旗子都是雷，游戏结束
	{
		
		isGameBegin = false;
		//gameOver = true;
		//MessageBox(NULL, TEXT("You Win!"), NULL, MB_USERICON | MB_YESNO);
		isGameOverState = ncWIN;
	}

}

void Game::Draw()
{
	//window.clear(Color::Color(255, 0, 255, 255));	//清屏
	window.clear();	//清屏
	//Prompt_info(width*GRIDSIZE + GRIDSIZE, GRIDSIZE);

	//绘制背景
	sBackground.setPosition(0, 0);
	window.draw(sBackground);
	//绘制舞台
	DrawGrid();
	DrawButton();
	DrawTimer();
	DrawScore();
	if (isGameOverState)
		DrawGameEnd();
	window.display();				//把显示缓冲区的内容，显示在屏幕上。SFML采用的是双缓冲机制
}
void Game::DrawGrid()
{
	for (int j = 0; j < stageHeight; j++)
		for (int i = 0; i < stageWidth; i++)
		{
			if (mGameData[j][i].isPress == true)
			{
				if (isGameOverState == ncNO)
				{
					sTiles.setTextureRect(IntRect(mGameData[j][i].mState * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
					sTiles.setPosition(mCornPoint.x + i * GRIDSIZE, mCornPoint.y + j * GRIDSIZE);
				}

				if (isGameOverState == ncLOSE)
				{
					sTiles.setTextureRect(IntRect(mGameData[j][i].mReal * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
					sTiles.setPosition(mCornPoint.x + i * GRIDSIZE, mCornPoint.y + j * GRIDSIZE);
				}

				if (isGameOverState == ncWIN)
				{
					sTiles.setTextureRect(IntRect(mGameData[j][i].mState * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
					sTiles.setPosition(mCornPoint.x + i * GRIDSIZE, mCornPoint.y + j * GRIDSIZE);
				}
				
			} 
			else
			{
				if (mGameData[j][i].isPress == true)
				{
					sTiles.setTextureRect(IntRect(mGameData[j][i].mState * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
					sTiles.setPosition(mCornPoint.x + i * GRIDSIZE, mCornPoint.y + j * GRIDSIZE);
				}
				else
				{
					sTiles.setTextureRect(IntRect(ncUNDOWN * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
					sTiles.setPosition(mCornPoint.x + i * GRIDSIZE, mCornPoint.y + j * GRIDSIZE);
				}
			}
			window.draw(sTiles);
		}
}
void Game::DrawButton()
{
	Vector2i LeftCorner;
	int ButtonWidth = 60;
	int ButtonHeight = 36;
	int detaX;
	detaX = (Window_width - ButtonWidth * 7) / 8;//等分宽度
	LeftCorner.y = Window_height - GRIDSIZE * 3;//指定高度

	//ButtonRectEasy
	LeftCorner.x = detaX;
	sButtons.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	设置按钮的位置坐标
	ButtonRectEasy.left = LeftCorner.x;											//记录按钮的位置区域
	ButtonRectEasy.top = LeftCorner.y;
	ButtonRectEasy.width = ButtonWidth;
	ButtonRectEasy.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectNormal;
	LeftCorner.x = detaX * 2 + ButtonWidth;
	sButtons.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	设置按钮的位置坐标
	ButtonRectNormal.left = LeftCorner.x;											//记录按钮的位置区域
	ButtonRectNormal.top = LeftCorner.y;
	ButtonRectNormal.width = ButtonWidth;
	ButtonRectNormal.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectHard;
	LeftCorner.x = detaX * 3 + ButtonWidth * 2;
	sButtons.setTextureRect(IntRect(2 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	设置按钮的位置坐标
	ButtonRectHard.left = LeftCorner.x;											//记录按钮的位置区域
	ButtonRectHard.top = LeftCorner.y;
	ButtonRectHard.width = ButtonWidth;
	ButtonRectHard.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectSkin
	LeftCorner.x = detaX * 4 + ButtonWidth * 3;
	sButtons.setTextureRect(IntRect(3 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	设置按钮的位置坐标
	ButtonRectSkin.left = LeftCorner.x;											//记录按钮的位置区域
	ButtonRectSkin.top = LeftCorner.y;
	ButtonRectSkin.width = ButtonWidth;
	ButtonRectSkin.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectBG
	LeftCorner.x = detaX * 5 + ButtonWidth * 4;
	sButtons.setTextureRect(IntRect(4 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	设置按钮的位置坐标
	ButtonRectBG.left = LeftCorner.x;											//记录按钮的位置区域
	ButtonRectBG.top = LeftCorner.y;
	ButtonRectBG.width = ButtonWidth;
	ButtonRectBG.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectRestart;
	LeftCorner.x = detaX * 6 + ButtonWidth * 5;
	sButtons.setTextureRect(IntRect(5 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	设置按钮的位置坐标
	ButtonRectRestart.left = LeftCorner.x;											//记录按钮的位置区域
	ButtonRectRestart.top = LeftCorner.y;
	ButtonRectRestart.width = ButtonWidth;
	ButtonRectRestart.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectQuit;
	LeftCorner.x = detaX * 7 + ButtonWidth * 6;
	sButtons.setTextureRect(IntRect(6 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	设置按钮的位置坐标
	ButtonRectQuit.left = LeftCorner.x;											//记录按钮的位置区域
	ButtonRectQuit.top = LeftCorner.y;
	ButtonRectQuit.width = ButtonWidth;
	ButtonRectQuit.height = ButtonHeight;
	window.draw(sButtons);
}
void Game::DrawScore()
{
	Vector2i LeftCorner;
	LeftCorner.x = Window_width - sCounter.getLocalBounds().width * 1.25;
	LeftCorner.y = sCounter.getLocalBounds().height * 0.5;
	sCounter.setPosition(LeftCorner.x, LeftCorner.y);	//计数器纹理的贴图位置
	window.draw(sCounter);

	int NumSize = sNum.getLocalBounds().height;
	LeftCorner.x = LeftCorner.x + sCounter.getLocalBounds().width - NumSize;
	LeftCorner.y = LeftCorner.y + sCounter.getLocalBounds().height * 0.5 - NumSize * 0.5;

	int mScore = mMineNum - mFlagCalc;
	//绘制个位数的数字
	if (mScore < 0)
	{
		mScore = 0;
	}
	int a = mScore % 10;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//纹理上取数字纹理
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//摆好位置
	window.draw(sNum);
	//绘制十位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//纹理上取数字纹理
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//摆好位置
	window.draw(sNum);
	//绘制百位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//纹理上取数字纹理
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//摆好位置
	window.draw(sNum);
}
void Game::DrawTimer()
{
	Vector2i LeftCorner;
	LeftCorner.x = sTimer.getLocalBounds().width * 0.25;
	LeftCorner.y = sTimer.getLocalBounds().height * 0.5;
	sTimer.setPosition(LeftCorner.x, LeftCorner.y);	//计数器纹理的贴图位置
	window.draw(sTimer);

	if (isGameBegin)
		mTime = gameClock.getElapsedTime().asSeconds();

	int mScore = mTime;
	if (mScore > 999)
		mScore = 999;

	int NumSize = sNum.getLocalBounds().height;
	LeftCorner.x = LeftCorner.x + sTimer.getLocalBounds().width - NumSize * 1.5;
	LeftCorner.y = LeftCorner.y + sTimer.getLocalBounds().height * 0.5 - NumSize * 0.5;

	//绘制个位数的数字
	int a = mScore % 10;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//纹理上取数字纹理
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//摆好位置
	window.draw(sNum);
	//绘制十位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//纹理上取数字纹理
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//摆好位置
	window.draw(sNum);
	//绘制百位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//纹理上取数字纹理
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//摆好位置
	window.draw(sNum);

}
void Game::DrawGameEnd()
{
	Vector2i LeftCorner;
	int ButtonWidth = 200;//sGameOver.getLocalBounds().width/2;
	int ButtonHeight = sGameOver.getLocalBounds().height;
	LeftCorner.x = (Window_width - ButtonWidth) / 2;//指定顶点坐标
	LeftCorner.y = (Window_height - ButtonHeight) / 2;//指定顶点坐标

	sGameOver.setPosition(LeftCorner.x, LeftCorner.y);							//	设置按钮的位置坐标

	if (isGameOverState == ncWIN)
	{
		sGameOver.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	}
	if (isGameOverState == ncLOSE)
	{
		sGameOver.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//读取按钮的纹理区域
	}
	window.draw(sGameOver);
}
void Game::Run()
{
	do {

		Initial();

		while (window.isOpen() && gameOver == false)
		{
			Input();

			Logic();

			Draw();
		}

	} while (!gameQuit);
}