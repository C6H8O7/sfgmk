namespace sfgmk
{
	PostShader::PostShader() : m_fTimer(0.0f), m_ShaderFromDataManager(false)
	{
		m_Shader = new sf::Shader;
	}

	PostShader::PostShader(const std::string& _file, sf::Shader::Type _type) : m_fTimer(0.0f), m_ShaderFromDataManager(false)
	{
		m_Shader = new sf::Shader;
		m_Shader->loadFromFile(_file, _type);
	}

	PostShader::PostShader(const std::string& _vertexFile, const std::string& _fragmentFile) : m_fTimer(0.0f), m_ShaderFromDataManager(false)
	{
		m_Shader = new sf::Shader;
		m_Shader->loadFromFile(_vertexFile, _fragmentFile);
	}

	PostShader::PostShader(sf::Shader& _shader) : m_fTimer(0.0f), m_ShaderFromDataManager(true)
	{
		m_Shader = &_shader;
	}

	PostShader::~PostShader()
	{
		if( !m_ShaderFromDataManager )
			delete m_Shader;
	}


	bool PostShader::applyPostShader(sf::RenderTexture& _renderTexture)
	{
		if( sf::Shader::isAvailable() )
		{
			// Laisser la taille par défaut "rogne" le shader dans la fenêtre 1280x720
			sf::Sprite sprite(_renderTexture.getTexture(), sf::IntRect(0, 0, 12800, 7200));

			_renderTexture.draw(sprite, m_Shader);

			return true;
		}

		return false;
	}

	void PostShader::applyPostShader(const sf::Shader& _Shader, sf::RenderTarget& _RenderTarget)
	{
		sf::Vector2f RenderSize = static_cast<sf::Vector2f>(_RenderTarget.getSize());

		sf::VertexArray Vertices(sf::TrianglesStrip, 4);
		Vertices[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(0, 1));
		Vertices[1] = sf::Vertex(sf::Vector2f(RenderSize.x, 0), sf::Vector2f(1, 1));
		Vertices[2] = sf::Vertex(sf::Vector2f(0, RenderSize.y), sf::Vector2f(0, 0));
		Vertices[3] = sf::Vertex(sf::Vector2f(RenderSize), sf::Vector2f(1, 0));

		sf::RenderStates States;
		States.shader = &_Shader;
		States.blendMode = sf::BlendNone;

		_RenderTarget.draw(Vertices, States);
	}
}