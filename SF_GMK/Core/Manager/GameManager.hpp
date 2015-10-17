/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		30/11/2014
	@brief		Manager général du jeu, regroupe l'accés aux différents composants et outils 
				nécessaires à celui-ci

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_GAME_MANAGER_HPP
#define SFGMK_GAME_MANAGER_HPP


struct stGameManagerTimers
{
	double dStateUpdate;
	double dStateDraw;
	double dEntityUpdate;
	double dEntitySort;
	double dParallaxeComputation;
	double dParallaxeDisplay;
	double dPhysic;
};

namespace sfgmk
{
	class GameManager : public SingletonTemplate<GameManager>
	{
		friend class SingletonTemplate<GameManager>;

		private:
			sf::RenderWindow* m_MainRenderWindow;
			sf::RenderTexture* m_MainRenderTexture;
			sf::Sprite m_RenderSprite;

			sf::Clock m_ClockTimeDelta;
			sf::Time m_Time;
			float m_fTimeDelta;
			stGameManagerTimers m_EntityTimers;

			StateMachine *m_StateMachine;

			std::map<std::string, Camera*> m_Cameras;
			Camera* m_CurrentCamera;
			Parallaxe m_Parallaxe;

			std::vector<Entity*> m_Entities;

			std::vector<PostShader*> m_PostShaders;

			sf::Image m_ScreenshotImage;

			GameManager();
			~GameManager();

		public:
			void init(sf::VideoMode& _Mode, const std::string& _WindowName, int _Style = sf::Style::Default);

			void update();
			void preLoop();
			void loop();
			void postLoop();

			sf::RenderWindow* getRenderWindow();
			sf::RenderTexture* getRenderTexture();

			float getTimeDelta();

			StateMachine* getStateMachine();

			void registerCamera(std::string _CameraName, Camera* _Camera);
			Camera* getCamera(std::string _CameraName);
			Camera* getCurrentCamera();
			void setDefaultCamera();
			bool setCurrentCamera(std::string _CameraName);
			bool removeCamera(std::string _CameraName);
			Parallaxe& getParallaxe();

			void updateEntities();
			const std::vector<Entity*>& getEntityVector();
			const int getEntityNumber();
			void addEntity(Entity* _Entity);
			void freeEntityVector();
			void sortEntityVector();

			void addPostShader(PostShader* _PostShader);
			void applyPostShaders();
			void freePostShaderVector();

			stGameManagerTimers& getTimers();

			void screenshot();
	};

	//Fonction qui permet de trier les entités par leur Z virtuel
	inline bool sortEntityByZ(Entity* _Entity1, Entity* _Entity2);
}


#endif