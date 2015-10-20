/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		19/12/2014
	@brief		Classe permettant de simuler une dimension Z sur les sprites et backgrounds, afin
				de créer une impression de profondeur, et de maitriser l'ordre d'affichage

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_PARALLAXE_HPP
#define SFGMK_PARALAXE_HPP


namespace sfgmk
{
	namespace engine
	{
		#define PARALLAXE_MAX_Z 100
		#define PARALLAXE_BEHIND_MEDIUM_PLAN_Z 1
		#define PARALLAXE_MEDIUM_PLAN_Z 0
		#define PARALLAXE_BEFORE_MEDIUM_PLAN_Z -1
		#define PARALLAXE_MIN_Z -100
		#define PARALLAXE_RATIO 100.0f

		struct sLAYER_ELEMENT
		{
			float fRealX, fRealY;
			float fVirtualX, fVirtualY;
			std::string sFilePath;
			std::string sFileName;
			Sprite* Sprite;
			int iWidth;
			int iHeight;
		};

		class SFGMK_API Parallaxe
		{
			friend class GraphicManager;
			friend class StateLoading;

			public:
				void update();
				void virtualCoordsComputation();

				void drawLayers(int _MaxZ, int _MinZ);

				bool loadLevel(const std::string& _DirPath, StateLoading* _State = NULL);
				bool unloadLevel(std::string _DirPath);
				void unloadAll();

				sLAYER_ELEMENT* getLayer(const std::string& _Name);
				const int getLayerAccount() const;
				const int getDrawAccount() const;
				void addDrawToAccount(const int _DrawNumber);

			private:
				Parallaxe();
				~Parallaxe();

				void drawLayer(std::multimap<int, sLAYER_ELEMENT*>::reverse_iterator& _Iterator, sf::RenderTexture* _RenderTexture);

				std::multimap<int, sLAYER_ELEMENT*> m_Layers;
				std::multimap<int, sLAYER_ELEMENT*>::reverse_iterator m_LayerIterator;
				int m_iEntityIndex;

				int m_iDrawAccount;
		};
	}
}


#endif