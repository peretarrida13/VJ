#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"


#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	Quad *quad;
	ShaderProgram program;
	float currentTime;
	glm::mat4 projection;
	float topRightX = 384.f;
	bool left = true;
	float topLeftX = 128.f;
	float scaleRight = 1;
	float scaleLeft = 1;

};


#endif // _SCENE_INCLUDE

