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

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// Yerçekimi olmayan bir fizik dünyasý oluþturur. Merminin, düþmanlarla collisonlarýný
	// takip etmek için kullanýlmýþtýr.
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

	// Fizik dünyasýný çizim yaptýðýmýz layera baðlarýz.
	auto layer = HelloWorld::create();
	scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
	// Scene in kurulmasýný bekleriz
    if ( !Scene::init() )
    {
        return false;
    }

	// Pencere sýnýrlarý belirlenir
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();
	
	// Arkaplan çizilir
	auto background = DrawNode::create();
	background->drawSolidRect(origin, winSize, Color4F(0.1, 0.1, 0.1, 1.0));
	this->addChild(background);

	// Oyuncu sprite'ý ekranýn altýna yerleþtirilir
	_player = Player::create();
	auto playerSize = _player->getContentSize();
	_player->setPosition(Vec2((winSize.width * 0.5), winSize.height * 0.1));
	this->addChild(_player);

	// Ýlerde kullanýcaðýmýz random methodlarý için generator
	// zaman kullanýlarak seedlenir
	srand((unsigned int)time(nullptr));

	// Ekrana 32 adet düþman yerleþtirir
	this->createEnemies();

	// 9 saniye aralýkla düþmanlarýn hareket sequence'ýný çaðýrýr.(sol,sað,sol,aþaðý)
	this->moveEnemy(0);
	this->schedule(schedule_selector(HelloWorld::moveEnemy), 9);

	// Dokunmayý algýlar
	auto eventListener = EventListenerTouchOneByOne::create();
	eventListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	eventListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	eventListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, _player);

	// Collision detection için:
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	// Her framede çaðrýlýcak update methodu
	this->schedule(schedule_selector(HelloWorld::updateScene));
	return true;
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();
}

// Her bir framede geminin hareketi, düþmanlarýn canlarý, özel gücün süresi vb.
// update edilmesi içindir init() methodunda schedule olarak kurulmuþtur
void HelloWorld::updateScene(float delta) {

	// Oyuncunun lokasyonu ve hareketi için:
	_player->update();

	// Oyunun bitmesi için:
	// Toplam düþman sayýsýný bildiðimiz için rakam olarak yazýldý ancak
	// düþman sayýsý dinamik olsaydý array lenght olarak da alýnabilirdi.
	if (_player->enemyDied >= _enemyNumber)
	{
		this->gameComplete();
	}

	for (int i = 0; i < 32; i++)
	{
		// Her bir düþmaný update eder:
		// Player nesnesinin gönderilme amacý her bir düþman öldüðünde player nesnesinin
		// süper güç þarjý deðiþkenini arttýrma gerekliliðidir.
		_enemyArray[i]->update(_player);

		// Eðer herhangi bir düþman oyuncuya ulaþýrsa oyun kaybedilir.
		// Spritelar tek tek deðil grup olarak hareket ettikleri için
		// individual olarak pozisyonlarý updatelenmez, bu yüzden physic bodysinin pozisyonu kullanýldý.
		auto enemyY = _enemyArray[i]->getPhysicsBody()->getPosition().y;
		if (enemyY <= this->getContentSize().height*0.2)
		{
			// Oyuncuya ulaþan PhysicsBody dünyadan kaldýrýldýysa (düþman ölüyse) oyunu bitirmez.
			if(_enemyArray[i]->getPhysicsBody()->isEnabled())this->gameOver(); 
		}
	}
	
}

// Bütün düþmanlar öldüðünde çaðrýlýr
void HelloWorld::gameComplete()
{
	Label* label = Label::create("Game Complete!", "Comic Sans MS", 52);
	label->setPosition(Vec2(this->getContentSize().width*0.5, this->getContentSize().height*0.5));

	// Oyun bitti yazýsýný ekler
	this->addChild(label);

	// Oyunu durdurur
	this->pause();
	_player->pause();
}

// Herhangi bir düþman oyuncuya ulaþýrsa çaðrýlýr
void HelloWorld::gameOver()
{
	Label* label = Label::create("Game Over", "Comic Sans MS", 52);
	label->setPosition(Vec2(this->getContentSize().width*0.5, this->getContentSize().height*0.5));

	// Oyun bitti yazýsýný ekler
	this->addChild(label);

	// Oyunu durdurur
	this->pause();
	_player->pause();
	_enemyGroup->pause();
}

