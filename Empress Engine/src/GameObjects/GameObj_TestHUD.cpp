#include "GameObj_TestHUD.h"
#include "../Logger.h"
#include "../RenderInterface.h"
#include "../Scene.h"

GameObject_HUD::GameObject_HUD() {
	x = rand()%1280;
	y = rand()%720;
}

GameObject_HUD::~GameObject_HUD() { }

void GameObject_HUD::update() {

	glm::vec2 gotpos = getLoadedScene()->getTileLayerPosition(0);
	getLoadedScene()->setTileLayerPosition(0, { gotpos[0] - 1, gotpos[1] });

}

void GameObject_HUD::render() {
	x = 0;
	y = 0;
	int scale = 4;
	drawUIQuad("tex1", { 32, 0 }, { 16, 32 }, { x*scale,y*scale }, { scale, scale }, 30, 0.0, 1.0);

	drawUIQuad("tex1", { 48, 0 }, { 1, 32 }, { (x + 16)*scale, y*scale }, { scale * 256 , scale }, 30, 0.0, 1.0);

	drawUIQuad("tex1", { 64, 0 }, { 16, 32 }, { (x + 16 + 256)*scale,y*scale }, { scale, scale }, 30, 0.0, 1.0);

}