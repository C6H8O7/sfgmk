namespace sfgmk
{
	namespace engine
	{
		Parallaxe::Parallaxe() : m_iEntityIndex(0), m_iDrawAccount(0)
		{
		}

		Parallaxe::~Parallaxe()
		{
			unloadAll();
		}


		void Parallaxe::update()
		{
			m_iDrawAccount = 0;

			//Calcule les positions virtuelles des entités en fonction de leur Z
			virtualCoordsComputation();
		}

		void Parallaxe::virtualCoordsComputation()
		{
			sf::Vector2f CameraOrigin = GRAPHIC_MANAGER->getCurrentCamera()->getRelativOriginWithZoomCompensation();
			std::vector<Entity*> EntityVector = ENTITY_MANAGER->getEntityVector();
			sf::Transformable* VirtualTransform = NULL;
			float fZ(0.0f);
			float fCoef(0.0f);

			//Layers
			for( std::pair<const int, sLAYER_ELEMENT*>& layer : m_Layers )
			{
				fZ = (float)layer.first;
				fZ != PARALLAXE_BEFORE_MEDIUM_PLAN_Z && fZ != PARALLAXE_BEHIND_MEDIUM_PLAN_Z ? fCoef = fZ / PARALLAXE_RATIO : fCoef = 0.0f;

				layer.second->fVirtualX = layer.second->fRealX + fCoef * CameraOrigin.x;
				layer.second->fVirtualY = layer.second->fRealY + fCoef * CameraOrigin.y;
			}

			//Entités
			for( Entity*& entity : EntityVector )
			{
				fZ = entity->getZ();
				fZ != PARALLAXE_BEFORE_MEDIUM_PLAN_Z && fZ != PARALLAXE_BEHIND_MEDIUM_PLAN_Z ? fCoef = fZ / PARALLAXE_RATIO : fCoef = 0.0f;
				VirtualTransform = &entity->getVirtualTransform();

				//Origine égale
				VirtualTransform->setOrigin(entity->getOrigin());

				//Scale virtuel
				sf::Vector2f entityScale(entity->getScale());
				VirtualTransform->setScale(entityScale.x * (1.0f - fCoef), entityScale.y * (1.0f - fCoef));

				//Rotation égale
				VirtualTransform->setRotation(entity->getRotation());

				//Position virtuelle
				sf::Vector2f entityPosition(entity->getPosition().x, entity->getPosition().y);
				VirtualTransform->setPosition(entityPosition.x + fCoef * CameraOrigin.x, entityPosition.y + fCoef * CameraOrigin.y);
			}
		}


		void Parallaxe::drawLayer(std::multimap<int, sLAYER_ELEMENT*>::reverse_iterator& _Iterator, sf::RenderTexture* _RenderTexture)
		{
			_Iterator->second->Sprite->setPosition(sf::Vector2f(_Iterator->second->fVirtualX, _Iterator->second->fVirtualY));
			_RenderTexture->draw(*_Iterator->second->Sprite);

			m_iDrawAccount++;
		}

		void Parallaxe::drawLayers(int _MaxZ, int _MinZ)
		{
			sf::Vector2f CameraCenter = GRAPHIC_MANAGER->getCurrentCamera()->getCenter();
			sf::Vector2f CameraSize = GRAPHIC_MANAGER->getCurrentCamera()->getSize();
			sf::Vector2f CameraOrigin = GRAPHIC_MANAGER->getCurrentCamera()->getCenter() - GRAPHIC_MANAGER->getCurrentCamera()->getSize() / 2.0f;
			sf::RenderTexture* Rendertexture = GRAPHIC_MANAGER->getRenderTexture();
			std::vector<Entity*> EntityVector = ENTITY_MANAGER->getEntityVector();
			sLAYER_ELEMENT* TempLayer = NULL;
			Entity* TempEntity = NULL;
			Sprite* TempSprite = NULL;
			sf::Vector2f EntityPosition;
			sf::IntRect SpriteSize;
			sf::Vector2f TempScale;

			//Init index
			if( _MaxZ == PARALLAXE_MAX_Z )
			{
				m_LayerIterator = m_Layers.rbegin();
				m_iEntityIndex = EntityVector.size() - 1;
			}

			//Draw
			for( int i(_MaxZ); i >= _MinZ; i-- )
			{
				//Layers
				while( m_LayerIterator != m_Layers.rend() && m_LayerIterator->first >= i )
				{
					TempLayer = (m_LayerIterator->second);

					if( math::RectsIntersects(CameraOrigin.x, CameraOrigin.y, CameraSize.x, CameraSize.y, TempLayer->fVirtualX, TempLayer->fVirtualY, (float)TempLayer->iWidth, (float)TempLayer->iHeight) ) //Test d'affichage
						drawLayer(m_LayerIterator, Rendertexture);

					++m_LayerIterator;
				}

				//Entity
				while( m_iEntityIndex >= 0 && (int)EntityVector[m_iEntityIndex]->getZ() >= i )
				{
					TempEntity = EntityVector[m_iEntityIndex];
					TempSprite = TempEntity->getSprite();

					if( TempSprite )
					{
						EntityPosition = TempEntity->getVirtualTransform().getPosition();
						SpriteSize = TempSprite->getTextureRect();

						//Test d'affichage
						if( sfgmk::math::RectsIntersects(CameraOrigin.x, CameraOrigin.y, CameraSize.x, CameraSize.y, EntityPosition.x, EntityPosition.y, (float)SpriteSize.width, (float)SpriteSize.height) )
						{
							TempEntity->draw(Rendertexture);
							m_iDrawAccount++;
						}
					}
					m_iEntityIndex--;
				}
			}
		}


		bool Parallaxe::loadLevel(const std::string& _DirPath, StateLoading* _State)
		{
			sfgmk::engine::DataManager* DataManager = DATA_MANAGER;
			DIR* LevelRepertory = NULL;
			struct dirent* ReadFile = NULL;

			//Path
			std::string sDirpath = _DirPath + "/layer";
			const char* cLayerFilePath = sDirpath.c_str();

			//Si erreur ouverture
			if( (LevelRepertory = opendir(cLayerFilePath)) == NULL )
				perror(cLayerFilePath);
			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for( int i(0); i < 2; i++ )
					readdir(LevelRepertory);

				while( (ReadFile = readdir(LevelRepertory)) != NULL ) //On récupére tous les fichiers contenus
				{
					//On crée le nouveau Layer
					sLAYER_ELEMENT* NewLayer = new sLAYER_ELEMENT;
					NewLayer->fRealX = 0, NewLayer->fRealY = 0;
					NewLayer->fVirtualX = 0, NewLayer->fVirtualY = 0;
					NewLayer->sFileName = ReadFile->d_name;
					std::string sCompleteName = NewLayer->sFileName;
					NewLayer->sFilePath = sDirpath + "/" + NewLayer->sFileName;
					NewLayer->sFileName = NewLayer->sFileName.substr(0, NewLayer->sFileName.find('_')); //Coupe du nom du fichier (on retire les coordonnées après le '_')
					DataManager->loadTexture(NewLayer->sFileName, NewLayer->sFilePath);
					NewLayer->Sprite = new Sprite(NewLayer->sFileName, false);

					NewLayer->iWidth = DataManager->getTexture(NewLayer->sFileName).getSize().x;
					NewLayer->iHeight = DataManager->getTexture(NewLayer->sFileName).getSize().y;

					//Recuperation du vector position
					int iX = sCompleteName.find_first_of('x');
					int iY = sCompleteName.find_first_of('y');
					int iZ = sCompleteName.find_first_of('z');
					int iEnd = sCompleteName.find_first_of('.');

					//Position X
					int iFactor = (int)pow(10.0f, (iY - iX - 2)); //Puissance de 10 + indice qui commence à 0 ==> décalage de 2
					for( int i(iX + 1); i < iY; ++i )
					{
						std::string sSubTemp = sCompleteName.substr(i, 1);
						NewLayer->fRealX += atoi(sSubTemp.c_str()) * iFactor; //On récupére les chiffres un par un, on les converti en int, on multiplie par le facteur calculé auparavant et on ajoute au total
						iFactor /= 10;
					}

					//Position Y
					iFactor = (int)pow(10.0f, (iZ - iY - 2));
					for( int i(iY + 1); i < iZ; ++i )
					{
						std::string sSubTemp = sCompleteName.substr(i, 1);
						NewLayer->fRealY += atoi(sSubTemp.c_str()) * iFactor;
						iFactor /= 10;
					}

					//Position Z
					iFactor = (int)pow(10.0f, (iEnd - iZ - 2));
					int iPositionZ(0);
					for( int i(iZ + 1); i < iEnd; ++i )
					{
						std::string sSubTemp = sCompleteName.substr(i, 1);
						iPositionZ += atoi(sSubTemp.c_str()) * iFactor;
						iFactor /= 10;
					}
					//Gestion des Z négatifs
					if( sCompleteName.substr(iZ + 1, 1) == "-" )
						iPositionZ = -iPositionZ;

					//Insertion dans la multimap
					m_Layers.insert(std::pair<int, sLAYER_ELEMENT*>(iPositionZ, NewLayer));
				}
				closedir(LevelRepertory);
				return true;
			}
			return false;
		}

		bool Parallaxe::unloadLevel(std::string _DirPath)
		{
			sfgmk::engine::DataManager* DataManager = DATA_MANAGER;
			DIR* LevelRepertory = NULL;
			struct dirent* ReadFile = NULL;
			std::vector<std::string> FileNameVector;

			//Path
			_DirPath += "/layer";
			const char* cLayerFilePath = _DirPath.c_str();

			//Si erreur ouverture
			if( (LevelRepertory = opendir(cLayerFilePath)) == NULL )
			{
				perror(cLayerFilePath);
				return false;
			}

			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for( int i(0); i < 2; i++ )
					readdir(LevelRepertory);

				//Déchargement du DatManager et stockage des noms de fichiers, en vue du déchargement de la parallaxe
				while( (ReadFile = readdir(LevelRepertory)) != NULL ) //On récupére tous les fichiers contenus
				{
					std::string FileName = ReadFile->d_name;
					FileName = FileName.substr(0, FileName.find('_')); //Coupe du nom du fichier (on retire les coordonnées après le '_')
					FileNameVector.push_back(FileName);

					DataManager->unloadTexture(FileName);
				}

				//Déchargement de la parallaxe
				bool bToErase(false);
				for( std::multimap<int, sLAYER_ELEMENT*>::iterator it = m_Layers.begin(); it != m_Layers.end(); bToErase ? it = m_Layers.erase(it) : ++it )
				{
					bToErase = false;

					for( std::vector<std::string>::reverse_iterator rit = FileNameVector.rbegin(); rit != FileNameVector.rend(); bToErase ? rit = FileNameVector.rend() : ++rit )
					{
						if( it->second->sFileName == (*rit) )
						{
							bToErase = true;
							(*rit) = FileNameVector[FileNameVector.size() - 1];
							FileNameVector.pop_back();
						}
					}
				}
				closedir(LevelRepertory);
				return true;
			}
		}

		void Parallaxe::unloadAll()
		{
			sfgmk::engine::DataManager* DataManager = DATA_MANAGER;

			for( auto it = m_Layers.begin(); it != m_Layers.end(); it = m_Layers.erase(it) )
			{
				DataManager->unloadTexture(it->second->sFileName);
				delete it->second->Sprite;
				delete it->second;
			}
		}


		sLAYER_ELEMENT* Parallaxe::getLayer(const std::string& _Name)
		{
			for( std::pair<const int, sLAYER_ELEMENT*> & layer : m_Layers )
			{
				if( layer.second->sFileName == _Name )
					return layer.second;
			}

			return NULL;
		}

		const int Parallaxe::getLayerAccount() const
		{
			return m_Layers.size();
		}

		const int Parallaxe::getDrawAccount() const
		{
			return m_iDrawAccount;
		}

		void Parallaxe::addDrawToAccount(const int _DrawNumber)
		{
			m_iDrawAccount += _DrawNumber;
		}
	}
}