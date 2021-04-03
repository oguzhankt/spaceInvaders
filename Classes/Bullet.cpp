#include "Bullet.h"
#include "HelloWorldScene.h"

// Merminin s�per olup olmad��� belirtilmezse normal mermi yarat�r
Bullet * Bullet::create()
{
	Bullet * bullet = new Bullet();
	// �nce nesnenin yarat�l�p yarat�lmad���na, sonra da belirtilen dosyan�n var olup olmad���na bakarak nesneyi kurar
	if (bullet && bullet->initWithFile("bullet.png"))
	{
		bullet->initBullet();
		return bullet;
	}

	// E�er yukarda herhangi bir sorun ya�an�rsa nesneyi siler ve null d�nd�r�r
	CC_SAFE_DELETE(bullet);
	return NULL;
}

// S�per olup olmad��� belirtilirse bu methodla yarat�l�r
Bullet * Bullet::create(bool isSuper)
{
	Bullet * bullet = new Bullet();

	if (bullet) // �nce nesne yarat�lm��m� diye bak�l�r
	{
		// Sona isteniler sprite bulundu mu diye bak�l�p sprite yarat�l�r
		if (isSuper)
		{
			if (bullet->initWithFile("bulletSuper.png"))
			{
				bullet->initBullet();
				return bullet;
			}
		}
		else
		{
			if (bullet->initWithFile("bullet.png"))
			{
				bullet->initBullet();
				return bullet;
			}
		}
	}
	
	// E�er yukarda herhangi bir sorun ya�an�rsa nesneyi siler ve null d�nd�r�r
	CC_SAFE_DELETE(bullet);
	return NULL;
}

void Bullet::initBullet()
{
	
}