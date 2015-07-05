//
//  HomeLayer.cpp
//
//  Created by King
//
//

#include "HomeLayer.h"
#include "Constants.h"
#include "SimpleAudioEngine.h"
#include "PlayGameConstants.h"
#include "LocalStorageManager.h"

#define HIDE_TIME 0.4f

using namespace cocos2d;
using namespace CocosDenshion;

static int counter_ads = 0;

void HomeLayer::onEnterTransitionDidFinish()
{
    if(_showAds)
    {
        counter_ads++;
        
        if(counter_ads >= COUNT_ADS)
            counter_ads = 0;
    }
}

HomeLayer::HomeLayer(GameLayer* gameLayer, bool showAds) : _gameLayer(gameLayer)
{    
    _showAds = false;
    _showAds = showAds;
    
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(HomeLayer::_enableButtons), NOTIFICATION_ENABLE_BUTTONS, NULL);
    
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(HomeLayer::_manageHowToPlay), NOTIFICATION_HOW_TO_PLAY, NULL);
    
    disable = false;
        
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    
    tabLeft = Sprite::create("tablero_title.png");
    tabLeft->setAnchorPoint(Point::ZERO);
    tabLeft->setPositionX(visibleOrigin.x);
    tabLeft->setPositionY(visibleOrigin.y);
    addChild(tabLeft);
    

	tabRight = Sprite::create("tablero_title.png");
	tabRight->setAnchorPoint(Vec2(0.5,0.5));
	tabRight->setRotation(180);
	tabRight->setPosition(Vec2(visibleSize.width - tabRight->getContentSize().width / 2, tabRight->getContentSize().height / 2));
	addChild(tabRight);

	// face
	face = MenuItemImage::create("face_begin.png", "face_begin_selected.png",CC_CALLBACK_1(HomeLayer::_onOptionPressed, this));
	face->setAnchorPoint(Vec2(0.5, 0.5));
	face->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	face->setTag(kFaceEasyMode);
	ScaleTo* scaleAction1 = ScaleTo::create(1.0f ,1.5f);
	ScaleTo* scaleAction2 = ScaleTo::create(1.0f, 1.0f);
	face->runAction(RepeatForever::create(Sequence::create(scaleAction1, scaleAction2, nullptr)));
	//addChild(face);


	float scale = 1.3f;
    float time_dt = 1.3f;

	// logo
	logoFont = Sprite::create("logo_font.png");
	logoFont->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4 - 30));
	logoFont->setScale(0.0f);
	//logoFont->runAction(Sequence::create(ScaleTo::create(0.5f,1.0),
	//	RepeatForever::create(Sequence::create(DelayTime::create(2 * time_dt), ScaleTo::create(0.5f * time_dt, scale), ScaleTo::create(0.5f * time_dt, 1.0f), NULL)),nullptr));
	logoFont->runAction(ScaleTo::create(0.5f,1.0));
	addChild(logoFont);

	// bubble
	bubble = Sprite::create("bubble_begin.png");
	bubble->setAnchorPoint(Point::ZERO);
	bubble->setPosition(0,0);
	addChild(bubble);

	// flower
	flower1 = Sprite::create("flower1.png");
	flower2 = Sprite::create("flower2.png");
	RepeatForever* repeatAction  = RepeatForever::create(RotateBy::create(time_dt, 10));
	flower1->runAction(repeatAction);
	flower2->runAction(repeatAction->clone());
	flower1->setPosition(visibleSize.width - 200, visibleSize.height - 125);
	flower2->setPosition(visibleSize.width - 75, visibleSize.height - 250);
	addChild(flower1);
	addChild(flower2);
   
    // How to Player
	LabelTTF* howToPlay = LabelTTF::create("How to Play", FONT_GAME, SIZE_RATE_APP);
	howToPlay->setColor(Color3B::WHITE);
    
	menuHowToPlay = MenuItemLabel::create(howToPlay, CC_CALLBACK_1(HomeLayer::_onOptionPressed, this));
    menuHowToPlay->setTag(kTagHowToPlay);
	menuHowToPlay->setPositionX(80 + menuHowToPlay->getContentSize().width/2);
	menuHowToPlay->setPositionY(50 + menuHowToPlay->getContentSize().height/2);
    menuHowToPlay->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5f, -2), RotateTo::create(0.5f, 2), NULL)));
    
    if(LocalStorageManager::showTutorial())
        menuHowToPlay->setVisible(false);
    
    // Sound management
    MenuItemImage* menuSoundOn = MenuItemImage::create("sound_on_off.png", "sound_on.png", NULL, NULL);
    MenuItemImage* menuSoundOff = MenuItemImage::create("sound_off_off.png", "sound_off.png", NULL, NULL);
    
    //menuSound = MenuItemToggle::createWithTarget(this, menu_selector(HomeLayer::_manageMusic), menuSoundOn, menuSoundOff, NULL);
	menuSound = MenuItemToggle::createWithCallback(CC_CALLBACK_1(HomeLayer::_manageMusic, this), menuSoundOn, menuSoundOff, NULL);
	menuSound->setPosition(visibleSize.width - 75, 75);
    
    if(LocalStorageManager::isMute())
        menuSound->setSelectedIndex(1);
    
    // Menu
    menu = Menu::create();
	menu->setPosition(Point::ZERO);

	menu->addChild(face);
    menu->addChild(menuSound);
    menu->addChild(menuHowToPlay);
    
    addChild(menu);

}

