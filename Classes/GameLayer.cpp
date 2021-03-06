//
//  GameLayer.cpp
//
//  Created by King
//

#include "GameLayer.h"

#include "ObstacleSimple.h"
#include "ObstacleDoble.h"
#include "ObstacleDobleAir.h"

#include "VehicleFrog.h"
#include "Constants.h"

#include "SimpleAudioEngine.h"

#include "HomeScene.h"

#include "LocalStorageManager.h"
#include "PlayGameConstants.h"

#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>


#define SP_PISTA "pista.png"
#define SP_CIELO "newbg.png"
#define SP_NUBE "nube.png"
#define SP_BG_BACK "flower.png"
#define SP_BG_MID "bubble.png"

#define DT_SPEED_PISTA 1.0f
#define DT_SPEED_OBSTACULOS (DT_SPEED_PISTA * 1.0f)
#define DT_SPEED_NUBE (DT_SPEED_PISTA * 0.2f)
#define DT_SPEED_BG_BACK (DT_SPEED_PISTA * 0.5f)
#define DT_SPEED_BG_MID (DT_SPEED_PISTA * 1.0f)
#define DT_SPEED_BG_FRONT (DT_SPEED_PISTA * 1.3f)
#define MIN_COLOR 100
#define MAX_COLOR 255

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

static int counter_ads = 0;

// 10up 11middle 12down 2col 6col43
int easyMap[] = {
    10,10,10,12,12,12,11,11,11,10,12,10,12,10,10,11,11,12,2,
    2,10,12,12,11,6,10,6,11,11,12,2,2,10,11,12,11,12,12,
    2,11,11,11,12,12,10,2,12,12,2,2,10,10,11,11,6,12,11,
    6,11,11,2,10,10,10,10,12,12,12,12,6,11,11,11,11,10,12
};


static int _currentMusic = 0;

vector<int> _vectorMap;
vector<MusicPlaying> _vectorMusics;
MusicPlaying _music;

// End Level definition

GameLayer::GameLayer(GameMode gameMode, GameLevel gameLevel) : _gameMode(gameMode)
{
	_touchListener = nullptr;

    srand(time(0));
    
    // Preload obstacles
    BaseObstacle* _tmp;
    
    _tmp = new ObstacleSimple();
    _tmp->release();
    
    _tmp = new ObstacleDoble();
    _tmp->release();
    
    _tmp = new ObstacleDobleAir();
    _tmp->release();
    
    // End obstacles
    _color = 255;
    _colorSign = -1;
    
    _selectRandomMusic();
    
    _player = NULL;
    _obstaclesJumped = 0;
    _obstaclesAvoided = 0;


    NotificationCenter* notificationCenter = NotificationCenter::getInstance();
    notificationCenter->addObserver(this, callfuncO_selector(GameLayer::pauseGame), NOTIFICATION_PAUSE_GAME, NULL);
    
    notificationCenter->addObserver(this, callfuncO_selector(GameLayer::resumeGame), NOTIFICATION_RESUME_GAME, NULL);
    
    notificationCenter->addObserver(this, callfuncO_selector(GameLayer::_playAgain), NOTIFICATION_PLAY_AGAIN, NULL);
    
    notificationCenter->addObserver(this, callfuncO_selector(GameLayer::_goHome), NOTIFICATION_GO_HOME, NULL);
    
    _createMap();
    _initLayers();
    _gameLevel = gameLevel;
    
}


void GameLayer::onEnterTransitionDidFinish()
{
    if(_gameMode == kGameModePlay || _gameMode == kGameModePlayAgain || _gameMode == kGameModeReplayView)
    {
        configureGame(_gameLevel);
        runGame();
    }
}
// music
void GameLayer::_selectRandomMusic()
{
    MusicPlaying mp;
    
    mp.bg_music = BG_MUSIC_01;
    mp.description = " ";//"BT Turbo Tunnel - VGMusic.com";
    _vectorMusics.push_back(mp);
    /*
    mp.bg_music = BG_MUSIC_02;
    mp.description = "Music by Diego Rodriguez";
    _vectorMusics.push_back(mp);
    
    mp.bg_music = BG_MUSIC_03;
    mp.description = "Turtle Blues - PlayOnLoop.com";
    _vectorMusics.push_back(mp);
    
    _music = _vectorMusics[_currentMusic];
    
    _currentMusic++;
    if(_currentMusic > (int)_vectorMusics.size() - 1)
        _currentMusic = 0;
    */
}

