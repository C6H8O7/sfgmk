StateScore::StateScore()
{
}

StateScore::~StateScore()
{
}


void StateScore::init()
{
	DATA_MANAGER->loadLevel(m_sRessourcesPath);
	PARALLAXE.loadLevel(m_sRessourcesPath);
	SOUND_MANAGER->loadLevel(m_sRessourcesPath);
}

void StateScore::update()
{
}

void StateScore::deinit()
{
	SOUND_MANAGER->stopAll();
	SOUND_MANAGER->unloadLevel(m_sRessourcesPath);

	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	PARALLAXE.unloadLevel(m_sRessourcesPath);
}


void StateScore::draw()
{
}