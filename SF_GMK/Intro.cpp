StateIntro::StateIntro()
{
	DATA_MANAGER->loadTexture("lightningCorpse", "data/sfgmk/texture/lightningCorpse.png");

	DATA_MANAGER->loadFont("fontText", "data/states/intro/font/KeepCalm-Medium.ttf");
	std::string S("Bonjour \nmaman \net papa");
	m_Title = new sfgmk::LightningText(S, DATA_MANAGER->getFont("fontText"), 150U, 50, 500);
}

StateIntro::~StateIntro()
{
	delete m_Title;
}


void StateIntro::init()
{
	DATA_MANAGER->loadLevel(m_sRessourcesPath);
	PARALLAXE.loadLevel(m_sRessourcesPath);
	SOUND_MANAGER->loadLevel(m_sRessourcesPath);
}

void StateIntro::update()
{
	m_Title->update(TIME_DELTA);

	if( SFGMK_KEYBOARD.getKeyState(sf::Keyboard::Return) == sfgmk::KEY_PRESSED )
		sfgmk::GameManager::getSingleton()->getStateMachine()->changeState(eMenuState);
}

void StateIntro::deinit()
{
	SOUND_MANAGER->stopAll();
	SOUND_MANAGER->unloadLevel(m_sRessourcesPath);

	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	PARALLAXE.unloadLevel(m_sRessourcesPath);
}


void StateIntro::draw()
{
	m_Title->draw(GAME_MANAGER->getRenderTexture());
}