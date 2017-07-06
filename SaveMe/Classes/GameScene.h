#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class GameScreen : public cocos2d::Layer
{
public:

// Functions
    static cocos2d::Scene* createScene();

    virtual bool init();
    void update(float dt);

    void spawnElement(float dt);
    void spawnPickup(void);
    void spawnCloud(float dt);

    void incrementScore(void);
    void removeTint(void);

    void initPhysics();
    bool onCollision(cocos2d::PhysicsContact& contact);
    void setPhysicsBody(cocos2d::Sprite* sprite, float radiusMultiplier);

	void GoToPauseScene(Ref *pSender);
	void GoToGameOverScene(Ref *pSender);

    void ScaleFinished(Ref *pSender);

	void movePlayerByAccelerometer(cocos2d::Acceleration* acceleration, cocos2d::Event* event);
	void movePlayerIfPossible(float newX);
	void initAccelerometer();

    void initBackButtonListener();
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void updateRainDensity(void);
    void updateHailDensity(void);
    void updateWind(float dt);

    // implement the "static create()" method manually
    CREATE_FUNC(GameScreen);

// Variables
    cocos2d::Sprite *backgroundSprite;
    cocos2d::Sprite *butterflySprite;
    
    cocos2d::Sprite *pickup;
    int pickupRoll;

    std::vector<cocos2d::Sprite *> raindrops;
    int rainDensity;

    std::vector<cocos2d::Sprite *> hailstones;
    int hailDensity;

    std::vector<cocos2d::Sprite *> clouds;

    float windSpeed;

    bool bIncremented;
    bool bScaled;
    cocos2d::Sprite* shieldSprite;
    int shield;
    int currentLevel;
    int score;
    cocos2d::Label* scoreLabel;
};

#endif // __GAME_SCENE_H__
