/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		15/06/2015
	@brief		Objet physique utilisé pour détecter les collisions

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_COLLIDER_HPP
#define SFGMK_COLLIDER_HPP


namespace sfgmk
{
		class Entity;

		enum eCOLLIDER_TYPE
		{
			eSphere = 1,
			eOBB = 2,
			eCOLLIDER_TYPE_NUMBER
		};

		class Collider
		{
			protected:
				eCOLLIDER_TYPE m_Type;
				bool m_bActive;
				bool m_bCollide;

				Entity* m_Entity;

			public:
				Collider(Entity* _Entity);
				~Collider();

				const eCOLLIDER_TYPE& getType();
				const bool& isActive();
				void setActive(bool _Boolean);
				const bool& Collide();
				void setCollide(bool _Boolean);

				sf::Vector2f getWorldCenter();

				Entity* getEntity();
		};
}


#endif