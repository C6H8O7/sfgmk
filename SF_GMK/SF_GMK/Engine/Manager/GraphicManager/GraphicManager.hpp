/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		17/10/2015
	@brief		Manager chargé de toute la partie graphique

--------------------------------------------------------------------------------------------------*/

#ifndef GRAPHIC_MANAGER_HPP
#define GRAPHIC_MANAGER_HPP


namespace sfgmk
{
	namespace engine
	{
		#define DEFAULT_CAMERA_NAME "DefaultCamera"

		class SFGMK_API GraphicManager : public SingletonTemplate<GraphicManager>
		{
			friend class SingletonTemplate<GraphicManager>;

			private:
				GraphicManager();
				~GraphicManager();

				sf::RenderWindow* m_RenderWindow;
				sf::RenderTexture* m_RenderTexture;
				sf::Sprite m_RenderSprite;
				Parallaxe m_Parallaxe;

				std::map<std::string, Camera*> m_Cameras;
				Camera* m_CurrentCamera;

				std::vector<PostShader*> m_PostShaders;

			public:
				void init(sf::VideoMode& _Mode, const std::string& _WindowName, int _Style);
				void initDefaultCamera();

				void set();
				void compute();
				void draw();
				void display();

				sf::RenderWindow* getRenderWindow();
				sf::RenderTexture* getRenderTexture();
				Parallaxe& getParallaxe();

				void registerCamera(std::string _CameraName, Camera* _Camera);
				Camera* getCamera(std::string _CameraName);
				Camera* getCurrentCamera();
				void setDefaultCamera();
				bool setCurrentCamera(std::string _CameraName);
				bool removeCamera(std::string _CameraName);

				void addPostShader(PostShader* _PostShader);
				void applyPostShaders();
				void freePostShaderVector();
		};
	}
}


#endif
