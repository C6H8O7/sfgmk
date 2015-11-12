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
				unsigned int m_uiEntityAccount;
				unsigned int m_uiEntityGlobalAccount;
				sf::Clock m_SortTimer;
				signed long long m_llSortTimer;

				bool m_bDrawId;
				sf::Sprite m_Sprite;
				sf::Text m_Text;
				sf::RenderTexture m_Render;
				sf::Font m_Font;
				sf::RectangleShape m_Rect;

			public:
				void update();

				void draw();

				void setDrawId();

				Entity* getEntity(const unsigned int& _Id);
				std::vector<Entity*>& getEntityVector();
				const unsigned int& getEntityNumber();
				void addEntity(Entity* _Entity);
				void removeEntity(const unsigned int& _Index);
				void freeEntityVector();

				void sortEntityVector();
				inline const signed long long& getSortTime() { return m_llSortTimer; }
		};

		//Fonction qui permet de trier les entités par leur Z virtuel
		inline bool sortEntityByZ(Entity* _Entity1, Entity* _Entity2);
	}
}


#endif