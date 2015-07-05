//
//  VehicleFrog.cpp
//
//  Created by King
//
//

#include "VehicleFrog.h"

using namespace cocos2d;

VehicleFrog::VehicleFrog() : BaseVehicle("face_normal.png")
{
    defaultTexture = getTexture();
	deadTexture = Sprite::create("dead.png")->getTexture();
    
    Animation* _jumpAnimation = Animation::create();
    _jumpAnimation->addSpriteFrameWithFileName("face_jump.png");
	_jumpAnimation->addSpriteFrameWithFileName("face_jump2.png");
	_jumpAnimation->addSpriteFrameWithFileName("face_jump3.png");
    _jumpAnimation->setRestoreOriginalFrame(false);
    _jumpAnimation->setDelayPerUnit(1.0f / 10.0f);
    
    jumpAnimation = Animate::create(_jumpAnimation);
    jumpAnimation->retain();
    
    Animation* _idleAnimation = Animation::create();
    
    _idleAnimation->addSpriteFrameWithFileName("face_normal2.png");
    _idleAnimation->addSpriteFrameWithFileName("face_normal3.png");
	_idleAnimation->addSpriteFrameWithFileName("face_normal4.png");
    
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

