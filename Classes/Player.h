
#ifndef PLAYER_H_
#define PLAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class Player : public cocos2d::Sprite {

public:
	static Player * create(void);
	void Player::update();

	void Player::increaseSuperCharge();
	void Player::turnOffSuper(float dt);
	void Player::shoot(float dt);

	// Hareket deðiþkeninin enkapsülasyonu
	void Player::setMovement(float value);
	float Player::getMovement();
	int enemyDied = 0;

private:

	// Süper mermi için
	bool isSuper = false;
	int superCharge = 0;

	// Runtime da deðiþtirilmeyeceði için constant
	const int speed = 5; // Geminin hýzý
	const int bulletActionDuration = 1.0; // Merminin hýzýyla ters orantýlý

	int movement;
	int direction;
	void initPlayer();
};

#endif /* PLAYER_H_ */