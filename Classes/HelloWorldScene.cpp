/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    //auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    //if (label == nullptr)
    //{
    //    problemLoading("'fonts/Marker Felt.ttf'");
    //}
    //else
    //{
    //    // position the label on the center of the screen
    //    label->setPosition(Vec2(origin.x + visibleSize.width/2,
    //                            origin.y + visibleSize.height - label->getContentSize().height));

    //    // add the label as a child to this layer
    //    this->addChild(label, 1);
    //}

	//リソース読み込み
	//背景1
	backG = Sprite::create("backG.png");
	this->addChild(backG);
	backG->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2 ));
    backG->setScale(2.3f);

	//キノコ
	mush = Sprite::create("mushroom.png");
	this->addChild(mush);
	mush->setPosition(Vec2(1200,150));
	mush->setScale(0.4f);

	//走るマリオ
	marioRun = Sprite::create("mario-run.png");
	this->addChild(marioRun);
	marioRun->setPosition(Vec2(100, 220));
	marioRun->setScale(0.6f);

	//背景2
	backS = Sprite::create("backS.png");
	this->addChild(backS);
	backS->setPosition(600, 400);
	backS->setScale(2.3f);
	backS->setOpacity(0);
	//正面マリオ
	marioF = Sprite::create("mario-front.png");
	this->addChild(marioF);
	marioF->setPosition(700, 100);
	marioF->setOpacity(0);

	//BGM1
	id =experimental::AudioEngine::play2d("3684.mp3",true);

	//アクション生成
	TintTo*tintTo1 = TintTo::create(3.0f, Color3B(100, 250, 100));
	TintTo*tintTo2 = TintTo::create(3.0f, Color3B(255, 100, 100));
	TintTo*tintTo3 = TintTo::create(3.0f, Color3B(100, 100, 250));

	MoveBy*moveMario = MoveBy::create(5.0f, Vec2(700, 0));
	MoveTo*moveMushu = MoveTo::create(5.0f, Vec2(800, 150));
	Blink*marioBlink = Blink::create(1.0f, 4);
	ScaleBy*marioRunScale = ScaleBy::create(1.0f,5.0f);
	RotateBy*mariorunRote = RotateBy::create(1.0f, 360);
	//一括フェード
	CallFunc*fade = CallFunc::create(CC_CALLBACK_0(HelloWorld::spritsfade, this));
	//後半画像セット
	CallFunc*change = CallFunc::create(CC_CALLBACK_0(HelloWorld::spritsChange, this));

	//キノコアクション
	Sequence*mushtint = Sequence::create(tintTo1, tintTo2, tintTo3,nullptr);
	RepeatForever*kinoko = RepeatForever::create(mushtint);
	Spawn*mushAct = Spawn::create(mushtint, moveMushu,nullptr);

	//走るマリオアクション
	Spawn*murioRunSpw = Spawn::create(marioRunScale, mariorunRote, nullptr);
	Sequence*marioRunseq = Sequence::create(moveMario, marioBlink,murioRunSpw,fade,change,nullptr);

	//アクション実行
	mush->runAction(mushAct);
	marioRun->runAction(marioRunseq);


	//updateが呼び出されるようにする
	this->scheduleUpdate();


   return true;
}



void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::update(float delta)
{

}


//SE再生
void HelloWorld::callSE(std::string name)
{
	int oudioID;
	oudioID = experimental::AudioEngine::play2d(name);
	
}

////フェード
void HelloWorld::spritsfade()
{
	FadeOut*fade = FadeOut::create(1.0f);
	marioRun->runAction(fade);
	mush->runAction(fade->clone());
	backG->runAction(fade->clone());

	//for (auto child : this->getChildren())
	//{
	//	FadeOut*fade = FadeOut::create(1.0f);
	//	child->runAction(fade);
	//}

	this->spritsChange();
}

