/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		19/02/2015
	@brief		Classe mod�le pour g�n�ration de d�gats flottants

--------------------------------------------------------------------------------------------------*/


#ifndef FLOATING_DAMAGE_HPP
#define FLOATING_DAMAGE_HPP


namespace sfgmk
{
	class FloatingDamage : public Entity
	{
		public:
			FloatingDamage();
			virtual ~FloatingDamage();

			virtual void update(float _TimeDelta);
			virtual void draw(sf::RenderTexture* _Render);

		private:
	};
}


#endif