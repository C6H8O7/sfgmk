namespace sfgmk
{
		LightningBolt::LightningBolt(const sf::Vector3f& _Position, const sf::Vector2f& _Destination, const bool& _Branch, const sf::Color& _Color, const float& _Thickness)
			: Entity(_Position), m_fThickness(_Thickness), m_Color(_Color), m_fAlpha(0.0f), m_bIsDecreasing(false), m_bBranch(_Branch), m_iDrawNumber(0)
		{
			m_Sprite = new Sprite();
			m_Sprite->setTexture(DATA_MANAGER->getTexture("sfgmk_lightningCorpse"));
			m_Sprite->setRelativeOrigin(0.0f, 0.5f);

			//Init
			sf::Vector2f Tangent = _Destination - math::Convert3dTo2d(_Position);
			sf::Vector2f Normal = math::Calc_UnitVector(sf::Vector2f(Tangent.y, -Tangent.y));
			float fLength = math::Calc_Norm(Tangent);

			m_iPointNumber = (int)(fLength * 0.25f);

			m_PointArray = new sf::Vector2f[m_iPointNumber];
			m_PointArray[0] = sf::Vector2f(_Position.x, _Position.y);
			m_PointArray[m_iPointNumber - 1] = _Destination;

			//Position of intermediates points
			float* fRatioArray = new float[m_iPointNumber - 1];
			for( int i(0); i < m_iPointNumber - 2; i++ )
				fRatioArray[i] = (float)RAND(0, 100) * 0.01f;
			fRatioArray[m_iPointNumber - 2] = 0.0f;

			//Sort the array
			sortBubble(fRatioArray, (m_iPointNumber - 2) + 1);

			const float fSway = 80.0f;
			const float fJaggedness = 1.0f / fSway;
			float fPreviousDisplacement = 0.0f;

			for( int i(1); i < m_iPointNumber - 1; i++ )
			{
				float fPos = fRatioArray[i];

				float fScale = (fLength * fJaggedness) * (fPos - fRatioArray[i - 1]); 	//Used to prevent sharp angles by ensuring very close positions also have small perpendicular variation.
				float envelope = fPos > 0.95f ? 20.0f * (1.0f - fPos) : 1; 				//Defines an envelope. Points near the middle of the bolt can be further from the central line.
				float fDisplacement = (float)RAND((int)-fSway, (int)fSway);

				fDisplacement -= (fDisplacement - fPreviousDisplacement) * (1.0f - fScale);
				fDisplacement *= envelope;

				m_PointArray[i] = m_PointArray[0] + (fPos * Tangent) + (fDisplacement * Normal);
				fPreviousDisplacement = fDisplacement;
			}

			delete[] fRatioArray;

			//TODO Branches
			/*if( m_bBranch )
			{
				sfgmk::LightningBolt* Branch = NULL;

				for( int i(1); i < m_iPointNumber - 1; i++ )
				{
					if( RAND(0, 1000) < LIGHTNING_BOLT_BRANCH_CHANCE )
					{
						sf::Vector2f Destination = sfgmk::MatrixRotation22(float(RAND(-30, 30))) * ((m_PointArray[i + 1] - m_PointArray[i]) * float(RAND(1, 100) * 0.01f));

						Branch = new sfgmk::LightningBolt(sf::Vector3f(m_PointArray[i].x, m_PointArray[i].y, _Position.z), Destination, 175U, m_bBranch, m_Color, m_fThickness);
						ADD_ENTITY(Branch);
					}
				}
			}*/
		}

		LightningBolt::~LightningBolt()
		{
			delete[] m_PointArray;
		}


		void LightningBolt::update(const float& _TimeDelta)
		{
			m_bIsDecreasing ? m_fAlpha -= _TimeDelta * LIGHTNING_BOLT_ALPHA_DECREASE_SPEED : m_fAlpha += _TimeDelta * LIGHTNING_BOLT_ALPHA_INCREASE_SPEED;
			m_fAlpha >= 255.0f ? m_bIsDecreasing = true : m_bIsDecreasing;
			m_Color.a = (int)m_fAlpha;

			if( m_fAlpha <= 0.0f )
				m_bIsAlive = false;

			m_iDrawNumber = 0;
		}

		void LightningBolt::draw(sf::RenderTexture* _Render)
		{
			const float ImageThickness = 8.0f;
			float fThicknessScale = m_fThickness / ImageThickness;

			m_Sprite->setColor(m_Color);

			for( int i(0); i < m_iPointNumber; i++ )
			{
				sf::Vector2f Vector(m_PointArray[i + 1] - m_PointArray[i]);
				float fNorm = math::Calc_Norm(Vector);
				float fRotation = RAD_TO_DEG(atan2(Vector.y, Vector.x));
				sf::Vector2f Position(0.0f, m_Sprite->getTextureRect().width * 0.5f);
				sf::Vector2f Scale(fNorm, fThicknessScale);

				m_Sprite->setRotation(fRotation);
				m_Sprite->setScale(Scale);
				m_Sprite->setPosition(m_PointArray[i]);
				_Render->draw(*m_Sprite);

				m_iDrawNumber++;
			}

			PARALLAXE.addDrawToAccount(m_iDrawNumber - 1);
		}


		void LightningBolt::setMax()
		{
			m_fAlpha = 255.0f;
			m_bIsDecreasing = true;
		}
}