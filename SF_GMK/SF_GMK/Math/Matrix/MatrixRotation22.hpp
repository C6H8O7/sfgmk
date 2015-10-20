/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		07/07/2015
	@brief		Matrice carré de dimension 2, classe spécialisée de matrix22 pour faciliter les rotations

--------------------------------------------------------------------------------------------------*/

#ifndef MATRIX_ROTATION_22_HPP
#define MATRIX_ROTATION_22_HPP


namespace sfgmk
{
	namespace math
	{
		class SFGMK_API MatrixRotation22 : public Matrix22
		{
			public:
				MatrixRotation22(float _Rotation);
				~MatrixRotation22();
		};
	}
}


#endif