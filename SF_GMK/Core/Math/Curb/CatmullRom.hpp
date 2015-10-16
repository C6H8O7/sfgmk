/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		09/03/2015
	@brief		Courbe d'interpolation

--------------------------------------------------------------------------------------------------*/


#ifndef CATMULL_ROM_HPP
#define CATMULL_ROM_HPP


namespace sfgmk
{
	class CatmullRom : public Curb
	{
		public:
			CatmullRom(int _PointNumber, float _Time = 1.0f);
			~CatmullRom();

			void update(float _TimeDelta);

			void computeCoords();

		private:
			int m_iPointNumber;
			int m_iCurrentPoint;
	};
}


#endif