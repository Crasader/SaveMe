#include "GameScene.h"
#include "PauseScene.h"
#include "GameOverScene.h"
#include "Constants.h"
#include "SimpleAudioEngine.h"
#include "SonarFrameworks.h"
#include "Achievements.h"
#include <sstream>
#include <string>
#include <iostream>
#include "../cocos2d/cocos/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

USING_NS_CC;

using namespace CocosDenshion;

void GameScreen::initBackButtonListener()
{
    auto listener = EventListenerKeyboard::create();
    
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){};
    listener->onKeyReleased = CC_CALLBACK_2(GameScreen::onKeyPressed, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScreen::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
    	Director::getInstance()->end();
    }
}

void GameScreen::initAccelerometer()
{
    
    Device::setAccelerometerEnabled(true);
    
    auto listener = EventListenerAcceleration::create(CC_CALLBACK_2(GameScreen::movePlayerByAccelerometer, this));
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScreen::movePlayerIfPossible(float newX)
{
    float sprWidth = butterflySprite->getBoundingBox().size.width;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    if(newX > origin.x && newX < (origin.x + visibleSize.width - butterflySprite->getContentSize().width))
    {
        butterflySprite->setPositionX(newX);
    }
}

void GameScreen::movePlayerByAccelerometer(cocos2d::Acceleration* acceleration, cocos2d::Event* event)
{
    movePlayerIfPossible(butterflySprite->getPositionX() + (acceleration->x * accelerationMult));
}

void GameScreen::GoToPauseScene(cocos2d::Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect(clickSoundPath);

    auto scene = PauseMenu::createScene();
    
    Director::getInstance()->pushScene(TransitionFade::create(fadeDuration, scene));
}

void GameScreen::GoToGameOverScene(cocos2d::Ref *pSender)
{
	auto scene = GameOver::createScene(score);
    
    Director::getInstance()->replaceScene(TransitionFade::create(fadeDuration, scene));
}

void GameScreen::ScaleFinished(cocos2d::Ref *pSender)
{
    bScaled = false;
}

Scene* GameScreen::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vect(0,0));
    
    // 'layer' is an autorelease object
    auto layer = GameScreen::create();

    //enable debug draw
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void GameScreen::initPhysics()
{
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScreen::onCollision,this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener,this);
}

void GameScreen::setPhysicsBody(cocos2d::Sprite* sprite, float radiusMultiplier)
{
    auto body = PhysicsBody::createCircle((sprite->getContentSize().width / 2) * radiusMultiplier);
    body->setContactTestBitmask(true);
    body->setDynamic(true);
    sprite->setPhysicsBody(body);
}

