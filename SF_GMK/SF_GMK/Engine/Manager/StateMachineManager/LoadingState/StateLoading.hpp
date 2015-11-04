/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		16/12/2014
	@brief		Etat de chargement de niveau

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_STATELOADING_HPP
#define SFGMK_STATELOADING_HPP


namespace sfgmk
{
	namespace engine
	{
		class SFGMK_API StateLoading : public State
		{
			private:
				enum eSTATE_LOADING_DATA_TYPE
				{
					eLevel = 0,
					eAsset,
					eSound,
					eSTATE_LOADING_DATA_TYPE_NUMBER
				};

				bool m_bThreadsLaunched;

				void ThreadLevel();
				void ThreadAsset();
				void ThreadSound();

			protected:
				int m_iStateToLoadId;
				std::string m_sStateToLoadDataPath;

				ThreadTemplate<const std::string&> m_LoadThreads[eSTATE_LOADING_DATA_TYPE_NUMBER];

				struct sLOADING_RESSOURCE
				{
					unsigned int uiRessourceToLoad;
					unsigned int uiRessourceLoaded;
				}m_RessourcesCounters[eSTATE_LOADING_DATA_TYPE_NUMBER];

				sf::Texture m_ButtonTexture[2];
				sf::Font m_Font;

			public:
				StateLoading();
				virtual ~StateLoading();

				virtual void init();
				virtual void update();
				virtual void deinit();

				virtual void draw();
		};
	}
}


#endif