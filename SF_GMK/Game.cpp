StateGame::StateGame()
{
}

StateGame::~StateGame()
{
}


void StateGame::init()
{
	LoadShipsStats();

	m_Grid = new sfgmk::Grid(sf::Vector2i(120, 66), sf::Vector2f(24.0f, 24.0f), sf::Vector3f(0.0f, 0.0f, 1.0f)); //120 66
	m_Grid->setDrawMidLines(true);
	m_Grid->setColor(sf::Color(0, 0, 150, 175));
	m_Grid->setMidLinesColor(sf::Color(0, 0, 150, 175));
	m_Grid->setBorderLinesColor(sf::Color::White);
	m_Grid->setIsScaledByParralax(false);
	m_Grid->setIsComputatedByParralax(false);
	m_Grid->setApplyBloom(true, 0.5f, 2.0f, 0.2f, sf::Vector2f(0.0005f, 0.0005f));
	ADD_ENTITY(m_Grid);

	AddPlayer(eTypeI);



	
	

	
	

	//sfgmk::PhysicManager::getSingleton()->setDraw(true);


	/*for( int i(0); i < 150; i++ )
	{
		sfgmk::Entity* Goomba = new sfgmk::Entity(sf::Vector3f((float)RAND(50, 1870), (float)RAND(50, 1030), 0.0f));
		Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
		float fScale = float(RAND(2, 4)) * 0.5f;
		Goomba->setScale(sf::Vector2f(fScale, fScale));
		Goomba->setRelativOrigin(0.5f, 0.5f);
		Goomba->setRotation(float(RAND(0, 360)));
		Goomba->addObbCollider();
		ADD_ENTITY(Goomba);
	}

	for( int i(0); i < 150; i++ )
	{
		sfgmk::Entity* Goomba = new sfgmk::Entity(sf::Vector3f((float)RAND(50, 1870), (float)RAND(50, 1030), 0.0f));
		Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
		float fScale = float(RAND(2, 4)) * 0.5f;
		Goomba->setScale(sf::Vector2f(fScale, fScale));
		Goomba->setRelativOrigin(0.5f, 0.5f);
		Goomba->setRotation(float(RAND(0, 360)));
		Goomba->addSphereCollider();
		ADD_ENTITY(Goomba);
	}*/

	/*sfgmk::Entity* Goomba = new sfgmk::Entity(sf::Vector3f(250.0f, 250.0f, 0));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	//float fScale = float(RAND(1, 4)) * 0.5f;
	//Goomba->setScale(sf::Vector2f(fScale, fScale));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	//	Goomba->setRotation(RAND(0, 360));
	Goomba->addObbCollider();
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(225.0f, 225.0f, 0));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	//float fScale = float(RAND(1, 4)) * 0.5f;
	//Goomba->setScale(sf::Vector2f(fScale, fScale));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	//	Goomba->setRotation(RAND(0, 360));
	Goomba->addSphereCollider();
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(225.0f, 275.0f, 0));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	//float fScale = float(RAND(1, 4)) * 0.5f;
	//Goomba->setScale(sf::Vector2f(fScale, fScale));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	//	Goomba->setRotation(RAND(0, 360));
	Goomba->addSphereCollider();
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(275.0f, 225.0f, 0));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	//float fScale = float(RAND(1, 4)) * 0.5f;
	//Goomba->setScale(sf::Vector2f(fScale, fScale));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	//	Goomba->setRotation(RAND(0, 360));
	Goomba->addSphereCollider();
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(275.0f, 275.0f, 0));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	//float fScale = float(RAND(1, 4)) * 0.5f;
	//Goomba->setScale(sf::Vector2f(fScale, fScale));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	//	Goomba->setRotation(RAND(0, 360));
	Goomba->addSphereCollider();
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(190.0f, 250.0f, 0));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	//float fScale = float(RAND(1, 4)) * 0.5f;
	//Goomba->setScale(sf::Vector2f(fScale, fScale));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	//	Goomba->setRotation(RAND(0, 360));
	Goomba->addSphereCollider();
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(310.0f, 250.0f, 0));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	//float fScale = float(RAND(1, 4)) * 0.5f;
	//Goomba->setScale(sf::Vector2f(fScale, fScale));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	//	Goomba->setRotation(RAND(0, 360));
	Goomba->addSphereCollider();
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(250.0f, 190.0f, 0));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	//float fScale = float(RAND(1, 4)) * 0.5f;
	//Goomba->setScale(sf::Vector2f(fScale, fScale));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	//	Goomba->setRotation(RAND(0, 360));
	Goomba->addSphereCollider();
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(250.0f, 310.0f, 0));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	//float fScale = float(RAND(1, 4)) * 0.5f;
	//Goomba->setScale(sf::Vector2f(fScale, fScale));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	//	Goomba->setRotation(RAND(0, 360));
	Goomba->addSphereCollider();
	ADD_ENTITY(Goomba);*/


	/*sfgmk::Entity* Goomba = new sfgmk::Entity(sf::Vector3f(100.0f, 100.0f, -1.0f));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	Goomba->addSphereCollider(50.0f);
	ADD_ENTITY(Goomba);


	Goomba = new sfgmk::Entity(sf::Vector3f(50.0f, 50.0f, -1.0f));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	Goomba->addSphereCollider(20.0f);
	ADD_ENTITY(Goomba);	

	Goomba = new sfgmk::Entity(sf::Vector3f(150.0f, 150.0f, -1.0f));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	Goomba->addSphereCollider(20.0f);
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(50.0f, 150.0f, -1.0f));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	Goomba->addSphereCollider(20.0f);
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(150.0f, 50.0f, -1.0f));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	Goomba->addSphereCollider(20.0f);
	ADD_ENTITY(Goomba);


	Goomba = new sfgmk::Entity(sf::Vector3f(100.0f, 50.0f, -1.0f));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	Goomba->addSphereCollider(20.0f);
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(100.0f, 150.0f, -1.0f));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	Goomba->addSphereCollider(20.0f);
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(50.0f, 100.0f, -1.0f));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	Goomba->addSphereCollider(20.0f);
	ADD_ENTITY(Goomba);

	Goomba = new sfgmk::Entity(sf::Vector3f(150.0f, 100.0f, -1.0f));
	Goomba->getSprite()->setTexture(DATA_MANAGER->getTexture("goomba1"));
	Goomba->setRelativOrigin(0.5f, 0.5f);
	Goomba->addSphereCollider(20.0f);
	ADD_ENTITY(Goomba);*/
	

	/*sfgmk::GenericList FuncArray;

	typedef void(*FuncType)();
	FuncType ptr = po;

	typedef int(*FuncTypeD)();
	FuncTypeD ptrD = popo;

	FuncArray.pushBack(&ptr, sizeof(FuncType*));
	FuncArray.pushBack(&ptrD, sizeof(FuncTypeD*));
	

	ptr = *((FuncType*)FuncArray[0]->pData);
	ptr();

	ptrD = *((FuncTypeD*)FuncArray[1]->pData);
	ptrD();

	FuncArray.clearList();




	std::cout << std::endl;*/


	/*sfgmk::GenericList Lis;
	int i = 0;
	int j = 1;
	int k = 2;
	int l = 3;
	Lis.addElement(&j, sizeof(int));
	Lis.pushBack(&k, sizeof(int));
	Lis.pushFront(&i, sizeof(int));
	Lis.addElement(&l, sizeof(int), 10);

	for( unsigned int i(0); i < Lis.getElementNumber(); i++ )
	{
		int iResult = *(int*)Lis.getElementData(i);
			std::cout << iResult << std::endl;
	}
	Lis.removeElement(3);

	std::cout << std::endl;
	

	for( unsigned int i(0); i < Lis.getElementNumber(); i++ )
	{
		int iResult = *(int*)Lis.getElementData(i);
		std::cout << iResult << std::endl;
	}

	Lis.clearList();

	for( unsigned int i(0); i < Lis.getElementNumber(); i++ )
	{
		int iResult = *(int*)Lis.getElementData(i);
		std::cout << iResult << std::endl;
	}*/
}