void GameLayer::_showAudioPlaying()
{
    Point origin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Size winSize = Director::getInstance()->getWinSize();
    
    LabelTTF* lblMusic = LabelTTF::create(
		CCString::createWithFormat("Playing %s", _music.description)->getCString(), FONT_GAME, 35.0f, 
		Size(winSize.width * 0.85f, winSize.height * 0.15f), TextHAlignment::RIGHT, TextVAlignment::TOP);
    
    lblMusic->setPositionX(origin.x + visibleSize.width * 0.55f);
    lblMusic->setPositionY(origin.y - visibleSize.height * 0.25f);
    
	MoveTo* ac1 = MoveTo::create(1.0f, Vec2(lblMusic->getPositionX(), origin.y + visibleSize.height * 0.05f));
    
    DelayTime* ac2 = DelayTime::create(2.1f);
    
    FadeOut* ac3 = FadeOut::create(0.9f);
    
    CallFuncN* ac4 = CallFuncN::create(this, callfuncN_selector(GameLayer::_removeNode));
    
    addChild(lblMusic, kDeepGameFinish);
    
    lblMusic->runAction(Sequence::create(ac1, ac2, ac3, ac4, NULL));
    
	SimpleAudioEngine::getInstance()->playBackgroundMusic(_music.bg_music, true);
    
}

GameLayer::~GameLayer()
{
    
    unscheduleAllSelectors();
    
	NotificationCenter * notificationCenter = NotificationCenter::getInstance();
    notificationCenter->removeObserver(this, NOTIFICATION_PAUSE_GAME);
    notificationCenter->removeObserver(this, NOTIFICATION_RESUME_GAME);
    notificationCenter->removeObserver(this, NOTIFICATION_PLAY_AGAIN);
    notificationCenter->removeObserver(this, NOTIFICATION_GO_HOME);

    CC_SAFE_RELEASE(_parallaxSky);
    CC_SAFE_RELEASE(_parallaxBGBack);
    CC_SAFE_RELEASE(_parallaxBGMid);

    CC_SAFE_RELEASE(_parallaxFloor);
	if(_arrayObstacles != nullptr)
		CC_SAFE_RELEASE(_arrayObstacles);
}

void GameLayer::_createMap()
{
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    int i = 0;
    float x = 0;
    float yPos = visibleOrigin.y * 0.5f;
    Sprite* _tmpSprite;
    
    
    // Creamos el parallax para la pista
    _parallaxFloor = Array::createWithCapacity(MAX_PARALLAX);
    _parallaxFloor->retain();

    Sprite* spFloor = Sprite::create(SP_PISTA);
	//Sprite* spFloor = Sprite::create("pista1.png");
    _playerStartY = spFloor->getContentSize().height * 0.55f + yPos;
	log("yPos = %f", yPos);
	log("spFloor.height = %f", spFloor->getContentSize().height);
	log("playerStartY = %f", _playerStartY);

	// 精灵的活动上下范围的前提条件
    _wallHeight = spFloor->getContentSize().height * 0.25f;
    log("wallHeight = %f", _wallHeight);
    // Creamos el cielo
    _parallaxSky = Array::createWithCapacity(2);
    _parallaxSky->retain();
    
    for(i = 0; i < 2; i++)
    {
        _tmpSprite = Sprite::create(SP_CIELO);
        _tmpSprite->setAnchorPoint(Point::ZERO);
        _tmpSprite->setPosition(Vec2(x, WIN_SIZE.height - _tmpSprite->getContentSize().height));
        addChild(_tmpSprite, kDeepSky);
        x += _tmpSprite->getContentSize().width;
        _parallaxSky->addObject(_tmpSprite);
    }
    
    // Creamos la nube    
    _spCloud = Sprite::create(SP_NUBE);
    _spCloud->setPosition(Vec2(WIN_SIZE.width + _spCloud->getContentSize().width * 0.7f, visibleSize.height - _spCloud->getContentSize().height * 0.9f + visibleOrigin.y));
    addChild(_spCloud, kDeepCloud);
    
    // Creamos el BG que esta mas atras
    x = 0;
    _parallaxBGBack = Array::createWithCapacity(MAX_PARALLAX);
    _parallaxBGBack->retain();
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        _tmpSprite = Sprite::create(SP_BG_BACK);
        _tmpSprite->setAnchorPoint(Point::ZERO);
        _tmpSprite->setPosition(Vec2(x, _playerStartY + _wallHeight * 1.9f));
        addChild(_tmpSprite, kDeepBGBack);
        _parallaxBGBack->addObject(_tmpSprite);
        x += _tmpSprite->getContentSize().width;
    }
    
    // Creamos el BG que esta al medio
    x = 0;
    _parallaxBGMid = Array::createWithCapacity(MAX_PARALLAX);
    _parallaxBGMid->retain();
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        _tmpSprite = Sprite::create(SP_BG_MID);
        _tmpSprite->setAnchorPoint(Point::ZERO);
        _tmpSprite->setPosition(Vec2(x, _playerStartY + _wallHeight * 1.0f));
        addChild(_tmpSprite, kDeepBGMid);
        _parallaxBGMid->addObject(_tmpSprite);
        x += _tmpSprite->getContentSize().width;
    }
    
    // Pista
    x = 0;
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        spFloor = Sprite::create(SP_PISTA);
        spFloor->setAnchorPoint(Vec2(0, 0));
        spFloor->setPosition(Vec2(x, yPos));
        _parallaxFloor->addObject(spFloor);
        addChild(spFloor, kDeepTracks);
        x += spFloor->getContentSize().width;
    }
    
	// init var
	OBSTACLE_SIMPLE_BOT_Y = 0;
	OBSTACLE_DOBLE_GROUND_Y = 10;
	OBSTACLE_SIMPLE_TOP_Y = _wallHeight * 4 - 200;
    OBSTACLE_DOBLE_AIR_Y = _playerStartY + _wallHeight * 1.8f;

	OBSTACLE_SIMPLE_MIDDLE_Y = 150;
    
}

