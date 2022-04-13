#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 50
#define FALL_STEP 2
#define DASH_STEP 2
#define DASH_MAX 50


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, STAND_UP_R, STAND_UP_L, STAND_DOWN_R, STAND_DOWN_L, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	dash = false;
	spritesheet.loadFromFile("images/madelein.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//      aquetes variables haurien de ser 20, 20
	sprite = Sprite::createSprite(glm::ivec2(20, 20), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
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
	posPlayer.x = 350 * level;
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::collision() {
	if (diagonalDashLeft) {
		dashFinish = -2;

		posPlayer.x -= 1;
		int aux = map->collisionMoveLeft(posPlayer, glm::ivec2(20, 20));
		if (aux == 2) {
			posPlayer.x = 350 * level;
			posPlayer.y = 240;
		}
		else if (aux == 1)
		{
			posPlayer.x += 1;
			sprite->changeAnimation(STAND_LEFT);
		}

		posPlayer.y -= 2;
		aux = map->collisionMoveUp(posPlayer, glm::ivec2(20, 20), &posPlayer.y);
		if (aux == 2) {
			posPlayer.x = 350 * level;
			posPlayer.y = 240;
		}
		else if (aux == 1) {
			bJumping = false;
			posPlayer.y += 2;
		}
	}
	else if (diagonalDashRight) {
		dashFinish = 2;

		posPlayer.x += 3;
		int aux = map->collisionMoveRight(posPlayer, glm::ivec2(20, 20));
		if (aux == 2) {
			posPlayer.x = 350 * level;
			posPlayer.y = 240;
		}
		else if (aux == 1)
		{
			posPlayer.x -= 3;
			sprite->changeAnimation(STAND_RIGHT);
		}

		posPlayer.y -= 2;
		aux = map->collisionMoveUp(posPlayer, glm::ivec2(20, 20), &posPlayer.y);
		if (aux == 2) {
			posPlayer.x = 350 * level;
			posPlayer.y = 240;
		}
		else if (aux == 1) {
			bJumping = false;
			posPlayer.y += 2;
		}
	}
	else {
		if (sprite->animation() == 0 || sprite->animation() == 6) {
			dashFinish = -2;

			posPlayer.x -= 3;
			int aux = map->collisionMoveLeft(posPlayer, glm::ivec2(20, 20));
			if (aux == 2) {
				posPlayer.x = 350 * level;
				posPlayer.y = 240;
			}
			else if (aux == 1)
			{
				posPlayer.x += 3;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		if (sprite->animation() == 1 || sprite->animation() == 7) {
			dashFinish = 2;

			posPlayer.x += 3;
			int aux = map->collisionMoveRight(posPlayer, glm::ivec2(20, 20));
			if (aux == 2) {
				posPlayer.x = 350 * level;
				posPlayer.y = 240;
			}
			else if (aux == 1)
			{
				posPlayer.x -= 3;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		if (sprite->animation() == 2 || sprite->animation() == 3) {
			posPlayer.y -= 2;
			int aux = map->collisionMoveUp(posPlayer, glm::ivec2(20, 20), &posPlayer.y);
			if (aux == 2) {
				posPlayer.x = 350 * level;
				posPlayer.y = 240;
			}
			else if (aux == 1) {
				bJumping = false;
				posPlayer.y += 2;
			}
		}
	}
	
}

void Player::checkTecles() {
	if (Game::instance().getKey('g')) {
		map->godMode = !map->godMode;
	}

	if (Game::instance().getKey('s')) {
		slowMode = !slowMode;

		if (slowMode) speed = 0.5;
		else speed = 1;
	}

	if (Game::instance().getKey('d')) {
		dashMode = !dashMode;
	}

	if (Game::instance().getKey('0')) {
		level = 1;
	}

	if (Game::instance().getKey('1')) {
		level = 2;
	}
	if (Game::instance().getKey('2')) {
		level = 4;
	}

	if (Game::instance().getKey('3')) {
		level = 4;
	}
	if (Game::instance().getKey('4')) {
		level = 5;
	}

	if (Game::instance().getKey('5')) {
		level = 6;
	}
	if (Game::instance().getKey('6')) {
		level = 7;
	}

	if (Game::instance().getKey('7')) {
		level = 8;
	}
	if (Game::instance().getKey('8')) {
		level = 9;
	}

	if (Game::instance().getKey('9')) {
		level = 10;
	}
}

void Player::update(int deltaTime)
{
	checkTecles();

	int wall = (350 * level) - 1;
	if (level == 0) {
		if (Game::instance().getKey('c') && Game::instance().getKey('x')) {
			level += 1;
			posPlayer.x = 350 * level;
		}
	}
	else {
		if (dash) {

			dashLength += DASH_STEP;

			if (dashLength == DASH_MAX) {
				dashLength = 0;
				startY = posPlayer.y;
				jumpAngle = 180;
				bJumping = false;
				dash = false;
				diagonalDashLeft = false;
				diagonalDashRight = false;
			} 

			collision();			
			
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
			return;
		} 
		sprite->update(deltaTime);

		if (sprite->animation() == STAND_UP_R || sprite->animation() == STAND_DOWN_L || sprite->animation() == STAND_UP_L || sprite->animation() == STAND_DOWN_R) {
			sprite->changeAnimation(last_animation);
		}

		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{

			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 2;

			int aux = map->collisionMoveLeft(posPlayer, glm::ivec2(20, 20));
			if (aux == 2) {
				posPlayer.x = 350 * level;
				posPlayer.y = 240;
			}
			else if (aux == 1)
			{
				this->dash = false;
				posPlayer.x += 2;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{


			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 2;

			int aux = map->collisionMoveRight(posPlayer, glm::ivec2(20, 20));
			if (aux == 2) {
				posPlayer.x = 350 * level;
				posPlayer.y = 240;
			}
			else if (aux == 1)
			{
				this->dash = false;
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
				if (sprite->animation() == STAND_RIGHT) sprite->changeAnimation(STAND_DOWN_R);
				else if (sprite->animation() == STAND_LEFT) sprite->changeAnimation(STAND_DOWN_L);
			}
		}
		else
		{
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}


		if (bJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			int posBefore = posPlayer.y;

			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle < 90) {
					int aux = map->collisionMoveUp(posPlayer, glm::ivec2(20, 20), &posPlayer.y);
					if (aux == 2) {
						climbLeft = false;
						climbRight = false;
						bJumping = false;
						posPlayer.x = 350 * level;
						posPlayer.y = 240;
					}
					else if (aux == 1) {
						climbLeft = false;
						climbRight = false;
						bJumping = false;
						posPlayer.y = posBefore;
					}
					else if (aux == 0) {
						bJumping = true;
					}
				}
				if (jumpAngle >= 90) {

					int aux = map->collisionMoveDown(posPlayer, glm::ivec2(20, 20), &posPlayer.y);
					if (aux == 2) {
						climbLeft = false;
						climbRight = false;

						bJumping = false;
						dashed = false;
						posPlayer.x = 350 * level;
						posPlayer.y = 240;
					}
					else if (aux == 1) {
						dashFinish = 0;
						climbLeft = false;
						climbRight = false;

						bJumping = false;
						dashed = false;
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

			int aux = map->collisionMoveDown(posPlayer, glm::ivec2(20, 20), &posPlayer.y);
			if (aux == 2) {
				climbLeft = false;
				climbRight = false;

				bJumping = false;
				dashed = false;

				posPlayer.x = 350 * level;
				posPlayer.y = 240;
			}
			else if (aux == 1)
			{
				climbLeft = false;
				climbRight = false;
				dashed = false;
				dashFinish = 0;

				if (Game::instance().getKey('c'))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}

		if (Game::instance().getKey('c')) {
			
			posPlayer.y += 3;
			int colDown = map->collisionMoveDown(posPlayer, glm::ivec2(20, 20), &posPlayer.y);
			posPlayer.y -= 3;
			if (colDown == 0) {
				posPlayer.x += 1;

				int colLeft = map->collisionMoveLeft(posPlayer, glm::ivec2(20, 20));
				if (colLeft == 2) {
					climbLeft = false;
					climbRight = false;

					bJumping = false;
					dashed = false;

					posPlayer.x = 350 * level;
					posPlayer.y = 240;
				}
				else if (colLeft == 1) {
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
					climbLeft = true;
					posPlayer.x -= 1;
				}

				int colRight = map->collisionMoveRight(posPlayer, glm::ivec2(20, 20));
				if (colRight == 2) {
					climbLeft = false;
					climbRight = false;

					bJumping = false;
					dashed = false;

					posPlayer.x = 350 * level;
					posPlayer.y = 240;
				}
				else if (colRight == 1) {
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
					climbRight = true;
				}
			}

		}


		if (Game::instance().getKey('x') && !dashed) {
			dash = true;
			dashed = true;

			if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
					diagonalDashRight = true;

				}

				if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
					diagonalDashLeft = true;

				}
			}

			if (dashMode) {
				dashed = false;
			}
		}

		if (posPlayer.x < 0) {
			posPlayer.x = 350 * level;
		}

		if (posPlayer.y < 0) {
			level += 1;
			posPlayer.y = 240;
			posPlayer.x = 350.0 * level;
		}

		if (climbRight) {
			cout << 'n' << endl;
			posPlayer.x -= 2;
		}
		if (climbLeft) {
			cout << 'm' << endl;
			posPlayer.x += 2;
		}
		
		posPlayer.x += dashFinish;

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x + dashFinish), float(tileMapDispl.y + posPlayer.y)));
	}
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




