#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Layer
{
public:

// Functions
    static cocos2d::Scene* createScene(int score);

    virtual bool init();
    
    void GoToGameScene(Ref *pSender);
	void GoToMainMenuScene(Ref *pSender);

	void initBackButtonListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // implement the "static create()" method manually
    CREATE_FUNC(GameOver);
};

#endif // __GAMEOVER_SCENE_H__
