//
//  Constants.h
//
//  Created by King
//

#ifndef BTEndlessTunnel_Constants_h
#define BTEndlessTunnel_Constants_h

#include "cocos2d.h"
#include "PlayGameConstants.h"
#include "AppMacros.h"

#define COUNT_ADS 0
#define FONT_GAME "NeonTech.ttf"
#define FONT_GAME_BMP "CartonSixBMP.fnt"
#define SIZE_SCORE_GAME 45.0f * CC_CONTENT_SCALE_FACTOR()
#define SIZE_RATE_APP 30.0f * CC_CONTENT_SCALE_FACTOR()
#define SIZE_RATE_END 30.0f * CC_CONTENT_SCALE_FACTOR()
#define SIZE_TUT_INST 47.0f * CC_CONTENT_SCALE_FACTOR()
#define SIZE_TUT_TITLE 80.0f * CC_CONTENT_SCALE_FACTOR()

#define SP_SHADOW "shadow2.png"
#define SHOW_FPS false
#define WIN_SIZE Director::getInstance()->getWinSize()

// Definición de ID's para Local Storage

#define USER_CONTROL_IS_JOYPAD "KEY_CONTROL_IS_JOYPAD"
#define USER_SAVE_REPLAY "KEY_REPLAY_NUMBER_%d"
#define USER_PLAYER_SELECTED "KEY_PLAYER_SELECTED"
#define USER_MAX_SCORE "KEY_MAX_SCORE"
#define USER_TOTAL_GAMES_PLAYED "KEY_TOTAL_GAMES_PLAYED"
#define USER_TOTAL_SCORE "KEY_TOTAL_SCORE"
#define USER_SCORE_IN_LEVEL "KEY_SCORE_LEVEL_%d"
#define USER_OBSTACLES_JUMPED "KEY_OBSTACLES_JUMPED"
#define USER_MANAGE_MUSIC "KEY_MUSIC"
#define USER_TUTORIAL "KEY_TUTORIAL_%d"

// Notifications
#define NOTIFICATION_PAUSE_GAME "NOT_PAUSE_GAME"
#define NOTIFICATION_RESUME_GAME "NOT_RESUME_GAME"
#define NOTIFICATION_PLAY_AGAIN "NOT_PLAY_AGAIN"
#define NOTIFICATION_GO_HOME "NOT_GO_HOME"
#define NOTIFICATION_ENABLE_BUTTONS "NOTIFICATION_ENABLE_BUTTONS"
#define NOTIFICATION_HOW_TO_PLAY "NOTIFICATION_HOW_TO_PLAY"

// Musics and SFX
#define BG_MUSIC_01 "zoidtrip.mp3"
//#define BG_MUSIC_02 "diego_music.mp3"
//#define BG_MUSIC_03 "POL-turtle-blues-short.mp3"

#define SFX_JUMP "jump.mp3"
#define SFX_SMASH "smash.mp3"
#define SFX_SWOOSH "swoosh.mp3"
#define SFX_BUTTON "button.mp3"
#define SFX_LIGHTNING "lightning.mp3"

// Game constants
#define MAX_PARALLAX 5
#define MAX_OBSTACLES 40

#define START_X_OBSTACLES designResolutionSize.width * 1.9f
#define START_WORLD_SPEED designResolutionSize.width * 0.5f
#define MIN_DISTANCE_OBSTACLES designResolutionSize.width / 1.8f

#define DRAW_COLLISIONS false
#define TEST_OBSTACLE false

// Music and Sound
#define BG_MUSIC_VOLUME 0.4f

#endif
