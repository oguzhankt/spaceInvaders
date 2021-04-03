#include "Bullet.h"
#include "HelloWorldScene.h"

// Merminin süper olup olmadýðý belirtilmezse normal mermi yaratýr
Bullet * Bullet::create()
{
	Bullet * bullet = new Bullet();
	// Önce nesnenin yaratýlýp yaratýlmadýðýna, sonra da belirtilen dosyanýn var olup olmadýðýna bakarak nesneyi kurar
	if (bullet && bullet->initWithFile("bullet.png"))
	{
		bullet->initBullet();
		return bullet;
	}

	// Eðer yukarda herhangi bir sorun yaþanýrsa nesneyi siler ve null döndürür
	CC_SAFE_DELETE(bullet);
	return NULL;
}

// Süper olup olmadýðý belirtilirse bu methodla yaratýlýr
Bullet * Bullet::create(bool isSuper)
{
	Bullet * bullet = new Bullet();

	if (bullet) // Önce nesne yaratýlmýþmý diye bakýlýr
	{
		// Sona isteniler sprite bulundu mu diye bakýlýp sprite yaratýlýr
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
	
	// Eðer yukarda herhangi bir sorun yaþanýrsa nesneyi siler ve null döndürür
	CC_SAFE_DELETE(bullet);
	return NULL;
}

void Bullet::initBullet()
{
	
}