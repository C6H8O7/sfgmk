StateDraft::StateDraft()
{
	for( int i(0); i < PLAYER_MAX_NUMBER; i++ )
	{
		sfgmk::Grid* NewPtr = NULL;
		m_Grids.pushBack(NewPtr);
	}

	sfgmk::initArrayValue(m_bPlayerHasJoined, sizeof(m_bPlayerHasJoined), false);
	sfgmk::initArrayValue(m_bPlayerHasChosen, sizeof(m_bPlayerHasJoined), false);
}

StateDraft::~StateDraft()
{
}


void StateDraft::init()
{
	DATA_MANAGER->loadLevel(m_sRessourcesPath);
	PARALLAXE.loadLevel(m_sRessourcesPath);
	SOUND_MANAGER->loadLevel(m_sRessourcesPath);
}

void StateDraft::update()
{
	if( SFGMK_KEYBOARD.getKeyState(sf::Keyboard::Return) == sfgmk::KEY_PRESSED )
		sfgmk::GameManager::getSingleton()->getStateMachine()->changeState(sfgmk::eLoadingState, eGameState);

	for( int i(0); i < PLAYER_MAX_NUMBER; i++ )
	{
		if( SFGMK_JOYSTICK_GET_BUTTON(i, BUTTON_A) == sfgmk::KEY_PRESSED )
		{
			//Rejoins la partie
			if( !m_bPlayerHasJoined[i] )
			{
 				m_bPlayerHasJoined[i] = true;
				addGrid(i);
			}

			//Choisis un vaisseau
			else if( !m_bPlayerHasChosen[i] )
			{
				m_bPlayerHasChosen[i] = true;
				m_Grids[i]->setApplyBloom(true, 0.5f, 2.0f, 0.2f, sf::Vector2f(0.0005f, 0.0005f));
			}
		}

		if( SFGMK_JOYSTICK_GET_BUTTON(i, BUTTON_B) == sfgmk::KEY_PRESSED )
		{
			//Quitte la sélection vaisseau
			if( m_bPlayerHasChosen[i] )
			{
				m_bPlayerHasChosen[i] = false;
				m_Grids[i]->setApplyBloom(false);
			}

			//Quitte la partie
			else if( m_bPlayerHasJoined[i] )
			{
				m_bPlayerHasJoined[i] = false;
				removeGrid(i);
			}
		}
	}
}

void StateDraft::deinit()
{
	m_Grids.clear();

	FREE_ENTITY_VECTOR;
	
	SOUND_MANAGER->stopAll();
	SOUND_MANAGER->unloadLevel(m_sRessourcesPath);

	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	PARALLAXE.unloadLevel(m_sRessourcesPath);
}


void StateDraft::draw()
{
	if (m_bPlayerHasJoined[0])
	{
		sfgmk::Sprite m_ship;
		m_ship.setTexture(DATA_MANAGER->getTexture("playerTypeI"));
		m_ship.setPosition(50.0f, 150.0f);
		GAME_MANAGER->getRenderTexture()->draw(m_ship);
	}
}


void StateDraft::addGrid(const int& _Number)
{
	m_Grids[_Number] = new sfgmk::Grid();
	m_Grids[_Number]->setPosition(sf::Vector3f(m_Grids[0]->getSize().x * (float)_Number, 0.0f, 0.0f) + sf::Vector3f(96.0f * (float)(_Number + 1), 100.0f, 0.0f));
	m_Grids[_Number]->setLineNumber(sf::Vector2i(16, 20));
	m_Grids[_Number]->setSpacing(sf::Vector2f(24.0f, 24.0f));
	m_Grids[_Number]->init();
	m_Grids[_Number]->setDrawMidLines(true);
	m_Grids[_Number]->setColor(sf::Color(0, 0, 150, 175));
	m_Grids[_Number]->setMidLinesColor(sf::Color(0, 0, 150, 175));
	m_Grids[_Number]->setBorderLinesColor(sf::Color::White);
	m_Grids[_Number]->setIsScaledByParralax(false);
	m_Grids[_Number]->setIsComputatedByParralax(false);
	ADD_ENTITY(m_Grids[_Number]);
}

void StateDraft::removeGrid(const int& _Number)
{
	m_Grids[_Number]->setIsAlive(false);
	m_Grids[_Number] = NULL;
}