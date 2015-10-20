/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		22/12/2014
	@brief		Classe ShaderWave, enfant de PostShader

--------------------------------------------------------------------------------------------------*/

#ifndef SHADERWAVE_H
#define SHADERWAVE_H


namespace sfgmk
{
	class SFGMK_API ShaderWave : public PostShader
	{
		public:
			ShaderWave(float _x, float _y, float _duration, float _maxRadius);
			~ShaderWave();

			virtual bool update(float _timeDelta);
			virtual bool isAlive();

		protected:
			float m_fX, m_fY;
			float m_fDuration;
			float m_fMaxRadius;
	};
}


#endif