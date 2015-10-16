StateCredit::StateCredit()
{
}

StateCredit::~StateCredit()
{
}


void StateCredit::init()
{
	DATA_MANAGER->loadLevel(m_sRessourcesPath);
	PARALLAXE.loadLevel(m_sRessourcesPath);
	SOUND_MANAGER->loadLevel(m_sRessourcesPath);
}

void StateCredit::update()
{
}

void StateCredit::deinit()
{
	SOUND_MANAGER->stopAll();
	SOUND_MANAGER->unloadLevel(m_sRessourcesPath);

	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	PARALLAXE.unloadLevel(m_sRessourcesPath);
}


void StateCredit::draw()
{
}