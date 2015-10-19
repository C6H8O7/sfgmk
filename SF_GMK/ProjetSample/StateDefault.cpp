#include "ProjetSample\IncludesProjet.hpp"


StateDefault::StateDefault()
{
	sfgmk::DATA_MANAGER->loadLevel(m_sRessourcesPath);
	sfgmk::PARALLAXE.loadLevel(m_sRessourcesPath);
}

StateDefault::~StateDefault()
{
	sfgmk::DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	sfgmk::PARALLAXE.unloadLevel(m_sRessourcesPath);
}

void StateDefault::init()
{
	goomba.setTexture(sfgmk::DATA_MANAGER->getTexture("goomba1"));
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