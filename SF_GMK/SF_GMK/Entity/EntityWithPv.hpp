/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		30/06/2015
	@brief		Entité de base avec points de vie

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_ENTITY_WITH_PV_HPP
#define SFGMK_ENTITY_WITH_PV_HPP


namespace sfgmk
{
	#define ENTITY_DEFAULT_PV 1

	class SFGMK_API EntityWithPv : public Entity
	{
		protected:
			int m_iPv;
			int m_iInitialPv;
			bool m_bDieWhenPvNull;
			LifeBar* m_Lifebar;
		
		public:
			EntityWithPv(const sf::Vector3f& _Position, const int& _InitialPv = ENTITY_DEFAULT_PV, const int& _CurrentPv = ENTITY_DEFAULT_PV, const bool& _DieWhenPvNull = true);
			virtual ~EntityWithPv();

			void update(const float& _TimeDelta);

			const int& getPv();
			void setPv(const int& _Pv);
			const int& removePv(const int& _PvToRemove);

			const int& getInitialPv();
			void setInitialPv(const int& _Pv);

			const bool& getDieWhenPvNull();
			void setDieWhenPvNull(const bool& _Boolean);

			bool addLifeBar();
			bool removeLifeBar();
	};
}


#endif