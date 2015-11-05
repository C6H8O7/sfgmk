/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		29/10/2015
	@brief		Affichage graphique des Pvs des entités

--------------------------------------------------------------------------------------------------*/


#ifndef LIFE_BAR_HPP
#define LIFE_BAR_HPP


namespace sfgmk
{
	class SFGMK_API LifeBar : public sf::Transformable
	{
		public:
			LifeBar(const sf::IntRect& _EntityRect, const bool& _DrawText);
			~LifeBar();

		private:
			sf::RectangleShape m_BarRect;
			sf::RectangleShape m_GaugeRect;
			sf::Text m_PvText;
			sf::Font m_PvFont;
			std::string m_sPvString;

			float m_fPvRatio;
			sf::Vector2f m_Scale;
			sf::Vector2f m_GaugeScale;

			bool m_bDrawPv;

		public:
			void update(const int& _CurrentLife, const int& _InitialLife, const sf::Vector2f& _Origin, const sf::Vector2f& _Scale, const float& _Rotation, const sf::Vector2f& _Position);
			void draw();

			bool setDrawPv();
			void setDrawPv(const bool& _Boolean);
	};
}


#endif