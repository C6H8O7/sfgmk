/*namespace sfgmk
{
	GameManager::GameManager() : m_MainRenderWindow(NULL), m_MainRenderTexture(NULL), m_CurrentCamera(NULL)
	{
		m_StateMachine = new engine::StateMachine();
	}

	GameManager::~GameManager()
	{
		//Destruction rendu
		delete m_MainRenderWindow;
		delete m_MainRenderTexture;

		//Destruction state machine
		if( m_StateMachine )
			delete m_StateMachine;

		//Destruction std::map caméras
		for( auto rit(m_Cameras.rbegin()); rit != m_Cameras.rend(); ++rit )
			delete(rit->second);
		m_Cameras.clear();

		//Destruction std::vector entités
		freeEntityVector();

		//Destruction post-shaders
		freePostShaderVector();
	}


	void GameManager::init(sf::VideoMode& _Mode, const std::string& _WindowName, int _Style)
	{
		if( m_MainRenderWindow )
			delete m_MainRenderWindow;
		if( m_MainRenderTexture )
			delete m_MainRenderTexture;

		m_MainRenderWindow = new sf::RenderWindow();
		m_MainRenderWindow->create(_Mode, _WindowName, _Style);
		m_MainRenderTexture = new sf::RenderTexture();
		m_MainRenderTexture->create(_Mode.width, _Mode.height);
	}


	void GameManager::update()
	{
		//Update du time delta
		m_Time = m_ClockTimeDelta.restart();
		m_fTimeDelta = m_Time.asSeconds();

		//Update de la caméra
		if( m_CurrentCamera )
		{
			m_CurrentCamera->update(m_fTimeDelta);
			m_MainRenderTexture->setView(*m_CurrentCamera);
		}
	}

	void GameManager::preLoop()
	{
		//Time delta et caméra
		this->update();

		//Quelque chose de mal
		if( !m_StateMachine )
			m_StateMachine = new engine::StateMachine();

		//Update inputs
		sfgmk::InputManager::getSingleton()->update();
		
		//Boucle events
		sf::Event event;
		while( m_MainRenderWindow->pollEvent(event) )
		{
			if( event.type == sf::Event::Closed )
				m_MainRenderWindow->close();

			sfgmk::InputManager::getSingleton()->handleEvent(event);
		}

		//Console
		sfgmk::ConsoleDev::getSingleton()->update(m_fTimeDelta);

		if( sfgmk::InputManager::getSingleton()->getKeyboard().getKeyState(sf::Keyboard::Quote) == sfgmk::KeyStates::KEY_PRESSED )
			sfgmk::ConsoleDev::getSingleton()->setActive();

		//Screenshot
		if( sfgmk::InputManager::getSingleton()->getKeyboard().getKeyState(sf::Keyboard::F12) == sfgmk::KeyStates::KEY_PRESSED )
			screenshot();

		//Son
		sfgmk::SoundManager::getSingleton()->update();

		//Clear render texture
		m_MainRenderTexture->clear(sf::Color::Black);
	}

	void GameManager::loop()
	{
		//Update état(s) courant(s)
		std::function<void()> StateMachineUpdate(std::bind(&engine::StateMachine::update, m_StateMachine));
		m_EntityTimers.dStateUpdate = measureFunctionExecutionTime(StateMachineUpdate);

		//Si state loading en cours, c'est une autre instance de parallaxe qui est utilisée
		Parallaxe* CurrentParallaxe = &m_Parallaxe;
		if( CURRENT_STATE->getID() == eLoadingState )
			CurrentParallaxe = ((sfgmk::StateLoading*)(CURRENT_STATE))->Get_Parallaxe();

		//Update des entités
		std::function<void()> EntityUpdate(std::bind(&GameManager::updateEntities, this));
		m_EntityTimers.dEntityUpdate = measureFunctionExecutionTime(EntityUpdate);

		//Physique
		std::function<void()> PhysicUpdate(std::bind(&PhysicManager::update, sfgmk::PhysicManager::getSingleton()));
		m_EntityTimers.dPhysic = measureFunctionExecutionTime(PhysicUpdate);

		//Trie les entités du vector en fonction de leur Z
		std::function<void()> EntitySort(std::bind(&GameManager::sortEntityVector, this));
		m_EntityTimers.dEntitySort = measureFunctionExecutionTime(EntitySort);

		//Calcul des coordonnées virtuelles pour l'affichage
		std::function<void()> ParallaxeUpdate(std::bind(&Parallaxe::update, CurrentParallaxe));
		m_EntityTimers.dParallaxeComputation = measureFunctionExecutionTime(ParallaxeUpdate);

		//Draw background (profondeur de 100 jusqu'à 0)
		std::function<void(int, int)> ParallaxeDrawB(std::bind(&Parallaxe::drawLayers, CurrentParallaxe, std::placeholders::_1, std::placeholders::_2));
		m_EntityTimers.dParallaxeDisplay = measureFunctionExecutionTime(ParallaxeDrawB, PARALLAXE_MAX_Z, PARALLAXE_MEDIUM_PLAN_Z);

		//Draw état courant, joueur... (tout ce qui est à 0 de profondeur)
		std::function<void()> StateMachineDraw(std::bind(&engine::StateMachine::draw, m_StateMachine));
		m_EntityTimers.dStateDraw = measureFunctionExecutionTime(StateMachineDraw);

		//Draw foreground (profondeur de -1 jusqu'à -100)
		std::function<void(int, int)> ParallaxeDrawF(std::bind(&Parallaxe::drawLayers, CurrentParallaxe, std::placeholders::_1, std::placeholders::_2));
		m_EntityTimers.dParallaxeDisplay += measureFunctionExecutionTime(ParallaxeDrawF, PARALLAXE_BEFORE_MEDIUM_PLAN_Z, PARALLAXE_MIN_Z);
	}

	void GameManager::postLoop()
	{
		applyPostShaders();

		//Rendu moteur physique
		if( sfgmk::PhysicManager::getSingleton()->getDraw() )
			sfgmk::PhysicManager::getSingleton()->draw(m_MainRenderTexture);

		//Rendu console
		sfgmk::ConsoleDev::getSingleton()->display(m_MainRenderTexture, m_CurrentCamera->getRelativOrigin());

		//Rendu final
		m_MainRenderTexture->display();
		m_RenderSprite.setTexture(m_MainRenderTexture->getTexture(), true);

		//Affichage fenetre
		m_MainRenderWindow->draw(m_RenderSprite);
		m_MainRenderWindow->display();
	}


	sf::RenderWindow* GameManager::getRenderWindow()
	{
		return m_MainRenderWindow;
	}

	sf::RenderTexture* GameManager::getRenderTexture()
	{
		return m_MainRenderTexture;
	}


	float GameManager::getTimeDelta()
	{
		return m_fTimeDelta;
	}


	engine::StateMachine* GameManager::getStateMachine()
	{
		return m_StateMachine;
	}


	void GameManager::registerCamera(std::string _CameraName, Camera* _Camera)
	{
		m_Cameras.insert(std::pair<std::string, Camera*>(_CameraName, _Camera));
	}

	Camera* GameManager::getCamera(std::string _CameraName)
	{
		Camera* CameraPtr = NULL;
		std::map<std::string, Camera*>::iterator it;

		if( (it = m_Cameras.find(_CameraName)) != m_Cameras.end() )
			CameraPtr = it->second;

		return CameraPtr;
	}

	Camera* GameManager::getCurrentCamera()
	{
		return m_CurrentCamera;
	}

	void GameManager::setDefaultCamera()
	{
		m_CurrentCamera = NULL;

		m_MainRenderTexture->setView(m_MainRenderTexture->getDefaultView());
	}

	bool GameManager::setCurrentCamera(std::string _CameraName)
	{
		Camera* TargetCamera = getCamera(_CameraName);

		if( TargetCamera != NULL )
		{
			m_CurrentCamera = TargetCamera;
			m_MainRenderTexture->setView(*TargetCamera);
			return true;
		}

		//Si la caméra recherchée n'est pas répertoriée, on affiche un message d'erreur
		printf("Camera introuvable\n");
		return false;
	}

	bool GameManager::removeCamera(std::string _CameraName)
	{
		std::map<std::string, Camera*>::iterator it;

		if( (it = m_Cameras.find(_CameraName)) != m_Cameras.end() )
		{
			delete(it->second);
			m_Cameras.erase(it);
			return true;
		}

		return false;
	}

	Parallaxe& GameManager::getParallaxe()
	{
		return m_Parallaxe;
	}


	void GameManager::updateEntities()
	{
		for( int i(m_Entities.size() - 1); i >= 0; i-- )
		{
			m_Entities[i]->update(m_fTimeDelta);

			//Suppression entités en fin de vie
			if( !m_Entities[i]->getIsAlive() )
			{
				int VectorEnd = m_Entities.size() - 1;
				Entity* Temp = m_Entities[i];
				m_Entities[i] = m_Entities[VectorEnd];
				m_Entities[VectorEnd] = Temp;
				delete Temp;
				m_Entities.pop_back();
			}
		}
	}

	const std::vector<Entity*>& GameManager::getEntityVector()
	{
		return m_Entities;
	}

	const int GameManager::getEntityNumber()
	{
		return m_Entities.size();
	}

	void GameManager::addEntity(Entity* _Entity)
	{
		m_Entities.push_back(_Entity);
	}

	void GameManager::freeEntityVector()
	{
		ClearPtrCntr(m_Entities);
	}

	void GameManager::sortEntityVector()
	{
		std::sort(m_Entities.begin(), m_Entities.end(), sortEntityByZ);
	}

	void GameManager::addPostShader(PostShader* _PostShader)
	{
		m_PostShaders.push_back(_PostShader);
	}

	void GameManager::applyPostShaders()
	{
		for( int i = (int)m_PostShaders.size() - 1; i >= 0; i-- )
		{
			PostShader *shader = m_PostShaders[i];

			if( shader->update(m_fTimeDelta) )
				shader->applyPostShader(*m_MainRenderTexture);
			else
			{
				delete shader;
				m_PostShaders[i] = m_PostShaders[m_PostShaders.size() - 1];
				m_PostShaders.pop_back();
			}
		}
	}

	void GameManager::freePostShaderVector()
	{
		ClearPtrCntr(m_PostShaders);
	}


	stGameManagerTimers& GameManager::getTimers()
	{
		return m_EntityTimers;
	}

	
	void GameManager::screenshot()
	{
		m_ScreenshotImage = m_MainRenderWindow->capture();

		time_t ttSecondes = 0;
		struct tm TimeSave;
		FILE* fSave = NULL;
		char cNomFichier[sizeof("%d-%d-%d_%dh%dm%ds.png")] = { '\0' };
		std::string sFileName = SFGMK_DATA_PATH + "screenshot/";

		//Recuperation date
		time(&ttSecondes);
		localtime_s(&TimeSave, &ttSecondes);;
		sprintf_s(cNomFichier, "%d-%d-%d_%dh%dm%ds.png", TimeSave.tm_year + 1900, TimeSave.tm_mon + 1, TimeSave.tm_mday, TimeSave.tm_hour, TimeSave.tm_min, TimeSave.tm_sec);
		sFileName += cNomFichier;

		m_ScreenshotImage.saveToFile(sFileName);
	}
	

	inline bool sortEntityByZ(Entity* _Entity1, Entity* _Entity2)
	{
		return(_Entity1->getZ() < _Entity2->getZ());
	}
}*/