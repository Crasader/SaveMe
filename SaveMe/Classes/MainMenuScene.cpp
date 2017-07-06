#include "MainMenuScene.h"
#include "GameScene.h"
#include "InfoScreen.h"
#include "SimpleAudioEngine.h"
#include "Constants.h"
#include "InstructionsScene.h"
#include "SonarFrameworks.h"

USING_NS_CC;

using namespace CocosDenshion;

void MainMenu::initBackButtonListener()
{
    auto listener = EventListenerKeyboard::create();
    
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){};
    listener->onKeyReleased = CC_CALLBACK_2(MainMenu::onKeyPressed, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void MainMenu::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
    	Director::getInstance()->end();
    }
}

void MainMenu::GoToInfoScene(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    auto scene = InfoScreen::createScene();

    Director::getInstance()->replaceScene(TransitionFade::create(fadeDuration, scene));
}

void MainMenu::GoToGameScene(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    auto scene = GameScreen::createScene();

    Director::getInstance()->replaceScene(TransitionFade::create(fadeDuration, scene));
}

void MainMenu::GoToInstructionsScene(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    auto scene = Instructions::createScene();

    Director::getInstance()->pushScene(TransitionFade::create(fadeDuration, scene));
}

void MainMenu::ShowLeaderboard(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    if(SonarCocosHelper::GooglePlayServices::isSignedIn())
    	SonarCocosHelper::GooglePlayServices::showLeaderboard(leaderboardID);
}

void MainMenu::ExitGame(Ref *pSender)
{
	SonarCocosHelper::RevMob::showFullscreenAd();

    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);
    
    this->scheduleOnce(schedule_selector(MainMenu::EndApplication), 1.0f);
}

void MainMenu::EndApplication(float dt)
{
    CCDirector::sharedDirector()->end();
}

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void MainMenu::initAudio()
{
    SimpleAudioEngine::getInstance()->preloadEffect(clickSoundPath);
    SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
}

void MainMenu::ShowAchievements(Ref *pSender)
{
	SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
		SonarCocosHelper::GooglePlayServices::showAchievements();
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    SonarCocosHelper::RevMob::showBannerAd();

    if(!SonarCocosHelper::GooglePlayServices::isSignedIn())
    	SonarCocosHelper::GooglePlayServices::signIn();

    initBackButtonListener();
    
    initAudio();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto menuTitle = MenuItemImage::create(menuTitlePath, menuTitlePath);
    
    auto playItem = MenuItemImage::create(menuPlayIdlePath, menuPlayPressedPath, CC_CALLBACK_1(MainMenu::GoToGameScene, this));
    
    auto instructionsItem = MenuItemImage::create(menuInstructionsIdlePath, menuInstructionsPressedPath, CC_CALLBACK_1(MainMenu::GoToInstructionsScene, this));

    auto achievementItem = MenuItemImage::create(menuAchievementIdlePath, menuAchievementPressedPath, CC_CALLBACK_1(MainMenu::ShowAchievements, this));

    auto leaderboardItem = MenuItemImage::create(menuLeaderboardIdlePath, menuLeaderboardPressedPath, CC_CALLBACK_1(MainMenu::ShowLeaderboard, this));

    auto exitItem = MenuItemImage::create(menuExitIdlePath, menuExitPressedPath, CC_CALLBACK_1(MainMenu::ExitGame, this));

    auto infoItem = MenuItemImage::create(menuInfoIdlePath, menuInfoPressedPath, CC_CALLBACK_1(MainMenu::GoToInfoScene, this));

    infoItem->setPosition(Point(visibleSize.width + origin.x - (infoItem->getContentSize().width * 0.75f),
                                visibleSize.height - infoItem->getContentSize().height + (infoItem->getContentSize().width / 4) + origin.y));

    menuTitle->setPosition(Point((visibleSize.width / 2) + origin.x,
                                ((8.5f * visibleSize.height) / 10) + origin.y));

    playItem->setPosition(Point((visibleSize.width / 2) + origin.x,
                                ((6.2f * visibleSize.height) / 10) + origin.y));

    instructionsItem->setPosition(Point((visibleSize.width / 2) + origin.x,
                                ((5.3f * visibleSize.height) / 10) + origin.y));

    achievementItem->setPosition(Point((visibleSize.width / 2) + origin.x,
                                    ((4.1f * visibleSize.height) / 10) + origin.y));

    leaderboardItem->setPosition(Point((visibleSize.width / 2) + origin.x,
                                    ((2.9f * visibleSize.height) / 10) + origin.y));

    exitItem->setPosition(Point((visibleSize.width / 2) + origin.x,
                                ((1.7f * visibleSize.height) / 10) + origin.y));


    auto menu = Menu::create(menuTitle, playItem, instructionsItem, achievementItem, leaderboardItem, exitItem, infoItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);

    auto bg = cocos2d::LayerColor::create(Color4B(182, 249, 255, 255));
    this->addChild(bg, -2);
    
    auto sun = Sprite::create(sunPath);
    sun->setPosition(Point(origin.x + (visibleSize.width * 0.75f), origin.y + (visibleSize.height * 0.9f)));
    this->addChild(sun, -1);

    return true;
}
