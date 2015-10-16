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

	class EntityWithPv : public Entity
	{
		protected:
			int m_iPv;
			bool m_bDieWhenPvNull;

		public:
			EntityWithPv(const bool& _DieWhenPvNull = true);
			EntityWithPv(const int& _Id, const bool& _DieWhenPvNull = true);
			virtual ~EntityWithPv();

			void update(const float& _TimeDelta);

			const int& getPv();
			void setPv(const int& _Pv);
			const int& removePv(const int& _PvToRemove);

			const bool& getDieWhenPvNull();
			void setDieWhenPvNull(const bool& _Boolean);
	};
}


#endif