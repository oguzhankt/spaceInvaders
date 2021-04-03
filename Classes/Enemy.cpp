#include "Enemy.h"

Enemy * Enemy::create()
{
	return NULL;
}

SmallEnemy * SmallEnemy::create()
{
	SmallEnemy * enemy = new SmallEnemy();

	// Önce nesnenin yaratýlýp yaratýlmadýðýna, sonra da belirtilen dosyanýn var olup olmadýðýna bakarak nesneyi kurar
	if (enemy && enemy->initWithFile("enemy1.png"))
	{
		enemy->autorelease();
		enemy->initEnemy();
		return enemy;
	}

	// Eðer yukarda herhangi bir sorun yaþanýrsa nesneyi siler ve null döndürür
	CC_SAFE_DELETE(enemy);
	return NULL;
}

BigEnemy * BigEnemy::create()
{
	BigEnemy * enemy = new BigEnemy();

	// Önce nesnenin yaratýlýp yaratýlmadýðýna, sonra da belirtilen dosyanýn var olup olmadýðýna bakarak nesneyi kurar
	if (enemy && enemy->initWithFile("enemy2.png"))
	{
		enemy->autorelease();
		enemy->initEnemy();
		return enemy;
	}

	// Eðer yukarda herhangi bir sorun yaþanýrsa nesneyi siler ve null döndürür
	CC_SAFE_DELETE(enemy);
	return NULL;
}

void Enemy::initEnemy()
{
}

void SmallEnemy::initEnemy()
{
	health = 50;
}

void BigEnemy::initEnemy()
{
	health = 100;
}

// Her framede düþmanlarýn update edilmesi için
// Player nesnesinin alýnma amacý her düþman öldüðünde, onu öldüren oyuncunun þarjýný "1" arttýrýr.
// alive == false olursa bu düþman updatelenmez.
void Enemy::update(Player *p)
{
	if (health <= 0 && alive)
	{
		// Burda da enemy objesini direk silmeme amacýmýz ilk kurulan arrayde hala yer kaplamasý gerektiðidir.
		// Bu yüzden sprite ý Hide yapýp, sadece fizik body sini dünyadan siliyoruz.
		this->runAction(Hide::create());
		this->getPhysicsBody()->setEnabled(false);
		this->getPhysicsBody()->removeFromWorld();
		
		// Þarj arttýrýlýr
		p->increaseSuperCharge();

		// Bu methodun sadece bir kere çalýþtýrýlmasý için alive deðiþkeni kullanýlýr, yoksa caný
		// 0 dan aþaðý indikten sonra her frame de bu method çaðrýlýrdý.
		alive = false;
	}
}

// Normal mermi deðdiðinde
void Enemy::healthDown()
{
	health -= 25;
}

// Süper mermi deðdiðinde
void Enemy::healthDownSuper()
{
	health -= 100;
}