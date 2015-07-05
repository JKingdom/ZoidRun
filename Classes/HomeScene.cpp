//
//  HomeScene.cpp
//  Created by King
//

#include "HomeScene.h"
#include "Constants.h"

using namespace cocos2d;

HomeScene::HomeScene(GameMode gameMode, GameLevel gameLevel, bool showAds)
{
   
	GameLayer* gameLayer = new GameLayer(gameMode, gameLevel);
    gameLayer->autorelease();
    addChild(gameLayer, kZGameLayer);
    
    if(gameMode == kGameModeHome)
    {
        HomeLayer* homeLayer = new HomeLayer(gameLayer, showAds);
        homeLayer->setVisible(!TEST_OBSTACLE);
        homeLayer->autorelease();
        addChild(homeLayer, kZHomeLayer);
    }
}

HomeScene* HomeScene::scene(GameMode gameMode, GameLevel gameLevel, bool showAds)
{
    HomeScene* scene = new HomeScene(gameMode, gameLevel, showAds);
    scene->autorelease();
    return scene;
}

