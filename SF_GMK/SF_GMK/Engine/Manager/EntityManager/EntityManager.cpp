#include <omp.h>

namespace sfgmk
{
	namespace engine
	{
		EntityManager::EntityManager() : m_uiEntityAccount(0U)
		{
		}

		EntityManager::~EntityManager()
		{
			freeEntityVector();
		}


		void EntityManager::update()
		{	
			for( int i(m_EntityVector.size() - 1); i >= 0; i-- )
			{
				m_EntityVector[i]->update(TIME_DELTA);
			
				//Suppression entités en fin de vie
				if( !m_EntityVector[i]->getIsAlive() )
					removeEntity(i);
			}
		}


		std::vector<Entity*>& EntityManager::getEntityVector()
		{
			return m_EntityVector;
		}

		const unsigned int& EntityManager::getEntityNumber()
		{
			return m_uiEntityAccount;
		}

		void EntityManager::addEntity(Entity* _Entity)
		{
			m_EntityVector.push_back(_Entity);
			m_uiEntityAccount++;
		}

		void EntityManager::removeEntity(const unsigned int& _Index)
		{
			DeletePtrCntrElement(m_EntityVector, _Index);
			m_uiEntityAccount--;
		}


		void EntityManager::freeEntityVector()
		{
			ClearPtrCntr(m_EntityVector);
		}


		void EntityManager::sortEntityVector()
		{
			std::sort(m_EntityVector.begin(), m_EntityVector.end(), sortEntityByZ);
		}


		inline bool sortEntityByZ(Entity* _Entity1, Entity* _Entity2)
		{
			return(_Entity1->getZ() < _Entity2->getZ());
		}
	}
}