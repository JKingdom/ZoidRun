//
//  SettingsLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "SettingsLayer.h"
#include "AppMacros.h"
#include "LocalStorageManager.h"
#include "Constants.h"
//#include "SimpleAudioEngine.h"
//#include "NativeUtils.h"

#define CONTROL_ALPHA 100

using namespace cocos2d;
//using namespace CocosDenshion;

Scene* SettingsLayer::scene()
{
    Scene* scene = Scene::create();
    
    SettingsLayer* layer = new SettingsLayer();
    layer->autorelease();
    
    scene->addChild(layer);
    return scene;
}

SettingsLayer::SettingsLayer()
{
	if(initWithColor(Color4B(0, 0, 0, 0.5)))
    {
        
        bool joypad = LocalStorageManager::isUsingJoypad();
        
		Size visibleSize = Director::getInstance()->getVisibleSize();
        Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
        
        Sprite* bg = Sprite::create("controls_options.png");
        bg->setPositionX(visibleOrigin.x + visibleSize.width * 0.5f);
        bg->setPositionY(visibleOrigin.y + visibleSize.height * 0.5f);
        addChild(bg);
        
		Point center = Point::ZERO;
        center.x = bg->getContentSize().width * 0.5f;
        center.y = bg->getContentSize().height * 0.5f;
        
        Menu* menu = Menu::create();
        menu->setPosition(Point::ZERO);
        menu->setAnchorPoint(Vec2(0, 0));
        
        // Joypad Mode
        //itemJoypad = CCMenuItemImage::create("control_joystick.png", "control_joystick.png", this, menu_selector(SettingsLayer::_onOptionPressed));
		itemJoypad = MenuItemImage::create("control_joystick.png", "control_joystick.png", CC_CALLBACK_1(SettingsLayer::_onOptionPressed, this));
        itemJoypad->setTag(kTagSettingJoypadMode);
        itemJoypad->setPositionX(center.x - itemJoypad->getContentSize().width * 0.5f);
        itemJoypad->setPositionY(center.y + itemJoypad->getContentSize().height * 0.05f);
        menu->addChild(itemJoypad);
        if(!joypad)
            itemJoypad->setOpacity(CONTROL_ALPHA);
        
        // Accel Mode
        //itemAccel = CCMenuItemImage::create("control_tilt.png", "control_tilt.png", this, menu_selector(SettingsLayer::_onOptionPressed));
		itemAccel = MenuItemImage::create("control_tilt.png", "control_tilt.png", CC_CALLBACK_1(SettingsLayer::_onOptionPressed, this));
        itemAccel->setTag(kTagSettingAccelMode);
        itemAccel->setPositionX(center.x + itemAccel->getContentSize().width * 0.6f);
        itemAccel->setPositionY(itemJoypad->getPositionY());
        menu->addChild(itemAccel);
        if(joypad)
            itemAccel->setOpacity(CONTROL_ALPHA);
        
        // Home button
        //MenuItemImage* itemHome = MenuItemImage::create("pause_home_off.png", "pause_home.png", this, menu_selector(SettingsLayer::_onOptionPressed));
		MenuItemImage* itemHome = MenuItemImage::create("pause_home_off.png", "pause_home.png", CC_CALLBACK_1(SettingsLayer::_onOptionPressed, this));
        itemHome->setTag(kTagSettingGoHome);
        itemHome->setPositionX(center.x);
        itemHome->setPositionY(center.y - itemHome->getContentSize().height * 1.6f);
        menu->addChild(itemHome);
        
        // Menu
        bg->addChild(menu);
        
    }
}

void SettingsLayer::_onOptionPressed(Ref *pSender)
{
    MenuItem* item = (MenuItem*) pSender;
    //SimpleAudioEngine::sharedEngine()->playEffect(SFX_BUTTON);
    switch(item->getTag())
    {
        case kTagSettingGoHome:
            NotificationCenter::getInstance()->postNotification(NOTIFICATION_ENABLE_BUTTONS);
            NotificationCenter::getInstance()->postNotification(NOTIFICATION_HOW_TO_PLAY);
            this->setVisible(false);
            break;
            
        case kTagSettingJoypadMode:
            //NativeUtils::sendAnalytics("Joypad Mode");
            itemJoypad->setOpacity(255);
            itemAccel->setOpacity(CONTROL_ALPHA);
            LocalStorageManager::setControlType(true);
            break;
            
        case kTagSettingAccelMode:
            //NativeUtils::sendAnalytics("Tilt Mode");
            itemJoypad->setOpacity(CONTROL_ALPHA);
            itemAccel->setOpacity(255);
            LocalStorageManager::setControlType(false);
            break;
    }
}