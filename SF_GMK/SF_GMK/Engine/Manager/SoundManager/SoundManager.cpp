namespace sfgmk
{
	namespace engine
	{
		SoundManager::SoundManager() : m_3DMinDistanceDefault(MIN_3D_DISTANCE_DEFAULT), m_3DMaxDistanceDefault(MAX_3D_DISTANCE_DEFAULT), m_bAnalyze(false), m_iChannelToAnalyze(0), m_bBeat(false), m_uiLastLoadLevelDataAccount(0U)
		{
			//Initialisation FMOD System
			FMOD::System_Create(&m_SoundSystem);
			m_SoundSystem->init(SOUND_MANAGER_TOTAL_CHANNEL, FMOD_INIT_NORMAL, NULL);

			//Définition des canaux / groupes de canaux virtuels
			m_SoundSystem->createChannelGroup("Group Music", &m_MusicGroup);
			m_SoundSystem->createChannelGroup("Group Sound", &m_SoundGroup);
			m_SoundSystem->getMasterChannelGroup(&m_MasterGroup);

			m_MasterGroup->addGroup(m_MusicGroup);
			m_MasterGroup->addGroup(m_SoundGroup);

			//Sampling
			m_RecShape.setSize(sf::Vector2f(REC_SHAPE_SIZE, 200.0f));
		}

		SoundManager::~SoundManager()
		{
			stopAll();
			unloadAll();

			m_MusicGroup->release();
			m_SoundGroup->release();

			m_SoundSystem->close();
			m_SoundSystem->release();
		}


		void SoundManager::update()
		{
			//Pre_loop
			m_SoundSystem->update();

			//Analyse du son
			if( m_bAnalyze )
				analyzeMusic();

			//Update du listener
			sf::Vector2f MousePos = MOUSE_WORLD_POS;
			FMOD_VECTOR listenerpos = { (float)MousePos.x, (float)MousePos.y, 0.0f };

			FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
			FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
			FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
			m_SoundSystem->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
		}


		FMOD::System* SoundManager::getSystem()
		{
			return m_SoundSystem;
		}

		FMOD::Channel** SoundManager::getChannels()
		{
			return m_Channel;
		}

		int SoundManager::getFreeChannel()
		{
			bool bIsUsed(true);
			int i(-1);

			//Parcours le tableau de canaux, et renvoie l'index du premier canal vide
			while( bIsUsed && i < SOUND_MANAGER_TOTAL_CHANNEL )
			{
				i++;
				m_Channel[i]->isPlaying(&bIsUsed);
			}

			//Si aucun canal libre, on renvoie -1
			if( i >= SOUND_MANAGER_TOTAL_CHANNEL )
				i = -1;

			return i;
		}


		std::string SoundManager::getFileName(char* _FilePath)
		{
			std::string Temp(_FilePath);
			Temp.erase(0, Temp.rfind('/') + 1);
			Temp.erase(Temp.rfind('.'), Temp.size());

			return Temp;
		}

		bool SoundManager::loadMusic(char* _FilePath, char* _MusicName, int _FmodMode)
		{
			FMOD::Sound* NewSound = NULL;

			if( m_SoundSystem->createStream(_FilePath, _FmodMode, 0, &NewSound) == FMOD_OK )
			{
				//Si un nom a été spécifié on l'utilise
				if( _MusicName )
					m_MusicMap.insert(std::pair<std::string, FMOD::Sound*>(_MusicName, NewSound));

				//Sinon on récupére le nom du fichier et on l'utilise comme nom
				else
					m_MusicMap.insert(std::pair<std::string, FMOD::Sound*>(getFileName(_FilePath).c_str(), NewSound));

				return true;
			}

			return false;
		}

		bool SoundManager::unloadMusic(char* _MusicName)
		{
			if( m_MusicMap.find(_MusicName) == m_MusicMap.end() )
				return false;
			else
			{
				m_MusicMap.find(_MusicName)->second->release();
				m_MusicMap.erase(_MusicName);
				return true;
			}
		}

		bool SoundManager::loadSound(char* _FilePath, char* _SoundName, int _FmodMode)
		{
			FMOD::Sound* NewSound = NULL;

			if( m_SoundSystem->createSound(_FilePath, _FmodMode, 0, &NewSound) == FMOD_OK )
			{
				NewSound->set3DMinMaxDistance(m_3DMinDistanceDefault, m_3DMaxDistanceDefault);

				//Si un nom a été spécifié on l'utilise
				if( _SoundName )
					m_SoundMap.insert(std::pair<std::string, FMOD::Sound*>(_SoundName, NewSound));

				//Sinon on récupére le nom du fichier et on l'utilise comme nom
				else
					m_SoundMap.insert(std::pair<std::string, FMOD::Sound*>(getFileName(_FilePath).c_str(), NewSound));

				return true;
			}

			return false;
		}

		bool SoundManager::unloadSound(char* _SoundName)
		{
			if( m_SoundMap.find(_SoundName) == m_SoundMap.end() )
				return false;
			else
			{
				m_SoundMap.find(_SoundName)->second->release();
				m_SoundMap.erase(_SoundName);
				return true;
			}
		}

		bool SoundManager::loadLevel(const std::string& _Dir)
		{
			DIR* LevelRepertory = NULL;
			struct dirent* ReadFile = NULL;
			bool bReturn(true);
			FMOD::Sound* NewSound = NULL;
			char* Path = (char*)_Dir.c_str();

			//Music
			std::string DataDir = _Dir + "/audio" + "/music";
			LevelRepertory = opendir(DataDir.c_str());

			//Si erreur ouverture
			if( LevelRepertory == NULL )
			{
				if (SFGML_ENABLE_DIR_DEBUG)
					perror(DataDir.c_str());
				bReturn = false;
			}
			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for( int i(0); i < 2; i++ )
					readdir(LevelRepertory);

				while( (ReadFile = readdir(LevelRepertory)) != NULL ) //On récupére tous les fichiers contenus
				{
					std::string Name = ReadFile->d_name;
					Name = Name.substr(0, Name.find('.'));
					if( m_SoundSystem->createStream((DataDir + '/' + ReadFile->d_name).c_str(), FMOD_CREATESTREAM, 0, &NewSound) == FMOD_OK )
						m_MusicMap.insert(std::pair<std::string, FMOD::Sound*>(Name, NewSound));

					//Si on est en stateLoading, on compte les fichiers chargés
					//if( _State )
						//_State->AddToCounter(eSound);
				}
				closedir(LevelRepertory);
			}

			//Son
			DataDir = _Dir + "/audio" + "/sound";
			LevelRepertory = opendir(DataDir.c_str());

			//Si erreur ouverture
			if( LevelRepertory == NULL )
			{
				if (SFGML_ENABLE_DIR_DEBUG)
					perror(DataDir.c_str());
				bReturn = false;
			}
			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for( int i(0); i < 2; i++ )
					readdir(LevelRepertory);

				while( (ReadFile = readdir(LevelRepertory)) != NULL ) //On récupére tous les fichiers contenus
				{
					std::string Name = ReadFile->d_name;
					Name = Name.substr(0, Name.find('.'));
					if( m_SoundSystem->createStream((DataDir + '/' + ReadFile->d_name).c_str(), FMOD_3D, 0, &NewSound) == FMOD_OK )
						m_SoundMap.insert(std::pair<std::string, FMOD::Sound*>(Name, NewSound));
				}
				closedir(LevelRepertory);
			}

			return bReturn;
		}

		const unsigned int& SoundManager::getLastLoadLevelDataAccount()
		{
			return m_uiLastLoadLevelDataAccount;
		}

		bool SoundManager::unloadLevel(std::string _Dir)
		{
			DIR* LevelRepertory = NULL;
			struct dirent* ReadFile = NULL;
			bool bReturn(true);
			char* Path = (char*)_Dir.c_str();

			//Music
			std::string DataDir = _Dir + "/audio" + "/music";
			LevelRepertory = opendir(DataDir.c_str());

			//Si erreur ouverture
			if( LevelRepertory == NULL )
			{
				if (SFGML_ENABLE_DIR_DEBUG)
					perror(Path);
				bReturn = false;
			}
			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for( int i(0); i < 2; i++ )
					readdir(LevelRepertory);

				while( (ReadFile = readdir(LevelRepertory)) != NULL ) //On récupére tous les fichiers contenus
				{
					std::string Name = ReadFile->d_name;
					Name = Name.substr(0, Name.find('.'));
					unloadMusic((char*)Name.c_str());
				}
				closedir(LevelRepertory);
			}

			//Son
			DataDir = _Dir + "/audio" + "/sound";
			LevelRepertory = opendir(DataDir.c_str());

			//Si erreur ouverture
			if( LevelRepertory == NULL )
			{
				if (SFGML_ENABLE_DIR_DEBUG)
					perror(Path);
				bReturn = false;
			}
			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for( int i(0); i < 2; i++ )
					readdir(LevelRepertory);

				while( (ReadFile = readdir(LevelRepertory)) != NULL ) //On récupére tous les fichiers contenus
				{
					std::string Name = ReadFile->d_name;
					Name = Name.substr(0, Name.find('.'));
					unloadSound((char*)Name.c_str());
				}
				closedir(LevelRepertory);
			}

			return bReturn;
		}

		void SoundManager::unloadAll()
		{
			//Musique
			for( auto it = m_MusicMap.begin(); it != m_MusicMap.end(); it = m_MusicMap.erase(it) )
				it->second->release();

			//Sons
			for( auto it = m_SoundMap.begin(); it != m_SoundMap.end(); it = m_SoundMap.erase(it) )
				it->second->release();
		}


		int SoundManager::playMusic(std::string _MusicToplay, bool _Loop)
		{
			int i(0);

			//Si canal libre trouvé, lecture du son
			if( (i = getFreeChannel()) < SOUND_MANAGER_TOTAL_CHANNEL )
			{
				if( m_MusicMap.find(_MusicToplay) != m_MusicMap.end() )
				{
					m_SoundSystem->playSound(FMOD_CHANNEL_REUSE, m_MusicMap.find(_MusicToplay)->second, 0, &m_Channel[i]);
					m_Channel[i]->setChannelGroup(m_MusicGroup);

					if( _Loop )
						m_Channel[i]->setMode(FMOD_LOOP_NORMAL);

					return i;
				}
				else
					printf("Musique introuvable\n");
			}

			return -1;
		}

		int SoundManager::playSound(std::string _SoundToplay, bool _Loop)
		{
			int i(0);

			//Si canal libre trouvé, lecture du son
			if( (i = getFreeChannel()) < SOUND_MANAGER_TOTAL_CHANNEL )
			{
				if( m_SoundMap.find(_SoundToplay) != m_SoundMap.end() )
				{
					m_SoundSystem->playSound(FMOD_CHANNEL_REUSE, m_SoundMap.find(_SoundToplay)->second, 0, &m_Channel[i]);
					m_Channel[i]->setChannelGroup(m_SoundGroup);

					if( _Loop )
						m_Channel[i]->setMode(FMOD_LOOP_NORMAL);

					return i;
				}
				else
					printf("Son introuvable\n");

			}

			return -1;
		}

		void SoundManager::playAll()
		{
			m_MusicGroup->setPaused(false);
			m_SoundGroup->setPaused(false);
		}

		void SoundManager::pauseAll()
		{
			m_MusicGroup->setPaused(true);
			m_SoundGroup->setPaused(true);
		}

		void SoundManager::stopAll()
		{
			m_MusicGroup->stop();
			m_SoundGroup->stop();
		}

		void SoundManager::playAllMusic()
		{
			m_MusicGroup->setPaused(false);
		}

		void SoundManager::pauseAllMusic()
		{
			m_MusicGroup->setPaused(true);
		}

		void SoundManager::stopAllMusic()
		{
			m_MusicGroup->stop();
		}

		void SoundManager::playAllSounds()
		{
			m_SoundGroup->setPaused(false);
		}

		void SoundManager::pauseAllSounds()
		{
			m_SoundGroup->setPaused(true);
		}

		void SoundManager::stopAllSounds()
		{
			m_SoundGroup->stop();
		}


		void SoundManager::setMasterVolume(float _Value)
		{
			m_MasterGroup->setVolume(_Value);
		}

		void SoundManager::setMusicVolume(float _Value)
		{
			m_MusicGroup->setVolume(_Value);
		}

		void SoundManager::setSoundVolume(float _Value)
		{
			m_SoundGroup->setVolume(_Value);
		}


		void SoundManager::setMasterPitch(float _Value)
		{
			m_MasterGroup->setPitch(_Value);
		}

		void SoundManager::setMusicPitch(float _Value)
		{
			m_MusicGroup->setPitch(_Value);
		}

		void SoundManager::setSoundPitch(float _Value)
		{
			m_SoundGroup->setPitch(_Value);
		}


		void SoundManager::set3DMinDistance(float _Value)
		{
			m_3DMinDistanceDefault = _Value;
		}

		void SoundManager::set3DMaxDistance(float _Value)
		{
			m_3DMaxDistanceDefault = _Value;
		}


		float SoundManager::getCpuCharge()
		{
			float fCpu[5];
			m_SoundSystem->getCPUUsage(&fCpu[0], &fCpu[1], &fCpu[2], &fCpu[3], &fCpu[4]); //DSP, Stream, Geometry, Update, Total

			return fCpu[4];
		}


		void SoundManager::analyzeMusic()
		{
			float fLeftCanal[FREQUENCY_SAMPLE];
			float fRightCanal[FREQUENCY_SAMPLE];

			m_bBeat = false;

			m_SoundSystem->getSpectrum(fLeftCanal, FREQUENCY_SAMPLE, 0, FMOD_DSP_FFT_WINDOW_RECT);
			m_SoundSystem->getSpectrum(fRightCanal, FREQUENCY_SAMPLE, 1, FMOD_DSP_FFT_WINDOW_RECT);

			for( int i(0); i < FREQUENCY_SAMPLE; ++i )
				m_fFrequencyArray[i] = (fLeftCanal[i] + fRightCanal[i]) / 2.0f;

			fBeatTimer += TIME_DELTA;

			if( m_fFrequencyArray[0] != m_fBeatDectection[2] )
			{
				m_fBeatDectection[0] = m_fBeatDectection[1];
				m_fBeatDectection[1] = m_fBeatDectection[2];
				m_fBeatDectection[2] = m_fFrequencyArray[0];

				if( fBeatTimer >= 0.25f && m_fBeatDectection[1] > 0.5f && m_fBeatDectection[1] > m_fBeatDectection[0] && m_fBeatDectection[1] > m_fBeatDectection[2] )
				{
					m_bBeat = true;
					fBeatTimer = 0.0f;
				}
			}
		}

		void SoundManager::setAnalyzeMusic(bool _Boolean, int _Channel)
		{
			m_bAnalyze = _Boolean;
			m_iChannelToAnalyze = _Channel;
			fBeatTimer = 0.0f;
			m_fBeatDectection[0] = 0;
			m_fBeatDectection[1] = 0;
			m_fBeatDectection[2] = 0;
		}

		float* SoundManager::getSpectrumArray()
		{
			return m_fFrequencyArray;
		}

		void SoundManager::drawSpectrum()
		{
			sf::RenderTexture* RenderTexture = sfgmk::engine::GraphicManager::getSingleton()->getRenderTexture();
			float fScale(0.0f);
			float fVerticalPosition = (float)sfgmk::engine::GraphicManager::getSingleton()->getRenderWindow()->getSize().y + 1;

			for( int i(0); i < FREQUENCY_SAMPLE; ++i )
			{
				//Scale
				fScale = m_fFrequencyArray[i];
				m_RecShape.setScale(1.0f, -fScale);

				if( fScale > 0.5f )
					m_RecShape.setFillColor(sf::Color::Red);
				else
					m_RecShape.setFillColor(sf::Color::Green);

				m_RecShape.setPosition(i * (REC_SHAPE_SIZE + 1), fVerticalPosition);

				RenderTexture->draw(m_RecShape);
			}
		}

		bool SoundManager::getBeat()
		{
			return m_bBeat;
		}
	}
}