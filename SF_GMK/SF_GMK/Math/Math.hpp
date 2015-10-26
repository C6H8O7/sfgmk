/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		14/12/2014
	@brief		Regroupe les constantes utiles pour les calculs mathématiques, ainsi que des fonctions
				de calculs de base.

--------------------------------------------------------------------------------------------------*/


#ifndef MATH_HPP
#define MATH_HPP


namespace sfgmk
{
	namespace math
	{
		//Constantes
		#define G -9.81f
		#define EPSILON 0.00001f
		#define PI 3.14159265358979323846f
		#define PI_2 1.57079632679489661923f
		#define PI_4 0.785398163397448309616f
		#define PI_X_2 6.28318530718f

		#define X_UNIT_VECTOR_2D sf::Vector2f(1.0f, 0.0f)
		#define Y_UNIT_VECTOR_2D sf::Vector2f(0.0f, -1.0f)

		#define X_UNIT_VECTOR_3D sf::Vector3f(1.0f, 0.0f, 0.0f)
		#define Y_UNIT_VECTOR_3D sf::Vector2f(0.0f, -1.0f, 0.0f)
		#define Z_UNIT_VECTOR_3D sf::Vector2f(0.0f, 0.0f, 1.0f)

		//Macros
		#define ABS(A) ((A)>=(0)?(A):((-1)*(A)))
		#define RAND(MIN,MAX) ((MIN)+((rand())%((ABS(((MAX)-(MIN))))+(1)))) //Limite 32767
		#define SQUARE(a) (a)*(a)
		#define CUBE(a) a*a*a
		#define MAX(a,b) (((a) > (b)) ? (a) : (b))
		#define MIN(a,b) (((a) < (b)) ? (a) : (b))
		#define RAD_TO_DEG(a) ((a) * 57.2957795131f)
		#define DEG_TO_RAD(a) ((a) * 0.01745329251f)

		//Magic Square Root
		float SFGMK_API Msqrt(float _Number);	//Une iteration
		float SFGMK_API Msqrt(float _Number, int _Iteration);	//Plusieurs itérations (plus précis), un indice 3 donne généralement un résultat convenable

		//Puissance / Factorielle
		int SFGMK_API Pow(int _Number, int _Pow);
		float SFGMK_API Pow(float _Number, int _Pow);
		int SFGMK_API Factorial(int _Number);

		//Distance
		template<typename T>
		float Calc_Distance(const T& _X1, const T& _Y1, const T& _X2, const T& _Y2)
		{
			T X = _X1 - _X2;
			T Y = _Y1 - _Y2;

			return Msqrt(X*X + Y*Y);
		}

		template<typename T>
		T Calc_DistanceSquared(const T& _X1, const T& _Y1, const T& _X2, const T& _Y2)
		{
			T X = _X1 - _X2;
			T Y = _Y1 - _Y2;

			return (X*X + Y*Y);
		}

		template<typename T>
		float Calc_Distance(const T& _X1, const T& _Y1, const T& _Z1, const T& _X2, const T& _Y2, const T& _Z2)
		{
			T X = _X1 - _X2;
			T Y = _Y1 - _Y2;
			T Z = _Z1 - _Z2;

			return Msqrt(X*X + Y*Y + Z*Z);
		}

		template<typename T>
		T Calc_DistanceSquared(const T& _X1, const T& _Y1, const T& _Z1, const T& _X2, const T& _Y2, const T& _Z2)
		{
			T X = _X1 - _X2;
			T Y = _Y1 - _Y2;
			T Z = _Z1 - _Z2;

			return (X*X + Y*Y + Z*Z);
		}

		float SFGMK_API Calc_Distance(const sf::Vector2f& _Element1, const sf::Vector2f& _Element2);
		float SFGMK_API Calc_Distance(const sf::Vector2f& _Element1, const sf::Vector3f& _Element2);
		float SFGMK_API Calc_Distance(const sf::Vector3f& _Element1, const sf::Vector3f& _Element2);
		float SFGMK_API Calc_DistanceSquared(const sf::Vector2f& _Element1, const sf::Vector2f& _Element2);
		float SFGMK_API Calc_DistanceSquared(const sf::Vector3f& _Element1, const sf::Vector3f& _Element2);

		//Angle
		float SFGMK_API Calc_Angle(const sf::Vector2f& _VectorOne, const sf::Vector2f& _VectorTwo);

		//Vector
		sf::Vector2f SFGMK_API Calc_Vector(sf::Vector2f _OriginePoint, sf::Vector2f _FinalPoint);
		sf::Vector3f SFGMK_API Calc_Vector(sf::Vector3f _OriginePoint, sf::Vector3f _FinalPoint);
		sf::Vector3f SFGMK_API Convert2dTo3d(const sf::Vector2f& _Vector2d, const float& _Z = 0.0f);
		sf::Vector2f SFGMK_API Convert3dTo2d(const sf::Vector3f& _Vector3d);

		float SFGMK_API DotProduct(const sf::Vector2f& _Vector1, const sf::Vector2f& _Vector2);
		float SFGMK_API DotProduct(const sf::Vector3f& _Vector1, const sf::Vector3f& _Vector2);
		sf::Vector2f SFGMK_API CrossProduct(const sf::Vector2f& _Vector1, const sf::Vector2f& _Vector2);
		sf::Vector3f SFGMK_API CrossProduct(const sf::Vector3f& _Vector1, const sf::Vector3f& _Vector2);

		//Norme
		float SFGMK_API Calc_Norm(sf::Vector2f _Vector);
		float SFGMK_API Calc_Norm(sf::Vector3f _Vector);
		float SFGMK_API Calc_NormSquared(sf::Vector2f _Vector);
		float SFGMK_API Calc_NormSquared(sf::Vector3f _Vector);
		sf::Vector2f SFGMK_API Calc_UnitVector(sf::Vector2f _Vector);
		sf::Vector3f SFGMK_API Calc_UnitVector(sf::Vector3f _Vector);

		//Physique
		bool SFGMK_API ProjectionsIntersects(const float& _l1, const float& _r1, const float& _l2, const float& _r2);
		bool SFGMK_API RectsIntersects(const float& _x1, const float& _y1, const float& _w1, const float& _h1, const float& _x2, const float& _y2, const float& _w2, const float& _h2);
	}
}


#endif