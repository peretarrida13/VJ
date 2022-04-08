#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, STAND_UP_R, STAND_UP_L, STAND_DOWN_R, STAND_DOWN_L, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/madelein.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(STAND_UP_R, 8);
		sprite->addKeyframe(STAND_UP_R, glm::vec2(0.75, 0.0));

		sprite->setAnimationSpeed(STAND_UP_L, 8);
		sprite->addKeyframe(STAND_UP_L, glm::vec2(0.75, 0.25));

		sprite->setAnimationSpeed(STAND_DOWN_L, 8);
		sprite->addKeyframe(STAND_DOWN_L, glm::vec2(0.5, 0.0));

		sprite->setAnimationSpeed(STAND_DOWN_R, 8);
		sprite->addKeyframe(STAND_DOWN_R, glm::vec2(0.5, 0.25));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (sprite->animation() == STAND_UP_R || sprite->animation() == STAND_DOWN_L || sprite->animation() == STAND_UP_L || sprite->animation() == STAND_DOWN_R) {
		sprite->changeAnimation(last_animation);
	}

	if (Game::instance().getKey('x') && dash) {
		this->dash = false;
		cout << sprite->animation() << endl;
		if (sprite->animation() == 0) {
			posPlayer.x -= 5;
			//AQUI CANVI
			int aux = map->collisionMoveLeft(posPlayer, glm::ivec2(16, 16));
			if (aux == 2) {
				posPlayer.x = 0;
				posPlayer.y = 200;
			}
			else if (aux == 1)
			{
				this->dash = true;
				posPlayer.x += 5;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (sprite->animation() == 1) {
			posPlayer.x += 5;
			//AQUI CANVI
			int aux = map->collisionMoveRight(posPlayer, glm::ivec2(16, 16));
			if (aux == 2) {
				posPlayer.x = 0;
				posPlayer.y = 200;
			}
			else if (aux == 1)
			{
				this->dash = true;
				posPlayer.x -= 5;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		//AQUI CANVI
		int aux = map->collisionMoveLeft(posPlayer, glm::ivec2(16, 16));
		if (aux == 2) {
			posPlayer.x = 0;
			posPlayer.y = 200;
		}
		else if(aux == 1)
		{
			this->dash = true;
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		//AQUI CANVI
		int aux = map->collisionMoveRight(posPlayer, glm::ivec2(16, 16));
		if (aux == 2) {
			posPlayer.x = 0;
			posPlayer.y = 200;
		}
		else if(aux == 1)
		{
			this->dash = true;
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		if (sprite->animation() != STAND_UP_L && sprite->animation() != STAND_UP_R) {
			this->last_animation = sprite->animation();
			if (sprite->animation() == STAND_RIGHT) sprite->changeAnimation(STAND_UP_R);
			else if (sprite->animation() == STAND_LEFT) sprite->changeAnimation(STAND_UP_L);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		if (sprite->animation() != STAND_DOWN_R && sprite->animation() != STAND_DOWN_L) {
			this->last_animation = sprite->animation();
			if(sprite->animation() == STAND_RIGHT) sprite->changeAnimation(STAND_DOWN_R);
			else if (sprite->animation() == STAND_LEFT) sprite->changeAnimation(STAND_DOWN_L);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 50 * sin(3.14159f * jumpAngle / 180.f)); 
			if (jumpAngle < 90) {
				int aux = map->collisionMoveUp(posPlayer, glm::ivec2(16, 16), &posPlayer.y);
				if (aux == 2) {
					bJumping = false;
					posPlayer.x = 0;
					posPlayer.y = 200;
				}
				else if (aux == 1) {
					bJumping = false;
				}
				else if (aux == 0) {
					bJumping = true;
				}
			}
			if (jumpAngle >= 90) {
				int aux = map->collisionMoveDown(posPlayer, glm::ivec2(16, 16), &posPlayer.y);
				if (aux == 2) {
					bJumping = false;
					posPlayer.x = 0;
					posPlayer.y = 200;
				}
				else if (aux == 1) {
					bJumping = false;
				}
				else if (aux == 0) {
					bJumping = true;
				}
				
			}
				
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		int aux = map->collisionMoveDown(posPlayer, glm::ivec2(16, 16), &posPlayer.y);
		if (aux == 2) {
			bJumping = false;
			posPlayer.x = 0;
			posPlayer.y = 200;
		}
		else if(aux == 1)
		{
			this->dash = true;
			if(Game::instance().getKey('c'))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




