#include "stdafx.h"
#include "../../IncludesProjet.hpp"

using namespace sfgmk;


StateMenu::StateMenu()
{
	DATA_MANAGER->loadLevel(m_sRessourcesPath);
	PARALLAXE->loadLevel(m_sRessourcesPath);
	SOUND_MANAGER->loadLevel(m_sRessourcesPath);
}

StateMenu::~StateMenu()
{
	ENTITY_MANAGER->freeEntityVector();
	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	PARALLAXE->unloadLevel(m_sRessourcesPath);
	SOUND_MANAGER->unloadLevel(m_sRessourcesPath);
}


void StateMenu::init()
{
}

void StateMenu::update()
{
	if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::Return) == KEY_PRESSED )
		CHANGE_STATE_WITH_LOADING(eStateLoading, eStateGame);
}

void StateMenu::deinit()
{
}


void StateMenu::draw()
{
}