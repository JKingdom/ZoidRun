//
//  BaseVehicle.h
//
//  Created by King
//
//

#ifndef BaseVehicle_H
#define BaseVehicle_H

#include "cocos2d.h"

#define MAX_PLAYER_JUMP 140.0f

enum
{
    kActionJumpTag = 1000,
    kActionJumpAnimationTag = 1001,
    kActionIdleAnimationTag = 1002
};

enum ActorStates
{
    kStateNothing = 0,
    kStateIdle = 1,
    kStateJump = 2,
    kStateRun = 3,
    kStateBack = 4
};

#define SPEED_RIGHT_Y 1.5f
#define SPEED_RIGHT_X 0.1339745962f
#define SPEED_LEFT_X 0.8660254038f
#define SPEED_LEFT_Y 1.5f

class BaseVehicle : public cocos2d::Sprite
{
public:
    BaseVehicle(std::string filename);
    virtual ~BaseVehicle();
    
    virtual void dead();
    virtual void doJump();
    virtual void doMove(cocos2d::Point velocity);
    virtual void setLimits(float limitBotY, float height);
    
    virtual cocos2d::Rect getGroundCollision();
    virtual cocos2d::Rect getAirCollision();
    

	//#define CC_SYNTHESIZE(varType, varName, funName)\
	//protected: varType varName;\
	//public: virtual varType get##funName(void) const { return varName; }\
	//public: virtual void set##funName(varType var){ varName = var; }
	// 这个宏在定义protected变量的时候同时生成了get/set方法
    
    CC_SYNTHESIZE(ActorStates, state, State);
    CC_SYNTHESIZE(float, playerY, PlayerY);
    CC_SYNTHESIZE(cocos2d::Texture2D*, defaultTexture, DefaultTexture);
    CC_SYNTHESIZE(cocos2d::Texture2D*, deadTexture, DeadTexture);
    CC_SYNTHESIZE(cocos2d::Action*, jumpAnimation, JumpAnimation);
    CC_SYNTHESIZE(cocos2d::Action*, idleAnimation, IdleAnimation);
    CC_SYNTHESIZE(float, speed, Speed);

	// new add
	CC_SYNTHESIZE(bool, isLeft, IsLeft);
	CC_SYNTHESIZE(cocos2d::Point, leftSpeed, LeftSpeed);
	CC_SYNTHESIZE(cocos2d::Point, rightSpeed, RightSpeed);
private:
	cocos2d::Point baseSpeed;
    
private:
    void _updateShadow();
    void _updateEnd(float dt);
    
private:
    cocos2d::JumpBy* _jumpByAction;
    cocos2d::Sprite* _spShadow;
    
    float _limitBottomY;
    float _limitTopY;
    
};

#endif /* defined(BaseVehicle_H) */
