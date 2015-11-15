/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		19/11/2015
	@brief		Fichier d'entête de l'AI Manager

--------------------------------------------------------------------------------------------------*/

#ifndef AI_MANAGER_HPP
#define AI_MANAGER_HPP

namespace sfgmk
{
	namespace engine
	{
		class SFGMK_API AIManager : public SingletonTemplate<AIManager>
		{
			friend class SingletonTemplate<AIManager>;

			private:
				AIManager();
				~AIManager();

				bool m_bDrawAiState;
				sf::Sprite m_Sprite;
				sf::Text m_Text;
				sf::RenderTexture m_Render;
				sf::Font m_Font;
				sf::RectangleShape m_Rect;

			public:
				void update(const float& _TimeDelta);
				void draw(sf::RenderTexture* _render);

				void setDrawAiState();
		};
	}
}


#endif