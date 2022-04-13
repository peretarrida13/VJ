#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
	int level = 0;

private:
	void collision();
	void checkTecles();
	void climb(bool right);

	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	
	bool dash = false;
	int dashLength = 0;
	int dashFinish = 0;
	bool dashed = false;
	bool diagonalDashLeft = false;
	bool diagonalDashRight = false;

	bool climbRight = false;
	bool climbLeft = false;
	bool climbing = true;

	int last_animation;
	bool floor = true;

	bool slowMode = false; //S
	bool dashMode = false; //D

	float speed;
};


#endif // _PLAYER_INCLUDE


