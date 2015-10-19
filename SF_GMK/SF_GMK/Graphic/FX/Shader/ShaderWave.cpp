namespace sfgmk
{
	ShaderWave::ShaderWave(float _x, float _y, float _duration, float _maxRadius) : PostShader(DATA_MANAGER->getShader("wave.frag"))
	{
		m_fX = _x;
		m_fY = _y;
		m_fDuration = _duration;
		m_fMaxRadius = _maxRadius;
	}

	ShaderWave::~ShaderWave()
	{
	}

	bool ShaderWave::update(float _timeDelta)
	{
		m_fTimer += _timeDelta;

		float f = m_fTimer / m_fDuration;

		float totalRadius = f * m_fMaxRadius;
		float innerRadius = totalRadius * 3.0f / 4.0f;

		if( totalRadius < 0.0f )
			totalRadius = 0.0f;
		if( innerRadius < 0.0f )
			innerRadius = 0.0f;

		engine::Camera* CurrentCamera = CAMERA;
		sf::Vector2f pos = CurrentCamera->getCenter();
		sf::Vector2f size = CurrentCamera->getSize();

		pos.x -= size.x / 2.0f;
		pos.y -= size.y / 2.0f;

		m_Shader->setParameter("texture", sf::Shader::CurrentTexture);
		m_Shader->setParameter("view", pos.x, pos.y, size.x, size.y);
		m_Shader->setParameter("storm_position", m_fX, m_fY);
		m_Shader->setParameter("storm_total_radius", totalRadius);
		m_Shader->setParameter("storm_inner_radius", innerRadius);

		return isAlive();
	}

	bool ShaderWave::isAlive()
	{
		return m_fTimer < m_fDuration;
	}
}