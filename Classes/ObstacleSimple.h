//
//  ObstacleSimple.h
//
//  Created by King
//
//

#ifndef ObstacleSimple_H
#define ObstacleSimple_H

#include "BaseObstacle.h"

class ObstacleSimple : public BaseObstacle
{
public:
    ObstacleSimple();
    bool collision(BaseVehicle& vehicle);
};


class ObstacleSimpleRight : public BaseObstacle
{
public:
    ObstacleSimpleRight();
    bool collision(BaseVehicle& vehicle);
};
/*
class ObstacleSimpleCol : public BaseObstacle
{
private:
	int count;
public:
	ObstacleSimpleCol();
	ObstacleSimpleCol(int count);
    bool collision(BaseVehicle& vehicle);
};
*/
#endif /* defined(__BTEndlessTunnel__ObstacleSimple__) */
