StateMenu::StateMenu() 
{
}

StateMenu::~StateMenu()
{
}


void StateMenu::init()
{
	DATA_MANAGER->loadLevel(m_sRessourcesPath);
	PARALLAXE.loadLevel(m_sRessourcesPath);
	SOUND_MANAGER->loadLevel(m_sRessourcesPath);
}

void StateMenu::update()
{
	if( SFGMK_KEYBOARD.getKeyState(sf::Keyboard::Return) == sfgmk::KEY_PRESSED )
		sfgmk::GameManager::getSingleton()->getStateMachine()->changeState(eDraftState);
}

void StateMenu::deinit()
{
	SOUND_MANAGER->stopAll();
	SOUND_MANAGER->unloadLevel(m_sRessourcesPath);

	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	PARALLAXE.unloadLevel(m_sRessourcesPath);
}


void StateMenu::draw()
{
}