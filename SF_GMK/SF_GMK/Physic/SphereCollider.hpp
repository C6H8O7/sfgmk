/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		15/06/2015
	@brief		

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_SPHERE_COLLIDER_HPP
#define SFGMK_SPHERE_COLLIDER_HPP


namespace sfgmk
{
	class SFGMK_API SphereCollider : public Collider
	{
		private:
			float m_fRadius;

		public:
			SphereCollider(Entity* _Entity, float _Radius);
			~SphereCollider();

			const float& getRadius();

			float getWorldRadius();
			float getSquaredWorldRadius();

			void setRadius(const float& _Radius);
	};
}


#endif