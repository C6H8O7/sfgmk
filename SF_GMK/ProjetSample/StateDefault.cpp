#include "ProjetSample\IncludesProjet.hpp"


StateDefault::StateDefault()
{

}

StateDefault::~StateDefault()
{

}

void StateDefault::init()
{
	DATA_MANAGER->loadTexture("goomba", "../Data/sfgmk/texture/goomba1.png");

	goomba.setTexture(DATA_MANAGER->getTexture("goomba"));
}

void StateDefault::update()
{

}

void StateDefault::deinit()
{

}

void StateDefault::draw()
{
	sfgmk::engine::GraphicManager::getSingleton()->getRenderTexture()->draw(goomba);
}