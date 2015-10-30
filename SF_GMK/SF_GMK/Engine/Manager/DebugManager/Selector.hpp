/*--------------------------------------------------------------------------------------------------

@author		GMK
@date		30/10/2015
@brief		Outil de selection d'entités

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_SELECTOR_HPP
#define SFGMK_SELECTOR_HPP

namespace sfgmk
{
	namespace engine
	{
		enum SELECTOR_STATE
		{
			STANDBY,
			SELECTION,
			SELECTED
		};

		class SFGMK_API Selector : public Entity
		{
			DynamicArray<Entity*> m_SelectedEntities;

			SELECTOR_STATE m_State;
			sf::Vector2f m_Start, m_End;

		public:

			Selector();
			~Selector();

			void update(const float& _TimeDelta);

			virtual void onPhysicEnter();
			virtual void onPhysicCollision(Entity* _entity);
			virtual void onPhysicExit();
		};
	}
}

#endif