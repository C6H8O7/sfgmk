/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		18/12/2014
	@brief		Classe de base représentant une entité dans l'espace de jeu

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_ENTITY_HPP
#define SFGMK_ENTITY_HPP


namespace sfgmk
{
	class EntityTransformation;

	class Entity : public sf::Transformable
	{
		protected:
			int m_iId;
			bool m_bIsAlive;

			bool m_bIsComputatedByParallax;
			bool m_bIsScaledByParallax;
			float m_fPositionZ;

			Sprite* m_Sprite;
			std::vector<EntityTransformation*> m_Transformation;

			sf::Transformable m_VirtualTransform;

			Collider* m_Collider;

		public:
			Entity(const int& _Id = 0);
			Entity(const sf::Vector3f& _Position, const int& _Id = 0);
			virtual ~Entity();

			virtual void update(const float& _TimeDelta);
			virtual void finalize(const float& _TimeDelta);
			virtual void draw(sf::RenderTexture* _Render);

			const int& getId();
			void setId(int _Id);
			const bool& getIsAlive();
			void setIsAlive(bool _State);

			const bool& getIsComputatedByParralax();
			void setIsComputatedByParralax(bool _Boolean);
			const bool& getIsScaledByParralax();
			void setIsScaledByParralax(bool _Boolean);

			Sprite* getSprite();
			void setRelativOrigin(const float& _X, const float& _Y);
			void addTransformation(EntityTransformation* _Transformation);
			void removeTransformation(const int _Index);
			std::vector<EntityTransformation*>& getTransformation();

			const float& getZ();
			sf::Vector3f getPosition3D();
			void setPosition(const sf::Vector2f& position);
			void setPosition(const sf::Vector3f& _Position);
			void move(const sf::Vector2f& _Vector);
			void move(const sf::Vector3f& _Vector);

			sf::Transformable& getVirtualTransform();

			Collider* getCollider();
			void addSphereCollider();
			void addSphereCollider(const float& _Radius);
			void addObbCollider();
			void addObbCollider(const sf::Vector2f& _Size);

			virtual void physicCallBack(const int& _ColliderId);
	};
}


#endif