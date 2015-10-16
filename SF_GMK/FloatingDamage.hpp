/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		19/02/2015
	@brief		Classe modèle pour génération de dégats flottants

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