void GameLayer::configureGame(GameLevel gameLevel)
{
    
    _gameLevel = gameLevel;
    
    _accelVelocity = Point::ZERO;
    
    Point visOrigin = Director::getInstance()->getVisibleOrigin();
    Size visSize = Director::getInstance()->getVisibleSize();
    
    _menuPause = MenuItemImage::create("pause_off.png", "pause.png", CC_CALLBACK_1(GameLayer::pauseGame, this));
    _menuPause->setVisible(false);
    _menuPause->setPositionX(visOrigin.x + _menuPause->getContentSize().width * 0.9f);
    _menuPause->setPositionY(visOrigin.y + visSize.height - _menuPause->getContentSize().width * 0.6f);
    
    Menu* menu = Menu::create();
    menu->setAnchorPoint(Vec2(0, 0));
    menu->setPosition(Point::ZERO);
    menu->addChild(_menuPause);
    addChild(menu, kDeepPauseLayer);
    
    //setTouchEnabled(true);
	_eventDispatcher->resumeEventListenersForTarget(this);
    
	_worldSpeed = START_WORLD_SPEED;
    _minDistanceObstaclesX = 300;//*= 2.0f; // 1.0
    _worldSpeed *= 1.0f; // 0.9
    _vectorMap.insert(_vectorMap.begin(), easyMap, easyMap + sizeof(easyMap) / sizeof(int));
    
    _itemMap = 0;
    
    _gameState = kGameStarting;
    
    _pause = false;
    _gameOver = false;
    
    _createPlayer();
    _initElements();
    
}

void GameLayer::_initLayers()
{
    
    Point origin = Director::getInstance()->getVisibleOrigin();
    Size size = Director::getInstance()->getVisibleSize();
        
    _lblScore = LabelBMFont::create("0", FONT_GAME_BMP, SIZE_SCORE_GAME, kCCTextAlignmentRight);
    _lblScore->setAnchorPoint(Vec2(0, -0.5f));
    _lblScore->setVisible(false);
    _lblScore->setColor(Color3B::WHITE);
    _lblScore->setPosition(Vec2(origin.x + size.width * 0.9f, origin.y + size.height * 0.82f));
    addChild(_lblScore, kDeepScore);
    
    _pauseLayer = new PauseLayer();
    _pauseLayer->setVisible(false);
    _pauseLayer->setPosition(Vec2(0, -WIN_SIZE.height));
    _pauseLayer->setPositionY(0);
    _pauseLayer->autorelease();
    addChild(_pauseLayer, kDeepPauseLayer);
    
    _popUpLoseLayer = new PopUpLoseLayer();
    _popUpLoseLayer->setPosition(Vec2(0, -WIN_SIZE.height));
    // _popUpLoseLayer->setPositionY(0);
    _popUpLoseLayer->autorelease();
    addChild(_popUpLoseLayer, kDeepPopUpLoseLayer);
    
}

void GameLayer::_createPlayer()
{
    _player = new VehicleFrog();
	
    //_player->setLimits(_playerStartY - _wallHeight * 0.1f, _wallHeight * 0.9f);
	_player->setLimits(0, 250);
	log("limits: %f , %f", _playerStartY - _wallHeight * 0.1f, _wallHeight * 0.9f);
    _player->setPositionY(_playerStartY + _wallHeight * 0.5f);
	log("player Y = %f", _playerStartY + _wallHeight * 0.5f);
    _player->setPositionX(-_player->getContentSize().width * 2.5f);
    _player->autorelease();
    addChild(_player);
}

