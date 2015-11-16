namespace sfgmk
{
	namespace engine
	{
		GraphicManager::GraphicManager() : m_RenderWindow(NULL), m_RenderTexture(NULL), m_CurrentParallaxe(NULL), m_Parallaxe(NULL), m_CurrentCamera(NULL), m_Map(NULL), m_bDrawHud(false), m_bScreenshot(true)
		{
			m_Parallaxe = new Parallaxe();
			m_CurrentParallaxe = m_Parallaxe;
		}

		GraphicManager::~GraphicManager()
		{
			SAFE_DELETE(m_Map);
			cleanHud();

			m_CurrentParallaxe = NULL;
			SAFE_DELETE(m_Parallaxe);
			SAFE_DELETE(m_RenderTexture);
			SAFE_DELETE(m_RenderWindow);
			
			//Destruction std::map caméras
			for( auto rit(m_Cameras.rbegin()); rit != m_Cameras.rend(); ++rit )
				delete(rit->second);
			m_Cameras.clear();

			ClearPtrCntr(m_PostShaders);
		}


		void GraphicManager::init(sf::VideoMode& _Mode, const std::string& _WindowName, int _Style)
		{
			//Destruction précédent rendu si existant
			SAFE_DELETE(m_RenderWindow);
			SAFE_DELETE(m_RenderTexture);

			//Création nouveau rendu
			m_RenderWindow = new sf::RenderWindow();
			m_RenderWindow->create(_Mode, _WindowName, _Style);
			m_RenderTexture = new sf::RenderTexture();
			m_RenderTexture->create(_Mode.width, _Mode.height);

			initDefaultCamera();
		}

		void GraphicManager::initDefaultCamera()
		{
			Camera* MyCamera = new Camera();
			MyCamera->setSize(sf::Vector2f(m_RenderWindow->getSize()));
			MyCamera->setCenter(MyCamera->getSize() * 0.5f);
			registerCamera(DEFAULT_CAMERA_NAME, MyCamera);
			setCurrentCamera(DEFAULT_CAMERA_NAME);
		}


		void GraphicManager::set()
		{
			m_CurrentCamera->update(TIME_DELTA);
			m_RenderTexture->setView(*m_CurrentCamera);
	
			//Clear render texture
			m_RenderTexture->clear(sf::Color::Black);
		}

		void GraphicManager::update()
		{
			m_CurrentParallaxe->update();
		}

		void GraphicManager::draw()
		{
			m_CurrentParallaxe->drawLayers(PARALLAXE_MAX_Z, 2);

			if( m_Map )
				m_Map->draw(m_RenderTexture);

			m_CurrentParallaxe->drawLayers(1, PARALLAXE_MIN_Z);
		}

		void GraphicManager::display()
		{
			//Post effects
			applyPostShaders();

			//Rendu debug
			PHYSIC_MANAGER->draw(m_RenderTexture);
			AI_MANAGER->draw(m_RenderTexture);
			ENTITY_MANAGER->draw(m_RenderTexture);

			//Rendu hud
			drawHud();

			//Console
			CONSOLE.draw(m_RenderTexture);

			//Rendu final
			m_RenderTexture->display();
			m_RenderSprite.setTexture(m_RenderTexture->getTexture(), true);

			//Affichage fenetre
			m_RenderWindow->draw(m_RenderSprite);
			m_RenderWindow->display();

			//Screenshot
			if( INPUT_MANAGER->KEYBOARD_KEY(sf::Keyboard::F12) == KEY_PRESSED )
				screenshot();
		}


		sf::RenderWindow* GraphicManager::getRenderWindow()
		{
			return m_RenderWindow;
		}

		sf::RenderTexture* GraphicManager::getRenderTexture()
		{
			return m_RenderTexture;
		}

		Parallaxe* GraphicManager::getParallaxe()
		{
			return m_Parallaxe;
		}

		Parallaxe* GraphicManager::getCurrentParallaxe()
		{
			return m_CurrentParallaxe;
		}

		void GraphicManager::setCurrentParallaxe(Parallaxe* _Instance)
		{
			if( _Instance == NULL )
				m_CurrentParallaxe = m_Parallaxe;
			else
				m_CurrentParallaxe = _Instance;
		}


		void GraphicManager::setMap(TiledMap* _NewMap)
		{
			SAFE_DELETE(m_Map);
			m_Map = _NewMap;
		}

		void GraphicManager::removeMap()
		{
			SAFE_DELETE(m_Map);
		}


		void GraphicManager::addSpriteToHud(Sprite* _NewSprite)
		{
			m_Hud.pushBack(_NewSprite);
		}

		void GraphicManager::cleanHud()
		{
			m_Hud.deleteAndClear();
		}

		DynamicArray<Sprite*>* GraphicManager::getHud()
		{
			return &m_Hud;
		}

		void GraphicManager::drawHud()
		{
			if( m_bDrawHud )
			{
				unsigned int uiAccount = m_Hud.getElementNumber();

				if( uiAccount > 0 )
				{
					m_RenderTexture->setView(m_RenderTexture->getDefaultView());

					for( unsigned int i(0); i < uiAccount; i++ )
						m_RenderTexture->draw(*m_Hud[i]);

					m_RenderTexture->setView(*CAMERA);

					m_CurrentParallaxe->addDrawToAccount(uiAccount);
				}
			}
		}

		void GraphicManager::setDrawHud()
		{
			m_bDrawHud = !m_bDrawHud;
		}


		void GraphicManager::registerCamera(std::string _CameraName, Camera* _Camera)
		{
			m_Cameras.insert(std::pair<std::string, Camera*>(_CameraName, _Camera));
		}

		Camera* GraphicManager::getCamera(std::string _CameraName)
		{
			auto it = m_Cameras.find(_CameraName);
			return it->second;
		}

		Camera* GraphicManager::getCurrentCamera()
		{
			return m_CurrentCamera;
		}

		void GraphicManager::setDefaultCamera()
		{
			setCurrentCamera(DEFAULT_CAMERA_NAME);
		}

		bool GraphicManager::setCurrentCamera(std::string _CameraName)
		{
			Camera* TargetCamera = getCamera(_CameraName);

			if( TargetCamera != NULL )
			{
				m_CurrentCamera = TargetCamera;
				m_RenderTexture->setView(*TargetCamera);
				return true;
			}

			return false;
		}

		bool GraphicManager::removeCamera(std::string _CameraName)
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


		void GraphicManager::addPostShader(PostShader* _PostShader)
		{
			m_PostShaders.push_back(_PostShader);
		}

		void GraphicManager::applyPostShaders()
		{
			for( int i = (int)m_PostShaders.size() - 1; i >= 0; i-- )
			{
				PostShader *shader = m_PostShaders[i];

				if( shader->update(TIME_DELTA) )
					shader->applyPostShader(*m_RenderTexture);
				else
				{
					delete shader;
					m_PostShaders[i] = m_PostShaders[m_PostShaders.size() - 1];
					m_PostShaders.pop_back();
				}
			}
		}

		void GraphicManager::freePostShaderVector()
		{
			ClearPtrCntr(m_PostShaders);
		}


		bool GraphicManager::screenshot()
		{
			if( m_bScreenshot )
			{
				m_bScreenshot = false;
	
				m_ScreenshotImage = m_RenderWindow->capture();
				m_ScreenshotThread = new std::thread(&GraphicManager::screenshotSave, this);

				return true;
			}

			return false;
		}

		void GraphicManager::screenshotSave()
		{
			time_t ttSecondes;
			struct tm TimeSave;
			FILE* fSave = NULL;
			char cNomFichier[64] = { '\0' };
			std::string sFileName = "../data/screenshot/";
			
			//Recuperation date
			time(&ttSecondes);
			localtime_s(&TimeSave, &ttSecondes);
			sprintf_s(cNomFichier, "%d-%d-%d_%dh%dm%ds.png", TimeSave.tm_year + 1900, TimeSave.tm_mon + 1, TimeSave.tm_mday, TimeSave.tm_hour, TimeSave.tm_min, TimeSave.tm_sec);
			sFileName += cNomFichier;
		
			m_ScreenshotImage.saveToFile(sFileName);
		
			m_bScreenshot = true;
		}
	}
}