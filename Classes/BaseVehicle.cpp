//
//  BaseVehicle.cpp
//
//  Created by King
//

#include "BaseVehicle.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "Constants.h"

using namespace cocos2d;
using namespace CocosDenshion;

BaseVehicle::BaseVehicle(std::string filename)
{
	baseSpeed.x = 1;
	baseSpeed.y = 1.7320508076f;
	leftSpeed.x = baseSpeed.x * SPEED_LEFT_X;
	leftSpeed.y = baseSpeed.y * SPEED_LEFT_Y;
	rightSpeed.x = - baseSpeed.x * SPEED_RIGHT_X;
	rightSpeed.y = - baseSpeed.y * SPEED_RIGHT_Y;

    if(initWithFile(filename.c_str()))
    {
        _limitBottomY = 0;
        _limitTopY = 0;
        
        defaultTexture = NULL;
        jumpAnimation = NULL;
        idleAnimation = NULL;
        deadTexture = NULL;
        speed = 11.0f;
        state = kStateIdle;
        
        playerY = getPositionY() - getContentSize().height * 0.5f;
        
        _jumpByAction = JumpBy::create(0.6f, Vec2(0, 0), MAX_PLAYER_JUMP, 1);
        _jumpByAction->setTag(kActionJumpTag);
        _jumpByAction->retain();
        
        _spShadow = Sprite::create(SP_SHADOW);
        _spShadow->setPositionX(getContentSize().width * 0.5f);
        addChild(_spShadow, -1);
        _updateShadow();
        
    }
}

BaseVehicle::~BaseVehicle()
{
    CC_SAFE_RELEASE(_jumpByAction);
}

void BaseVehicle::doJump()
{
    int y = getPositionY() - getPlayerY() - getContentSize().height * 0.5f;
    if(y <= 1 && state != kStateJump/* && getActionByTag(kActionJumpTag) == NULL*/)
    {
        state = kStateJump;
        if(jumpAnimation != NULL && idleAnimation != NULL)
        {
            stopAction(idleAnimation);
            runAction(jumpAnimation);
        }
        runAction(_jumpByAction);
        SimpleAudioEngine::sharedEngine()->playEffect(SFX_JUMP);
    }
}

void BaseVehicle::dead()
{
    if(deadTexture != NULL)
    {
        stopAllActions();
        setTexture(deadTexture);
        SimpleAudioEngine::sharedEngine()->playEffect(SFX_SMASH);
        
        runAction(Blink::create(1.5f, 8));
        if(state == kStateJump)
        {
            float time = 1.0f;
            float times = 30;
            float y = _spShadow->getPositionY() + getPositionY();
            runAction(MoveTo::create(time, Vec2(getPositionX() + getContentSize().width * 0.15f, y)));
            schedule(CC_SCHEDULE_SELECTOR(BaseVehicle::_updateEnd), time / times, times, 0);
        }
    }
}

void BaseVehicle::_updateEnd(float dt)
{
    _updateShadow();
}

void BaseVehicle::doMove(Point vel)
{
	Point velocity;
	if(isLeft)
	{
		velocity = leftSpeed;
	}else
	{
		velocity = rightSpeed;
	}
	
    if(getActionByTag(kActionJumpTag) == NULL)
    {
        if(state == kStateJump)
        {
            if(defaultTexture != NULL)
            {
                setTexture(defaultTexture);
                if(idleAnimation != NULL)
                    runAction(idleAnimation);
            }
            state = kStateIdle;
        }
        playerY = getPositionY() - getContentSize().height * 0.5f;
    }
    else
    {
		velocity.y = 0;
        if(playerY + velocity.y > _limitTopY)
		{
            velocity.y = 0;
			isLeft = false;
		}
        else if(playerY + velocity.y < _limitBottomY){
            velocity.y = 0;
			isLeft = true;
		}
    }

    //Point newPosition = getPosition() + velocity;
	Point newPosition = getPosition();
	newPosition.y = newPosition.y + velocity.y;

		playerY += velocity.y;
    
    if(playerY > _limitTopY)
	{
        playerY = _limitTopY -1;
		isLeft = false;
	}
    else if(playerY < _limitBottomY)
	{
        playerY = _limitBottomY + 1;
		isLeft = true;
	}
    
    if(newPosition.x < getContentSize().width * 0.5f)
        newPosition.x = getContentSize().width * 0.5f;
    
    if(newPosition.x > WIN_SIZE.width * 0.8f)
        newPosition.x = WIN_SIZE.width * 0.8f;
    
    if(getActionByTag(kActionJumpTag) == NULL)
        newPosition.y = playerY + getContentSize().height * 0.5f;
    
    setPosition(newPosition);
    _updateShadow();

	//log("playerY = %f,  sprite x = %f, y = %f contentSize x = %f, y = %f", playerY, this->getPosition().x, this->getPosition().y, this->getContentSize().width, this->getContentSize().height);
}

Rect BaseVehicle::getGroundCollision()
{
    float x = getBoundingBox().getMinX() + getContentSize().width * 0.3f;
    float y = playerY;
    
    float width = getContentSize().width * 0.55f;
    float height = getContentSize().height * 0.3f;
    
    Rect rect = Rect(x, y, width, height);
    
    return rect;
}

Rect BaseVehicle::getAirCollision()
{
    float x = getBoundingBox().getMinX() + getContentSize().width * 0.3f;
    float y = getBoundingBox().getMinY() + getContentSize().height * 0.16f;
    float w = getContentSize().width * 0.55f;
    float h = getContentSize().width * 0.2f;
    
    Rect rect;
    rect.setRect(x, y, w, h);
    
    return rect;
}

void BaseVehicle::_updateShadow()
{
    _spShadow->setPositionY(playerY - getPositionY() + getContentSize().height * 0.55f);
}

void BaseVehicle::setLimits(float limitBotY, float height)
{
    _limitBottomY = limitBotY;
    _limitTopY = _limitBottomY + height;
}