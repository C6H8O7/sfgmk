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
			protected:
				enum eSTATE_LOADING_DATA_TYPE
				{
					eLevel = 0,
					eAsset,
					eSound,
					eSTATE_LOADING_DATA_TYPE_NUMBER
				};

				Parallaxe m_Parallaxe;

				int m_iStateToLoadId;
				std::string m_sStateToLoadDataPath;

				ThreadTemplate<const std::string&> m_LoadThreads[eSTATE_LOADING_DATA_TYPE_NUMBER];
				std::thread* m_WaitForLoadThread;
				bool m_bThreadsLaunched;
				bool m_bLoadThreadsOver;

				struct sLOADING_RESSOURCE
				{
					std::string sRessourcesName;
					unsigned int uiRessourceToLoad;
					unsigned int uiRessourceLoaded;
				}m_RessourcesCounters[eSTATE_LOADING_DATA_TYPE_NUMBER];

				sf::Texture m_ButtonTexture[2];
				Sprite m_Sprite;
				sf::Text m_Text;
				sf::Font m_Font;
				float m_fAngle;

				sf::RectangleShape m_BarRect;
				sf::RectangleShape m_GaugeRect;
				sf::Text m_LoadText;
				sf::Font m_LoadFont;
				std::string m_sLoadString;

			public:
				StateLoading();
				virtual ~StateLoading();

				virtual void init();
				virtual void update();
				virtual void deinit();

				virtual void draw();

				void WaitForLoadThreads();
		};
	}
}


#endif