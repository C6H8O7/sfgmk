/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		17/07/2015
	@brief		Classe mère des différents textes fx

--------------------------------------------------------------------------------------------------*/

#ifndef FX_TEXT_HPP
#define FX_TEXT_HPP

namespace sfgmk
{
		class FxText
		{
			protected:
			sf::Vector2u m_Size;
			sfgmk::DynamicArray<sf::Vector2f> m_PixelArray;

			sf::RenderTexture* m_Render;
			sf::RenderTexture* m_TextRender;
			sf::Sprite m_Sprite;
			sf::Color m_Color;

			public:
			FxText(std::string _String, sf::Font _Font, unsigned int _Size, sf::Color _Color = sf::Color::White);
			virtual ~FxText();

			virtual void update(const float& _TimeDelta) = 0;
			virtual void draw(sf::RenderTexture* _Render) = 0;
		};
}


#endif