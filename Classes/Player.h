
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

	// Hareket de�i�keninin enkaps�lasyonu
	void Player::setMovement(float value);
	float Player::getMovement();
	int enemyDied = 0;

private:

	// S�per mermi i�in
	bool isSuper = false;
	int superCharge = 0;

	// Runtime da de�i�tirilmeyece�i i�in constant
	const int speed = 5; // Geminin h�z�
	const int bulletActionDuration = 1.0; // Merminin h�z�yla ters orant�l�

	int movement;
	int direction;
	void initPlayer();
};

#endif /* PLAYER_H_ */