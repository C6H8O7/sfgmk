/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		15/06/2015
	@brief		Manager de la physique, gére les collisions/réponses aux collisions

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_PHYSIC_MANAGER_HPP
#define SFGMK_PHYSIC_MANAGER_HPP


namespace sfgmk
{
	class PhysicManager : public SingletonTemplate<PhysicManager>
	{
		friend class SingletonTemplate<PhysicManager>;

		private:
			sfgmk::DynamicArray<Collider*> m_PhysicObjects;
			bool m_bDrawCollider;

			PhysicManager();
			~PhysicManager();

			void drawCollider(Collider* _Collider, sf::RenderTexture* _RenderTexture);

		public:
			const bool& getDraw();
			void setDraw(const bool& _Boolean);

			void update();	

			void draw(sf::RenderTexture* _RenderTexture);

			void addCollider(Collider* _NewCollider);
			void removeCollider(Collider* _ColliderToRemove);

			bool testCollision(Collider* _Collider1, Collider* _Collider2);

			bool OverlapBoxes(sf::Vector2f _Box1Center, sf::Vector2f _Box1HalfSize, sf::Vector2f _Box1Ortho[3], sf::Vector2f _Box2Center, sf::Vector2f _Box2HalfSize, sf::Vector2f _Box2Ortho[3]);

			bool collisionSphereSphere(SphereCollider* _Collider1, SphereCollider* _Collider2, sfgmk::Entity* _Entity1, sfgmk::Entity* _Entity2);
			bool collisionObbObb(ObbCollider* _Collider1, ObbCollider* _Collider2, sfgmk::Entity* _Entity1, sfgmk::Entity* _Entity2);

			bool collisionSphereObb(SphereCollider* _SphereCollider, ObbCollider* _BoxCollider, sfgmk::Entity* _Entity1, sfgmk::Entity* _Entity2);
	};
}


#endif