bool GameScreen::onCollision(PhysicsContact& contact)
{
 
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto bodyA = contact.getShapeA()->getBody();
    auto bodyB = contact.getShapeB()->getBody();

    bool bFound = false;

    if((butterflySprite->getPhysicsBody() == bodyA))
    {
        for(int i = 0; i < raindrops.size(); ++i)
        {
            if(raindrops[i]->getPhysicsBody() == bodyB)
            {
                this->removeChild(raindrops[i]);
                raindrops.erase(raindrops.begin() + i);       
                bFound = true;

                if(shield > 0)
                    --shield;
                else
                {
                    --currentLevel;
                    auto action = MoveBy::create(2.0, Point(0, -visibleSize.height / 10.0f));
                    butterflySprite->runAction(action);
                }

                break;
            }
        }

        if(!bFound)
        {
            for(int i = 0; i < hailstones.size(); ++i)
            {
                if(hailstones[i]->getPhysicsBody() == bodyB)
                {
                    this->removeChild(hailstones[i]);
                    hailstones.erase(hailstones.begin() + i);       
                    
                    if(shield > 0)
                        --shield;
                    else
                    {
                        currentLevel = currentLevel - 2;
                        auto action = MoveBy::create(2.0, Point(0, -visibleSize.height / 5.0f));
                        butterflySprite->runAction(action);
                    }

                    bFound = true;
                    break;
                }
            }    
        }       

        if(!bFound)
        {
            if(pickup->getPhysicsBody() == bodyB)
            {
                this->removeChild(pickup);
                pickup = nullptr;

                if(pickupRoll == 1)
                {
                	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
                	{
                		SonarCocosHelper::GooglePlayServices::incrementAchievement(ImDry, 1);
                		SonarCocosHelper::GooglePlayServices::incrementAchievement(INeedWater, 1);
                	}

                    if(currentLevel < 5 && !bScaled && (shield <= 0))
                    {
                        ++currentLevel;
                        auto action = MoveBy::create(2.0, Point(0, visibleSize.height / 10.0f));
                        butterflySprite->runAction(action);            
                    }
                }
                else if(pickupRoll == 2)
                {
                	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
					{
						SonarCocosHelper::GooglePlayServices::incrementAchievement(Flyer, 1);
						SonarCocosHelper::GooglePlayServices::incrementAchievement(FlyLikeABird, 1);
					}

                    if(currentLevel < 5 && !bScaled && (shield <= 0))
                    {
                        while(currentLevel < 5)
                        {
                            auto action = MoveBy::create(2.0, Point(0, visibleSize.height / 10.0f));
                            butterflySprite->runAction(action);                   

                            ++currentLevel;
                        }
                    }
                }
                else if(pickupRoll == 3)
                {
                	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
					{
						SonarCocosHelper::GooglePlayServices::incrementAchievement(ProtectMe, 1);
						SonarCocosHelper::GooglePlayServices::incrementAchievement(Indestructable, 1);
					}

                	if(!bScaled && (shield <= 0))
                	{
                        shield = shield + 3;

                        if(shield > 3)
                            shield = 3;

                        shieldSprite->setVisible(true);
                	}
                }
                else if(pickupRoll == 4)
                {
                	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
					{
						SonarCocosHelper::GooglePlayServices::incrementAchievement(Daring, 1);
						SonarCocosHelper::GooglePlayServices::incrementAchievement(LetsTryThat, 1);
					}

                    if(!bScaled && (shield <= 0))
                    {
                        bScaled = true;

                        // Shrink the butterfly
                        auto scale1 = ScaleTo::create(1, 0.5f, 0.5f);
                        auto scale2 = ScaleTo::create(5, 0.5f, 0.5f);
                        auto scale3 = ScaleTo::create(1, 1, 1);
                        auto scaleFinished = CallFuncN::create(CC_CALLBACK_1(GameScreen::ScaleFinished, this));
            
                        butterflySprite->runAction(Sequence::create(scale1, scale2, scale3, scaleFinished, nullptr));
                    }
                }
                else
                {
                	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
					{
						SonarCocosHelper::GooglePlayServices::incrementAchievement(Daring, 1);
						SonarCocosHelper::GooglePlayServices::incrementAchievement(LetsTryThat, 1);
					}

                    if(!bScaled && (shield <= 0))
                    {
                        bScaled = true;

                        // Expand the butterfly
                        auto scale1 = ScaleTo::create(1, 1.5f, 1.5f);
                        auto scale2 = ScaleTo::create(5, 1.5f, 1.5f);
                        auto scale3 = ScaleTo::create(1, 1, 1);
                        auto scaleFinished = CallFuncN::create(CC_CALLBACK_1(GameScreen::ScaleFinished, this));
            
                        butterflySprite->runAction(Sequence::create(scale1, scale2, scale3, scaleFinished, nullptr));    
                    }
                }
            }
        }
    }
    else if((butterflySprite->getPhysicsBody() == bodyB))
    {
        for(int i = 0; i < raindrops.size(); ++i)
        {
            if(raindrops[i]->getPhysicsBody() == bodyA)
            {
                this->removeChild(raindrops[i]);
                raindrops.erase(raindrops.begin() + i);       
                bFound = true;
        
                if(shield > 0)
                    --shield;
                else
                {
                    --currentLevel;
                    auto action = MoveBy::create(2.0, Point(0, -visibleSize.height / 10.0f));
                    butterflySprite->runAction(action);
                }

                break;
            }
        }

        if(!bFound)
        {
            for(int i = 0; i < hailstones.size(); ++i)
            {
                if(hailstones[i]->getPhysicsBody() == bodyA)
                {
                    this->removeChild(hailstones[i]);
                    hailstones.erase(hailstones.begin() + i);       
                    
                    if(shield > 0)
                        --shield;
                    else
                    {
                        currentLevel = currentLevel - 2;
                        auto action = MoveBy::create(2.0, Point(0, -visibleSize.height / 5.0f));
                        butterflySprite->runAction(action);
                    }

                    bFound = true;
                    break;
                }
            }    
        }

        if(!bFound)
        {
            if(pickup->getPhysicsBody() == bodyA)
            {
                this->removeChild(pickup);
                pickup = nullptr;

                if(pickupRoll == 1)
                {
                	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
					{
						SonarCocosHelper::GooglePlayServices::incrementAchievement(ImDry, 1);
						SonarCocosHelper::GooglePlayServices::incrementAchievement(INeedWater, 1);
					}

                    if(currentLevel < 5 && !bScaled && (shield <= 0))
                    {
                        ++currentLevel;
                        auto action = MoveBy::create(2.0, Point(0, visibleSize.height / 10.0f));
                        butterflySprite->runAction(action);            
                    }
                }
                else if(pickupRoll == 2)
                {
                	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
					{
						SonarCocosHelper::GooglePlayServices::incrementAchievement(Flyer, 1);
						SonarCocosHelper::GooglePlayServices::incrementAchievement(FlyLikeABird, 1);
					}

                    if(currentLevel < 5 && !bScaled && (shield <= 0))
                    {
                        while(currentLevel < 5)
                        {
                            auto action = MoveBy::create(2.0, Point(0, visibleSize.height / 10.0f));
                            butterflySprite->runAction(action);                   

                            ++currentLevel;
                        }
                    }
                }
                else if(pickupRoll == 3)
                {
                	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
					{
						SonarCocosHelper::GooglePlayServices::incrementAchievement(ProtectMe, 1);
						SonarCocosHelper::GooglePlayServices::incrementAchievement(Indestructable, 1);
					}

                	if(!bScaled && (shield <= 0))
                	{
                		shield = shield + 3;

						if(shield > 3)
							shield = 3;

						shieldSprite->setVisible(true);
                	}
                }
                else if(pickupRoll == 4)
                {
                	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
					{
						SonarCocosHelper::GooglePlayServices::incrementAchievement(Daring, 1);
						SonarCocosHelper::GooglePlayServices::incrementAchievement(LetsTryThat, 1);
					}

                    if(!bScaled && (shield <= 0))
                    {
                        bScaled = true;

                        // Shrink the butterfly
                        auto scale1 = ScaleTo::create(1, 0.5f, 0.5f);
                        auto scale2 = ScaleTo::create(5, 0.5f, 0.5f);
                        auto scale3 = ScaleTo::create(1, 1, 1);
                        auto scaleFinished = CallFuncN::create(CC_CALLBACK_1(GameScreen::ScaleFinished, this));
            
                        butterflySprite->runAction(Sequence::create(scale1, scale2, scale3, scaleFinished, nullptr));    
                    }
                }
                else
                {
                	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
					{
						SonarCocosHelper::GooglePlayServices::incrementAchievement(Daring, 1);
						SonarCocosHelper::GooglePlayServices::incrementAchievement(LetsTryThat, 1);
					}

                    if(!bScaled && (shield <= 0))
                    {
                        bScaled = true;

                        // Expand the butterfly
                        auto scale1 = ScaleTo::create(1, 1.5f, 1.5f);
                        auto scale2 = ScaleTo::create(5, 1.5f, 1.5f);
                        auto scale3 = ScaleTo::create(1, 1, 1);
                        auto scaleFinished = CallFuncN::create(CC_CALLBACK_1(GameScreen::ScaleFinished, this));
            
                        butterflySprite->runAction(Sequence::create(scale1, scale2, scale3, scaleFinished, nullptr));    
                    }
                }
            }
        }
    }
    
    return false;
}

