//
//  ObstacleDoble.cpp
//
//  Created by King
//
//

#include "ObstacleDoble.h"

using namespace cocos2d;

ObstacleDoble::ObstacleDoble() : BaseObstacle("obstaculo_col.png")
{
    obstacType = kJumpObstacle;
    sameCollisionArea = false;
    /*
    Rect collideArea1 = Rect(getContentSize().width * 0.1f, getContentSize().height * 0.5f, getContentSize().width * 0.5f, getContentSize().height * 0.5f);
    
    Rect collideArea2 = Rect(getContentSize().width * 0.3f, 0, getContentSize().width * 0.5f, getContentSize().height * 0.5f);
    
    vCollision.push_back(collideArea1);
    vCollision.push_back(collideArea2);
    */

	this->setAnchorPoint(Point::ZERO);

	float width = getContentSize().width;
	float height = getContentSize().height;

	//vCollision.push_back(Rect(width * 0.05f, 0, width * 0.3f, height * 0.25f));
	//vCollision.push_back(Rect(width * 0.25f, height * 0.25f, width * 0.25f, height * 0.25f));
	vCollision.push_back(Rect(width * 0.4f, height * 0.5f, width * 0.333f, height * 0.25f));
	vCollision.push_back(Rect(width * 0.57f, height * 0.75f, width * 0.333f, height * 0.25f));
	vCollision.push_back(Rect(width * 0.73f, height, width * 0.333f, height * 0.25f));
	vCollision.push_back(Rect(width * 0.90f, height * 1.25f, width * 0.333f, height * 0.25f));
}