//
//  AppMacros.h
//
//  Created by King
//
//

#ifndef AppMacros_h
#define AppMacros_h

#include "cocos2d.h"

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
}Resource;

static Resource smallResource  =  { cocos2d::Size(512, 384),   "512x384" };
static Resource mediumResource =  { cocos2d::Size(1024, 768),  "1024x768"   };

static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);

// The font size 24 is designed for small resolution, so we should change it to fit for current design resolution
#define TITLE_FONT_SIZE  (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / smallResource.size.width * 24)


#endif