void GameScreen::spawnElement(float dt)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    int rainRoll = (arc4random() % (6 - rainDensity)) + 1;

    if(rainRoll == 1)
    {
        Sprite *tempDrop = Sprite::create(raindropPath);

        int xRandomPosition = (arc4random() % (int)(visibleSize.width - (tempDrop->getContentSize().width / 2))) + (tempDrop->getContentSize().width / 2);
        xRandomPosition = xRandomPosition + (windSpeed * 0.75f * visibleSize.width);
        tempDrop->setPosition(Point(xRandomPosition + origin.x, visibleSize.height + origin.y));
 
        float rotationAngle = CC_RADIANS_TO_DEGREES(atan2(-windSpeed * visibleSize.width, -raindropSpeed * visibleSize.height));
        rotationAngle = rotationAngle - 180.0f;

        auto action = RotateBy::create(0.0f, rotationAngle);
        tempDrop->runAction(action);

        setPhysicsBody(tempDrop, 0.5f);

        raindrops.push_back(tempDrop);
        this->addChild(raindrops[raindrops.size() - 1], -1);
    }

    int cornerDropRoll = (arc4random() % 2) + 1;

    if(cornerDropRoll == 1)
    {
    	Sprite *tempDrop = Sprite::create(raindropPath);

        int positionRoll = (arc4random() % 2) + 1;

        if(positionRoll == 1)
            tempDrop->setPosition(Point(origin.x + (windSpeed * 0.75f * visibleSize.width), visibleSize.height + origin.y));
        else
            tempDrop->setPosition(Point(origin.x + visibleSize.width - (tempDrop->getContentSize().width) + (windSpeed * 0.75f * visibleSize.width), visibleSize.height + origin.y));

        float rotationAngle = CC_RADIANS_TO_DEGREES(atan2(-windSpeed * visibleSize.width, -raindropSpeed * visibleSize.height));
        rotationAngle = rotationAngle - 180.0f;

        auto action = RotateBy::create(0.0f, rotationAngle);
        tempDrop->runAction(action);

        setPhysicsBody(tempDrop, 0.5f);

        raindrops.push_back(tempDrop);
        this->addChild(raindrops[raindrops.size() - 1], -1);   
    }

    if(score > 250)
    {
        int hailRoll = (arc4random() % (10 - hailDensity)) + 1;

        if(hailRoll == 1)
        {
            int hailIndex = (arc4random() % 3) + 1;        

            __String* hailstoneString = __String::createWithFormat("Images/GameScreen/hailstone%i.png", hailIndex);

            Sprite *tempHail = Sprite::create(hailstoneString->getCString());

            int xRandomPosition = (arc4random() % (int)(visibleSize.width - (tempHail->getContentSize().width / 2))) + (tempHail->getContentSize().width / 2);
            tempHail->setPosition(Point(xRandomPosition + origin.x, visibleSize.height + origin.y));
        
            setPhysicsBody(tempHail, 0.8f);

            hailstones.push_back(tempHail);
            this->addChild(hailstones[hailstones.size() - 1], -1);
        }
    }
}

