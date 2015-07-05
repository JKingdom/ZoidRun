//
//  PauseLayer.h
//
//  Created by King
//
//

#ifndef PauseLayer_H
#define PauseLayer_H

#include "cocos2d.h"

enum PauseButtons
{
    kTagPauseResumeGame = 0,
    kTagPauseGoHome = 1,
    kTagPausePlayAgain = 2
};

class PauseLayer : public cocos2d::LayerColor
{
public:
    PauseLayer();
    
private:
    void _onOptionPressed(cocos2d::Ref* pSender);
};

#endif /* defined(__BTEndlessTunnel__PauseLayer__) */
