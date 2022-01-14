#include "Game.h"
Game::Game()
{
	
	Window_width = 860;
	Window_height = 600;
	RL_ClkJudge_flag = false;
	gameLvL = 2;
	imgBGno = 1;
	imgSkinNo = 5;
	window.create(sf::VideoMode(Window_width, Window_height), L"�̲�Դ��ɨ��",sf::Style::Close|sf::Style::Titlebar);
}


Game::~Game()
{
}

void Game::Initial()
{
	window.setFramerateLimit(60);	//ÿ������Ŀ��֡��

	gridSize = GRIDSIZE;//---->�����λ�õĿ�Ĵ�С

	switch (gameLvL)
	{
	case 1:
		stageWidth = LVL1_WIDTH;
		stageHeight = LVL1_HEIGHT;
		mMineNum = LVL1_NUM;//��ʼ���׵ĸ���
		break;
	case 2:
		stageWidth = LVL2_WIDTH;
		stageHeight = LVL2_HEIGHT;
		mMineNum = LVL2_NUM;//��ʼ���׵ĸ���
		break;
	case 3:
		stageWidth = LVL3_WIDTH;
		stageHeight = LVL3_HEIGHT;
		mMineNum = LVL3_NUM;//��ʼ���׵ĸ���
		break;
	default:
		break;
	}

	gameOver = false;
	gameQuit = false;
	isGameOverState = ncNO;
	mFlagCalc = 0;//��ʼ�����ӵ�����
	isGameBegin = false;//��ʼ����Ϸ�Ƿ�ʼ
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
	IniData();//��ʼ������
	LoadMediaData();//�����ز�
}
void Game::LoadMediaData()
{
	/*	if (!font.loadFromFile("data/fonts/simsun.ttc"))//ѡ�����壬SFML����ֱ�ӷ���ϵͳ�����壬��������壬��Ҫ�Լ�����
		{
			std::cout << "����û���ҵ�" << std::endl;
		}
		text.setFont(font);*/

	std::stringstream ss;
	ss << "data/images/BK0" << imgBGno << ".jpg";//"data/images/BK01.jpg"

	if (!tBackground.loadFromFile(ss.str()))//��������ͼƬ
	{
		std::cout << "BK image û���ҵ�" << std::endl;
	}

	ss.str("");//����ַ���
	ss << "data/images/Game" << imgSkinNo << ".jpg";//"data/images/Game1.jpg"
	if (!tTiles.loadFromFile(ss.str()))
	{
		std::cout << "Game Skin image û���ҵ�" << std::endl;
	}

	if (!tNum.loadFromFile("data/images/num.jpg"))
	{
		std::cout << "num.jpg û���ҵ�" << std::endl;
	}
	if (!tTimer.loadFromFile("data/images/jishiqi.jpg"))
	{
		std::cout << "jishiqi.jpg û���ҵ�" << std::endl;
	}
	if (!tCounter.loadFromFile("data/images/jishuqi.jpg"))
	{
		std::cout << "jishuqi.jpg û���ҵ�" << std::endl;
	}
	if (!tButtons.loadFromFile("data/images/button.jpg"))
	{
		std::cout << "button.jpg û���ҵ�" << std::endl;
	}
	if (!tGameOver.loadFromFile("data/images/gameover.jpg"))
	{
		std::cout << "gameover.jpg û���ҵ�" << std::endl;
	}
	sBackground.setTexture(tBackground);					//���þ�����������
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

	for (j = 0; j < stageHeight; j++)//���п���Ϊ����δ���
		for (i = 0; i < stageWidth; i++)
		{
			mGameData[j][i].mState = ncUNDOWN;
			mGameData[j][i].isPress = false;
			mGameData[j][i].before = false;
		}
}
void Game::MineSet(int Py, int Px)//����
{
	int mCount, i, j, k, l;
	mCount = 0;

	srand(time(NULL));               //�õ�ǰϵͳʱ����Ϊ�����������������

	//�������
	do {
		k = rand() % stageHeight;//���������
		l = rand() % stageWidth;
		if (k == Py && l == Px)
			continue;//���������Ϊ��ǰ��һ�ε����λ�ã�����������

		if (mGameData[k][l].mState == ncUNDOWN)
		{
			mGameData[k][l].mState = ncMINE;
			mGameData[k][l].mStateBackUp = ncMINE;//����״̬
			mCount++;
		}
	} while (mCount != mMineNum);

	//����ֵ	
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
						}//����(i,j)��Χ�׵���Ŀ

				switch (mCount)//����״̬
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
		if (event.type == sf::Event::Resized)//����������������¼�
		{
			Choose[8] = true;
		}
	}
}

