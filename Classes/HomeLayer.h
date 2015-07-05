//
//  HomeLayer.h
//
//  Created by King
//
//

#ifndef HomeLayer_H
#define HomeLayer_H

#include "cocos2d.h"
#include "GameLayer.h"


enum HomeButtons
{
    kFaceEasyMode = 0,

    kTagHowToPlay = 1
};

class HomeLayer : public cocos2d::Layer
{
public:
    HomeLayer(GameLayer* gameLayer, bool showAds = false);
    virtual ~HomeLayer();
    void onEnterTransitionDidFinish();
    
private:
	//center
	cocos2d::Sprite* logoFont;
	cocos2d::MenuItemImage* face;
	// left
	cocos2d::Sprite* tabLeft;
	cocos2d::Sprite* bubble;
    cocos2d::MenuItemLabel* menuHowToPlay;
	// right
	cocos2d::Sprite* tabRight;
    cocos2d::MenuItemToggle* menuSound;
	cocos2d::Sprite* flower1;
	cocos2d::Sprite* flower2;
    
private:
	void _onOptionPressed(cocos2d::Ref* pSender);
    void _finishHideLayer(float dt);
    void _manageMusic(cocos2d::Ref* pSender);
    
    void _hideToLeft();
    void _hideToRight();
    
    void _enableButtons(Ref* pSender);
    void _disableButtons();
    
    void _manageHowToPlay(Ref* pSender);
    
private:
    bool _showAds;
    bool disable;
    GameLayer* _gameLayer;
    cocos2d::Menu* menu;
};

#endif /* defined(__BTEndlessTunnel__HomeLayer__) */
