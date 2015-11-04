/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		25/11/2014
	@brief		Fichier d'entête du DataManager

--------------------------------------------------------------------------------------------------*/

#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

namespace sfgmk
{
	namespace engine
	{
		#define ENCRYPT_KEY				"l0lz~H4x0r~2000~Ilum1n4t1~Zl4t4n"
		#define ENCRYPT_KEY_LENGTH		strlen(ENCRYPT_KEY)

		class SFGMK_API DataManager : public SingletonTemplate<DataManager>
		{
			friend class SingletonTemplate<DataManager>;

			private:
				DataManager();
				~DataManager();

				std::map<std::string, sf::Texture*> m_TextureMap;
				sf::Texture m_TextureError;

				std::map<std::string, Animation*> m_AnimationMap;

				std::map<std::string, sf::Font*> m_FontMap;

				std::map<std::string, sf::Shader*> m_ShaderMap;

			public:
				enum eResourceType
				{
					TYPE_IMAGE = 1,
					TYPE_ANIM,
					TYPE_SOUND,
					TYPE_MUSIC,
					TYPE_FONT,
					TYPE_SHADER_VERTEX,
					TYPE_SHADER_FRAGMENT
				};

				struct GMKFileHeader
				{
					unsigned long compressedSize;
					unsigned long uncompressedSize;
					char resourceName[20];
					char resourceType;
				};

				void loadSfgmkRessources();
				void unloadSfgmkRessources();

				bool loadTexture(const std::string& _resName, const std::string& _filePath);
				bool loadTextureFromMemory(std::string _resName, const void *_memoryLocation, size_t size);
				bool unloadTexture(const std::string& _resName);
				sf::Texture& getTexture(const std::string& _resName);

				bool addAnimation(std::string _resName, Animation *_animation);
				bool loadAnimation(const std::string& _resName, const std::string& _filePath);
				bool unloadAnimation(const std::string& _resName);
				Animation& getAnimation(const std::string& _resName);

				bool loadFont(const std::string& _resName, const std::string& _filePath);
				bool loadFontFromMemory(std::string _resName, const void *_memoryLocation, size_t size);
				bool unloadFont(const std::string& _resName);
				sf::Font& getFont(const std::string& _resName);

				// Les sf::Shader ne sont pas copiables, on utilise donc de l'agrégation pour les shaders récupérés via le datamanager.
				bool loadShader(const std::string& _resName, const std::string& _filePath, const sf::Shader::Type& _type);
				bool loadShaderFromMemory(std::string _resName, std::string _shaderText, sf::Shader::Type _type);
				bool unloadShader(const std::string& _resName);
				sf::Shader& getShader(const std::string& _resName);

				bool loadFromDataBank(std::string _filePath);
				static void encryptDecryptData(unsigned char *_data, int _dataSize, const char *_key, int _keyLength);

				bool loadRessource(const eResourceType& _DataType, std::string _Name, std::string _FilePath);
				bool loadLevel(const std::string& _DirPath);
				bool unloadLevel(const std::string& _DirPath);
				void unloadAll();
		};
	}
}

#endif