//後半
void HelloWorld::spritsChange()
{
	

	FadeIn*fadein = FadeIn::create(1.0f);
	DelayTime*delay = DelayTime::create(1.5f);
	SkewTo*skew1 = SkewTo::create(1.0f, 10, 10);
	SkewTo*skew2 = SkewTo::create(1.0f, -10, 5);
	SkewTo*skew3 = SkewTo::create(1.0f, 0, 0);
	SkewTo*skew4 = SkewTo::create(1.0f, 10, -5);
	SkewTo*skew5 = SkewTo::create(1.0f, -10, -10);
	Sequence*skew6 = Sequence::create(skew1, skew2,skew3,skew4,skew5, nullptr);
	RepeatForever*skew = RepeatForever::create(skew6);

	CallFunc*mob = CallFunc::create(CC_CALLBACK_0(HelloWorld::mobset, this));
	CallFunc*sound = CallFunc::create(CC_CALLBACK_0(HelloWorld::soundset, this));

	Sequence*seqB = Sequence::create(delay, fadein,sound,mob, nullptr);
	Sequence*seqM = Sequence::create(delay, fadein, nullptr);

	backS->runAction(seqB);
	marioF->runAction(seqM->clone());
	marioF->runAction(skew);

}

void HelloWorld::soundset()
{
	experimental::AudioEngine::pause(id);
	experimental::AudioEngine::play2d("8588.mp3",true);
}


//モブ
void HelloWorld::mobset()
{
	//ノコノコ1号
	noco1 = Sprite::create("noco.png");
	this->addChild(noco1);
	noco1->setScale(0.6f);
	noco1->setPosition(1000, 500);
	noco1->setFlippedX(true);

	//ノコノコ2号
	noco2 = Sprite::create("noco.png");
	this->addChild(noco2);
	noco2->setScale(0.6f);
	noco2->setPosition(200, 100);

	//クリボー1号
	kuri1 = Sprite::create("Goomba.png");
	this->addChild(kuri1);
	kuri1->setScale(0.08f);
	kuri1->setPosition(300, 400);

	//クリボー2号
	kuri2 = Sprite::create("Goomba.png");
	this->addChild(kuri2);
	kuri2->setScale(0.08f);
	kuri2->setPosition(1000, 300);
	kuri2->setRotation(90.0f);

	this->mobmove();

}

//モブ動作
void HelloWorld::mobmove()
{
	//クリボーアクション
	RotateBy*rotemob = RotateBy::create(0.5f, 45.0f);
	ScaleTo*kuriScale1 = ScaleTo::create(9.0f, 0.5f);
	ScaleTo*kuriScale2 = ScaleTo::create(1.0f, 0.08f);
	DelayTime*delay1 = DelayTime::create(5.0f);
	Sequence*kuriScale = Sequence::create(kuriScale1, kuriScale2,delay1,nullptr);

	RepeatForever*rotekuri = RepeatForever::create(rotemob);
	RepeatForever*kuriscaler = RepeatForever::create(kuriScale);


	//ノコノコアクション
	JumpBy*nocojump1 = JumpBy::create(5.0f,Vec2(100.0,0),50.0f,5);
	JumpBy*nocojump2 = JumpBy::create(5.0f, Vec2(-100.0, 0),50.0f, 5);

	Sequence*nocoseq1 = Sequence::create(nocojump1, nocojump2, nullptr);
	Sequence*nocoseq2 = Sequence::create(nocojump2->clone(), nocojump1->clone(), nullptr);

	RepeatForever*nocoF1 = RepeatForever::create(nocoseq1);
	RepeatForever*nocoF2 = RepeatForever::create(nocoseq2);


	kuri1->runAction(rotekuri);
	kuri1 -> runAction(kuriscaler);
	kuri2->runAction(rotekuri->clone());
	kuri2 -> runAction(kuriscaler->clone());

	noco1->runAction(nocoF1);
	noco2->runAction(nocoF2);
}