void Game::unCover()
{
	int i, j;

	for (j = 0; j < stageHeight; j++)//���п���Ϊ����δ���
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


void Game::RButtonDown(Vector2i mPoint)//------->����һ�
{
	int i, j;

	i = (mPoint.x - mCornPoint.x) / gridSize;
	j = (mPoint.y - mCornPoint.y) / gridSize;

	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)//���������ڷ�Χ��
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
void Game::LButtonDblClk(Vector2i mPoint)//------->������2��
{
	int i, j, k, l, lvl;

	i = (mPoint.x - mCornPoint.x) / gridSize;
	j = (mPoint.y - mCornPoint.y) / gridSize;

	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)//���������ڷ�Χ��
	{
		if (mGameData[j][i].isPress == true)//����ѱ����
		{
			if (mGameData[j][i].mState != ncFLAG)//�����ǰ�鲻������
				for (k = j - 1; k < j + 2; k++)
					for (l = i - 1; l < i + 2; l++)//������Χ8������
						if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
						{
							if (mGameData[k][l].mState == ncFLAG)//���״̬������
							{
								if (mGameData[k][l].mStateBackUp != ncMINE) //���ԭ��״̬������
								{
									//gameOver = true;
									//MessageBox(NULL, TEXT("Game Over!"), NULL, MB_ICONINFORMATION | MB_YESNO);
									isGameOverState = ncLOSE;
									isGameBegin = false;
									//mGameData[j][i].mState = ncBOMBING;
									unCover();
								}

							}
							else {//���״̬��������
								if (mGameData[k][l].isPress == false)
								{
									mGameData[k][l].isPress = true;
									if (mGameData[k][l].mState == ncMINE)//���Ϊ��
									{
										//gameOver = true;
										//MessageBox(NULL, TEXT("Game Over!"), NULL, MB_ICONWARNING | MB_YESNO);
										isGameBegin = false;
										isGameOverState = ncLOSE;
										mGameData[k][l].mState = ncBOMBING;
										unCover();
									}
									if (mGameData[k][l].mState == ncNULL)//���Ϊ�ռ������ҿտ�						
										NullClick(k, l);
								}
							}
						}
		}
	}
}
void Game::LButtonDown(Vector2i mPoint)//------->������1��
{
	int i, j;



	i = (mPoint.x - mCornPoint.x) / gridSize;//��ȡ��굱ǰ����Ŀ��λ��
	j = (mPoint.y - mCornPoint.y) / gridSize;

	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)//���������ڷ�Χ��
	{
		if (isGameBegin == false)//�����Ϸδ��ʼ
		{
			isGameBegin = true;//��Ϸ��ʼ
			gameClock.restart();
			LClk = true;
			MineSet(j, i);//���֮�����������
		}

		if (isGameBegin == true && RClk == true)
		{
			MineSet(j, i);
			LClk = true;
			RClk = false;

		}
		
		if (mGameData[j][i].mState != ncFLAG)//���״̬��������
			if (mGameData[j][i].isPress == false)
			{
				mGameData[j][i].isPress = true;//��ǰ�鱻���
				if (mGameData[j][i].mState == ncMINE)//���Ϊ��
				{
					//gameOver = true;
					//MessageBox(NULL, TEXT("Game Over!"), NULL, MB_ICONWARNING | MB_YESNO);
					isGameBegin = false;
					isGameOverState = ncLOSE;
					mGameData[j][i].mState = ncBOMBING;
					mGameData[j][i].mReal = ncBOMBING;
					
				}
			}


		if (mGameData[j][i].mState == ncNULL)//�����ǰ����Ŀ��״̬Ϊ��
			NullClick(j, i);//����δ������Ŀտ�
	}
}
void Game::RL_ButtonDown(Vector2i mPoint)//���Ҽ�ͬʱ���
{
	int i, j, k, l;
	i = (mPoint.x - mCornPoint.x) / gridSize;
	j = (mPoint.y - mCornPoint.y) / gridSize;
	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)
	{
		if (mGameData[j][i].isPress == true)//����ѱ����
		{
			if (mGameData[j][i].mState != ncFLAG)//�����ǰ�鲻������
			{
				for (k = j - 1; k < j + 2; k++)//����һ��8����
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
void Game::RL_ClkJudge()//���Ҽ�˫�����ж�
{
	int i, j, k, l;
	i = (RL_Point.x - mCornPoint.x) / gridSize;
	j = (RL_Point.y - mCornPoint.y) / gridSize;
	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)
	{
		if (mGameData[j][i].isPress == true)//����Ѿ��������
		{
			if (mGameData[j][i].mState != ncFLAG)//�����ǰ�鲻������
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
void Game::NullClick(int j, int i)//���ҿտ�
{
	int k, l;
	for (k = j - 1; k < j + 2; k++)
		for (l = i - 1; l < i + 2; l++)
			if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
			{
				if (mGameData[k][l].isPress == false)
				{
					mGameData[k][l].isPress = true; //��ʾΪ�ѵ��
					if (mGameData[k][l].mState == ncNULL) {//���״̬Ϊ��
						NullClick(k, l);//��������ҿտ�
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
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Mouse::isButtonPressed(sf::Mouse::Right))//���Ҽ�ͬʱ
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
				P1 = Mouse::getPosition(window);//SFML��clock�����������getElapsedTime()��restart()�����򵥣�

				if (mouseClickTimer.getElapsedTime().asMilliseconds() <= 300 && P2.x - P1.x < GRIDSIZE / 4 && P2.y - P1.y < GRIDSIZE / 4)
				{
					LButtonDblClk(P2);
				}

				//��ť�ж�
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
				if (imgBGno > 7)//����ͼ������
					imgBGno = 1;
				LoadMediaData();
			}
			if (ButtonRectSkin.contains(event.mouseButton.x, event.mouseButton.y))
			{
				imgSkinNo++;
				if (imgSkinNo > 6)//Ƥ��������
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
	if (mFlagCalc == mMineNum)//�жϲ���������ǲ��Ƕ�����
	{
		for (i = 0; i < stageWidth; i++)
			for (j = 0; j < stageHeight; j++)
			{
				if (mGameData[j][i].mState == ncFLAG)
					if (mGameData[j][i].mStateBackUp == ncMINE)
						c++;
			}
	}
	else {//�жϲ��ʣ�µĿ��ǲ��Ƕ�����
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

	if (c == mMineNum&&p==i*j)//����������Ӷ����ף���Ϸ����
	{
		
		isGameBegin = false;
		//gameOver = true;
		//MessageBox(NULL, TEXT("You Win!"), NULL, MB_USERICON | MB_YESNO);
		isGameOverState = ncWIN;
	}

}

void Game::Draw()
{
	//window.clear(Color::Color(255, 0, 255, 255));	//����
	window.clear();	//����
	//Prompt_info(width*GRIDSIZE + GRIDSIZE, GRIDSIZE);

	//���Ʊ���
	sBackground.setPosition(0, 0);
	window.draw(sBackground);
	//������̨
	DrawGrid();
	DrawButton();
	DrawTimer();
	DrawScore();
	if (isGameOverState)
		DrawGameEnd();
	window.display();				//����ʾ�����������ݣ���ʾ����Ļ�ϡ�SFML���õ���˫�������
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
	detaX = (Window_width - ButtonWidth * 7) / 8;//�ȷֿ��
	LeftCorner.y = Window_height - GRIDSIZE * 3;//ָ���߶�

	//ButtonRectEasy
	LeftCorner.x = detaX;
	sButtons.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//��ȡ��ť����������
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	���ð�ť��λ������
	ButtonRectEasy.left = LeftCorner.x;											//��¼��ť��λ������
	ButtonRectEasy.top = LeftCorner.y;
	ButtonRectEasy.width = ButtonWidth;
	ButtonRectEasy.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectNormal;
	LeftCorner.x = detaX * 2 + ButtonWidth;
	sButtons.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//��ȡ��ť����������
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	���ð�ť��λ������
	ButtonRectNormal.left = LeftCorner.x;											//��¼��ť��λ������
	ButtonRectNormal.top = LeftCorner.y;
	ButtonRectNormal.width = ButtonWidth;
	ButtonRectNormal.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectHard;
	LeftCorner.x = detaX * 3 + ButtonWidth * 2;
	sButtons.setTextureRect(IntRect(2 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//��ȡ��ť����������
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	���ð�ť��λ������
	ButtonRectHard.left = LeftCorner.x;											//��¼��ť��λ������
	ButtonRectHard.top = LeftCorner.y;
	ButtonRectHard.width = ButtonWidth;
	ButtonRectHard.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectSkin
	LeftCorner.x = detaX * 4 + ButtonWidth * 3;
	sButtons.setTextureRect(IntRect(3 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//��ȡ��ť����������
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	���ð�ť��λ������
	ButtonRectSkin.left = LeftCorner.x;											//��¼��ť��λ������
	ButtonRectSkin.top = LeftCorner.y;
	ButtonRectSkin.width = ButtonWidth;
	ButtonRectSkin.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectBG
	LeftCorner.x = detaX * 5 + ButtonWidth * 4;
	sButtons.setTextureRect(IntRect(4 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//��ȡ��ť����������
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	���ð�ť��λ������
	ButtonRectBG.left = LeftCorner.x;											//��¼��ť��λ������
	ButtonRectBG.top = LeftCorner.y;
	ButtonRectBG.width = ButtonWidth;
	ButtonRectBG.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectRestart;
	LeftCorner.x = detaX * 6 + ButtonWidth * 5;
	sButtons.setTextureRect(IntRect(5 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//��ȡ��ť����������
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	���ð�ť��λ������
	ButtonRectRestart.left = LeftCorner.x;											//��¼��ť��λ������
	ButtonRectRestart.top = LeftCorner.y;
	ButtonRectRestart.width = ButtonWidth;
	ButtonRectRestart.height = ButtonHeight;
	window.draw(sButtons);
	//ButtonRectQuit;
	LeftCorner.x = detaX * 7 + ButtonWidth * 6;
	sButtons.setTextureRect(IntRect(6 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//��ȡ��ť����������
	sButtons.setPosition(LeftCorner.x, LeftCorner.y);							//	���ð�ť��λ������
	ButtonRectQuit.left = LeftCorner.x;											//��¼��ť��λ������
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
	sCounter.setPosition(LeftCorner.x, LeftCorner.y);	//�������������ͼλ��
	window.draw(sCounter);

	int NumSize = sNum.getLocalBounds().height;
	LeftCorner.x = LeftCorner.x + sCounter.getLocalBounds().width - NumSize;
	LeftCorner.y = LeftCorner.y + sCounter.getLocalBounds().height * 0.5 - NumSize * 0.5;

	int mScore = mMineNum - mFlagCalc;
	//���Ƹ�λ��������
	if (mScore < 0)
	{
		mScore = 0;
	}
	int a = mScore % 10;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//������ȡ��������
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//�ں�λ��
	window.draw(sNum);
	//����ʮλ��������
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//������ȡ��������
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//�ں�λ��
	window.draw(sNum);
	//���ư�λ��������
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//������ȡ��������
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//�ں�λ��
	window.draw(sNum);
}
void Game::DrawTimer()
{
	Vector2i LeftCorner;
	LeftCorner.x = sTimer.getLocalBounds().width * 0.25;
	LeftCorner.y = sTimer.getLocalBounds().height * 0.5;
	sTimer.setPosition(LeftCorner.x, LeftCorner.y);	//�������������ͼλ��
	window.draw(sTimer);

	if (isGameBegin)
		mTime = gameClock.getElapsedTime().asSeconds();

	int mScore = mTime;
	if (mScore > 999)
		mScore = 999;

	int NumSize = sNum.getLocalBounds().height;
	LeftCorner.x = LeftCorner.x + sTimer.getLocalBounds().width - NumSize * 1.5;
	LeftCorner.y = LeftCorner.y + sTimer.getLocalBounds().height * 0.5 - NumSize * 0.5;

	//���Ƹ�λ��������
	int a = mScore % 10;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//������ȡ��������
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//�ں�λ��
	window.draw(sNum);
	//����ʮλ��������
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//������ȡ��������
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//�ں�λ��
	window.draw(sNum);
	//���ư�λ��������
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//������ȡ��������
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//�ں�λ��
	window.draw(sNum);

}
void Game::DrawGameEnd()
{
	Vector2i LeftCorner;
	int ButtonWidth = 200;//sGameOver.getLocalBounds().width/2;
	int ButtonHeight = sGameOver.getLocalBounds().height;
	LeftCorner.x = (Window_width - ButtonWidth) / 2;//ָ����������
	LeftCorner.y = (Window_height - ButtonHeight) / 2;//ָ����������

	sGameOver.setPosition(LeftCorner.x, LeftCorner.y);							//	���ð�ť��λ������

	if (isGameOverState == ncWIN)
	{
		sGameOver.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//��ȡ��ť����������
	}
	if (isGameOverState == ncLOSE)
	{
		sGameOver.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));//��ȡ��ť����������
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