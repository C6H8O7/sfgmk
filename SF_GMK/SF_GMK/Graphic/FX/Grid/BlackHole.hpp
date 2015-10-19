/*--------------------------------------------------------------------------------------------------

	@author		GMK (sources: Terence J. Grant / http://gamedevelopment.tutsplus.com/tutorials/make-a-neon-vector-shooter-for-ios-virtual-gamepads-and-black-holes--gamedev-14633?utm_source=Tuts+&utm_medium=website&utm_campaign=relatedtutorials&utm_content=sidebar&WT.mc_id=Tuts+_website_relatedtutorials_sidebar
	@date		23/04/2015
	@brief		Trou noir affectant la grille et les entités proches

--------------------------------------------------------------------------------------------------*/

#ifndef BLACK_HOLE_HPP
#define BLACK_HOLE_HPP


namespace sfgmk
{
	class BlackHole : public Entity
	{
		private:
			Grid* m_Grid;
			float m_fSprayAngle;

		public:
			BlackHole();
			~BlackHole();

			void update(const float& _TimeDelta);

			void setGrid(Grid* _Grid);
	};
}


#endif