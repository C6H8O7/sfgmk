namespace sfgmk
{
	namespace math
	{
		MatrixRotation22::MatrixRotation22(float _Rotation) : Matrix22(cos(_Rotation), -sin(_Rotation), sin(_Rotation), cos(_Rotation))
		{
		}

		MatrixRotation22::~MatrixRotation22()
		{
		}
	}
}