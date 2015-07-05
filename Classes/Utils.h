//
//  Utils.h
//
//  Created by King
//
//

#ifndef Utils_H
#define Utils_H

#include "cocos2d.h"

class Utils
{
public:
    static void unlockAchievement(const char* achievementID);
    static void incrementAchievement(const char* achievementID, int numSteps);
    static void incrementPercentageAchievement(const char* achievementID, float percentage);
    static void submitScore(const char* leaderboardID, float score);
    static float randomBetween(float min, float max);
};

#endif /* defined(__BTEndlessTunnel__Utils__) */