void HomeLayer::_manageMusic(cocos2d::Ref* pSender)
{
	if(disable )
        return;
    
	SimpleAudioEngine::getInstance()->playEffect(SFX_BUTTON);
        
    bool state = LocalStorageManager::isMute();
    LocalStorageManager::setMute(!state);
}

HomeLayer::~HomeLayer()
{
    NotificationCenter::getInstance()->removeObserver(this, NOTIFICATION_ENABLE_BUTTONS);
    NotificationCenter::getInstance()->removeObserver(this, NOTIFICATION_HOW_TO_PLAY);
}

void HomeLayer::_onOptionPressed(Ref *pSender)
{
	if(disable)
        return;
    
    SimpleAudioEngine::getInstance()->playEffect(SFX_BUTTON);
    
    MenuItem* item = (MenuItem*) pSender;
    bool runGame = false;
    
    switch (item->getTag()) {
            
        case kTagHowToPlay:
            LocalStorageManager::isTutorialOn(true);
            runGame = true;
            _gameLayer->configureGame(kGameLevelEasy);
            break;
       
        case kFaceEasyMode:
            runGame = true;
            _gameLayer->configureGame(kGameLevelEasy);
            break;

        default:
            break;
    }
    
    if(runGame)
    {
        _disableButtons();
        disable = true;
        _hideToLeft();
        _hideToRight();
		face->stopAllActions();
		ScaleTo *action = ScaleTo::create(HIDE_TIME,0.01f);
		logoFont->runAction(action);
		face->runAction(Sequence::create(action->clone(),[this](){this->removeChild(face);},nullptr));
        scheduleOnce(schedule_selector(HomeLayer::_finishHideLayer), HIDE_TIME + 0.1f);
    }
    
}

void HomeLayer::_hideToLeft()
{
    MoveBy* move = MoveBy::create(HIDE_TIME, Vec2(-WIN_SIZE.width * 0.8f, 0));
	tabLeft->runAction(move);
	bubble->runAction(move->clone());
	menuHowToPlay->runAction(move->clone());
}

void HomeLayer::_hideToRight()
{
    MoveBy* move = MoveBy::create(HIDE_TIME, Vec2(WIN_SIZE.width * 0.8f, 0));
    tabRight->runAction(move);
    menuSound->runAction(move->clone());
	flower1->runAction(move->clone());
	flower2->runAction(move->clone());
}

void HomeLayer::_finishHideLayer(float dt)
{
    this->setVisible(false);
    _gameLayer->playGame();
    this->removeFromParent();    
}

void HomeLayer::_enableButtons(Ref* pSender)
{
    
	for (auto nod : menu->getChildren()) {
		MenuItem* item = (MenuItem*) nod;
		item->setEnabled(true);
	}
}

void HomeLayer::_disableButtons()
{
    
	for (auto nod : menu->getChildren()) {
		MenuItem* item = (MenuItem*) nod;
		item->setEnabled(false);
	}
}

void HomeLayer::_manageHowToPlay(Ref* pSender)
{
    bool state = !LocalStorageManager::showTutorial();
    menuHowToPlay->setVisible(state);
}
