/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		15/06/2015
	@brief

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_OBB_COLLIDER_HPP
#define SFGMK_OBB_COLLIDER_HPP


namespace sfgmk
{
#define OBB_COLLIDER_POINT_NUMBER 4

		class ObbCollider : public Collider
		{
			private:
			sf::Vector2f m_Size;
			sf::Vector2f m_Min;
			sf::Vector2f m_Max;

			public:
			ObbCollider(Entity* _Entity, sf::Vector2f _Size);
			~ObbCollider();

			void computeRect();

			const sf::Vector2f& getSize();
			sf::Vector2f getWorldSize();
			const sf::Vector2f& getMin();
			const sf::Vector2f& getMax();

			sf::Vector2f getRight();
			sf::Vector2f getUp();
		};
}


#endif