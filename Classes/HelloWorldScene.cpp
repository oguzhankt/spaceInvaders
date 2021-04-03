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
	// Yer�ekimi olmayan bir fizik d�nyas� olu�turur. Merminin, d��manlarla collisonlar�n�
	// takip etmek i�in kullan�lm��t�r.
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

	// Fizik d�nyas�n� �izim yapt���m�z layera ba�lar�z.
	auto layer = HelloWorld::create();
	scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
	// Scene in kurulmas�n� bekleriz
    if ( !Scene::init() )
    {
        return false;
    }

	// Pencere s�n�rlar� belirlenir
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();
	
	// Arkaplan �izilir
	auto background = DrawNode::create();
	background->drawSolidRect(origin, winSize, Color4F(0.1, 0.1, 0.1, 1.0));
	this->addChild(background);

	// Oyuncu sprite'� ekran�n alt�na yerle�tirilir
	_player = Player::create();
	auto playerSize = _player->getContentSize();
	_player->setPosition(Vec2((winSize.width * 0.5), winSize.height * 0.1));
	this->addChild(_player);

	// �lerde kullan�ca��m�z random methodlar� i�in generator
	// zaman kullan�larak seedlenir
	srand((unsigned int)time(nullptr));

	// Ekrana 32 adet d��man yerle�tirir
	this->createEnemies();

	// 9 saniye aral�kla d��manlar�n hareket sequence'�n� �a��r�r.(sol,sa�,sol,a�a��)
	this->moveEnemy(0);
	this->schedule(schedule_selector(HelloWorld::moveEnemy), 9);

	// Dokunmay� alg�lar
	auto eventListener = EventListenerTouchOneByOne::create();
	eventListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	eventListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	eventListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, _player);

	// Collision detection i�in:
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	// Her framede �a�r�l�cak update methodu
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

// Her bir framede geminin hareketi, d��manlar�n canlar�, �zel g�c�n s�resi vb.
// update edilmesi i�indir init() methodunda schedule olarak kurulmu�tur
void HelloWorld::updateScene(float delta) {

	// Oyuncunun lokasyonu ve hareketi i�in:
	_player->update();

	// Oyunun bitmesi i�in:
	// Toplam d��man say�s�n� bildi�imiz i�in rakam olarak yaz�ld� ancak
	// d��man say�s� dinamik olsayd� array lenght olarak da al�nabilirdi.
	if (_player->enemyDied >= _enemyNumber)
	{
		this->gameComplete();
	}

	for (int i = 0; i < 32; i++)
	{
		// Her bir d��man� update eder:
		// Player nesnesinin g�nderilme amac� her bir d��man �ld���nde player nesnesinin
		// s�per g�� �arj� de�i�kenini artt�rma gereklili�idir.
		_enemyArray[i]->update(_player);

		// E�er herhangi bir d��man oyuncuya ula��rsa oyun kaybedilir.
		// Spritelar tek tek de�il grup olarak hareket ettikleri i�in
		// individual olarak pozisyonlar� updatelenmez, bu y�zden physic bodysinin pozisyonu kullan�ld�.
		auto enemyY = _enemyArray[i]->getPhysicsBody()->getPosition().y;
		if (enemyY <= this->getContentSize().height*0.2)
		{
			// Oyuncuya ula�an PhysicsBody d�nyadan kald�r�ld�ysa (d��man �l�yse) oyunu bitirmez.
			if(_enemyArray[i]->getPhysicsBody()->isEnabled())this->gameOver(); 
		}
	}
	
}

// B�t�n d��manlar �ld���nde �a�r�l�r
void HelloWorld::gameComplete()
{
	Label* label = Label::create("Game Complete!", "Comic Sans MS", 52);
	label->setPosition(Vec2(this->getContentSize().width*0.5, this->getContentSize().height*0.5));

	// Oyun bitti yaz�s�n� ekler
	this->addChild(label);

	// Oyunu durdurur
	this->pause();
	_player->pause();
}

// Herhangi bir d��man oyuncuya ula��rsa �a�r�l�r
void HelloWorld::gameOver()
{
	Label* label = Label::create("Game Over", "Comic Sans MS", 52);
	label->setPosition(Vec2(this->getContentSize().width*0.5, this->getContentSize().height*0.5));

	// Oyun bitti yaz�s�n� ekler
	this->addChild(label);

	// Oyunu durdurur
	this->pause();
	_player->pause();
	_enemyGroup->pause();
}