void GameScreen::updateRainDensity(void)
{
    if(rainDensity < 5)
    {
        if((score % 25) == 0)
        {
            ++rainDensity;
        }
    }

}

void GameScreen::updateHailDensity(void)
{
    if(score > 250 && hailDensity < 5)
    {
        if((score % 150) == 0)
        {
            ++hailDensity;
        }
    }

}

void GameScreen::updateWind(float dt)
{
    int roll = (arc4random() % 3);
    --roll;

    windSpeed = windSpeed + (roll / 10.0f);

    if(windSpeed < 0)
        windSpeed = 0;
    else if(windSpeed > 1.0f)
        windSpeed = 1.0f;
}

void GameScreen::spawnCloud(float dt)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    int spawnRoll = (arc4random() % 2);

    if(spawnRoll == 0)
    {
        int roll = (arc4random() % 3) + 1;

        __String* cloudString = __String::createWithFormat("Images/GameScreen/cloud%i.png", roll);

        auto cloud = Sprite::create(cloudString->getCString());    

        int yPosition = (arc4random() % (int)(visibleSize.height / 5.0f)) + origin.y + (visibleSize.height * 0.8f);
        cloud->setPosition(origin.x + visibleSize.width + cloud->getContentSize().width, origin.y + yPosition);

        auto action = MoveTo::create(cloudSpeed, Point(origin.x - cloud->getContentSize().width, cloud->getPositionY()));
        
        cloud->runAction(action);    

        clouds.push_back(cloud);        
        this->addChild(clouds[clouds.size() - 1], -1);
    }
}

