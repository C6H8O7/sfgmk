#include "ProjetSample\IncludesProjet.hpp"


StateDefault::StateDefault()
{
	DATA_MANAGER->loadLevel(m_sRessourcesPath);
	sfgmk::engine::GraphicManager::getSingleton()->getParallaxe().loadLevel(m_sRessourcesPath);
}

StateDefault::~StateDefault()
{
	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	sfgmk::engine::GraphicManager::getSingleton()->getParallaxe().unloadLevel(m_sRessourcesPath);
}

void StateDefault::init()
{
	goomba.setTexture(DATA_MANAGER->getTexture("goomba1"));
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