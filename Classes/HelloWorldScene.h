/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

 // Obje kategorileri define edilmi�tir. �lerde bu enum'� kullanarak
 // hangi obje t�rlerinin collisionlar� izlenicek onu belirleyece�iz.
enum class PhysicsCategory {
	None = 0,
	Enemy = (1 << 0),    // 1
	Bullet = (1 << 1), // 2
	All = Enemy | Bullet // 3
};

USING_NS_CC;

class HelloWorld : public Scene
{

private:
	Player* _player;
	Sprite* _enemyGroup; // D��man spritelar�n�n gruplan�ca�� sprite
	const int _enemyNumber = 32;
	Enemy* _enemyArray[32] = {}; // D��manlar�n tutuluca�� array

public:
    static Scene* createScene();
	void updateScene(float interval);
	void gameComplete();
	void gameOver();
    virtual bool init();

	// Dokunma
	bool onTouchBegan(Touch *touch, Event *unused_event);
	bool onTouchMoved(Touch *touch, Event *unused_event);
	bool onTouchEnded(Touch *touch, Event *unused_event);

	// Collision
	bool onContactBegan(PhysicsContact & contact);

	// D��manlar
	void createEnemies();
	void moveEnemy(float dt);
	
    void menuCloseCallback(Ref* pSender);
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
