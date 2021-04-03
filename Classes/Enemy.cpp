#include "Enemy.h"

Enemy * Enemy::create()
{
	return NULL;
}

SmallEnemy * SmallEnemy::create()
{
	SmallEnemy * enemy = new SmallEnemy();

	// �nce nesnenin yarat�l�p yarat�lmad���na, sonra da belirtilen dosyan�n var olup olmad���na bakarak nesneyi kurar
	if (enemy && enemy->initWithFile("enemy1.png"))
	{
		enemy->autorelease();
		enemy->initEnemy();
		return enemy;
	}

	// E�er yukarda herhangi bir sorun ya�an�rsa nesneyi siler ve null d�nd�r�r
	CC_SAFE_DELETE(enemy);
	return NULL;
}

BigEnemy * BigEnemy::create()
{
	BigEnemy * enemy = new BigEnemy();

	// �nce nesnenin yarat�l�p yarat�lmad���na, sonra da belirtilen dosyan�n var olup olmad���na bakarak nesneyi kurar
	if (enemy && enemy->initWithFile("enemy2.png"))
	{
		enemy->autorelease();
		enemy->initEnemy();
		return enemy;
	}

	// E�er yukarda herhangi bir sorun ya�an�rsa nesneyi siler ve null d�nd�r�r
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

// Her framede d��manlar�n update edilmesi i�in
// Player nesnesinin al�nma amac� her d��man �ld���nde, onu �ld�ren oyuncunun �arj�n� "1" artt�r�r.
// alive == false olursa bu d��man updatelenmez.
void Enemy::update(Player *p)
{
	if (health <= 0 && alive)
	{
		// Burda da enemy objesini direk silmeme amac�m�z ilk kurulan arrayde hala yer kaplamas� gerekti�idir.
		// Bu y�zden sprite � Hide yap�p, sadece fizik body sini d�nyadan siliyoruz.
		this->runAction(Hide::create());
		this->getPhysicsBody()->setEnabled(false);
		this->getPhysicsBody()->removeFromWorld();
		
		// �arj artt�r�l�r
		p->increaseSuperCharge();

		// Bu methodun sadece bir kere �al��t�r�lmas� i�in alive de�i�keni kullan�l�r, yoksa can�
		// 0 dan a�a�� indikten sonra her frame de bu method �a�r�l�rd�.
		alive = false;
	}
}

// Normal mermi de�di�inde
void Enemy::healthDown()
{
	health -= 25;
}

// S�per mermi de�di�inde
void Enemy::healthDownSuper()
{
	health -= 100;
}