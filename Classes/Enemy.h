
#ifndef ENEMY_H_
#define ENEMY_H_

#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;

class Enemy : public cocos2d::Sprite {
public:
	static Enemy * create(void);
	void update(Player *p);

	void healthDown();
	void healthDownSuper();
	
protected:
	// Her düþmanda can olucaðý için parent classta tanýmlandý
	// Ayrýca alt classlarýn eriþimi için protected
	bool alive = true;
	int health;

	void initEnemy();
};

class SmallEnemy : public Enemy {
public:
	static SmallEnemy * create(void);

private:
	void initEnemy();

};

class BigEnemy : public Enemy {
public:
	static BigEnemy * create(void);

private:
	void initEnemy();

};

#endif /* ENEMY_H_ */