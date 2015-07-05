//
//  ObstacleSimple.cpp
//
//  Created by King
//
//

#include "ObstacleSimple.h"

using namespace cocos2d;

ObstacleSimple::ObstacleSimple() : BaseObstacle("muro_2b.png")
{
    sameCollisionArea = true;
    obstacType = kSimpleObstacle;
    
    Rect collideArea = Rect(getContentSize().width * 0.25f, getContentSize().height * 0.1f,getContentSize().width * 0.6f, getContentSize().height * 0.8f);
    
    vCollision.push_back(collideArea);
}

bool ObstacleSimple::collision(BaseVehicle &vehicle)
{
    float top, bottom;
    Rect area = getBoundingBox();
    
    bottom = area.getMinY() + getContentSize().height * 0.0f;
    top = bottom + getContentSize().height * 0.37f;
    
    float y;//= vehicle.getGroundCollision().getMinY() + (vehicle.getGroundCollision().getMaxY() - vehicle.getGroundCollision().getMinY()) * 0.25f;
    
    y = vehicle.getPlayerY() + vehicle.getContentSize().height * 0.3f * 0.5f;
    
    if(y < bottom || y > top)
        return false;
    
    // CCLog("Pass %f ; %f ; %f", y, top, bottom);
    
    return BaseObstacle::collision(vehicle);
}


ObstacleSimpleRight::ObstacleSimpleRight() : BaseObstacle("muro_2b1.png")
{
    sameCollisionArea = true;
    obstacType = kSimpleObstacle;
    
    Rect collideArea = Rect(getContentSize().width * 0.25f, getContentSize().height * 0.1f,getContentSize().width * 0.6f, getContentSize().height * 0.8f);
    
    vCollision.push_back(collideArea);
}

bool ObstacleSimpleRight::collision(BaseVehicle &vehicle)
{
    float top, bottom;
    Rect area = getBoundingBox();
    
    bottom = area.getMinY() + getContentSize().height * 0.0f;
    top = bottom + getContentSize().height * 0.37f;
    
    float y;//= vehicle.getGroundCollision().getMinY() + (vehicle.getGroundCollision().getMaxY() - vehicle.getGroundCollision().getMinY()) * 0.25f;
    
    y = vehicle.getPlayerY() + vehicle.getContentSize().height * 0.3f * 0.5f;
    
    if(y < bottom || y > top)
        return false;
    
	//log("Pass y = %f ; top = %f ; botton = %f, vehicle.getplayerY = %f", y, top, bottom, vehicle.getPlayerY());
    
    return BaseObstacle::collision(vehicle);
}
/*
ObstacleSimpleCol::ObstacleSimpleCol()
{
	ObstacleSimpleCol(4);
}


ObstacleSimpleCol::ObstacleSimpleCol(int _count) : count(_count)
{

	this->setAnchorPoint(Point::ZERO);

	float x= 0;
	float y = 0;
	for(int i = 0; i< count; i++)
	{
		ObstacleSimple *tmp = new ObstacleSimple();
		tmp->autorelease();
		tmp->setPosition(x,y);
		tmp->setZOrder(i);
		tmp->setTag(i);
		this->addChild(tmp);
		x += 43.25f;
		y += 50.0f;
	}
}

bool ObstacleSimpleCol::collision(BaseVehicle &vehicle) 
{
	for(auto tmp : this->getChildren())
	{
		ObstacleSimple * node = (ObstacleSimple* )tmp;
		if(node -> collision(vehicle))
		{
			return true;
		}
	}
	return false;
}
*/