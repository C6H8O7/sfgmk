/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		16/12/2014
	@brief		Etat de chargement de niveau

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_STATELOADING_H
#define SFGMK_STATELOADING_H


namespace sfgmk
{
	namespace engine
	{
		enum eSTATE_LOADING_DATA_TYPE
		{
			eLevel = 0,
			eAsset,
			eSound,
			eSTATE_LOADING_DATA_TYPE_NUMBER
		};

		struct sLOADING_RESSOURCE
		{
			unsigned int iRessourceToLoad;
			unsigned int iRessourceLoaded;
		};

		class SFGMK_API StateLoading : public State
		{
			protected:
				std::thread m_Threads[eSTATE_LOADING_DATA_TYPE_NUMBER];
				std::mutex m_Mutex;
				std::function<bool(const std::string&, StateLoading*)> m_ThreadFunctions[eSTATE_LOADING_DATA_TYPE_NUMBER];

				int m_iStateToLoadId;
				std::string m_sStateToLoadDataPath;
				sLOADING_RESSOURCE m_iRessourcesCounter[eSTATE_LOADING_DATA_TYPE_NUMBER];
				int m_iLoadingPercentage[eSTATE_LOADING_DATA_TYPE_NUMBER];

				bool m_bThreadLaunched;
				bool m_bLoadOver;
				bool m_bThreadOver;

				engine::Parallaxe* m_LinkedParallaxe;

				sf::RenderTexture m_RenderTexture;
				bool m_bRenderTextureCreated;
				sf::Texture m_ButtonTexture[2];
				sf::Font m_Font;
				float m_fAngle;

			public:
				StateLoading();
				virtual ~StateLoading();

				virtual void init();
				virtual void update();
				virtual void deinit();

				virtual void draw();

				void CheckLoadingProgress();
				void AddToCounter(const eSTATE_LOADING_DATA_TYPE& _DataType);

				engine::Parallaxe* Get_Parallaxe();
		};
	}
}


#endif