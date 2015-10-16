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
	#define RAND(MIN,MAX) ((MIN)+((rand())%((ABS(((MAX)-(MIN))))+(1))))
	#define SQUARE(a) (a)*(a)
	#define CUBE(a) a*a*a
	#define MAX(a,b) (((a) > (b)) ? (a) : (b))
	#define MIN(a,b) (((a) < (b)) ? (a) : (b))
	#define RAD_TO_DEG(a) ((a) * 57.2957795131f)
	#define DEG_TO_RAD(a) ((a) * 0.01745329251f)

	//Magic Square Root
	float Msqrt(float _Number);	//Une iteration
	float Msqrt(float _Number, int _Iteration);	//Plusieurs itérations (plus précis), un indice 3 donne généralement un résultat convenable

	//Puissance / Factorielle
	int Pow(int _Number, int _Pow);
	float Pow(float _Number, int _Pow);
	int Factorial(int _Number);

	//Distance
	float Calc_Distance(float _X1, float _Y1, float _X2, float _Y2);
	float Calc_Distance(float _X1, float _Y1, float _Z1, float _X2, float _Y2, float _Z2);
	float Calc_Distance(sf::Vector2f _Element1, sf::Vector2f _Element2);
	float Calc_Distance(sf::Vector2f _Element1, sf::Vector3f _Element2);
	float Calc_Distance(sf::Vector3f _Element1, sf::Vector3f _Element2);
	float Calc_DistanceSquared(sf::Vector2f _Element1, sf::Vector2f _Element2);
	float Calc_DistanceSquared(sf::Vector3f _Element1, sf::Vector3f _Element2);

	//Angle
	float Calc_Angle(sf::Vector2f _VectorOne, sf::Vector2f _VectorTwo);

	//Vector
	sf::Vector2f Calc_Vector(sf::Vector2f _OriginePoint, sf::Vector2f _FinalPoint);
	sf::Vector3f Calc_Vector(sf::Vector3f _OriginePoint, sf::Vector3f _FinalPoint);
	sf::Vector3f Convert2dTo3d(const sf::Vector2f _Vector2d, const float _Z = 0.0f);
	sf::Vector2f Convert3dTo2d(const sf::Vector3f _Vector3d);

	float DotProduct(const sf::Vector2f& _Vector1, const sf::Vector2f& _Vector2);
	float DotProduct(const sf::Vector3f& _Vector1, const sf::Vector3f& _Vector2);
	sf::Vector2f CrossProduct(const sf::Vector2f& _Vector1, const sf::Vector2f& _Vector2);
	sf::Vector3f CrossProduct(const sf::Vector3f& _Vector1, const sf::Vector3f& _Vector2);

	//Norme
	float Calc_Norm(sf::Vector2f _Vector);
	float Calc_Norm(sf::Vector3f _Vector);
	float Calc_NormSquared(sf::Vector2f _Vector);
	float Calc_NormSquared(sf::Vector3f _Vector);
	sf::Vector2f Calc_UnitVector(sf::Vector2f _Vector);
	sf::Vector3f Calc_UnitVector(sf::Vector3f _Vector);

	//Physique
	bool ProjectionsIntersects(float _l1, float _r1, float _l2, float _r2);
	bool RectsIntersects(float _x1, float _y1, float _w1, float _h1, float _x2, float _y2, float _w2, float _h2);
}


#endif