// 8/8/8/8 olmak �zere toplam 32 adet d��man yarat�l�r ve bir parent objeye ba�lan�r
// 2 sat�r g��l� 2 sat�r zay�f d��man olu�turulur. Bu d��manlar oyuncuya do�ru hareket etmeye ba�larlar.
void HelloWorld::createEnemies() {

	_enemyGroup = Sprite::create();
	int enemyCtr = 0; // Arrayi doldururken kullan�l�r

	// 32 d��man koyar;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			auto enemy = Enemy::create();

			// Yarat�l�cak d��man t�r�n�n create methodu �a�r�l�r
			if(i == 0 || i == 2) enemy = BigEnemy::create();
			else enemy = SmallEnemy::create();

			enemy->setTag(enemyCtr); // Her bir d��man daha sonra collision detectionda kullan�lmak �zere unique taglenir
			_enemyArray[enemyCtr] = enemy; // Tipi (Enemy) olan arraye at�l�rlar
			enemyCtr++;

			// Fizik yap�s�n�n �ekli ve b�y�kl��� belirlenir, verimlilik a��s�ndan
			// basit bir dikd�rtgen olarak define edildi.
			auto enemySize = enemy->getContentSize();
			auto physicsBody = PhysicsBody::createBox(Size(enemySize.width, enemySize.height), PhysicsMaterial(0.1f, 1.0f, 0.0f));

			// Kendimiz MoveTo kullanarak hareket ettirice�imiz i�in dynamic.
			physicsBody->setDynamic(true);

			// Obje t�r� define edilir
			physicsBody->setCategoryBitmask((int)PhysicsCategory::Enemy);

			// Obje dynamic oldu�u i�in collision olarak zaten ba�ka objelerden
			// etkilenmez. Kod anla��labilirli�i a��s�ndan define edildi
			physicsBody->setCollisionBitmask((int)PhysicsCategory::None);

			// Enemy objesiyle contact etti�inde notifikasyon yollanmas� i�in:
			physicsBody->setContactTestBitmask((int)PhysicsCategory::Bullet);
			enemy->setPhysicsBody(physicsBody);

			auto enemyContentSize = enemy->getContentSize();
			auto selfContentSize = this->getContentSize();
			enemy->setPosition(Vec2((selfContentSize.width * (j+1)/10) + enemyContentSize.width, selfContentSize.height * (10-(i+1))/10));
			
			// �lerde �niform hareket etmeleri i�in tek bir sprite parent�na eklendi.
			_enemyGroup->addChild(enemy);

		}
	}

	// En son olarak b�t�n grup Scene'imize eklendi.
	this->addChild(_enemyGroup);
}

// B�t�n d��manlar�n ayn� anda hareket etmesi i�in burada define edilmi�tir. B�t�n d��manlar
// parent bir sprite'a ba�l� oldu�u i�in, parent sprite a uygulanan move komutlar� hepsini etkiler.
// B�ylece �niform bir �ekilde hareketleri sa�lanm��t�r.
void HelloWorld::moveEnemy(float dt) {

	auto actionMoveLeft = MoveBy::create(1.0, Vec2(-50,0));
	auto actionMoveRight = MoveBy::create(2.0, Vec2(100, 0));
	auto actionMoveDown = MoveBy::create(1.0, Vec2(0,-50));
	auto actionStop = DelayTime::create(1);
	_enemyGroup->runAction(Sequence::create(actionMoveLeft, actionStop, actionMoveRight, actionStop, actionMoveLeft, actionStop, actionMoveDown, nullptr));

}

// Dokunmaya ba�lan�ld��� zaman ekrandaki lokasyona g�re gemiyi sa�a sola hareket ettirir
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

// Ekran�n sa��na dokunmaya ba�lay�p hi� kald�rmadan sol tarafa hareket ettirilirse
// bu durumun alg�lanmas� i�in kurulmu�tur.
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

// Ekrandan dokunma/mouse kalkt��� zaman gemiyi durdurur.
bool HelloWorld::onTouchEnded(Touch *touch, Event *unused_event) {

	_player->setMovement(0);
	return true;

}

// Mermi d��manlara �arpt��� zaman can�n�n g�t�r�lmesini hesaplar
bool HelloWorld::onContactBegan(PhysicsContact &contact) {

	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	// �lk �arpan obje, t�r�nden ba��ms�z "nodeA" oluca�� i�in hangisinin hangi tip oldu�u
	// tek tek denenir ve �arpan mermiye g�re d��man�n can� azal�r.
	// (TAG)
	// 99: Normal mermi
	// 999: S�per mermi
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

