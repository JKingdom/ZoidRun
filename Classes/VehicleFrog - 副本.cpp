//
//  VehicleFrog.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#include "VehicleFrog.h"

using namespace cocos2d;

VehicleFrog::VehicleFrog() : BaseVehicle("bicho_0001.png")
{
    defaultTexture = getTexture();
    deadTexture = Sprite::create("bicho_0004.png")->getTexture();
    
    Animation* _jumpAnimation = Animation::create();
    _jumpAnimation->addSpriteFrameWithFileName("bicho_0003.png");
    _jumpAnimation->setRestoreOriginalFrame(false);
    _jumpAnimation->setDelayPerUnit(1.0f / 10.0f);
    
    jumpAnimation = Animate::create(_jumpAnimation);
    jumpAnimation->retain();
    
    Animation* _idleAnimation = Animation::create();
    
    _idleAnimation->addSpriteFrameWithFileName("bicho_0001.png");
    _idleAnimation->addSpriteFrameWithFileName("bicho_0002.png");
    
    _idleAnimation->setRestoreOriginalFrame(false);
    _idleAnimation->setDelayPerUnit(1.0f / 12.0f);
    
    idleAnimation = RepeatForever::create(Animate::create(_idleAnimation));
    idleAnimation->retain();
    
    runAction(idleAnimation);
    
}

VehicleFrog::~VehicleFrog()
{
    CC_SAFE_RELEASE(jumpAnimation);
    CC_SAFE_RELEASE(idleAnimation);
}

Face::Face() : BaseVehicle("")
{
	defaultTexture = getTexture();
}