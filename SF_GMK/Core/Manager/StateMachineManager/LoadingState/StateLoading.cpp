namespace sfgmk
{
	StateLoading::StateLoading() : m_bThreadLaunched(false), m_bLoadOver(false), m_bThreadOver(false), m_LinkedParallaxe(NULL), m_bRenderTextureCreated(false), m_fAngle(PI_2)
	{
		m_iStateToLoadId = STATE_MACHINE->getStateToLoadId();
		m_sStateToLoadDataPath = STATE_BANK->getStateRessourcePath(m_iStateToLoadId);

		m_LinkedParallaxe = new Parallaxe();
		m_LinkedParallaxe->loadLevel(m_sRessourcesPath);
	
		sfgmk::initArrayValue(m_iLoadingPercentage, eSTATE_LOADING_DATA_TYPE_NUMBER, 0);

		m_ButtonTexture[0].loadFromFile(m_sRessourcesPath +  "/texture/toucheEnter.png");
		m_ButtonTexture[1].loadFromFile(m_sRessourcesPath + "/texture/buttonA.png");
		m_Font.loadFromFile(m_sRessourcesPath + "/font/Fipps-Regular.otf");
	}

	StateLoading::~StateLoading()
	{
		SAFE_DELETE(m_LinkedParallaxe);
	}

	void StateLoading::init()
	{
		//Calcule le nombre de fichiers à charger
		m_iRessourcesCounter[eLevel].iRessourceLoaded = 0;
		m_iRessourcesCounter[eLevel].iRessourceToLoad = sfgmk::getNumberOfFileInDir(m_sStateToLoadDataPath + "/layer");
	
		m_iRessourcesCounter[eAsset].iRessourceLoaded = 0;
		m_iRessourcesCounter[eAsset].iRessourceToLoad = sfgmk::getNumberOfFileInDir(m_sStateToLoadDataPath + "/texture")
														+ sfgmk::getNumberOfFileInDir(m_sStateToLoadDataPath + "/animation")
														+ sfgmk::getNumberOfFileInDir(m_sStateToLoadDataPath + "/font")
														+ sfgmk::getNumberOfFileInDir(m_sStateToLoadDataPath + "/shader");
		
		m_iRessourcesCounter[eSound].iRessourceLoaded = 0;
		m_iRessourcesCounter[eSound].iRessourceToLoad = sfgmk::getNumberOfFileInDir(m_sStateToLoadDataPath + "/audio/music")
														+ sfgmk::getNumberOfFileInDir(m_sStateToLoadDataPath + "/audio/sound");
		
		//Init les fonctions
		m_ThreadFunctions[eLevel] = std::function<bool(const std::string&, StateLoading*)>(std::bind(&Parallaxe::loadLevel, &PARALLAXE, std::placeholders::_1, std::placeholders::_2));
		m_ThreadFunctions[eAsset] = std::function<bool(const std::string&, StateLoading*)>(std::bind(&DataManager::loadLevel, DATA_MANAGER, std::placeholders::_1, std::placeholders::_2));
		m_ThreadFunctions[eSound] = std::function<bool(const std::string&, StateLoading*)>(std::bind(&SoundManager::loadLevel, SOUND_MANAGER, std::placeholders::_1, std::placeholders::_2));
	}

	void StateLoading::update()
	{
		//Init les threads
		if( !m_bThreadLaunched )
		{
			m_bThreadLaunched = true;
			m_Threads[eLevel] = std::thread(m_ThreadFunctions[eLevel], m_sStateToLoadDataPath, this);
			m_Threads[eAsset] = std::thread(m_ThreadFunctions[eAsset], m_sStateToLoadDataPath, this);
			m_Threads[eSound] = std::thread(m_ThreadFunctions[eSound], m_sStateToLoadDataPath, this);
		}

		//Checke l'avancement du chargement
		CheckLoadingProgress();

		//Attend la fin des threads, quand tout a été chargé
		if( m_bLoadOver && !m_bThreadOver )
		{
			m_Threads[eLevel].join();
			m_Threads[eAsset].join();
			m_Threads[eSound].join();

			m_bThreadOver = true;
		}

		//Threads terminés, permet de passer à l'état suivant
		else if( m_bThreadOver )
		{
			if( INPUT_MANAGER->getKeyboard().getKeyState(sf::Keyboard::Return) == KEY_PRESSED 
				|| SFGMK_JOYSTICK_GET_BUTTON(0, BUTTON_A) == KEY_PRESSED )
				CHANGE_STATE(m_iStateToLoadId);
		}
	}

	void StateLoading::deinit()
	{
	}


	void StateLoading::draw()
	{
		//Affiche les touches à presser pour passer à l'état suivant
		if( m_bThreadOver )
		{
			sf::RenderTexture* MainRenderTexture(GAME_MANAGER->getRenderTexture());
			sf::Vector2u MainRenderTextureSize = MainRenderTexture->getSize();
			sfgmk::Sprite Sprite;
			unsigned int uiTextSize = 32U;
			sf::Vector2f TextRectSize;
			sf::Vector2f WindowSizeRatio(MainRenderTextureSize.x / 1920.0f, MainRenderTextureSize.y / 1080.0f);

			//Clignotement du texte
			m_fAngle += PI_2 * TIME_DELTA;
			unsigned int uiAlpha = (unsigned int)(255 * ABS(cos(m_fAngle)));

			//Création texte
			sf::Text Text("Press      or    to continue", m_Font, uiTextSize);
			Text.setColor(sf::Color(150, 150, 150, 255));
			Text.setPosition(sf::Vector2f(0.0f, (float)uiTextSize * 0.25f));
			TextRectSize = sf::Vector2f(Text.getLocalBounds().width, Text.getLocalBounds().height);
			
			//Init texture de rendu à la bonne taille lors du premier appel
			if( !m_bRenderTextureCreated )
			{
				m_bRenderTextureCreated = true;
				m_RenderTexture.create((unsigned int)TextRectSize.x, (unsigned int)TextRectSize.y);
			}

			//Clear texture
			m_RenderTexture.clear(sf::Color(0, 0, 0, 0));

			//Draw et finalisation du rendu
			Sprite.setTexture(m_ButtonTexture[0], true);
			Sprite.setPosition(sf::Vector2f(170.0f, 0.0f));
			m_RenderTexture.draw(Sprite);
			Sprite.setTexture(m_ButtonTexture[1], true);
			Sprite.setPosition(sf::Vector2f(300.0f, -10.0f));
			m_RenderTexture.draw(Sprite);
			m_RenderTexture.draw(Text);
			m_RenderTexture.display();

			//Draw dans le rendu principal
			Sprite.setTexture(m_RenderTexture.getTexture(), true);
			Sprite.setScale(WindowSizeRatio.x, WindowSizeRatio.y);
			Sprite.setColor(sf::Color(255, 255, 255, uiAlpha));
			Sprite.setPosition((MainRenderTextureSize.x * 0.5f - TextRectSize.x * 0.5f * WindowSizeRatio.x), (MainRenderTextureSize.y - 2.5f * TextRectSize.y * WindowSizeRatio.y));
			GAME_MANAGER->getRenderTexture()->draw(Sprite);
		}
	}


	void StateLoading::CheckLoadingProgress()
	{
		std::lock_guard<std::mutex> Lock(m_Mutex);

		unsigned int uiCounter(0u);

		for( int i(0); i < eSTATE_LOADING_DATA_TYPE_NUMBER; i++ )
		{
			//Sécurité division par 0
			if( m_iRessourcesCounter[i].iRessourceToLoad == 0 )
				m_iLoadingPercentage[i] = 100;

			//Calcul pourcentage chargé
			else
				m_iLoadingPercentage[i] = (int)((float)m_iRessourcesCounter[i].iRessourceLoaded / (float)m_iRessourcesCounter[i].iRessourceToLoad * 100.0f);

			//Compte les chargements terminés
			if( m_iRessourcesCounter[i].iRessourceLoaded == m_iRessourcesCounter[i].iRessourceToLoad )
				uiCounter++;
		}
	
		//Si tous les chargements terminés, on signale que le passage à l'état suivant est ok
		if( uiCounter == eSTATE_LOADING_DATA_TYPE_NUMBER )
			m_bLoadOver = true;
	}

	void StateLoading::AddToCounter(const eSTATE_LOADING_DATA_TYPE& _DataType)
	{
		std::lock_guard<std::mutex> Lock(m_Mutex);

		m_iRessourcesCounter[_DataType].iRessourceLoaded++;
	}


	Parallaxe* StateLoading::Get_Parallaxe()
	{
		return m_LinkedParallaxe;
	}
}