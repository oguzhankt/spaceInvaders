
#ifndef BULLET_H_
#define BULLET_H_

#include "cocos2d.h"

USING_NS_CC;

class Bullet : public cocos2d::Sprite {
public:

	static Bullet * create(void);
	static Bullet * create(bool super);

	void initBullet();

};

#endif /* Bullet_H_ */