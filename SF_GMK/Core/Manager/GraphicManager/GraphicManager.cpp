namespace sfgmk
{
	namespace engine
	{
		GraphicManager::GraphicManager() : m_RenderWindow(NULL), m_RenderTexture(NULL), m_CurrentCamera(NULL)
		{

		}

		GraphicManager::~GraphicManager()
		{
			SAFE_DELETE(m_RenderWindow);
			SAFE_DELETE(m_RenderTexture);

			//ClearPtrCntr(m_Cameras); // pas possib

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
			sfgmk::Camera* MyCamera = new sfgmk::Camera();
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

		void GraphicManager::compute()
		{
			m_Parallaxe.update();
		}

		void GraphicManager::draw()
		{
			m_Parallaxe.drawLayers(PARALLAXE_MAX_Z, PARALLAXE_MEDIUM_PLAN_Z);
			m_Parallaxe.drawLayers(PARALLAXE_BEFORE_MEDIUM_PLAN_Z, PARALLAXE_MIN_Z);
		}

		void GraphicManager::display()
		{
			applyPostShaders();

			//Rendu moteur physique
			if( sfgmk::PhysicManager::getSingleton()->getDraw() )
				sfgmk::PhysicManager::getSingleton()->draw(m_RenderTexture);

			//Rendu console
			sfgmk::ConsoleDev::getSingleton()->display(m_RenderTexture, m_CurrentCamera->getRelativOrigin());

			//Rendu final
			m_RenderTexture->display();
			m_RenderSprite.setTexture(m_RenderTexture->getTexture(), true);

			//Affichage fenetre
			m_RenderWindow->draw(m_RenderSprite);
			m_RenderWindow->display();
		}

		sf::RenderWindow* GraphicManager::getRenderWindow()
		{
			return m_RenderWindow;
		}

		sf::RenderTexture* GraphicManager::getRenderTexture()
		{
			return m_RenderTexture;
		}

		Parallaxe& GraphicManager::getParallaxe()
		{
			return m_Parallaxe;
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
	}
}