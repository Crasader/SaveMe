#ifndef __INSTRUCTIONS_SCENE_H__
#define __INSTRUCTIONS_SCENE_H__

#include "cocos2d.h"

class Instructions : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void initAudio();

	void GoToMainMenu(Ref *pSender);
	void ExitGame(Ref *pSender);

	void initBackButtonListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Instructions);
};

#endif // __INSTRUCTIONS_SCENE_H__
