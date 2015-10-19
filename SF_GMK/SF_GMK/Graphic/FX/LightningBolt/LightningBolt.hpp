/*--------------------------------------------------------------------------------------------------

	@author		GMK (sources: Michael Hoffman / http://gamedevelopment.tutsplus.com
	@date		17/02/2015
	@brief		Permet de générer un éclair avec différents paramètres

--------------------------------------------------------------------------------------------------*/


#ifndef LIGHTNING_BOLT_HPP
#define LIGHTNING_BOLT_HPP


namespace sfgmk
{
#define LIGHTNING_BOLT_ALPHA_INCREASE_SPEED 2040
#define LIGHTNING_BOLT_ALPHA_DECREASE_SPEED 510
#define LIGHTNING_BOLT_BRANCH_CHANCE 1

		class LightningBolt : public Entity
		{
			private:
			sf::Vector2f* m_PointArray;
			int m_iPointNumber;
			float m_fThickness;

			sf::Texture m_LightningBoltTexture;
			sf::Color m_Color;
			float m_fAlpha;
			bool m_bIsDecreasing;

			bool m_bBranch;

			int m_iDrawNumber;

			public:
			LightningBolt(const sf::Vector3f& _Position, const sf::Vector2f& _Destination, const bool& _Branch, const sf::Color& _Color = sf::Color::White, const float& _Thickness = 1.0f);
			~LightningBolt();

			void update(const float& _TimeDelta);
			void draw(sf::RenderTexture* _Render);

			void setMax();
		};
}


#endif