void GameLayer::_initElements()
{
    
    int i = 0;
    float x;
        
    // Obstacles
    _arrayObstacles = Array::create();
    _arrayObstacles->retain();
    
    x = START_X_OBSTACLES;
    
    for(i = 0; i < MAX_OBSTACLES; i++)
    {
        _createObstacle(x);
        
        BaseObstacle* lastObstacle = (BaseObstacle*) _arrayObstacles->lastObject();
        x = lastObstacle->getPositionX() + _minDistanceObstaclesX;
    }

}

void GameLayer::_createObstacle(float x)
{
    //BaseObstacle* obstacle = NULL;
	Sprite* obstacle = NULL;
    float y;
    int z = 0;
    
    int type = _vectorMap[_itemMap];
	//10 11 12 2 6
	//int type = 12;
    
    if(type == 0)
    {
        obstacle = new ObstacleSimple();
        y = OBSTACLE_SIMPLE_BOT_Y;
        z = OBSTACLE_SIMPLE_BOT_Y;
    }
    else if(type == 1)
    {
        obstacle = new ObstacleSimple();
        y = OBSTACLE_SIMPLE_TOP_Y;
        z = OBSTACLE_SIMPLE_TOP_Y;
    }
    else if(type == 2)
    {
        obstacle = new ObstacleDoble();
		obstacle->setAnchorPoint(Point::ZERO);
        y = OBSTACLE_DOBLE_GROUND_Y;
        z = WIN_SIZE.height * 0.5f;
    }
    else if(type == 3)
    {
        obstacle = new ObstacleDobleAir();
        y = OBSTACLE_DOBLE_AIR_Y;
    }
    else
    {
        _createMultipleObstacles(x, type);
    }
    
    if(obstacle != NULL && type <= 3)
    {
        obstacle->setPosition(Vec2(x, y));
        obstacle->autorelease();
        _arrayObstacles->addObject(obstacle);
        addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
    }


    _itemMap++;
    if(_itemMap > (int)_vectorMap.size() - 1)
        _itemMap = 0;
}

