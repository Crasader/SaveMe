#include "InfoScreen.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include "SonarFrameworks.h"
#include "Constants.h"

USING_NS_CC;

using namespace CocosDenshion;

void InfoScreen::initBackButtonListener()
{
    auto listener = EventListenerKeyboard::create();

    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){};
    listener->onKeyReleased = CC_CALLBACK_2(InfoScreen::onKeyPressed, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void InfoScreen::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
    	Director::getInstance()->end();
    }
}

void InfoScreen::GoToMainMenu(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    auto scene = MainMenu::createScene();

    Director::getInstance()->replaceScene(TransitionFade::create(fadeDuration, scene));
}

Scene* InfoScreen::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = InfoScreen::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool InfoScreen::init()
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

    auto label1 = Label::createWithTTF("Developer", fontPath, 18);
    label1->setColor(ccc3(255, 0, 0));
    label1->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    label1->setPosition(Point((visibleSize.width / 2) + origin.x, ((9.0f * visibleSize.height) / 10) + origin.y));
    this->addChild(label1, 1);

    auto label2 = Label::createWithTTF("Parikshit Saraswat", fontPath, 12);
    label2->setColor(ccc3(255, 0, 0));
    label2->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    label2->setPosition(Point((visibleSize.width / 2) + origin.x, ((8.0f * visibleSize.height) / 10) + origin.y));
    this->addChild(label2, 1);

    auto label3 = Label::createWithTTF("Attributions", fontPath, 18);
    label3->setColor(ccc3(255, 0, 0));
    label3->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    label3->setPosition(Point((visibleSize.width / 2) + origin.x, ((6.0f * visibleSize.height) / 10) + origin.y));
    this->addChild(label3, 1);

    auto label4 = Label::createWithTTF("Butterfly sprite sheet", fontPath, 12);
    label4->setColor(ccc3(255, 0, 0));
    label4->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    label4->setPosition(Point((visibleSize.width / 2) + origin.x, ((5.0f * visibleSize.height) / 10) + origin.y));
    this->addChild(label4, 1);

    auto label5 = Label::createWithTTF("2005-2013 Julien Jorge", fontPath, 12);
    label5->setColor(ccc3(255, 0, 0));
    label5->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    label5->setPosition(Point((visibleSize.width / 2) + origin.x, ((4.2f * visibleSize.height) / 10) + origin.y));
    this->addChild(label5, 1);

    auto label6 = Label::createWithTTF("<julien.jorge@stuff-o-matic.com>", fontPath, 12);
    label6->setColor(ccc3(255, 0, 0));
    label6->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    label6->setPosition(Point((visibleSize.width / 2) + origin.x, ((3.4f * visibleSize.height) / 10) + origin.y));
    this->addChild(label6, 1);

    auto mainMenuItem = MenuItemImage::create(pauseMenuIdlePath, pauseMenuPressedPath, CC_CALLBACK_1(InfoScreen::GoToMainMenu, this));

    mainMenuItem->setPosition(Point((visibleSize.width / 2) + origin.x, ((2.0f * visibleSize.height) / 10) + origin.y));

    auto menu = Menu::create(mainMenuItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);

    auto bg = cocos2d::LayerColor::create(Color4B(182, 249, 255, 255));
    this->addChild(bg, -2);

    auto sun = Sprite::create(sunPath);
    sun->setPosition(Point(origin.x + (visibleSize.width * 0.75f), origin.y + (visibleSize.height * 0.9f)));
    this->addChild(sun, -1);

    return true;
}
