#include "GameOverScene.h"
#include "PauseScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include "SonarFrameworks.h"
#include "Constants.h"
#include "Achievements.h"

USING_NS_CC;

using namespace CocosDenshion;

int lastScore;

void GameOver::initBackButtonListener()
{
    auto listener = EventListenerKeyboard::create();
    
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){};
    listener->onKeyReleased = CC_CALLBACK_2(GameOver::onKeyPressed, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameOver::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
    	Director::getInstance()->end();
    }
}

void GameOver::GoToGameScene(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    auto scene = GameScreen::createScene();
    
    Director::getInstance()->replaceScene(TransitionFade::create(fadeDuration, scene));
}

void GameOver::GoToMainMenuScene(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    auto scene = MainMenu::createScene();
 
    Director::getInstance()->replaceScene(TransitionFade::create(fadeDuration, scene));
}

Scene* GameOver::createScene(int score)
{
    lastScore = score;

    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOver::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    initBackButtonListener();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto menuTitle = MenuItemImage::create(gameOverPath, gameOverPath);
    
    auto retryItem = MenuItemImage::create(gameOverRetryIdlePath, gameOverRetryPressedPath, CC_CALLBACK_1(GameOver::GoToGameScene, this));
    
    auto mainMenuItem = MenuItemImage::create(gameOverMenuIdlePath, gameOverMenuPressedPath, CC_CALLBACK_1(GameOver::GoToMainMenuScene, this));
 
    menuTitle->setPosition(Point((visibleSize.width / 2) + origin.x, ((9 * visibleSize.height) / 10) + origin.y));

    __String* scoreString = __String::createWithFormat("Score : %i", lastScore);

    auto scoreLabel = Label::createWithTTF(scoreString->getCString(), fontPath, 32);
    scoreLabel->setPosition(Point((visibleSize.width / 2) + origin.x, ((7 * visibleSize.height) / 10) + origin.y));
    scoreLabel->enableOutline(Color4B(0, 0, 0, 100), 1);
    this->addChild(scoreLabel, 1);

    UserDefault * def = UserDefault::sharedUserDefault();
    int highscore = def->getIntegerForKey("SaveMeHighscore", 0);

    if(highscore < lastScore)
    {
        highscore = lastScore;
        def->setIntegerForKey("SaveMeHighscore", highscore);
        def->flush();
    }

    if(SonarCocosHelper::GooglePlayServices::isSignedIn())
    {
    	SonarCocosHelper::GooglePlayServices::submitScore(leaderboardID, highscore);

    	if(lastScore < 10)
    		SonarCocosHelper::GooglePlayServices::unlockAchievement(Unlucky);

    	if(lastScore == 100)
    	    SonarCocosHelper::GooglePlayServices::unlockAchievement(Precision);

    	if(lastScore >= 150)
    		SonarCocosHelper::GooglePlayServices::unlockAchievement(ICanFly);

    	if(lastScore >= 400)
    	    SonarCocosHelper::GooglePlayServices::unlockAchievement(Survivor);

    	if(lastScore >= 1000)
    	    SonarCocosHelper::GooglePlayServices::unlockAchievement(IllMakeIt);

    	if(lastScore >= 5000)
    	    SonarCocosHelper::GooglePlayServices::unlockAchievement(ThisIsAJoke);
    }



    __String* highscoreString = __String::createWithFormat("High Score : %i", highscore);

    auto highscoreLabel = Label::createWithTTF(highscoreString->getCString(), fontPath, 24);
    highscoreLabel->setPosition(Point((visibleSize.width / 2) + origin.x, ((5.5 * visibleSize.height) / 10) + origin.y));
    highscoreLabel->enableOutline(Color4B(0, 0, 0, 100), 1);
    this->addChild(highscoreLabel, 1);    

    retryItem->setPosition(Point((visibleSize.width / 2) + origin.x, ((4 * visibleSize.height) / 10) + origin.y));

    mainMenuItem->setPosition(Point((visibleSize.width / 2) + origin.x, ((2 * visibleSize.height) / 10) + origin.y));

    auto menu = Menu::create(menuTitle, retryItem, mainMenuItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
 
    auto bg = cocos2d::LayerColor::create(Color4B(182, 249, 255, 255));
    this->addChild(bg, -2);
 
    auto sun = Sprite::create(sunPath);
    sun->setPosition(Point(origin.x + (visibleSize.width * 0.75f), origin.y + (visibleSize.height * 0.9f)));
    this->addChild(sun, -1);

    return true;
}
