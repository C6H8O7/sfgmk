/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		20/11/2015
	@brief		Classe permettant de dessiner des formes

--------------------------------------------------------------------------------------------------*/

#ifndef SHAPE_DRAWER_HPP
#define SHAPE_DRAWER_HPP


namespace sfgmk
{
	class ShapeDrawer
	{
		public:
			ShapeDrawer();
			~ShapeDrawer();

		private:
			sf::RenderTexture* m_GraphicManagerRenderTexturePtr;

			sf::CircleShape m_Circle;
			sf::RectangleShape m_Rectangle;

		public:
			void setRenderTexture(sf::RenderTexture* _Render);

			void drawLine(const sf::Vertex Points[2]);
			void drawLine(const sf::Vertex Points[2], sf::RenderTexture* _Render);

			void drawCircle(const sf::Vector2f& _Position, const float& _Radius, const unsigned short& _PointCount, 
							const sf::Color& _FilLColor = sf::Color::White, const int& _OutlineThickness = 0, const sf::Color& _OutlineColor = sf::Color::Transparent, const sf::Texture* _texture = NULL, const sf::IntRect& _TextureRect = sf::IntRect());
			void drawCircle(const sf::Vector2f& _Position, const float& _Radius, const unsigned short& _PointCount, sf::RenderTexture* _Render, 
							const sf::Color& _FilLColor = sf::Color::White, const int& _OutlineThickness = 0, const sf::Color& _OutlineColor = sf::Color::Transparent, const sf::Texture* _texture = NULL, const sf::IntRect& _TextureRec = sf::IntRect());

			void drawRectangle(const sf::Vector2f& _Position, const sf::Vector2f& _Size, 
							   const sf::Color& _FilLColor = sf::Color::White, const int& _OutlineThickness = 0, const sf::Color& _OutlineColor = sf::Color::Transparent, const sf::Texture* _texture = NULL, const sf::IntRect& _TextureRect= sf::IntRect());
			void drawRectangle(const sf::Vector2f& _Position, const sf::Vector2f& _Size, sf::RenderTexture* _Render, 
							   const sf::Color& _FilLColor = sf::Color::White, const int& _OutlineThickness = 0, const sf::Color& _OutlineColor = sf::Color::Transparent, const sf::Texture* _texture = NULL, const sf::IntRect& _TextureRect= sf::IntRect());
	};
}


#endif