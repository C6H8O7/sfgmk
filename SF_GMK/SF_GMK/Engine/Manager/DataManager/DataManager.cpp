namespace sfgmk
{
	namespace engine
	{
		DataManager::DataManager() : m_uiLastLoadLevelDataAccount(0U)
		{
			sf::Image image;
			image.create(64U, 64U, sf::Color(255, 0, 255, 255));

			m_TextureError.loadFromImage(image);

			loadSfgmkRessources();
		}

		DataManager::~DataManager()
		{
			unloadAll();
		}


		void DataManager::loadSfgmkRessources()
		{
			loadLevel(DATA_PATH);
		}

		void DataManager::unloadSfgmkRessources()
		{
			unloadLevel(DATA_PATH);
		}


		bool DataManager::loadTexture(const std::string& _resName, const std::string& _filePath)
		{
			sf::Texture *texture = new sf::Texture;

			bool result = texture->loadFromFile(_filePath);

			m_TextureMap[_resName] = texture;

			if (!result)
				delete texture;

			return result;
		}

		bool DataManager::loadTextureFromMemory(std::string _resName, const void *_memoryLocation, size_t size)
		{
			sf::Texture *texture = new sf::Texture;

			bool result = texture->loadFromMemory(_memoryLocation, size);

			m_TextureMap[_resName] = texture;

			if (!result)
				delete texture;

			return result;
		}

		bool DataManager::unloadTexture(const std::string& _resName)
		{
			std::map<std::string, sf::Texture*>::iterator it = m_TextureMap.find(_resName);

			if (it != m_TextureMap.end())
			{
				delete it->second;
				m_TextureMap[_resName] = 0;
				return true;
			}

			return false;
		}

		sf::Texture& DataManager::getTexture(const std::string& _resName)
		{
			std::map<std::string, sf::Texture*>::iterator it = m_TextureMap.find(_resName);

			if (it != m_TextureMap.end())
				return *it->second;

			std::cout << "ERROR TEXTURE" << std::endl;
			return m_TextureError;
		}


		bool DataManager::addAnimation(std::string _resName, Animation *_animation)
		{
			m_AnimationMap[_resName] = _animation;

			return true;
		}

		bool DataManager::loadAnimation(const std::string& _resName, const std::string& _filePath)
		{
			Animation *anim = new Animation;

			anim->LoadFromFile(_filePath);

			return addAnimation(_resName, anim);
		}

		bool DataManager::unloadAnimation(const std::string& _resName)
		{
			std::map<std::string, Animation*>::iterator it = m_AnimationMap.find(_resName);

			if (it != m_AnimationMap.end())
			{
				it->second->FreeTextures();
				delete it->second;
				m_AnimationMap[_resName] = 0;
				return true;
			}

			return false;
		}

		Animation& DataManager::getAnimation(const std::string& _resName)
		{
			return *m_AnimationMap[_resName];
		}


		bool DataManager::loadFont(const std::string& _resName, const std::string& _filePath)
		{
			sf::Font *font = new sf::Font;

			bool result = font->loadFromFile(_filePath);

			m_FontMap[_resName] = font;

			if (!result)
				delete font;

			return result;
		}

		bool DataManager::loadFontFromMemory(std::string _resName, const void *_memoryLocation, size_t size)
		{
			sf::Font *font = new sf::Font;

			bool result = font->loadFromMemory(_memoryLocation, size);

			m_FontMap[_resName] = font;

			if (!result)
				delete font;

			return result;
		}

		bool DataManager::unloadFont(const std::string& _resName)
		{
			std::map<std::string, sf::Font*>::iterator it = m_FontMap.find(_resName);

			if (it != m_FontMap.end())
			{
				delete it->second;
				m_FontMap[_resName] = 0;
				return true;
			}

			return false;
		}

		sf::Font& DataManager::getFont(const std::string& _resName)
		{
			return *m_FontMap[_resName];
		}


		bool DataManager::loadShader(const std::string& _resName, const std::string& _filePath, const sf::Shader::Type& _type)
		{
			sf::Shader *shader = new sf::Shader;

			bool result = shader->loadFromFile(_filePath, _type);

			m_ShaderMap[_resName] = shader;

			if (!result)
				delete shader;

			return result;
		}

		bool DataManager::loadShaderFromMemory(std::string _resName, std::string _shaderText, sf::Shader::Type _type)
		{
			sf::Shader *shader = new sf::Shader;

			bool result = shader->loadFromMemory(_shaderText, _type);

			m_ShaderMap[_resName] = shader;

			if (!result)
				delete shader;

			return result;
		}

		bool DataManager::unloadShader(const std::string& _resName)
		{
			std::map<std::string, sf::Shader*>::iterator it = m_ShaderMap.find(_resName);

			if (it != m_ShaderMap.end())
			{
				delete it->second;
				m_ShaderMap[_resName] = 0;
				return true;
			}

			return false;
		}

		sf::Shader& DataManager::getShader(const std::string& _resName)
		{
			return *m_ShaderMap[_resName];
		}


		bool DataManager::loadFromDataBank(std::string _filePath)
		{
			FILE *bankFile = 0;
			std::vector<GMKFileHeader> headers;
			long bankFileSize = 0, headerCount = 0;

			if (fopen_s(&bankFile, _filePath.c_str(), "rb+"))
			{
				std::cout << "error opening " << _filePath << std::endl;
				return false;
			}

			fseek(bankFile, 0, SEEK_END);
			bankFileSize = ftell(bankFile);
			fseek(bankFile, 0, SEEK_SET);

			fread(&headerCount, sizeof(headerCount), 1, bankFile);

			if (feof(bankFile) || headerCount <= 0)
			{
				std::cout << "error reading " << _filePath << std::endl;
				fclose(bankFile);
				return false;
			}

			for (int i = 0; i < headerCount; i++)
			{
				GMKFileHeader header = { 0 };
				fread(&header, sizeof(header), 1, bankFile);
				headers.push_back(header);
			}

			for (int i = 0; i < headerCount; i++)
			{
				GMKFileHeader header = headers[i];
				encryptDecryptData((unsigned char*)&header, sizeof(GMKFileHeader), ENCRYPT_KEY, ENCRYPT_KEY_LENGTH);

				unsigned char *compressedFile = (unsigned char*)calloc(1, header.compressedSize);
				fread(compressedFile, 1, header.compressedSize, bankFile);
				encryptDecryptData(compressedFile, header.compressedSize, ENCRYPT_KEY, ENCRYPT_KEY_LENGTH);

				unsigned char *uncompressedFile = (unsigned char*)calloc(1, header.uncompressedSize);
				uncompress((Bytef*)uncompressedFile, (uLong*)&header.uncompressedSize, (const Bytef*)compressedFile, header.compressedSize);

				switch (header.resourceType)
				{
				case TYPE_IMAGE:
					loadTextureFromMemory(header.resourceName, uncompressedFile, header.uncompressedSize);
					break;

				default:
					break;
				}

				free(compressedFile);
				free(uncompressedFile);
			}

			fclose(bankFile);
			return true;
		}

		void DataManager::encryptDecryptData(unsigned char *_data, int _dataSize, const char *_key, int _keyLength)
		{
			for (int i = 0; i < _dataSize; i++)
				_data[i] = _data[i] ^ _key[i % _keyLength];
		}


		bool DataManager::loadRessource(const eResourceType& _DataType, std::string _Name, std::string _FilePath)
		{
			switch (_DataType)
			{
			case TYPE_IMAGE:
				loadTexture(_Name, _FilePath);
				break;

			case TYPE_ANIM:
				loadAnimation(_Name, _FilePath);
				break;

			case TYPE_FONT:
				loadFont(_Name, _FilePath);
				break;

			case TYPE_SHADER_VERTEX:
				loadShader(_Name, _FilePath, sf::Shader::Vertex);
				break;

			case TYPE_SHADER_FRAGMENT:
				loadShader(_Name, _FilePath, sf::Shader::Fragment);
				break;

			default:
				return false;
				break;
			}

			return true;
		}

		bool DataManager::loadLevel(const std::string& _DirPath)
		{
			DIR* LevelRepertory = NULL;
			struct dirent* ReadFile = NULL;
			bool bReturn(true);

			m_uiLastLoadLevelDataAccount = 0U;

			//Textures
			std::string DataDir = _DirPath + "/texture";
			LevelRepertory = opendir(DataDir.c_str());

			//Si erreur ouverture
			if (LevelRepertory == NULL)
			{
				perror(DataDir.c_str());
				bReturn = false;
			}

			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for (int i(0); i < 2; i++)
					readdir(LevelRepertory);

				while ((ReadFile = readdir(LevelRepertory)) != NULL) //On récupére tous les fichiers contenus
				{
					std::string Name = ReadFile->d_name;
					Name = Name.substr(0, Name.find('.'));
					loadRessource(TYPE_IMAGE, Name, DataDir + '/' + ReadFile->d_name);
					m_uiLastLoadLevelDataAccount++;
				}
				closedir(LevelRepertory);
			}

			//Animations
			DataDir = _DirPath + "/animation";
			LevelRepertory = opendir(DataDir.c_str());

			//Si erreur ouverture
			if (LevelRepertory == NULL)
			{
				perror(DataDir.c_str());
				bReturn = false;
			}

			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for (int i(0); i < 2; i++)
					readdir(LevelRepertory);

				while ((ReadFile = readdir(LevelRepertory)) != NULL) //On récupére tous les fichiers contenus
				{

					std::string Name = ReadFile->d_name;
					Name = Name.substr(0, Name.find('.'));
					loadRessource(TYPE_ANIM, Name, DataDir + '/' + ReadFile->d_name);
					m_uiLastLoadLevelDataAccount++;
				}
				closedir(LevelRepertory);
			}

			//Fonts
			DataDir = _DirPath + "/font";
			LevelRepertory = opendir(DataDir.c_str());

			//Si erreur ouverture
			if (LevelRepertory == NULL)
			{
				perror(DataDir.c_str());
				bReturn = false;
			}

			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for (int i(0); i < 2; i++)
					readdir(LevelRepertory);

				while ((ReadFile = readdir(LevelRepertory)) != NULL) //On récupére tous les fichiers contenus
				{

					std::string Name = ReadFile->d_name;
					Name = Name.substr(0, Name.find('.'));
					loadRessource(TYPE_FONT, Name, DataDir + '/' + ReadFile->d_name);
					m_uiLastLoadLevelDataAccount++;

				}
				closedir(LevelRepertory);
			}

			//Shaders
			DataDir = _DirPath + "/shader";
			LevelRepertory = opendir(DataDir.c_str());

			//Si erreur ouverture
			if (LevelRepertory == NULL)
			{
				perror(DataDir.c_str());
				bReturn = false;
			}

			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for (int i(0); i < 2; i++)
					readdir(LevelRepertory);

				while ((ReadFile = readdir(LevelRepertory)) != NULL) //On récupére tous les fichiers contenus
				{
					eResourceType ShaderType;
					std::string Name = ReadFile->d_name;
					if (Name.substr(Name.find('.')) == "frag")
						ShaderType = TYPE_SHADER_FRAGMENT;
					else if (Name.substr(Name.find('.')) == "vert")
						ShaderType = TYPE_SHADER_VERTEX;
					Name = Name.substr(0, Name.find('.'));
					loadRessource(ShaderType, Name, DataDir + '/' + ReadFile->d_name);
					m_uiLastLoadLevelDataAccount++;
				}
				closedir(LevelRepertory);
			}

			return bReturn;
		}

		const unsigned int& DataManager::getLastLoadLevelDataAccount()
		{
			return m_uiLastLoadLevelDataAccount;
		}

		bool DataManager::unloadLevel(const std::string& _DirPath)
		{
			DataManager* DataManager = DATA_MANAGER;
			DIR* LevelRepertory = NULL;
			struct dirent* ReadFile = NULL;
			std::vector<std::string> FileNameVector;
			bool bReturn(true);

			//Path
			std::string sDataFilePath = _DirPath + "/texture";

			//Si erreur ouverture
			if ((LevelRepertory = opendir(sDataFilePath.c_str())) == NULL)
			{
				perror(sDataFilePath.c_str());
				bReturn = false;
			}

			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for (int i(0); i < 2; i++)
					readdir(LevelRepertory);

				//Déchargement du DatManager et stockage des noms de fichiers, en vue du déchargement de la parallaxe
				while ((ReadFile = readdir(LevelRepertory)) != NULL) //On récupére tous les fichiers contenus
				{
					std::string FileName = ReadFile->d_name;
					FileName = FileName.substr(0, FileName.find('.'));
					unloadTexture(FileName);
				}
			}

			//Path
			sDataFilePath = _DirPath + "/animation";

			//Si erreur ouverture
			if ((LevelRepertory = opendir(sDataFilePath.c_str())) == NULL)
			{
				perror(sDataFilePath.c_str());
				bReturn = false;
			}

			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for (int i(0); i < 2; i++)
					readdir(LevelRepertory);

				//Déchargement du DatManager et stockage des noms de fichiers, en vue du déchargement de la parallaxe
				while ((ReadFile = readdir(LevelRepertory)) != NULL) //On récupére tous les fichiers contenus
				{
					std::string FileName = ReadFile->d_name;
					FileName = FileName.substr(0, FileName.find('.'));
					unloadAnimation(FileName);
				}
			}

			//Path
			sDataFilePath = _DirPath + "/font";

			//Si erreur ouverture
			if ((LevelRepertory = opendir(sDataFilePath.c_str())) == NULL)
			{
				perror(sDataFilePath.c_str());
				bReturn = false;
			}

			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for (int i(0); i < 2; i++)
					readdir(LevelRepertory);

				//Déchargement du DatManager et stockage des noms de fichiers, en vue du déchargement de la parallaxe
				while ((ReadFile = readdir(LevelRepertory)) != NULL) //On récupére tous les fichiers contenus
				{
					std::string FileName = ReadFile->d_name;
					FileName = FileName.substr(0, FileName.find('.'));
					unloadFont(FileName);
				}
			}

			//Path
			sDataFilePath = _DirPath + "/shader";

			//Si erreur ouverture
			if ((LevelRepertory = opendir(sDataFilePath.c_str())) == NULL)
			{
				perror(sDataFilePath.c_str());
				bReturn = false;
			}

			//Si dossier trouvé
			else
			{
				//Caractères '.' et ".."
				for (int i(0); i < 2; i++)
					readdir(LevelRepertory);

				//Déchargement du DatManager et stockage des noms de fichiers, en vue du déchargement de la parallaxe
				while ((ReadFile = readdir(LevelRepertory)) != NULL) //On récupére tous les fichiers contenus
				{
					std::string FileName = ReadFile->d_name;
					unloadShader(FileName);
				}
			}

			return bReturn;
		}

		void DataManager::unloadAll()
		{
			//Textures
			for( auto it(m_TextureMap.begin()); it != m_TextureMap.end(); it = m_TextureMap.erase(it) )
				delete it->second;

			//Animations
			for( auto it(m_AnimationMap.begin()); it != m_AnimationMap.end(); it = m_AnimationMap.erase(it) )
				delete it->second;

			//Fonts
			for( auto it(m_FontMap.begin()); it != m_FontMap.end(); it = m_FontMap.erase(it) )
				delete it->second;

			//Shader
			for( auto it(m_ShaderMap.begin()); it != m_ShaderMap.end(); it = m_ShaderMap.erase(it) )
				delete it->second;
		}
	}
}