void GameLayer::_createMultipleObstacles(float x, int type)
{
    float y;
    int z = 0, i = 0;
    BaseObstacle* obstacle;
    
    float distance = 0;
    float dt_distance = 2.0f;
    
    if(type == 4)
    {
        // Crear 2 obstaculos simples abajo
        y = OBSTACLE_SIMPLE_BOT_Y;
        z = OBSTACLE_SIMPLE_BOT_Y;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new ObstacleSimple();
            distance = obstacle->getContentSize().width * dt_distance * 1.5f;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
        
    }
    else if(type == 5)
    {
        // Crear 2 obstaculos simples arriba
        y = OBSTACLE_SIMPLE_TOP_Y;
        z = OBSTACLE_SIMPLE_TOP_Y;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new ObstacleSimple();
            distance = obstacle->getContentSize().width * dt_distance * 1.5f;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
        
    }
    else if(type == 6)
    {
        // Crear 3 obstaculos dobles en tierra
        y = OBSTACLE_DOBLE_GROUND_Y;
        z = WIN_SIZE.height * 0.5f;
        
        for(i = 0; i < 3; i++)
        {
            obstacle = new ObstacleDoble();
			obstacle->setAnchorPoint(Point::ZERO);
            distance = obstacle->getContentSize().width * dt_distance;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(3);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
        
    }
    else if(type == 7)
    {
        // Crear 3 obstaculos dobles en el aire
        y = OBSTACLE_DOBLE_AIR_Y;
        
        for(i = 0; i < 3; i++)
        {
            obstacle = new ObstacleDobleAir();
            distance = obstacle->getContentSize().width * dt_distance;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(3);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
    }
    else if(type == 8)
    {
        // Crear 2 obstaculos dobles en tierra
        y = OBSTACLE_DOBLE_GROUND_Y;
        z = WIN_SIZE.height * 0.5f;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new ObstacleDoble();
            distance = obstacle->getContentSize().width * dt_distance;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
        
    }
    else if(type == 9)
    {
        // Crear 2 obstaculos dobles en el aire
        y = OBSTACLE_DOBLE_AIR_Y;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new ObstacleDobleAir();
            distance = obstacle->getContentSize().width * dt_distance;
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(Vec2(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += distance;
        }
    }
	// 2015 / 6 / 14  find the low fix method
	else if(type == 10)
    {
        // Crear 2 obstaculos simples arriba
        y = OBSTACLE_SIMPLE_TOP_Y;
        z = OBSTACLE_SIMPLE_TOP_Y;

		float currentX = x;
		float currentY = y;
        
        for(i = 0; i < 4; i++)
        {
            obstacle = new ObstacleSimple();
            distance = obstacle->getContentSize().width * dt_distance * 1.5f;


            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(1);
                //obstacle->setDistanceObjects(distance);
            }
            
            obstacle->setTag((i - 1) * -1);
			currentX += 43.25;
			currentY += 50;
			obstacle->setPosition(Vec2(currentX, currentY));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            //
        }
        
    }
	else if(type == 11)
    {
		y = OBSTACLE_SIMPLE_MIDDLE_Y;
        z = OBSTACLE_SIMPLE_MIDDLE_Y;

		float currentX = x;
		float currentY = y;
		for(i = 0 ; i < 2; i++)
		{
			obstacle = new ObstacleSimpleRight();
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
            }
            
            obstacle->setTag((i - 1) * -1);
			currentX += 43.25;
			currentY += 50;
			obstacle->setPosition(Vec2(currentX, currentY));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
		}
    }
	else if(type == 12)
    {
        // Crear 2 obstaculos simples arriba
        y = OBSTACLE_SIMPLE_BOT_Y;
        z = OBSTACLE_SIMPLE_BOT_Y;

		float currentX = x;
		float currentY = y;
        
        for(i = 0; i < 3; i++)
        {
            obstacle = new ObstacleSimple();
            distance = obstacle->getContentSize().width * dt_distance * 1.5f;


            obstacle->setNumObjects(0);
            if(i == 0)
            {
				currentX += obstacle->getContentSize().width/2;
				currentY += obstacle->getContentSize().height/2;
                obstacle->setNumObjects(1);
                //obstacle->setDistanceObjects(distance);
			}else
			{
				currentX += 43.25;
				currentY += 50;
			}
            
            obstacle->setTag((i - 1) * -1);
			
			obstacle->setPosition(Vec2(currentX, currentY));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            //x += distance;
        }
        x += distance;
    }

}


void GameLayer::playGame()
{
    _gameMode = kGameModePlay;
    runGame();
}

void GameLayer::runGame()
{    
	// set update
    unscheduleUpdate();
    this->scheduleUpdate();


	// set touch listener
	if(_touchListener != nullptr)
	{
		_eventDispatcher->removeEventListener(_touchListener);
		_touchListener = nullptr;
	}
	_touchListener = EventListenerTouchOneByOne::create();
	_touchListener->setSwallowTouches(true);
	_touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::GameTouchesBegan,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

void GameLayer::pauseGame(Ref* pSender)
{
    if(_gameState == kGameFinish)
        return;
    
    if(_gameMode == kGameModePlay || _gameMode == kGameModePlayAgain || _gameMode == kGameModeReplayView)
    {
        if(!_pause)
        {
			SimpleAudioEngine::getInstance()->playEffect(SFX_BUTTON);
            
            _pause = true;
            _previousGameState = _gameState;
            _gameState = kGamePause;
            
			pause();
            _pauseAllActions();
            
            _menuPause->setVisible(false);
            _pauseLayer->setVisible(true);     
        }
    }
}

void GameLayer::resumeGame(Ref* pSender)
{
    if(_gameState == kGameFinish)
        return;
    
    if(_gameMode == kGameModePlay || _gameMode == kGameModePlayAgain || _gameMode == kGameModeReplayView)
    {
        if(_gameState == kGamePause)
        {
            
            _gameState = _previousGameState;
            _pauseLayer->setVisible(false);
            _menuPause->setVisible(true);
            _resumeEvents();
        }
    }
}

void GameLayer::_resumeEvents()
{
	resume();
    _resumeAllActions();
    _pause = false;
}


void GameLayer::_removeNode(Node *node)
{
    node->removeFromParentAndCleanup(false);
}


void GameLayer::_gameLogic(float dt)
{
   
	_player->doMove(Point::ZERO);
    
    // Increment map speed
    _worldSpeed += dt * 1;
    
    _lblScore->setString(String::createWithFormat("%d", _obstaclesAvoided)->getCString());
    
    int z = (WIN_SIZE.height - (_player->getPlayerY() + _player->getContentSize().height * 0.75f)) + kDeepGameElements;
    
    this->reorderChild(_player, z);
    
    Ref* object;
    Sprite* sprite;
    BaseObstacle* obstacle;
    float spriteWidth;
    
    // Move cloud
    if(_spCloud->getPositionX() <= -_spCloud->getContentSize().width * 0.5f)
        _spCloud->setPositionX(WIN_SIZE.width + _spCloud->getContentSize().width * 0.7f);
    _spCloud->setPositionX(_spCloud->getPositionX() - _worldSpeed * dt * DT_SPEED_NUBE);
    
    Color3B color((int)_color, (int)_color, (int)_color);

	((Sprite*) _parallaxSky->getObjectAtIndex(0))->setColor(color);
    ((Sprite*) _parallaxSky->getObjectAtIndex(1))->setColor(color);
    _spCloud->setColor(color);

    CCARRAY_FOREACH(_parallaxBGBack, object)
    {
        sprite = (Sprite*) object;
        spriteWidth = sprite->getContentSize().width;
        
        if(sprite->getPositionX() <= -spriteWidth)
        {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxBGBack->count() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt * DT_SPEED_BG_BACK);
        sprite->setColor(color);
    }

    // Bucle for _parallaxBGMid
    CCARRAY_FOREACH(_parallaxBGMid, object)
    {
        sprite = (Sprite*) object;
        spriteWidth = sprite->getContentSize().width;
        
        if(sprite->getPositionX() <= -spriteWidth)
        {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxBGMid->count() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt * DT_SPEED_BG_MID);
        sprite->setColor(color);
    }

    _color += dt * _colorSign * 3;
    
    if(_color < MIN_COLOR && _colorSign == -1)
        _colorSign = 1;
    else if(_color > MAX_COLOR && _colorSign == 1)
        _colorSign = -1;
    
    if(_color > MAX_COLOR)
        _color = MAX_COLOR;
    
    if(_color < MIN_COLOR)
        _color = MIN_COLOR;
    
    // Bucle for _parallaxFloor
    CCARRAY_FOREACH(_parallaxFloor, object)
    {
        sprite = (Sprite*) object;
        spriteWidth = sprite->getContentSize().width;
        
        if(sprite->getPositionX() <= -spriteWidth)
        {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxFloor->count() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt * DT_SPEED_PISTA);
        
    }
    
    Array* _removeObstacles = Array::create();
    
    // Move obstacles, detect collisions, play sfx and remove
    CCARRAY_FOREACH(_arrayObstacles, object)
    {
        obstacle = (BaseObstacle*) object;
        float positionX = obstacle->getPositionX();
     
        obstacle->doUpdate(positionX, _worldSpeed * dt * DT_SPEED_OBSTACULOS);
        
        if(obstacle->getPositionX() < -obstacle->getContentSize().width * 0.5f)
        {
            _removeObstacles->addObject(obstacle);
        }
        else
        {
            if(obstacle->getPositionX() < WIN_SIZE.width && obstacle->getPositionX() > 0 && obstacle->collision(*_player))
            {

				log("collision!");
                
                _lblScore->setVisible(false);
                _menuPause->setVisible(false);
                _player->dead();
                this->reorderChild(_player, kDeepGameFinish);
                _gameOver = true;
                _gameState = kGameFinish;
                break;
            }
            else
            {
                if(!obstacle->getPassPlayerSFX() && obstacle->getPositionX() + obstacle->getContentSize().width * 1.0f < _player->getPositionX())
                {
                    obstacle->setPassPlayerSFX(true);
                    if(obstacle->getObstacType() == kJumpObstacle)
                    {
                        _obstaclesJumped++;
                    }
                    _obstaclesAvoided++;
					SimpleAudioEngine::getInstance()->playEffect(SFX_SWOOSH);
                }
                
            }
        }
        
    }
    
    // Remove and Add objects
    if(_removeObstacles->count() > 0)
    {
        CCARRAY_FOREACH(_removeObstacles, object)
        {
			BaseObstacle* lastObstacle = (BaseObstacle*) _arrayObstacles->lastObject();
            
            obstacle = (BaseObstacle*) object;
            int currentTag = obstacle->getTag();
            _arrayObstacles->removeObject(obstacle);
            obstacle->removeFromParentAndCleanup(false);
            
            if(currentTag == 1)
            {
                float x = lastObstacle->getPositionX() + _minDistanceObstaclesX;
                _createObstacle(x);
            }
            
        }
    }

}


bool GameLayer::GameTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    if(!_pause && !_gameOver)
    {
        if(_gameState == kGameReady)
        {
            Point location = touch->getLocationInView();
            
            if(location.x <= WIN_SIZE.width * 0.5f)
                _player->doJump();
			else
			{
				if(_player->getIsLeft())
				{
					_player->setIsLeft(false);
				}else 
				{
					_player->setIsLeft(true);
				}
			}

			return true;
        }
    }
	return false;
}

void GameLayer::update(float dt)
{
    if(_pause)
        return;
    
    if(_gameState == kGameStarting)
    {
        
        _gameState = kGamePreparing;
		// init position
		_player->runAction(MoveTo::create(1.5f, Vec2(250, _player->getPositionY())));
    }
    
    else if(_gameState == kGamePreparing)
    {
        
        if(_player->getNumberOfRunningActions() <= 1)
        {
            
            _lblScore->setVisible(true);
            
            if(LocalStorageManager::showTutorial())
                _showTutorial();
            else
                _gameIsReady();
        }
        
    }
    else if(_gameState == kGameReady)
    {
        if(!_gameOver)
        {
            _gameLogic(dt);
        }
        
    }
    else if(_gameOver && _gameState == kGameFinish)
    {
        if(_player->getNumberOfRunningActions() == 0)
        {
            _gameState = kGameEnd;
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            
            _checkAchievements();
            
			// set touch enabled
			_eventDispatcher->pauseEventListenersForTarget(this);

	
            _menuPause->setVisible(false);
            _popUpLoseLayer->updateScore(_gameLevel, _obstaclesAvoided * kScoreFactor, _obstaclesAvoided);
            _popUpLoseLayer->runAction(MoveBy::create(0.25f, Vec2(0, WIN_SIZE.height)));
            unscheduleUpdate();
        }
    }

}


void GameLayer::_gameIsReady()
{
    _gameState = kGameReady;
	_eventDispatcher->resumeEventListenersForTarget(this);
    _menuPause->setVisible(true);
    

    if(!LocalStorageManager::isMute())
        _showAudioPlaying();
}

void GameLayer::_showTutorial()
{
    
    unscheduleUpdate();
    _pauseAllActions();
    
    const char* tap_to_jump = "Tap to Jump";
    const char* tilt_move = "Tap to Change \n Direction";
    const char* tap_continue = "Tap here to continue";
    const char* avoid_obstacles = "Avoid the obstacles!";

    _gameState = kGameTutorial;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    Point center = Point::ZERO;
    center.x = visibleOrigin.x + visibleSize.width * 0.5f;
    center.y = visibleOrigin.y + visibleSize.height * 0.5f;
    
	LayerColor* layer = LayerColor::create(Color4B(0, 0, 0, 0.2f));
    layer->setTag(kTagTutorialLayer);


	// right
	Sprite* spriteChange = Sprite::create("tap.png");
	spriteChange->setPositionX(visibleSize.width - spriteChange->getContentSize().width/2);
    spriteChange->setPositionY(visibleOrigin.y + spriteChange->getContentSize().height * 0.4f);
    layer->addChild(spriteChange);
    spriteChange->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5f, Vec2(0, 9)), MoveBy::create(0.5f, Vec2(0, -9)), NULL)));

    // change Tutorial
    LabelTTF* lblChange = LabelTTF::create(tilt_move, FONT_GAME, SIZE_TUT_INST);
	lblChange->setColor(Color3B::BLACK);
    lblChange->setPosition(center);
    lblChange->setPositionY(spriteChange->getPositionY() + spriteChange->getContentSize().height * 0.6f);
	lblChange->setPositionX(spriteChange->getPositionX() - lblChange->getContentSize().width/2);
    layer->addChild(lblChange);

    // left
	Sprite* spriteJump = Sprite::create("tap.png");
	spriteJump->setFlippedX(true);
    spriteJump->setPositionX(spriteJump->getContentSize().width/2);
    spriteJump->setPositionY(visibleOrigin.y + spriteJump->getContentSize().height * 0.4f);
    layer->addChild(spriteJump);
    spriteJump->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5f, Vec2(0, -9)), MoveBy::create(0.5f, Vec2(0, 9)), NULL)));

    // Jump Tutorial
    LabelTTF* lblJump = LabelTTF::create(tap_to_jump, FONT_GAME, SIZE_TUT_INST);
	lblJump->setColor(Color3B::BLACK);
    lblJump->setPosition(center);
    lblJump->setPositionY(spriteJump->getPositionY() + spriteJump->getContentSize().height * 0.6f);
	lblJump->setPositionX(spriteJump->getPositionX() + lblJump->getContentSize().width/2);
    layer->addChild(lblJump);
    
    LabelTTF* lblCloseTutorial = LabelTTF::create(tap_continue, FONT_GAME, SIZE_TUT_TITLE);
	lblCloseTutorial->setColor(Color3B::BLACK);
    MenuItemLabel* menuCloseTutorial = MenuItemLabel::create(lblCloseTutorial, CC_CALLBACK_1(GameLayer::_finishTutorial, this));
    menuCloseTutorial->setPosition(center);
    menuCloseTutorial->setPositionY(visibleOrigin.y + visibleSize.height - menuCloseTutorial->getContentSize().height * 1.9f);
    
    // Avoid the obstacles
    LabelTTF* lblAvoid = LabelTTF::create(avoid_obstacles, FONT_GAME, SIZE_TUT_INST, Size(visibleSize.width * 0.8f, visibleSize.height), TextHAlignment::CENTER, TextVAlignment::CENTER);
	lblAvoid->setColor(Color3B::BLACK);
    lblAvoid->setPosition(center);
    lblAvoid->setPositionY(visibleOrigin.y +  menuCloseTutorial->getContentSize().height * 1.2f);
    layer->addChild(lblAvoid);
    
    Menu* menu = Menu::create(menuCloseTutorial, NULL);
    menu->setPosition(Point::ZERO);
    layer->addChild(menu);
    
    addChild(layer, kDeepTutorial);
    
    menuCloseTutorial->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5f, -2), RotateTo::create(0.5f, 2), NULL)));
}

