namespace sfgmk
{
	LightningText::LightningText(std::string _String, sf::Font _Font, unsigned int _Size, unsigned int _Precision, unsigned int _BoltChance, sf::Color _Color)
		: FxText(_String, _Font, _Size, _Color), m_uiPrecision(_Precision), m_uiBoltChance(_BoltChance)
	{
		m_uiArraySize = m_PixelArray.getElementNumber();
		m_MersenneTwister19937Generator.seed((unsigned long)std::time(0));

		m_Shader = &DATA_MANAGER->getShader("intensityChange");
		m_PreviousFrameRender.create(m_Size.x, m_Size.y);
		m_PreviousFrameRender.clear();
	}

	LightningText::~LightningText()
	{
		m_Bolts.deleteAndClear();
	}


	void LightningText::update(const float& _TimeDelta)
	{
		LightningBolt* NewBolt = NULL;
		sf::Vector2f NearestParticle, OtherPoint;
		float fNearestDistance(0.0f), fDistance(0.0f);

		m_Render->clear(EMPTY_COLOR);
		m_Bolts.deleteContent();

		for( unsigned int i(0U); i < m_uiArraySize; i++ )
		{
			if( RAND(0, m_uiBoltChance) == 0 )
			{
				NearestParticle = sf::Vector2f(0.0f, 0.0f);
				fNearestDistance = (float)LONG_MAX;
				for( unsigned int j(0U); j < m_uiPrecision; j++ )
				{
					std::uniform_int_distribution<long long> m_Distribution(0, m_uiArraySize);
					OtherPoint = m_PixelArray[(const unsigned int)m_Distribution(m_MersenneTwister19937Generator)];
					fDistance = math::Calc_DistanceSquared(m_PixelArray[i], OtherPoint);

					if( fDistance < fNearestDistance && fDistance > 100.0f ) //Ne sélectionne un nouveau point que si il est plus proche que le précédent, et ne crée pas un bolt trop court	10.0f * 10.0f
					{
						fNearestDistance = fDistance;
						NearestParticle = OtherPoint;
					}
				}

				if( fNearestDistance < 40000.0f && fNearestDistance > 100.0f ) //Limite les tailles max et min des bolts	200.0f * 200.0f    10.0f * 10.0f	
				{
					NewBolt = new LightningBolt(math::Convert2dTo3d(m_PixelArray[i], -1.0f), NearestParticle, false, m_Color);
					NewBolt->setMax();
					m_Bolts.pushBack(NewBolt);
				}
			}
		}
	}

	void LightningText::draw(sf::RenderTexture* _Render)
	{
		//Dessine la frame précédente, avec effet d'estompement
		m_Shader->setParameter("_Texture", m_PreviousFrameRender.getTexture());
		m_Shader->setParameter("_Ratio", 0.96f);
		PostShader::applyPostShader(*m_Shader, *m_Render);

		//Dessine les nouveaux bolts
		for( unsigned int i(0U); i < m_Bolts.getElementNumber(); i++ )
			m_Bolts[i]->draw(m_Render);
		m_Render->display();

		//Dessine le tout dans le rendu principal
		m_Sprite.setTexture(m_Render->getTexture());
		_Render->draw(m_Sprite);

		//Stocke la frame courante pour la prochaine boucle
		m_PreviousFrameRender.clear(EMPTY_COLOR);
		m_PreviousFrameRender.draw(m_Sprite);
		m_PreviousFrameRender.display();
	}
}