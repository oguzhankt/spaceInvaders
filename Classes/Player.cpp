#include "Player.h"
#include "HelloWorldScene.h"

Player * Player::create()
{
	Player * player = new Player();

	// �nce nesnenin yarat�l�p yarat�lmad���na, sonra da belirtilen dosyan�n var olup olmad���na bakarak nesneyi kurar
	if (player && player->initWithFile("player.png")) 
	{
		player->initPlayer();
		return player;
	}

	// E�er yukarda herhangi bir sorun ya�an�rsa nesneyi siler ve null d�nd�r�r
	CC_SAFE_DELETE(player);
	return NULL;
}

void Player::initPlayer()
{
	movement = 0;

	// shoot methodu belirli zaman aral�klar�yla �a�r�larak ate� edilmesi sa�lan�r
	this->schedule(schedule_selector(Player::shoot), 0.6);
}

void Player::setMovement(float value)
{
	movement = value;
}

float Player::getMovement()
{
	return movement;
}

void Player::increaseSuperCharge()
{
	// E�er s�per mermi zaten at�l�yosa �arj artt�r�lmaz. Bu sayede s�per mermiyle de 5 d��man �ld�rerek
	// sonsuza kadar s�per mermi atman�n �n�ne ge�ilir
	if(!isSuper)superCharge++;

	// superCharge de�i�keni her 5 skorda s�f�rland��� i�in ayriyetten bunu tutuyoruz.
	// Oyunun bitmesi i�in kullan�l�cak
	enemyDied++; 
}

// Mermi sprite'� ve fizik objesi olu�turup ileri olarak hareket edicek �ekilde g�nderir.
// Mermilerin ate� edildikten sonra gemi ile beraber sa�a sola gitmesini istemedi�imiz i�in player 
// objesinin child'� olarak de�il ayr� bir obje olarak yarat�l�r.
void Player::shoot(float dt) {

	auto bullet = Bullet::create(isSuper);
	auto parentScene = this->getParent();

	// (TAG)
	// 99 : Normal mermi
	// 999 : Super mermi
	if(isSuper) bullet->setTag(999);
	else bullet->setTag(99);

	auto bulletSize = bullet->getContentSize();
	auto parentSize = parentScene->getContentSize(); // Ana sceneimizin boyutlar�
	

	// Fizik �ekli ve b�y�kl��� define edilir
	auto physicsBody = PhysicsBody::createBox(Size(bulletSize.width, bulletSize.height),
		PhysicsMaterial(0.1f, 1.0f, 0.0f));

	// Kendimiz MoveTo kullanarak hareket ettirice�imiz i�in dynamic.
	physicsBody->setDynamic(true);

	// Obje t�r� define edilir
	physicsBody->setCategoryBitmask((int)PhysicsCategory::Bullet);

	// Obje dynamic oldu�u i�in collision olarak zaten ba�ka objelerden
	// etkilenmez. Kod anla��labilirli�i a��s�ndan define edildi
	physicsBody->setCollisionBitmask((int)PhysicsCategory::None);

	// Enemy objesiyle contact etti�inde notifikasyon yollanmas� i�in:
	physicsBody->setContactTestBitmask((int)PhysicsCategory::Enemy);
	bullet->setPhysicsBody(physicsBody);

	bullet->setPosition(this->getPosition());

	// Mermi yarat�ld�ktan sonra gemiyle beraber hareket etmemesi gerekti�i i�in parenta eklendi
	parentScene->addChild(bullet);

	auto actionMove = MoveTo::create(bulletActionDuration, Vec2(this->getPositionX(), parentSize.height + bulletSize.height));
	auto actionRemove = RemoveSelf::create();
	bullet->runAction(Sequence::create(actionMove, actionRemove, nullptr));

}

void Player::update()
{
	auto parentSize = this->getParent()->getContentSize();

	if (movement != 0)
	{
		this->setPositionX(this->getPositionX() + (movement*speed));

		if (this->getPositionX() > parentSize.width) this->setPositionX(0);
		if (this->getPositionX() < 0) this->setPositionX(parentSize.width);
	}

	if (superCharge >= 5)
	{
		superCharge = 0;
		isSuper = true;
		this->scheduleOnce(schedule_selector(Player::turnOffSuper), 3.0); // 3 saniye sonra super g�� biter.
	}

}

void Player::turnOffSuper(float dt)
{
	isSuper = false;
}