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
			LifeBar(const int& _InitialLife, const int& _CurrentLife);
			~LifeBar();

		private:
			

		public:
			void update(const int& _CurrentLife);
			void draw();
	};
}


#endif