// 8/8/8/8 olmak üzere toplam 32 adet düþman yaratýlýr ve bir parent objeye baðlanýr
// 2 satýr güçlü 2 satýr zayýf düþman oluþturulur. Bu düþmanlar oyuncuya doðru hareket etmeye baþlarlar.
void HelloWorld::createEnemies() {

	_enemyGroup = Sprite::create();
	int enemyCtr = 0; // Arrayi doldururken kullanýlýr

	// 32 düþman koyar;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			auto enemy = Enemy::create();

			// Yaratýlýcak düþman türünün create methodu çaðrýlýr
			if(i == 0 || i == 2) enemy = BigEnemy::create();
			else enemy = SmallEnemy::create();

			enemy->setTag(enemyCtr); // Her bir düþman daha sonra collision detectionda kullanýlmak üzere unique taglenir
			_enemyArray[enemyCtr] = enemy; // Tipi (Enemy) olan arraye atýlýrlar
			enemyCtr++;

			// Fizik yapýsýnýn þekli ve büyüklüðü belirlenir, verimlilik açýsýndan
			// basit bir dikdörtgen olarak define edildi.
			auto enemySize = enemy->getContentSize();
			auto physicsBody = PhysicsBody::createBox(Size(enemySize.width, enemySize.height), PhysicsMaterial(0.1f, 1.0f, 0.0f));

			// Kendimiz MoveTo kullanarak hareket ettiriceðimiz için dynamic.
			physicsBody->setDynamic(true);

			// Obje türü define edilir
			physicsBody->setCategoryBitmask((int)PhysicsCategory::Enemy);

			// Obje dynamic olduðu için collision olarak zaten baþka objelerden
			// etkilenmez. Kod anlaþýlabilirliði açýsýndan define edildi
			physicsBody->setCollisionBitmask((int)PhysicsCategory::None);

			// Enemy objesiyle contact ettiðinde notifikasyon yollanmasý için:
			physicsBody->setContactTestBitmask((int)PhysicsCategory::Bullet);
			enemy->setPhysicsBody(physicsBody);

			auto enemyContentSize = enemy->getContentSize();
			auto selfContentSize = this->getContentSize();
			enemy->setPosition(Vec2((selfContentSize.width * (j+1)/10) + enemyContentSize.width, selfContentSize.height * (10-(i+1))/10));
			
			// Ýlerde üniform hareket etmeleri için tek bir sprite parentýna eklendi.
			_enemyGroup->addChild(enemy);

		}
	}

	// En son olarak bütün grup Scene'imize eklendi.
	this->addChild(_enemyGroup);
}

// Bütün düþmanlarýn ayný anda hareket etmesi için burada define edilmiþtir. Bütün düþmanlar
// parent bir sprite'a baðlý olduðu için, parent sprite a uygulanan move komutlarý hepsini etkiler.
// Böylece üniform bir þekilde hareketleri saðlanmýþtýr.
void HelloWorld::moveEnemy(float dt) {

	auto actionMoveLeft = MoveBy::create(1.0, Vec2(-50,0));
	auto actionMoveRight = MoveBy::create(2.0, Vec2(100, 0));
	auto actionMoveDown = MoveBy::create(1.0, Vec2(0,-50));
	auto actionStop = DelayTime::create(1);
	_enemyGroup->runAction(Sequence::create(actionMoveLeft, actionStop, actionMoveRight, actionStop, actionMoveLeft, actionStop, actionMoveDown, nullptr));

}

// Dokunmaya baþlanýldýðý zaman ekrandaki lokasyona göre gemiyi saða sola hareket ettirir
bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event) {

	auto selfContentSize = this->getContentSize();
	Vec2 touchLocation = touch->getLocation();

	if (touchLocation.x > selfContentSize.width / 2)
	{
		_player->setMovement(1.0);
	}
	else
	{
		_player->setMovement(-1.0);
	}

	return true;
}

// Ekranýn saðýna dokunmaya baþlayýp hiç kaldýrmadan sol tarafa hareket ettirilirse
// bu durumun algýlanmasý için kurulmuþtur.
bool HelloWorld::onTouchMoved(Touch *touch, Event *unused_event) {

	auto selfContentSize = this->getContentSize();
	Vec2 touchLocation = touch->getLocation();

	if (touchLocation.x > selfContentSize.width / 2)
	{
		_player->setMovement(1.0);
	}
	else
	{
		_player->setMovement(-1.0);
	}

	return true;
}

// Ekrandan dokunma/mouse kalktýðý zaman gemiyi durdurur.
bool HelloWorld::onTouchEnded(Touch *touch, Event *unused_event) {

	_player->setMovement(0);
	return true;

}

// Mermi düþmanlara çarptýðý zaman canýnýn götürülmesini hesaplar
bool HelloWorld::onContactBegan(PhysicsContact &contact) {

	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	// Ýlk çarpan obje, türünden baðýmsýz "nodeA" olucaðý için hangisinin hangi tip olduðu
	// tek tek denenir ve çarpan mermiye göre düþmanýn caný azalýr.
	// (TAG)
	// 99: Normal mermi
	// 999: Süper mermi
	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == 99) 
		{
			nodeA->removeFromParentAndCleanup(true);
			_enemyArray[nodeB->getTag()]->healthDown();
		}
		else if (nodeB->getTag() == 99)
		{
			nodeB->removeFromParentAndCleanup(true);
			_enemyArray[nodeA->getTag()]->healthDown();
		}
		else if (nodeA->getTag() == 999)
		{
			nodeA->removeFromParentAndCleanup(true);
			_enemyArray[nodeB->getTag()]->healthDownSuper();
		}
		else if (nodeB->getTag() == 999)
		{
			nodeB->removeFromParentAndCleanup(true);
			_enemyArray[nodeA->getTag()]->healthDownSuper();
		}
	}
	
	return true;
}

