/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		17/10/2015
	@brief		Manager dans lequel sont stockées les entités du programme

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_ENTITY_MANAGER_HPP
#define SFGMK_ENTITY_MANAGER_HPP


namespace sfgmk
{
	namespace engine
	{
		class SFGMK_API EntityManager : public SingletonTemplate<EntityManager>
		{
			friend class SingletonTemplate<EntityManager>;

			private:
			EntityManager();
			~EntityManager();

			std::vector<Entity*> m_EntityVector;

			public:
			void update();

			const std::vector<Entity*>& getEntityVector();
			const int getEntityNumber();
			void addEntity(Entity* _Entity);
			void freeEntityVector();

			void sortEntityVector();
		};

		//Fonction qui permet de trier les entités par leur Z virtuel
		inline bool sortEntityByZ(Entity* _Entity1, Entity* _Entity2);
	}
}


#endif