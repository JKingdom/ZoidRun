//
//  Utils.cpp
//
//  Created by King
//
//

#include "Utils.h"
#include "AppMacros.h"

using namespace cocos2d;

void Utils::unlockAchievement(const char *achievementID)
{
}

void Utils::incrementAchievement(const char *achievementID, int numSteps)
{
}

void Utils::incrementPercentageAchievement(const char *achievementID, float percentage)
{
}

void Utils::submitScore(const char* leaderboardID, float score)
{
}

float Utils::randomBetween(float min, float max)
{
    float random = min + CCRANDOM_0_1() * (max - min + 1);
    return random;
}