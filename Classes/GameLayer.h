//
//  GameLayer.h
//
//  Created by King
//
//

#ifndef GameLayer_H
#define GameLayer_H

struct MusicPlaying
{
    const char* bg_music;
    const char* description;
};

enum GameTag
{
    kTagTutorialLayer = 100
};

enum GameMode
{
    kGameModeHome = 0,
    kGameModePlay = 1,
    kGameModeEnd = 2,
    kGameModeReplayView = 3,
    kGameModePlayAgain = 4
};

enum GameState
{
    kGameStarting = 0,
    kGamePreparing = 1,
    kGameReady = 2,
    kGamePause = 3,
    kGameFinish = 4,
    kGameTutorial = 5,
    kGameEnd = 6
};

enum GameLevel
{
    kGameLevelEasy = 0,
    kGameLevelNormal = 1,
    kGameLevelHard = 2,
    kGameLevelStory = 3,
    kGameLevelNone = 4
};

enum GameDeep
{
    kDeepSky = -9999,
    kDeepCloud = -9900,
    kDeepBGBack = -9500,
    kDeepBGMid = -9250,
    kDeepBGFront = -9000,
    kDeepTracks = -8500,
    kDeepGameElements = -2500,
    kDeepGameFinish = 500,
    kDeepScore = 1000,
    kDeepPauseLayer = 2500,
    kDeepPopUpLoseLayer = 3500,
    kDeepTutorial = 2200
};

#define kScoreFactor 100.0f

#include "cocos2d.h"

#include "BaseVehicle.h"
#include "AppMacros.h"
#include "PauseLayer.h"
#include "PopUpLoseLayer.h"
#include "BaseObstacle.h"

class GameLayer : public cocos2d::Layer
{
public:
    
    GameLayer(GameMode gameMode = kGameModeHome, GameLevel = kGameLevelNone);
    virtual ~GameLayer();
    
    void configureGame(GameLevel gameLevel = kGameLevelEasy);
    void pauseGame(Ref* pSender);
    void resumeGame(Ref* pSender);
    void playGame();
    void runGame();
    
    void update(float dt);
    

	
	// Ã¿Ö¡¸üÐÂ
    //void draw();

	// ´¥Ãþ
    //void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    bool GameTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	    
    void onEnterTransitionDidFinish();
    
private:
    void _createMap();
    void _initLayers();
    void _createPlayer();
    void _initElements();
    
    void _createObstacle(float x);
    void _createMultipleObstacles(float x, int type);
    
    void _resumeEvents();
    void _removeNode(cocos2d::Node* node);
    void _gameLogic(float dt);
    
    void _playAgain(Ref* pSender);
    
    void _goHome(Ref* pSender);
    
    void _pauseAllActions();
    void _resumeAllActions();
    
    void _checkAchievements();
    
    void _showAudioPlaying();
    
    void _selectRandomMusic();
    
    void _gameIsReady();
    void _showTutorial();
    void _finishTutorial(cocos2d::Ref* object);
    
    
private:
    BaseVehicle* _player;    
    
    
    PauseLayer* _pauseLayer;
    PopUpLoseLayer* _popUpLoseLayer;
    
    GameState _gameState;
    GameState _previousGameState;
    
    GameMode _gameMode;
    GameLevel _gameLevel;

	cocos2d::EventListenerTouchOneByOne * _touchListener;
    
    cocos2d::Sprite* _spCloud;
    cocos2d::Array* _parallaxSky;
    cocos2d::Array* _parallaxBGBack;
    cocos2d::Array* _parallaxBGMid;
    cocos2d::Array* _parallaxBGFront;
    
    cocos2d::Array* _parallaxFloor;
    
    cocos2d::Array* _arrayObstacles;
    
    cocos2d::MenuItemImage* _menuPause;
    
    bool _pause;
    bool _gameOver;
    //bool _isJoypad;
    
    float _worldSpeed;
    float _minDistanceObstaclesX;
    
    int _itemMap;
    int _obstaclesAvoided;
    int _obstaclesJumped;
    
    float _color;
    int _colorSign;
    
    float _playerStartY;
    float _wallHeight;
    
    float OBSTACLE_SIMPLE_BOT_Y;
    float OBSTACLE_SIMPLE_TOP_Y;
    float OBSTACLE_DOBLE_AIR_Y;
    float OBSTACLE_DOBLE_GROUND_Y;

	float OBSTACLE_SIMPLE_MIDDLE_Y;
    
    cocos2d::CCLabelBMFont* _lblScore;
    cocos2d::Point _accelVelocity;
    
    
};

#endif /* defined(__BTEndlessTunnel__GameLayer__) */
