//
//  PopUpLoseLayer.cpp
//  Created by King
//
//

#include "PopUpLoseLayer.h"
#include "Constants.h"
#include "HomeScene.h"
#include "AppMacros.h"
#include "LocalStorageManager.h"
#include "PlayGameConstants.h"
#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

PopUpLoseLayer::PopUpLoseLayer()
{
    
	if(initWithColor(Color4B(0, 0, 0, 100.0f / 255.0f)))
    {
        _leaderboardID = "";
        
        _score = 0;
        disable = false;
        
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
		Point origin = Vec2(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height* 0.5f);
        
        // BGWhite
        Sprite* bg = Sprite::create("gameover_screen.png");
        bg->setPosition(origin);
        addChild(bg);
        
        float w = bg->getContentSize().width;
        float h = bg->getContentSize().height;
		Point o = Vec2(w * 0.5f, h * 0.5f);
        
        // Badge and record
		spBadge = Sprite::create("dead.png");
		spBadge->setPosition(Vec2(o.x, o.y + spBadge->getContentSize().height * 0.5f + 30));
        bg->addChild(spBadge, 10);
	
		_lblScore = LabelTTF::create("", FONT_GAME, SIZE_RATE_END, Size(w * 0.5f, h * 0.15f),TextHAlignment::LEFT, TextVAlignment::CENTER);
        _lblScore->setPosition(Vec2(o.x + spBadge->getContentSize().width * 0.8f + 50, o.y - spBadge->getContentSize().height * 0.1f - 5));
		_lblScore->setColor(Color3B::WHITE);
        _lblScore->setRotation(-3);
        bg->addChild(_lblScore);
        
		_lblMaxScore = LabelTTF::create("", FONT_GAME, _lblScore->getFontSize(), Size(w * 0.5f, h * 0.15f), TextHAlignment::LEFT, TextVAlignment::CENTER);
        _lblMaxScore->setPosition(Vec2(_lblScore->getPositionX(), _lblScore->getPositionY() - spBadge->getContentSize().height * 0.28f - 15));
        _lblMaxScore->setColor(Color3B::WHITE);
        _lblMaxScore->setRotation(_lblScore->getRotation());
        bg->addChild(_lblMaxScore);
        
        // Home button
        MenuItemImage* itemHome =MenuItemImage::create("pause_home_off.png", "pause_home.png", CC_CALLBACK_1(PopUpLoseLayer::_onOptionPressed, this));
        itemHome->setTag(kTagGoHome);
        //itemHome->setPositionX(itemScore->getPositionX() + itemScore->getContentSize().width * 1.5f);
        //itemHome->setPositionY(itemScore->getPositionY());
		itemHome->setPositionX(o.x - itemHome->getContentSize().width * 2 / 3);
        itemHome->setPositionY(o.y - itemHome->getContentSize().height -50);
        
        // Play again button
        MenuItemImage* itemPlayAgain = MenuItemImage::create("pause_replay_off.png", "pause_replay.png", CC_CALLBACK_1(PopUpLoseLayer::_onOptionPressed, this));
        itemPlayAgain->setTag(kTagPlayAgain);
        itemPlayAgain->setPositionX(o.x + itemHome->getContentSize().width *2 /3);
        itemPlayAgain->setPositionY(o.y - itemHome->getContentSize().height * 0.8 -50);
        //itemHome->setPositionY(itemHome->getPositionY() + itemHome->getContentSize().height * 0.1f);
        
        // Menu
        Menu* menu = Menu::create();
        menu->setAnchorPoint(Vec2(0, 0));
        menu->setPosition(Point::ZERO);
        
        //menu->addChild(itemScore);
        menu->addChild(itemHome);
        menu->addChild(itemPlayAgain);
        
        bg->addChild(menu);

        
    }
}

void PopUpLoseLayer::updateScore(int level, float score, int obstaclesAvoided)
{

    _lblScore->setString(String::createWithFormat("%d (%d x %d)", (int) score, obstaclesAvoided, (int) kScoreFactor)->getCString());
    long longScore = (long) score;
    
    _level = level;
    _score = longScore;
    _obstaclesAvoided = obstaclesAvoided;
    // Check Achievements
    
    int totalGamesPlayed = LocalStorageManager::getTotalGamesPlayed();
    
    
    LocalStorageManager::setScore(score);
    
    long totalScore = (long) LocalStorageManager::getTotalScore();
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_AVERAGE_1000_OR_MORE_IN_50_GAMES_OR_MORE) &&  LocalStorageManager::getTotalGamesPlayed() >= 50 && LocalStorageManager::getAverageScore() >= 1000)
    {
        LocalStorageManager::unlockAchievement(ACH_AVERAGE_1000_OR_MORE_IN_50_GAMES_OR_MORE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_TOTAL_SCORE_100000_OR_MORE) && totalScore >= 100000)
    {
        LocalStorageManager::unlockAchievement(ACH_GET_TOTAL_SCORE_100000_OR_MORE);
    }
    
    long scoreInLevel = (long) LocalStorageManager::getScoreInLevel(level);
    
    if(longScore > scoreInLevel)
    {
        if(level == kGameLevelEasy)
        {
            _leaderboardID = LEAD_EASY_MODE;
        }
        else if (level == kGameLevelNormal)
        {
            _leaderboardID = LEAD_NORMAL_MODE;
        }
        else if(level == kGameLevelHard)
        {
            _leaderboardID = LEAD_HARD_MODE;
        }
        
        LocalStorageManager::setScoreInLevel(score, level);
        scoreInLevel = score;
        
        spBadge->setTexture(Sprite::create("face_normal4.png")->getTexture());
    }
    
    _lblMaxScore->setString(String::createWithFormat("%lu", scoreInLevel)->getCString());
}

void PopUpLoseLayer::_onOptionPressed(Ref *pSender)
{
    if(disable)
        return;
    
    MenuItem* item = (MenuItem*) pSender;
    SimpleAudioEngine::getInstance()->playEffect(SFX_BUTTON);
    
    switch (item->getTag()) {
        case kTagPlayAgain:
			NotificationCenter::getInstance()->postNotification(NOTIFICATION_PLAY_AGAIN);
            break;
            
        case kTagGoHome:
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
            Scene* scene = HomeScene::scene(kGameModeHome);
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
            break;
    }
}