void StateGame::update()
{
	/*if( SFGMK_KEYBOARD.getKeyState(sf::Keyboard::V) == sfgmk::KeyStates::KEY_PRESSED || SFGMK_KEYBOARD.getKeyState(sf::Keyboard::V) == sfgmk::KeyStates::KEY_DOWN )
		m_Grid->applyDirectedForce(sf::Vector3f(-250.0f, 0.0f, 0.0f), sf::Vector3f(250.0f, 250.0f, 0.0f), 200.0f);

	if( SFGMK_KEYBOARD.getKeyState(sf::Keyboard::B) == sfgmk::KeyStates::KEY_PRESSED || SFGMK_KEYBOARD.getKeyState(sf::Keyboard::B) == sfgmk::KeyStates::KEY_DOWN )
		m_Grid->applyExplosiveForce(200.0f, sf::Vector3f(250.0f, 250.0f, 0.0f), 50.0f);

	if( SFGMK_KEYBOARD.getKeyState(sf::Keyboard::N) == sfgmk::KeyStates::KEY_PRESSED || SFGMK_KEYBOARD.getKeyState(sf::Keyboard::N) == sfgmk::KeyStates::KEY_DOWN )
		m_Grid->applyImplosiveForce(250.0f, sf::Vector3f(250.0f, 250.0f, 0.0f), 200.0f);*/

	static sfgmk::LightningBolt* Bolt = NULL;
	if( SFGMK_KEYBOARD.getKeyState(sf::Keyboard::L) == sfgmk::KeyStates::KEY_PRESSED )
	{
		Bolt = new sfgmk::LightningBolt(sf::Vector3f(1920.0f * 0.5f, 100.0f, 0.0f), sf::Vector2f(1920.0f * 0.5f, 800.0f), true, sf::Color::White, 1);
		ADD_ENTITY(Bolt);
	}
	if( SFGMK_KEYBOARD.getKeyState(sf::Keyboard::M) == sfgmk::KeyStates::KEY_PRESSED )
	{
		Bolt = new sfgmk::LightningBolt(sf::Vector3f(100.0f, 500.0f, 0.0f), sf::Vector2f(800, 550.0f), true, sf::Color::White, 1);
		ADD_ENTITY(Bolt);
	}

	if( SFGMK_KEYBOARD.getKeyState(sf::Keyboard::K) == sfgmk::KeyStates::KEY_PRESSED )
	{
		Bolt = new sfgmk::LightningBolt(sf::Vector3f(1600.0f, 100.0f, 0.0f), sf::Vector2f(1700.0f, 110.0f), true, sf::Color::White, 1);
		ADD_ENTITY(Bolt);
	}
}

