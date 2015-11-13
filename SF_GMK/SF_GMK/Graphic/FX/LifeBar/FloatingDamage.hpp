#ifndef FLOATING_DAMAGE_HPP
#define FLOATING_DAMAGE_HPP


namespace sfgmk
{
	class SFGMK_API FloatingDamage : public Entity
	{
		public:
			FloatingDamage(int _Value, int _InitialPv, Entity* _Target);
			~FloatingDamage();

			void update(const float& _TimeDelta);
			void draw(sf::RenderTexture* _Render);

		private:
			sf::Text m_Text;
			float m_fAngle;
			float m_fAlpha;
	};
}


#endif