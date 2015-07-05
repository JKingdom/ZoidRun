//
//  PopUpLoseLayer.h
//
//  Created by King
//
//

#ifndef PopUpLoseLayer_H
#define PopUpLoseLayer_H

#include "cocos2d.h"

enum PopUpLoseButtons
{
    kTagPlayAgain = 0,
    kTagGoHome = 1,
};

class PopUpLoseLayer : public cocos2d::LayerColor
{
public:
    PopUpLoseLayer();
    void updateScore(int level, float score, int obstaclesAvoided);
    
private:
    void _onOptionPressed(cocos2d::Ref* pSender);
    void _finishHideLayer();
    
private:
    const char* _leaderboardID;
    
    int _obstaclesAvoided;
    int _level;
    long _score;
    
    cocos2d::LabelTTF* _lblScore;
    cocos2d::LabelTTF* _lblMaxScore;
    cocos2d::Sprite* spBadge;
    bool disable;
};

#endif /* defined(__BTEndlessTunnel__PopUpLoseLayer__) */