void StateGame::deinit()
{
	m_PlayerArray.clear();

	FREE_ENTITY_VECTOR;

	SOUND_MANAGER->stopAll();
	SOUND_MANAGER->unloadLevel(m_sRessourcesPath);

	DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	PARALLAXE.unloadLevel(m_sRessourcesPath);
}


void StateGame::draw()
{
}


sfgmk::Grid* StateGame::getGrid()
{
	return m_Grid;
}


sfgmk::DynamicArray<Player*>& StateGame::getPlayers()
{
	return m_PlayerArray;
}

void StateGame::AddPlayer(const ePLAYER_SHIP_TYPE& _ShipType)
{
	Player* NewPlayer = new Player(m_PlayerArray.getElementNumber(), _ShipType, m_Grid);
	NewPlayer->setRelativOrigin(0.5f, 0.5f);
	NewPlayer->setPosition(sf::Vector3f(1920.0f * 0.5f, 1080.0f * 0.5f, 0.0f));
	NewPlayer->addObbCollider();
	m_PlayerArray.pushBack(NewPlayer);
	ADD_ENTITY(NewPlayer);
	GAME_MANAGER->getCurrentCamera()->linkToEntity(NewPlayer);
}


void StateGame::LoadShipsStats()
{
	FILE* File = NULL;
	DIR* Repertory = NULL;
	struct dirent* ReadFile = NULL;
	unsigned int iNumberOfFile(0U);

	//Si erreur ouverture
	if( (Repertory = opendir(SHIPS_STATS_PATH.c_str())) == NULL )
		perror(SHIPS_STATS_PATH.c_str());

	//Si dossier trouvé
	else
	{
		//Caractères '.' et ".."
		for( int i(0); i < 2; i++ )
			readdir(Repertory);

		while( (ReadFile = readdir(Repertory)) != NULL && iNumberOfFile < (sizeof(m_ShipsStats) / sizeof(ePLAYER_SHIP_TYPE)) ) //On récupére tous les fichiers contenus dans la limite de la taille du tableau
		{
			std::string Name = ReadFile->d_name;
			
			fopen_s(&File, (SHIPS_STATS_PATH + Name).c_str(), "rt");
			{
				//Stats
				fscanf_s(File, "%d", &m_ShipsStats[iNumberOfFile].iLife);
				sfgmk::goToChar(File, '\n');
				fscanf_s(File, "%f", &m_ShipsStats[iNumberOfFile].fSpeed);
				sfgmk::goToChar(File, '\n');
				fscanf_s(File, "%f", &m_ShipsStats[iNumberOfFile].fAcceleration);
				sfgmk::goToChar(File, '\n');
				fscanf_s(File, "%f", &m_ShipsStats[iNumberOfFile].fAccelerationReactivity);
				sfgmk::goToChar(File, '\n');
				fscanf_s(File, "%f", &m_ShipsStats[iNumberOfFile].fDecelerationReactivity);
				sfgmk::goToChar(File, '\n');
				fscanf_s(File, "%f", &m_ShipsStats[iNumberOfFile].fRotation);

				//Cooldowns
				sfgmk::goToChar(File, '\n');
				fscanf_s(File, "%f", &m_ShipsStats[iNumberOfFile].fCooldownPrimaryWeapon);

				fclose(File);
			}

			iNumberOfFile++;
		}
	}
}

const sSHIP_STAT& StateGame::Get_ShipsStats(const ePLAYER_SHIP_TYPE& _Type)
{
	return m_ShipsStats[_Type];
}