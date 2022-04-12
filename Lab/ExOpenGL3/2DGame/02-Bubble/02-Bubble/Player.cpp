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
		
	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (sprite->animation() == STAND_UP_R || sprite->animation() == STAND_DOWN_L || sprite->animation() == STAND_UP_L || sprite->animation() == STAND_DOWN_R) {
		sprite->changeAnimation(last_animation);
	}
	
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		climbLeft = false;
		climbRight = false;

		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		
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

		climbLeft = false;
		climbRight = false;

		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;

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
		climbLeft = false;
		climbRight = false;

		if (sprite->animation() != STAND_UP_L && sprite->animation() != STAND_UP_R) {
			this->last_animation = sprite->animation();
			if (sprite->animation() == STAND_RIGHT) sprite->changeAnimation(STAND_UP_R);
			else if (sprite->animation() == STAND_LEFT) sprite->changeAnimation(STAND_UP_L);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		climbLeft = false;
		climbRight = false;

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

	if (Game::instance().getKey('x') && this->dash) {
		this->dash = false;
		climbLeft = false;
		climbRight = false;

		if (sprite->animation() == 0 || sprite->animation() == 6) {
			if (bJumping) posPlayer.x -= 35;
			else posPlayer.x -= 5;
			int aux = map->collisionMoveLeft(posPlayer, glm::ivec2(16, 16));
			if (aux == 2) {
				posPlayer.x = 0;
				posPlayer.y = 200;
			}
			else if (aux == 1)
			{
				this->dash = true;
				if (bJumping) posPlayer.x += 35;
				else posPlayer.x += 5;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (sprite->animation() == 1 || sprite->animation() == 7) {
			if (bJumping) posPlayer.x += 35;
			else posPlayer.x += 5;
			int aux = map->collisionMoveRight(posPlayer, glm::ivec2(16, 16));
			if (aux == 2) {
				posPlayer.x = 0;
				posPlayer.y = 200;
			}
			else if (aux == 1)
			{
				this->dash = true;
				if (bJumping) posPlayer.x -= 35;
				else posPlayer.x -= 5;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (sprite->animation() == 2 || sprite->animation() == 3) {
			posPlayer.y -= 50;
			int aux = map->collisionMoveUp(posPlayer, glm::ivec2(16, 16), &posPlayer.y);
			if (aux == 2) {
				posPlayer.x = 0;
				posPlayer.y = 200;
			}
			else if (aux == 1) {
				bJumping = false;
				posPlayer.y += 50;
			}

		}
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
					climbLeft = false;
					climbRight = false;

					bJumping = false;
					posPlayer.x = 0;
					posPlayer.y = 200;
				}
				else if (aux == 1) {
					climbLeft = false;
					climbRight = false;

					bJumping = false;
				}
				else if (aux == 0) {
					bJumping = true;
				}
				
			}

			//	PARA MIRAR SI HAY SE PUEDE HACER CLIMB:
			// 1. CREAR NUEVO VECTOR CON UN +1 EN LA CORDENADA X (MIRAR SI HAY COLISION SI HAY COLISION I SE CLICA 'C' ACTIVAR CLIMB DERECHO
			// 2. REPETIR PARA LA DERECHA
			
			glm::ivec2 aux_posPlayer;

			aux_posPlayer.y = posPlayer.y;
			aux_posPlayer.x = posPlayer.x + 1;

			int colRight = map->collisionMoveRight(aux_posPlayer, glm::ivec2(16, 16));
			if (colRight == 2) {
				posPlayer.x = 0;
				posPlayer.y = 200;
			}
			else if (colRight == 1)
			{
				if (Game::instance().getKey('c'))
				{
					cout << "ur" << endl;

					climbRight = true;
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}

			aux_posPlayer.y = posPlayer.y;
			aux_posPlayer.x = posPlayer.x - 1;

			int colLeft = map->collisionMoveLeft(aux_posPlayer, glm::ivec2(16, 16));
			if (colLeft == 2) {
				posPlayer.x = 0;
				posPlayer.y = 200;
			}
			else if (colLeft == 1)
			{
				if (Game::instance().getKey('c'))
				{
					cout << "ul" << endl;

					climbLeft = true;
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}
	}
	else
	{
		
		posPlayer.y += FALL_STEP;
		
		int aux = map->collisionMoveDown(posPlayer, glm::ivec2(16, 16), &posPlayer.y);
		if (aux == 2) {
			climbLeft = false;
			climbRight = false;

			bJumping = false;
			posPlayer.x = 0;
			posPlayer.y = 200;
		}
		else if (aux == 1)
		{
			climbLeft = false;
			climbRight = false;

			this->dash = true;
			if (Game::instance().getKey('c'))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}

		glm::ivec2 aux_posPlayer;

		aux_posPlayer.y = posPlayer.y;
		aux_posPlayer.x = posPlayer.x + 1;

		int colRight = map->collisionMoveRight(aux_posPlayer, glm::ivec2(16, 16));
		if (colRight == 2) {
			posPlayer.x = 0;
			posPlayer.y = 200;
		}
		else if (colRight == 1)
		{
			if (Game::instance().getKey('c'))
			{
				cout << "dr" << endl;

				climbRight = true;
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}

		aux_posPlayer.y = posPlayer.y;
		aux_posPlayer.x = posPlayer.x - 1;

		int colLeft = map->collisionMoveLeft(aux_posPlayer, glm::ivec2(16, 16));
		if (colLeft == 2) {
			posPlayer.x = 0;
			posPlayer.y = 200;
		}
		else if (colLeft == 1)
		{
			if (Game::instance().getKey('c'))
			{
				cout << "dl" << endl;
				climbLeft = true;
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
		
	}
	
	if (posPlayer.x < 0) {
		posPlayer.x = 0;
	}

	if (climbRight) {
		posPlayer.x -= 2;
	}
	
	if (climbLeft) {
		posPlayer.x += 2;
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




