namespace sfgmk
{
	namespace engine
	{
		StateLoading::StateLoading() : m_iStateToLoadId(STATE_MACHINE->getStateToLoadId()), m_sStateToLoadDataPath(STATE_MACHINE_MANAGER->getStateRessourcePath(m_iStateToLoadId)), m_bThreadsLaunched(false)
		{
			m_ButtonTexture[0] = DATA_MANAGER->getTexture("sfgmk_toucheEnter");
			m_ButtonTexture[1] = DATA_MANAGER->getTexture("sfgmk_buttonA");
			m_Font = DATA_MANAGER->getFont("sfgmk_Fipps-Regular");
		}

		StateLoading::~StateLoading()
		{
		}


		void StateLoading::init()
		{
			//Calcule le nombre de fichiers à charger
			m_RessourcesCounters[eLevel].uiRessourceLoaded = 0;
			m_RessourcesCounters[eLevel].uiRessourceToLoad = getNumberOfFileInDir(m_sStateToLoadDataPath + "/layer");

			m_RessourcesCounters[eAsset].uiRessourceLoaded = 0;
			m_RessourcesCounters[eAsset].uiRessourceToLoad = getNumberOfFileInDir(m_sStateToLoadDataPath + "/texture")
															+ getNumberOfFileInDir(m_sStateToLoadDataPath + "/animation")
															+ getNumberOfFileInDir(m_sStateToLoadDataPath + "/font")
															+ getNumberOfFileInDir(m_sStateToLoadDataPath + "/shader");

			m_RessourcesCounters[eSound].uiRessourceLoaded = 0;
			m_RessourcesCounters[eSound].uiRessourceToLoad = getNumberOfFileInDir(m_sStateToLoadDataPath + "/audio/music")
															+ getNumberOfFileInDir(m_sStateToLoadDataPath + "/audio/sound");

			//Init les fonctions
			sfgmk::FoncterTemplateInstance<Parallaxe, bool, const std::string&>* PtrFuncLevel = new sfgmk::FoncterTemplateInstance<Parallaxe, bool, const std::string&>(&PARALLAXE, &Parallaxe::loadLevel);
			sfgmk::FoncterTemplateInstance<DataManager, bool, const std::string&>* PtrFuncAsset = new sfgmk::FoncterTemplateInstance<DataManager, bool, const std::string&>(DATA_MANAGER, &DataManager::loadLevel);
			sfgmk::FoncterTemplateInstance<SoundManager, bool, const std::string&>* PtrFuncAudio = new sfgmk::FoncterTemplateInstance<SoundManager, bool, const std::string&>(SOUND_MANAGER, &SoundManager::loadLevel);

			m_LoadThreads[eLevel].SetFunc(PtrFuncLevel);
			m_LoadThreads[eAsset].SetFunc(PtrFuncAsset);
			m_LoadThreads[eSound].SetFunc(PtrFuncAudio);
		}

		void StateLoading::update()
		{
			std::cout << "		UP		" << std::endl;

			if( !m_bThreadsLaunched )
			{
				m_LoadThreads[eLevel].Launch(m_sStateToLoadDataPath);
				m_LoadThreads[eAsset].Launch(m_sStateToLoadDataPath);
				m_LoadThreads[eSound].Launch(m_sStateToLoadDataPath);
				m_bThreadsLaunched = true;
				std::cout << "LAUNCH THREADS" << std::endl;
			}

			m_LoadThreads[eLevel].Wait();
			m_LoadThreads[eAsset].Wait();
			m_LoadThreads[eSound].Wait();

			std::cout << "THREADS finis" << std::endl;

			//Threads terminés, permet de passer à l'état suivant
/*			else if( m_bThreadOver )
			{
				if( INPUT_MANAGER->getKeyboard().getKeyState(sf::Keyboard::Return) == KEY_PRESSED
				   || JOYSTICK_GET_BUTTON(0, BUTTON_A) == KEY_PRESSED )
					CHANGE_STATE(m_iStateToLoadId);
			}*/
		}

		void StateLoading::deinit()
		{
		}


		void StateLoading::draw()
		{
			//Affiche les touches à presser pour passer à l'état suivant
			/*if( m_bThreadOver )
			{
				sf::RenderTexture* MainRenderTexture(GRAPHIC_MANAGER->getRenderTexture());
				sf::Vector2u MainRenderTextureSize = MainRenderTexture->getSize();
				Sprite Sprite;
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
				GRAPHIC_MANAGER->getRenderTexture()->draw(Sprite);
			}*/
		}


		
			/*std::lock_guard<std::mutex> Lock(m_Mutex);

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
				m_bLoadOver = true;*/
		
	}
}