void GameLayer::_finishTutorial(cocos2d::Ref *object)
{
    
    LocalStorageManager::isTutorialOn(false);
    
    MenuItem* item = (MenuItem *) object;
    item->setEnabled(false);
    
    
    LayerColor* layer = (LayerColor *) getChildByTag(kTagTutorialLayer);
    layer->removeFromParent();
    
    scheduleUpdate();
    _resumeAllActions();
    _gameIsReady();
    // pauseGame();
}

void GameLayer::_checkAchievements()
{
    
    long longScore = (long) (_obstaclesAvoided * kScoreFactor);
    
    // Obstacles avoidment
    if(_gameLevel == kGameLevelEasy && _obstaclesAvoided >= 100)
    {
        
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_100_OBSTACLES_IN_EASY_MODE))
        {
            LocalStorageManager::unlockAchievement(ACH_AVOID_100_OBSTACLES_IN_EASY_MODE);
        }
        
    }
    else if(_gameLevel == kGameLevelNormal && _obstaclesAvoided >= 50)
    {
        
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_50_OBSTACLES_IN_NORMAL_MODE))
        {
            LocalStorageManager::unlockAchievement(ACH_AVOID_50_OBSTACLES_IN_NORMAL_MODE);
        }
        
    }
    else if(_gameLevel == kGameLevelHard && _obstaclesAvoided >= 25)
    {
        
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_25_OBSTACLES_IN_HARD_MODE))
        {
            LocalStorageManager::unlockAchievement(ACH_AVOID_25_OBSTACLES_IN_HARD_MODE);
        }
        
    }
    
    if(_gameLevel == kGameLevelHard && _obstaclesAvoided >= 100)
    {
        
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_100_OBSTACLES_IN_HARD_MODE))
        {
            LocalStorageManager::unlockAchievement(ACH_AVOID_100_OBSTACLES_IN_HARD_MODE);
        }
        
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_MORE_THAN_3000) && longScore > 3000)
    {
        LocalStorageManager::unlockAchievement(ACH_MORE_THAN_3000);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_10000_OR_MORE_IN_EASY_MODE) && _gameLevel == kGameLevelEasy && longScore >= 10000)
    {
        LocalStorageManager::unlockAchievement(ACH_GET_10000_OR_MORE_IN_EASY_MODE);
        
    }

    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_8000_OR_MORE_IN_NORMAL_MODE) && _gameLevel == kGameLevelNormal && longScore >= 8000)
    {
        LocalStorageManager::unlockAchievement(ACH_GET_8000_OR_MORE_IN_NORMAL_MODE);
        
    }

    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_5000_OR_MORE_IN_HARD_MODE) && _gameLevel == kGameLevelHard && longScore >= 5000)
    {
        LocalStorageManager::unlockAchievement(ACH_GET_5000_OR_MORE_IN_HARD_MODE);
    }

    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_30K_IN_EASY_MODE) && _gameLevel == kGameLevelEasy && longScore >= 30000)
    {
        LocalStorageManager::unlockAchievement(ACH_GET_30K_IN_EASY_MODE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_15K_IN_NORMAL_MODE) && _gameLevel == kGameLevelNormal && longScore >= 15000)
    {
        LocalStorageManager::unlockAchievement(ACH_GET_15K_IN_NORMAL_MODE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_10K_IN_HARD_MODE) && _gameLevel == kGameLevelHard && longScore >= 10000)
    {
        LocalStorageManager::unlockAchievement(ACH_GET_10K_IN_HARD_MODE);
    }
    
    if(_obstaclesJumped > 0)
    {
        int totalObstaclesJumped = LocalStorageManager::getObstaclesJumped() + _obstaclesJumped;
        LocalStorageManager::updateObstaclesJumped(totalObstaclesJumped);
    }
    
    _obstaclesJumped = 0;
    
}

void GameLayer::_playAgain(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Scene* scene = HomeScene::scene(kGameModePlayAgain, _gameLevel);
	Director::getInstance()->replaceScene(TransitionFadeDown::create(0.5f, scene));
}

void GameLayer::_goHome(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Scene* scene = HomeScene::scene(kGameModeHome, kGameLevelNone, true);
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

void GameLayer::_pauseAllActions()
{
	for (auto sprite : this->getChildren()) 
	{
		if(sprite != nullptr)
			//sprite->pauseSchedulerAndActions();
			sprite->pause();
	}
}

void GameLayer::_resumeAllActions()
{
	for (auto sprite : this->getChildren()) 
	{
		if(sprite != nullptr)
			//sprite->resumeSchedulerAndActions();
			sprite->resume();
	}
}

