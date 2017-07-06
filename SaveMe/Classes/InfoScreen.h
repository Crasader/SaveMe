#ifndef __INFO_SCREEN_H__
#define __INFO_SCREEN_H__

#include "cocos2d.h"

class InfoScreen : public cocos2d::Layer
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
    CREATE_FUNC(InfoScreen);
};

#endif // __INFO_SCREEN_H__
