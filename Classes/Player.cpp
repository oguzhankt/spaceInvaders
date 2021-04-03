#include "Player.h"
#include "HelloWorldScene.h"

Player * Player::create()
{
	Player * player = new Player();

	// Önce nesnenin yaratýlýp yaratýlmadýðýna, sonra da belirtilen dosyanýn var olup olmadýðýna bakarak nesneyi kurar
	if (player && player->initWithFile("player.png")) 
	{
		player->initPlayer();
		return player;
	}

	// Eðer yukarda herhangi bir sorun yaþanýrsa nesneyi siler ve null döndürür
	CC_SAFE_DELETE(player);
	return NULL;
}

void Player::initPlayer()
{
	movement = 0;

	// shoot methodu belirli zaman aralýklarýyla çaðrýlarak ateþ edilmesi saðlanýr
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
	// Eðer süper mermi zaten atýlýyosa þarj arttýrýlmaz. Bu sayede süper mermiyle de 5 düþman öldürerek
	// sonsuza kadar süper mermi atmanýn önüne geçilir
	if(!isSuper)superCharge++;

	// superCharge deðiþkeni her 5 skorda sýfýrlandýðý için ayriyetten bunu tutuyoruz.
	// Oyunun bitmesi için kullanýlýcak
	enemyDied++; 
}

// Mermi sprite'ý ve fizik objesi oluþturup ileri olarak hareket edicek þekilde gönderir.
// Mermilerin ateþ edildikten sonra gemi ile beraber saða sola gitmesini istemediðimiz için player 
// objesinin child'ý olarak deðil ayrý bir obje olarak yaratýlýr.
void Player::shoot(float dt) {

	auto bullet = Bullet::create(isSuper);
	auto parentScene = this->getParent();

	// (TAG)
	// 99 : Normal mermi
	// 999 : Super mermi
	if(isSuper) bullet->setTag(999);
	else bullet->setTag(99);

	auto bulletSize = bullet->getContentSize();
	auto parentSize = parentScene->getContentSize(); // Ana sceneimizin boyutlarý
	

	// Fizik þekli ve büyüklüðü define edilir
	auto physicsBody = PhysicsBody::createBox(Size(bulletSize.width, bulletSize.height),
		PhysicsMaterial(0.1f, 1.0f, 0.0f));

	// Kendimiz MoveTo kullanarak hareket ettiriceðimiz için dynamic.
	physicsBody->setDynamic(true);

	// Obje türü define edilir
	physicsBody->setCategoryBitmask((int)PhysicsCategory::Bullet);

	// Obje dynamic olduðu için collision olarak zaten baþka objelerden
	// etkilenmez. Kod anlaþýlabilirliði açýsýndan define edildi
	physicsBody->setCollisionBitmask((int)PhysicsCategory::None);

	// Enemy objesiyle contact ettiðinde notifikasyon yollanmasý için:
	physicsBody->setContactTestBitmask((int)PhysicsCategory::Enemy);
	bullet->setPhysicsBody(physicsBody);

	bullet->setPosition(this->getPosition());

	// Mermi yaratýldýktan sonra gemiyle beraber hareket etmemesi gerektiði için parenta eklendi
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
		this->scheduleOnce(schedule_selector(Player::turnOffSuper), 3.0); // 3 saniye sonra super güç biter.
	}

}

void Player::turnOffSuper(float dt)
{
	isSuper = false;
}