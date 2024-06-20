#include "GameObj_TestHUD.h"
#include "../Logger.h"
#include "../RenderInterface.h"

GameObject_HUD::GameObject_HUD() {
	x = 100;
	y = 0;
}

GameObject_HUD::~GameObject_HUD() { }

void GameObject_HUD::update() {
}

void GameObject_HUD::render() {
	x = 0;
	y = 0;
	int scale = 4;
	drawUIQuad("tex1", { 96, 0 }, { 16, 32 }, { x*scale,y*scale }, { scale, scale }, 30, 0.0, 1.0);

	drawUIQuad("tex1", { 112, 0 }, { 1, 32 }, { (x + 16)*scale, y*scale }, { scale * 256 , scale }, 30, 0.0, 1.0);

	drawUIQuad("tex1", { 128, 0 }, { 16, 32 }, { (x + 16 + 256)*scale,y*scale }, { scale, scale }, 30, 0.0, 1.0);

}