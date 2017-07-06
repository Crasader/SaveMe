#include "InstructionsScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include "SonarFrameworks.h"
#include "Constants.h"

USING_NS_CC;

using namespace CocosDenshion;

void Instructions::initBackButtonListener()
{
    auto listener = EventListenerKeyboard::create();
    
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){};
    listener->onKeyReleased = CC_CALLBACK_2(Instructions::onKeyPressed, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void Instructions::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
    	Director::getInstance()->end();
    }
}

void Instructions::GoToMainMenu(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    auto scene = MainMenu::createScene();

    Director::getInstance()->replaceScene(TransitionFade::create(fadeDuration, scene));
}

Scene* Instructions::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Instructions::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Instructions::init()
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

    auto objectiveLabel = Label::createWithTTF("Objective", fontPath, 18);
    objectiveLabel->setColor(ccc3(255, 0, 0));
    objectiveLabel->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    objectiveLabel->setPosition(Point((visibleSize.width / 2) + origin.x, ((9.5f * visibleSize.height) / 10) + origin.y));
    this->addChild(objectiveLabel, 1);

    auto objective1 = Label::createWithTTF("Use accelerometer to dodge", fontPath, 12);
    objective1->setColor(ccc3(255, 0, 0));
    objective1->enableOutline(Color4B(0, 0, 0, 100), 0.5f);
    
    objective1->setPosition(Point((visibleSize.width / 2) + origin.x, ((8.5f * visibleSize.height) / 10) + origin.y));
    this->addChild(objective1, 1);

    auto objective2 = Label::createWithTTF("and save the butterfly", fontPath, 12);
    objective2->setColor(ccc3(255, 0, 0));
    objective2->enableOutline(Color4B(0, 0, 0, 100), 0.5f);
    
    objective2->setPosition(Point((visibleSize.width / 2) + origin.x, ((8.1f * visibleSize.height) / 10) + origin.y));
    this->addChild(objective2, 1);

    auto objective3 = Label::createWithTTF("from the thunderstorm.", fontPath, 12);
    objective3->setColor(ccc3(255, 0, 0));
    objective3->enableOutline(Color4B(0, 0, 0, 100), 0.5f);
    
    objective3->setPosition(Point((visibleSize.width / 2) + origin.x, ((7.7f * visibleSize.height) / 10) + origin.y));
    this->addChild(objective3, 1);

    auto pickupLabel = Label::createWithTTF("Pickups", fontPath, 18);
    pickupLabel->setColor(ccc3(255, 0, 0));
    pickupLabel->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    pickupLabel->setPosition(Point((visibleSize.width / 2) + origin.x, ((7.0f * visibleSize.height) / 10) + origin.y));
    this->addChild(pickupLabel, 1);

    auto noteLabel = Label::createWithTTF("* Only one pickup takes effect at a time", fontPath, 8);
    noteLabel->setColor(ccc3(255, 0, 0));
    noteLabel->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    noteLabel->setPosition(Point((visibleSize.width / 2) + origin.x, ((6.5f * visibleSize.height) / 10) + origin.y));
    this->addChild(noteLabel, 1);

    auto pickupX = Sprite::create("Images/GameScreen/pickup4.png");
    auto labelX = Label::createWithTTF("Either shrinks or expands", fontPath, 12);
    labelX->setColor(ccc3(255, 0, 0));
    labelX->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    pickupX->setPosition(Point((visibleSize.width / 2) + origin.x, ((6.0f * visibleSize.height) / 10) + origin.y));
    labelX->setPosition(Point((visibleSize.width / 2) + origin.x, ((5.5f * visibleSize.height) / 10) + origin.y));

    this->addChild(pickupX, 1);
    this->addChild(labelX, 1);

    auto pickup1 = Sprite::create("Images/GameScreen/pickup1.png");
    auto label1 = Label::createWithTTF("Increases level by 1", fontPath, 12);
    label1->setColor(ccc3(255, 0, 0));
    label1->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    pickup1->setPosition(Point((visibleSize.width / 2) + origin.x, ((5.0f * visibleSize.height) / 10) + origin.y));
    label1->setPosition(Point((visibleSize.width / 2) + origin.x, ((4.5f * visibleSize.height) / 10) + origin.y));

    this->addChild(pickup1, 1);
    this->addChild(label1, 1);

    auto pickup2 = Sprite::create("Images/GameScreen/pickup2.png");
    auto label2 = Label::createWithTTF("Restores original level", fontPath, 12);
    label2->setColor(ccc3(255, 0, 0));
    label2->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    pickup2->setPosition(Point((visibleSize.width / 2) + origin.x, ((4.0f * visibleSize.height) / 10) + origin.y));
    label2->setPosition(Point((visibleSize.width / 2) + origin.x, ((3.5f * visibleSize.height) / 10) + origin.y));

    this->addChild(pickup2, 1);
    this->addChild(label2, 1);

    auto pickup3 = Sprite::create("Images/GameScreen/pickup3.png");
    auto label3 = Label::createWithTTF("Grants 3 charges of shield", fontPath, 12);
    label3->setColor(ccc3(255, 0, 0));
    label3->enableOutline(Color4B(0, 0, 0, 100), 0.5f);

    pickup3->setPosition(Point((visibleSize.width / 2) + origin.x, ((3.0f * visibleSize.height) / 10) + origin.y));
    label3->setPosition(Point((visibleSize.width / 2) + origin.x, ((2.5f * visibleSize.height) / 10) + origin.y));

    this->addChild(pickup3, 1);
    this->addChild(label3, 1);

    auto mainMenuItem = MenuItemImage::create(pauseMenuIdlePath, pauseMenuPressedPath, CC_CALLBACK_1(Instructions::GoToMainMenu, this));

    mainMenuItem->setPosition(Point((visibleSize.width / 2) + origin.x, ((1.5f * visibleSize.height) / 10) + origin.y));

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