void GameScreen::spawnPickup(void)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    pickupRoll = (arc4random() % 5) + 1;

    __String* pickupString = __String::createWithFormat("Images/GameScreen/pickup%i.png", pickupRoll);

    pickup = Sprite::create(pickupString->getCString());

    int xRandomPosition = (arc4random() % (int)(visibleSize.width - (pickup->getContentSize().width / 2))) + (pickup->getContentSize().width / 2);
    pickup->setPosition(Point(xRandomPosition + origin.x, visibleSize.height + origin.y));

    setPhysicsBody(pickup, 1.0f);    

    this->addChild(pickup, -1);
}

void GameScreen::incrementScore(void)
{
    ++score;

    __String* scoreString = __String::createWithFormat("Score : %i", score);
    scoreLabel->setString(scoreString->getCString());

    updateRainDensity();
    updateHailDensity();

    if((score % 50) == 0 && pickup == nullptr)
        spawnPickup();
}

void GameScreen::removeTint(void)
{
    shieldSprite->setVisible(false);
}

void GameScreen::update(float dt)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    shieldSprite->setPosition(Point(butterflySprite->getPositionX(), butterflySprite->getPositionY()));

    if(shield <= 0)
        removeTint();

    if(currentLevel <= 0)
    {
        auto moveFinished = CallFuncN::create(CC_CALLBACK_1(GameScreen::GoToGameOverScene, this));
        auto moveBy = MoveBy::create(2.0, Point(0.0f, -visibleSize.height / 1000.0f));
        auto sequence = Sequence::create(moveBy, moveFinished, nullptr);
        butterflySprite->runAction(sequence);
    }

    for(int i = 0; i < clouds.size(); ++i)
    {
        if(clouds[i]->getPositionX() < (origin.x - (clouds[i]->getContentSize().width / 2)))
        {
            this->removeChild(clouds[i]);
            clouds.erase(clouds.begin() + i);
        }
    }

    for (int i = 0; i < raindrops.size(); i++)
    {
        raindrops[i]->setPosition(Point(raindrops[i]->getPosition().x - (windSpeed * visibleSize.width * dt),
                                        raindrops[i]->getPosition().y - (raindropSpeed * visibleSize.height * dt)));

        if (raindrops[i]->getPosition().y < origin.y - (visibleSize.height / 5.0f))
        {
            this->removeChild(raindrops[i]);
            raindrops.erase(raindrops.begin() + i);

            incrementScore();
        }
    }

    for (int i = 0; i < hailstones.size(); i++)
    {
        hailstones[i]->setPosition(Point(hailstones[i]->getPosition().x, hailstones[i]->getPosition().y - (hailstoneSpeed * visibleSize.height * dt)));

        if (hailstones[i]->getPosition().y < origin.y - (visibleSize.height / 5.0f))
        {
            this->removeChild(hailstones[i]);
            hailstones.erase(hailstones.begin() + i);

            incrementScore();
        }
    }

    if(pickup != nullptr)
    {
        float speed;

        if(pickupRoll == 1)
            speed = upPickupSpeed;
        else if(pickupRoll == 2)
            speed = allUpPickupSpeed;
        else if(pickupRoll == 3)
            speed = shieldPickupSpeed;
        else
            speed = mysteryPickupSpeed;

        pickup->setPosition(Point(pickup->getPosition().x, pickup->getPosition().y - (speed * visibleSize.height * dt)));        

        if (pickup->getPosition().y < origin.y - (visibleSize.height / 5.0f))
        {
            this->removeChild(pickup);
            pickup = nullptr;
        }
    }
}

