//
//  HomeScene.h
//
//  Created by King
//
//

#ifndef HomeScene_H
#define HomeScene_H

#include "cocos2d.h"

#include "GameLayer.h"
#include "HomeLayer.h"

enum
{
    kZHomeLayer = 99999,
    kZHudLayer = 9999,
    kZGameLayer = 999
};

class HomeScene : public cocos2d::CCScene
{
private:
    HomeScene(GameMode gameMode, GameLevel gameLevel = kGameLevelNone, bool showAds = false);
public:
    static HomeScene* scene(GameMode gameMode, GameLevel gameLevel = kGameLevelNone, bool showAds = false);
};

#endif /* defined(__BTEndlessTunnel__HomeScene__) */
