#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void initAudio();

    void ShowAchievements(Ref *pSender);

    void GoToInfoScene(Ref *pSender);
	void GoToGameScene(Ref *pSender);
	void GoToInstructionsScene(Ref *pSender);
	void ExitGame(Ref *pSender);

	void ShowLeaderboard(Ref *pSender);
    void EndApplication(float dt);

	void initBackButtonListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
};

#endif // __MAINMENU_SCENE_H__