// on "init" you need to initialize your instance
bool GameScreen::init()
{
	bIncremented = false;
    bScaled= false;
    pickup = nullptr;
    shield = 0;
    score = 0;
    rainDensity = 1;
    hailDensity = 1;
    windSpeed = 0.0f;

    if(!bIncremented)
    {
    	bIncremented = true;

    	if(SonarCocosHelper::GooglePlayServices::isSignedIn())
		{
			SonarCocosHelper::GooglePlayServices::unlockAchievement(Welcome);
		}
    }

    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    SonarCocosHelper::RevMob::showBannerAd();

    initAccelerometer();
    initPhysics();
    initBackButtonListener();

    currentLevel = initialLevel;

    this->schedule(schedule_selector(GameScreen::spawnElement), 0.25f);
    this->schedule(schedule_selector(GameScreen::updateWind), 5.0f);
    this->schedule(schedule_selector(GameScreen::spawnCloud), 10.0f);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto pauseItem = MenuItemImage::create(pauseIdlePath, pausePressedPath, CC_CALLBACK_1(GameScreen::GoToPauseScene, this));
    
    pauseItem->setPosition(Point(pauseItem->getContentSize().width - (pauseItem->getContentSize().width / 4) + origin.x,
                                 visibleSize.height - pauseItem->getContentSize().height + (pauseItem->getContentSize().width / 4) + origin.y));
    
    auto menu = Menu::create(pauseItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);

    butterflySprite = Sprite::create(butterfly1Path);
    butterflySprite->setAnchorPoint(Vec2(0, 0));
    butterflySprite->setPosition(Point(origin.x + (visibleSize.width * 0.1f), origin.y + ((visibleSize.height * currentLevel) / 10.0f)));
    setPhysicsBody(butterflySprite, 1.0f);
    this->addChild(butterflySprite, 1);

    shieldSprite = Sprite::create("Images/GameScreen/shield.png");
    shieldSprite->setPosition(Point(butterflySprite->getPositionX(), butterflySprite->getPositionY()));
    shieldSprite->setAnchorPoint(Vec2(0, 0));
    this->addChild(shieldSprite, 1);
    shieldSprite->setVisible(false);

    Vector<SpriteFrame*> frames;
    
    Size playerSize = butterflySprite->getContentSize();
    frames.pushBack(SpriteFrame::create(butterfly1Path, Rect(0, 0, playerSize.width, playerSize.height)));
    frames.pushBack(SpriteFrame::create(butterfly2Path, Rect(0, 0, playerSize.width, playerSize.height)));
    frames.pushBack(SpriteFrame::create(butterfly3Path, Rect(0, 0, playerSize.width, playerSize.height)));
    frames.pushBack(SpriteFrame::create(butterfly2Path, Rect(0, 0, playerSize.width, playerSize.height)));
    auto animation = Animation::createWithSpriteFrames(frames, animationSwitchTime);
    auto animate = Animate::create(animation);
    butterflySprite->runAction(RepeatForever::create(animate));

    __String* scoreString = __String::createWithFormat("Score : %i", score);

    scoreLabel = Label::createWithTTF(scoreString->getCString(), fontPath, 18);
    scoreLabel->setPosition(Point(origin.x + (visibleSize.width / 2.0f), pauseItem->getPositionY()));
    scoreLabel->enableOutline(Color4B(0, 0, 0, 100), 1);
    this->addChild(scoreLabel, 1);

    setKeepScreenOnJni(true);

    this->scheduleUpdate();

    auto bg = cocos2d::LayerColor::create(Color4B(182, 249, 255, 255));
    this->addChild(bg, -2);

    auto sun = Sprite::create(sunPath);
    sun->setPosition(Point(origin.x + (visibleSize.width * 0.75f), origin.y + (visibleSize.height * 0.9f)));
    this->addChild(sun, -1);

    return true;
}
