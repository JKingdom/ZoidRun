//
//  SettingsLayer.h
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#ifndef SettingsLayer_H
#define SettingsLayer_H

#include "cocos2d.h"

enum SettingsTags
{
    kTagSettingJoypadMode = 0,
    kTagSettingAccelMode = 1,
    kTagSettingGoHome = 2
};

class SettingsLayer : public cocos2d::LayerColor
{
public:
    SettingsLayer();
    static cocos2d::Scene* scene();
    
private:
    void _onOptionPressed(cocos2d::Ref* pSender);
    cocos2d::MenuItemImage* itemJoypad;
    cocos2d::MenuItemImage* itemAccel;
};

#endif /* defined(__BTEndlessTunnel__SettingsLayer__) */
