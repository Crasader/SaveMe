#include "PauseScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include "SonarFrameworks.h"
#include "Constants.h"

USING_NS_CC;

using namespace CocosDenshion;

void PauseMenu::initBackButtonListener()
{
    auto listener = EventListenerKeyboard::create();
    
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){};
    listener->onKeyReleased = CC_CALLBACK_2(PauseMenu::onKeyPressed, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void PauseMenu::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
    	Director::getInstance()->end();
    }
}

void PauseMenu::Resume(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    Director::getInstance()->popScene();
}

void PauseMenu::GoToMainMenuScene(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    auto scene = MainMenu::createScene();
 
    Director::getInstance()->popScene();
    Director::getInstance()->replaceScene(TransitionFade::create(fadeDuration, scene));
}

void PauseMenu::Retry(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    auto scene = GameScreen::createScene();

    Director::getInstance()->popScene();
    Director::getInstance()->replaceScene(TransitionFade::create(fadeDuration, scene));
}

Scene* PauseMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = PauseMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PauseMenu::init()
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

    auto resumeItem = MenuItemImage::create(pauseResumeIdlePath, pauseResumePressedPath, CC_CALLBACK_1(PauseMenu::Resume, this));
    
    auto retryItem = MenuItemImage::create(pauseRetryIdlePath, pauseRetryPressedPath, CC_CALLBACK_1(PauseMenu::Retry, this));
    
    auto mainMenuItem = MenuItemImage::create(pauseMenuIdlePath, pauseMenuPressedPath, CC_CALLBACK_1(PauseMenu::GoToMainMenuScene, this));
    
    resumeItem->setPosition(Point((visibleSize.width / 2) + origin.x,
                                ((7 * visibleSize.height) / 10) + origin.y));

    retryItem->setPosition(Point((visibleSize.width / 2) + origin.x,
                                ((5 * visibleSize.height) / 10) + origin.y));

    mainMenuItem->setPosition(Point((visibleSize.width / 2) + origin.x,
                                ((3 * visibleSize.height) / 10) + origin.y));    

    auto menu = Menu::create(resumeItem, retryItem, mainMenuItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);

    auto bg = cocos2d::LayerColor::create(Color4B(182, 249, 255, 255));
    this->addChild(bg, -2);

    auto sun = Sprite::create(sunPath);
    sun->setPosition(Point(origin.x + (visibleSize.width * 0.75f), origin.y + (visibleSize.height * 0.9f)));
    this->addChild(sun, -1);

    return true;
}
