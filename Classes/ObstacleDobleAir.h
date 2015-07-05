//
//  ObstacleDobleAir.h
//
//  Created by King
//
//

#ifndef ObstacleDobleAir_H
#define ObstacleDobleAir_H

#include "BaseObstacle.h"

class ObstacleDobleAir : public BaseObstacle
{
public:
    ObstacleDobleAir();
    ~ObstacleDobleAir();
    bool collision(BaseVehicle& vehicle);
    void doUpdate(float x, float speed);
    
private:
    cocos2d::Sprite* _spShadow;
};

#endif /* defined(__BTEndlessTunnel__ObstacleDobleAir__) */
