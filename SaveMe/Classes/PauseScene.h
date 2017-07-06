#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "cocos2d.h"

class PauseMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void Resume(Ref *pSender);
	void GoToMainMenuScene(Ref *pSender);
	void Retry(Ref *pSender);
    
	void initBackButtonListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(PauseMenu);
};

#endif // __PAUSE_SCENE_H__
