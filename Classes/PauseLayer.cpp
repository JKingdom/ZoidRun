//
//  PauseLayer.cpp
//
//  Created by King
//
//

#include "PauseLayer.h"
#include "AppMacros.h"
#include "Constants.h"
#include "HomeScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

PauseLayer::PauseLayer()
{
	if(initWithColor(Color4B(0, 0, 0, 100.0f / 255.0f)))
    {
        Size visibleSize =Director::getInstance()->getVisibleSize();
		Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
		Point origin = Vec2(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height* 0.5f);
        
        // BGWhite
        Sprite* bg = Sprite::create("pause_screen.png");
        bg->setPosition(origin);
        addChild(bg);
        
        float w = bg->getContentSize().width;
        float h = bg->getContentSize().height;
		Point o = Vec2(w * 0.5f, h * 0.5f);
        
        // Buttons
        
        
        // Play again button
        MenuItemImage* itemPlayAgain = MenuItemImage::create("pause_replay_off.png", "pause_replay.png", CC_CALLBACK_1(PauseLayer::_onOptionPressed, this));
        itemPlayAgain->setTag(kTagPausePlayAgain);
        itemPlayAgain->setPositionX(o.x - itemPlayAgain->getContentSize().width);
        itemPlayAgain->setPositionY(o.y - itemPlayAgain->getContentSize().height * 0.9f);
        
        MenuItemImage* itemResume = MenuItemImage::create("pause_play_off.png", "pause_play.png",CC_CALLBACK_1(PauseLayer::_onOptionPressed, this));
        itemResume->setTag(kTagPauseResumeGame);
        itemResume->setPositionX(itemPlayAgain->getPositionX() + itemResume->getContentSize().width * 1.1f);
        itemResume->setPositionY(itemPlayAgain->getPositionY());
        
        // Home button
        MenuItemImage* itemHome = MenuItemImage::create("pause_home_off.png", "pause_home.png", CC_CALLBACK_1(PauseLayer::_onOptionPressed, this));
        itemHome->setTag(kTagPauseGoHome);
        itemHome->setPositionX(itemResume->getPositionX() + itemResume->getContentSize().width * 1.1f);
        itemHome->setPositionY(itemResume->getPositionY());
        
        itemPlayAgain->setPositionY(itemPlayAgain->getPositionY() - itemPlayAgain->getContentSize().height * 0.3f);
        itemResume->setPositionY(itemResume->getPositionY() - itemResume->getContentSize().height * 0.15f);
        
        
        // Menu
        Menu* menu = Menu::create();
        menu->setAnchorPoint(Vec2(0, 0));
		menu->setPosition(Point::ZERO);
        
        menu->addChild(itemResume);
        menu->addChild(itemHome);
        menu->addChild(itemPlayAgain);
        
        bg->addChild(menu);
    }
    
}

void PauseLayer::_onOptionPressed(Ref *pSender)
{
    MenuItem* item = (MenuItem*) pSender;
    SimpleAudioEngine::getInstance()->playEffect(SFX_BUTTON);
    
    switch (item->getTag()) {
        case kTagPauseResumeGame:
			NotificationCenter::getInstance()->postNotification(NOTIFICATION_RESUME_GAME);
            break;
            
        case kTagPausePlayAgain:
			NotificationCenter::getInstance()->postNotification(NOTIFICATION_PLAY_AGAIN);
            break;
            
        case kTagPauseGoHome:
			NotificationCenter::getInstance()->postNotification(NOTIFICATION_GO_HOME);
            break;
            
    }
}

