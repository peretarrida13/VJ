#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 8

#define INIT_PLAYER_X_TILES 0
#define INIT_PLAYER_Y_TILES 11

Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	//afegir variables que modificarem cada cop que es passa un nivell (posPlayer.y < 0)
	projection = glm::ortho(screen_left, float(SCREEN_WIDTH/1.5) - screen_right, float(SCREEN_HEIGHT/1.75), 0.f); 
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	if (past_level != player->level) {
		if (player->level != 0) {
			screen_left = 320 * player->level;
			screen_right = -320 * player->level;
		}

		screen_left = 32.0 + 320*player->level;
		screen_right = -75.0 - 320*player->level;

		projection = glm::ortho(screen_left, float(SCREEN_WIDTH / 1.5) - screen_right, float(SCREEN_HEIGHT / 1.75), 0.f);
		cout << screen_left << ' ' << screen_right << endl;

		past_level = player->level;

		cout << past_level << ' ' << player->level << endl;
	}

	currentTime += deltaTime;
	player->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	if(player->level != 0) player->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}