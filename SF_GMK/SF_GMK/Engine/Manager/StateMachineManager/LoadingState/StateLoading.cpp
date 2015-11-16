namespace sfgmk
{
	namespace engine
	{
		StateLoading::StateLoading() : m_iStateToLoadId(STATE_MACHINE->getStateToLoadId()), m_sStateToLoadDataPath(STATE_MACHINE_MANAGER->getStateRessourcePath(m_iStateToLoadId)), m_bThreadsLaunched(false), m_bLoadThreadsOver(false), m_fAngle(0.0f), m_sLoadString("")
		{
			//Charge le state loading en lui-même
			DATA_MANAGER->loadLevel(m_sRessourcesPath);
			m_Parallaxe.loadLevel(m_sRessourcesPath);	//Attention on charge la parallaxe "privé" du loading, pas la générale du programme
			GRAPHIC_MANAGER->setCurrentParallaxe(&m_Parallaxe);
			SOUND_MANAGER->loadLevel(m_sRessourcesPath);

			m_RessourcesCounters[eLevel].sRessourcesName = "Layers";
			m_RessourcesCounters[eAsset].sRessourcesName = "Assets";
			m_RessourcesCounters[eSound].sRessourcesName = "Sons";

			m_ButtonTexture[0] = DATA_MANAGER->getTexture("sfgmk_toucheEnter");
			m_ButtonTexture[1] = DATA_MANAGER->getTexture("sfgmk_buttonA");
			m_Font = DATA_MANAGER->getFont("sfgmk_Fipps-Regular");

			//Barres de progression
			m_LoadFont = DATA_MANAGER->getFont("sfgmk_ConsoleFont1");
			m_LoadText.setFont(m_LoadFont);
			m_LoadText.setCharacterSize(22);
			m_LoadText.setColor(sf::Color::Black);

			m_BarRect.setOutlineThickness(2.0f);
			m_BarRect.setOutlineColor(sf::Color::Black);
			m_BarRect.setFillColor(sf::Color::White);
			m_BarRect.setSize(sf::Vector2f((float)GRAPHIC_MANAGER->getRenderTexture()->getSize().x * 0.25f, 26.0f));

			m_GaugeRect.setOutlineThickness(0);
			m_GaugeRect.setSize(m_BarRect.getSize());
		}

		StateLoading::~StateLoading()
		{
			DATA_MANAGER->unloadLevel(m_sRessourcesPath);
			GRAPHIC_MANAGER->setCurrentParallaxe();
			m_Parallaxe.unloadLevel(m_sRessourcesPath);
			SOUND_MANAGER->unloadLevel(m_sRessourcesPath);
		}


		void StateLoading::init()
		{
			//Rendu
			unsigned int uiTextSize = 32U;
			sf::Vector2f TextRectSize;
			m_Text.setFont(m_Font);
			m_Text.setCharacterSize(uiTextSize);
			m_Text.setString("Press      or    to continue");
			m_Text.setColor(sf::Color(150, 150, 150, 255));
			m_Text.setPosition(sf::Vector2f(0.0f, (float)uiTextSize * 0.25f));
			TextRectSize = sf::Vector2f(m_Text.getLocalBounds().width, m_Text.getLocalBounds().height);

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
			sfgmk::FoncterTemplateInstance<Parallaxe, bool, const std::string&>* PtrFuncLevel = new sfgmk::FoncterTemplateInstance<Parallaxe, bool, const std::string&>(PARALLAXE, &Parallaxe::loadLevel);
			sfgmk::FoncterTemplateInstance<DataManager, bool, const std::string&>* PtrFuncAsset = new sfgmk::FoncterTemplateInstance<DataManager, bool, const std::string&>(DATA_MANAGER, &DataManager::loadLevel);
			sfgmk::FoncterTemplateInstance<SoundManager, bool, const std::string&>* PtrFuncAudio = new sfgmk::FoncterTemplateInstance<SoundManager, bool, const std::string&>(SOUND_MANAGER, &SoundManager::loadLevel);

			m_LoadThreads[eLevel].SetFunc(PtrFuncLevel);
			m_LoadThreads[eAsset].SetFunc(PtrFuncAsset);
			m_LoadThreads[eSound].SetFunc(PtrFuncAudio);
		}

		void StateLoading::update()
		{
			if( !m_bThreadsLaunched )
			{
				m_LoadThreads[eLevel].Launch(m_sStateToLoadDataPath);
				m_LoadThreads[eAsset].Launch(m_sStateToLoadDataPath);
				m_LoadThreads[eSound].Launch(m_sStateToLoadDataPath);

				m_bThreadsLaunched = true;
				m_WaitForLoadThread = new std::thread(&StateLoading::WaitForLoadThreads, this);
			}

			//Update counters
			m_RessourcesCounters[eLevel].uiRessourceLoaded = PARALLAXE->getLastLoadLevelDataAccount();
			m_RessourcesCounters[eAsset].uiRessourceLoaded = DATA_MANAGER->getLastLoadLevelDataAccount();
			m_RessourcesCounters[eSound].uiRessourceLoaded = SOUND_MANAGER->getLastLoadLevelDataAccount();

			//Chargement terminé, on peut passer à l'état suivant
			if( m_bLoadThreadsOver )
			{
				//Clignotement du texte
				m_fAngle += PI_2 * TIME_DELTA;

				//Input
				if( INPUT_MANAGER->getKeyboard().getKeyState(sf::Keyboard::Return) == KEY_PRESSED
				   || JOYSTICK_GET_BUTTON(0, BUTTON_A) == KEY_PRESSED )
					CHANGE_STATE(m_iStateToLoadId);
			}
		}

		void StateLoading::deinit()
		{
		}


		void StateLoading::draw()
		{
			sf::RenderTexture* MainRenderTexture(GRAPHIC_MANAGER->getRenderTexture());
			sf::Vector2u MainRenderTextureSize = MainRenderTexture->getSize();

			//Barres de progression
			for( int i(0); i < eSTATE_LOADING_DATA_TYPE_NUMBER; i++ )
			{
				float fRatio = 1.0f;
				if( m_RessourcesCounters[i].uiRessourceToLoad != 0 )
					fRatio = (float)(m_RessourcesCounters[i].uiRessourceLoaded / m_RessourcesCounters[i].uiRessourceToLoad);
				sf::Vector2f GaugeScale(fRatio, 1.0f);

				//Gauge color
				if( fRatio > 0.5f )
					m_GaugeRect.setFillColor(sf::Color::Green);
				else
				{
					if( fRatio > 0.25f )
						m_GaugeRect.setFillColor(sf::Color(255, 140, 0, 255));
					else
						m_GaugeRect.setFillColor(sf::Color::Red);
				}

				m_LoadText.setString(m_RessourcesCounters[i].sRessourcesName + ": " + std::to_string(m_RessourcesCounters[i].uiRessourceLoaded) + "/" + std::to_string(m_RessourcesCounters[i].uiRessourceToLoad));

				m_BarRect.setPosition(sf::Vector2f(MainRenderTextureSize.x * 0.5f - m_BarRect.getSize().x * 0.5f, MainRenderTextureSize.y - m_BarRect.getSize().y * (eSTATE_LOADING_DATA_TYPE_NUMBER - (i - 1)) - 10.0f * (eSTATE_LOADING_DATA_TYPE_NUMBER - i)));
				MainRenderTexture->draw(m_BarRect);
				m_GaugeRect.setPosition(m_BarRect.getPosition());
				MainRenderTexture->draw(m_GaugeRect);
				m_LoadText.setPosition(m_BarRect.getPosition() + sf::Vector2f(m_BarRect.getSize().x * 0.5f - m_LoadText.getGlobalBounds().width * 0.5f, m_BarRect.getSize().y * 0.5f - m_LoadText.getGlobalBounds().height * 0.5f));
				MainRenderTexture->draw(m_LoadText);
			}

			//Affiche les touches à presser pour passer à l'état suivant
			if( m_bLoadThreadsOver )
			{
				//Position à l'écran
				sf::Vector2f Decal(MainRenderTextureSize.x * 0.5f - m_Sprite.getSize().x * 0.5f, MainRenderTextureSize.y - m_Sprite.getSize().y * 2.75f);

				//Texte qui "clignote"
				m_Sprite.setColor(sf::Color(255, 255, 255, (unsigned int)(255 * ABS(cos(m_fAngle)))));
				m_Text.setColor(sf::Color(255, 255, 255, (unsigned int)(255 * ABS(cos(m_fAngle)))));

				//Boutons
				m_Sprite.setTexture(m_ButtonTexture[0], true);
				m_Sprite.setPosition(sf::Vector2f(-166.0f, 65.0f) + Decal);
				MainRenderTexture->draw(m_Sprite);

				m_Sprite.setTexture(m_ButtonTexture[1], true);
				m_Sprite.setPosition(sf::Vector2f(-30.0f, 58.0f) + Decal);
				MainRenderTexture->draw(m_Sprite);

				//Texte
				m_Text.setPosition(Decal - sf::Vector2f(m_Text.getGlobalBounds().width * 0.5f, -75.0f));
				MainRenderTexture->draw(m_Text);
				MainRenderTexture->display();
			}
		}


		void StateLoading::WaitForLoadThreads()
		{
			m_LoadThreads[eLevel].Wait();
			m_LoadThreads[eAsset].Wait();
			m_LoadThreads[eSound].Wait();
		
			m_bLoadThreadsOver = true;
		}
	}
}