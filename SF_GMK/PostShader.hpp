/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		22/12/2014
	@brief		Classe PostShader, appliquée au rendu final

--------------------------------------------------------------------------------------------------*/

#ifndef POSTSHADER_H
#define POSTSHADER_H


namespace sfgmk
{
	class PostShader
	{
		public:
			PostShader();
			PostShader(const std::string& _file, sf::Shader::Type _type);
			PostShader(const std::string& _vertexFile, const std::string& _fragmentFile);
			PostShader(sf::Shader& _shader);
			~PostShader();

			bool applyPostShader(sf::RenderTexture& _renderTexture);
			static void applyPostShader(const sf::Shader& _Shader, sf::RenderTarget& _RenderTarget);

			virtual bool update(float _timeDelta) = 0;
			virtual bool isAlive() = 0;

	protected:
		bool m_ShaderFromDataManager;
		sf::Shader *m_Shader;

		float m_fTimer;
	};
}


#endif