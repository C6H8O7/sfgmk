namespace sfgmk
{
		Grid::Grid() : m_BloomEffect(NULL), m_bDrawMidLines(false), m_bApplyBloom(false), m_iDrawNumber(0)
		{
		}

		Grid::Grid(const sf::Vector2i& _Lines, const sf::Vector2f& _Spacing, const sf::Vector3f& _Position) : Entity(_Position),
			m_BloomEffect(NULL), m_iLineNumber(_Lines), m_fSpacing(_Spacing), m_bDrawMidLines(false), m_bApplyBloom(false), m_iDrawNumber(0)
		{
			m_Color[0] = sf::Color::Blue;
			m_Color[1] = sf::Color::Blue;
			m_Color[1].a = 150;

			init();
		}

		Grid::~Grid()
		{
			delete[] m_Points;
			delete[] m_FixedPoints;

			m_Springs.clear();

			delete m_RenderTexture;

			if( m_BloomEffect != NULL )
				delete m_BloomEffect;
		}


		void Grid::init()
		{
			m_iPointNumber = m_iLineNumber.x * m_iLineNumber.y;
			m_Points = new PointMass[m_iPointNumber];
			m_FixedPoints = new PointMass[m_iPointNumber];

			m_Size = sf::Vector2f((unsigned int)(m_iLineNumber.x - 1) * m_fSpacing.x, (unsigned int)(m_iLineNumber.y - 1) * m_fSpacing.y);
			m_ScreenSize = sf::Vector2f(GRAPHIC_MANAGER->getRenderWindow()->getSize());

			m_RenderTexture = new sf::RenderTexture();
			m_RenderTexture->create(MAX((unsigned int)GRAPHIC_MANAGER->getCurrentCamera()->getSize().x, (unsigned int)m_Size.x + 4U), MAX((unsigned int)GRAPHIC_MANAGER->getCurrentCamera()->getSize().y, (unsigned int)m_Size.y + 4U));

			int iColumn(0), iRow(0);
			sf::Vector2f Position = getPosition();
			for( float fY(Position.y); fY <= Position.y + m_fSpacing.y * (m_iLineNumber.y - 1); fY += m_fSpacing.y )
			{
				for( float fX(Position.x); fX <= Position.x + m_fSpacing.x * (m_iLineNumber.x - 1); fX += m_fSpacing.x )
				{
					setPointMass(m_Points, iColumn, iRow, PointMass(sf::Vector3f(fX, fY, 0), 1));
					setPointMass(m_FixedPoints, iColumn, iRow, PointMass(sf::Vector3f(fX, fY, 0), 0));
					iColumn++;
				}
				iRow++;
				iColumn = 0;
			}

			//Link the point masses with springs
			for( int y(0); y < m_iLineNumber.y; y++ )
			{
				for( int x(0); x < m_iLineNumber.x; x++ )
				{
					if( x == 0 || y == 0 || x == m_iLineNumber.x - 1 || y == m_iLineNumber.y - 1 )
					{
						m_Springs.push_back(Spring(getPointMass(m_FixedPoints, x, y), getPointMass(m_Points, x, y), 10.0f, 1.0f)); //Le stiffness élevé permet de "verrouiller" les bords de la grille
						m_Springs[m_Springs.size() - 1].m_bBehind = true;
					}
					else if( x % 3 == 0 && y % 3 == 0 )
					{
						m_Springs.push_back(Spring(getPointMass(m_FixedPoints, x, y), getPointMass(m_Points, x, y), 0.002f, 0.02f));
						m_Springs[m_Springs.size() - 1].m_bBehind = true;
					}

					if( x > 0 )
						m_Springs.push_back(Spring(getPointMass(m_Points, x - 1, y), getPointMass(m_Points, x, y), 0.28f, 0.06f));

					if( y > 0 )
						m_Springs.push_back(Spring(getPointMass(m_Points, x, y - 1), getPointMass(m_Points, x, y), 0.28f, 0.06f));
				}
			}
		}

		void Grid::setLineNumber(const sf::Vector2i& _Lines)
		{
			m_iLineNumber = _Lines;
		}

		void Grid::setSpacing(const sf::Vector2f& _Spacing)
		{
			m_fSpacing = _Spacing;
		}

		const sf::Vector2f& Grid::getSize()
		{
			return m_Size;
		}


		void Grid::setPointMass(PointMass* _Array, const int& _Column, const int& _Row, const PointMass& _Point)
		{
			int iIndex = _Row * m_iLineNumber.x + _Column;

			_Array[iIndex] = _Point;
		}

		PointMass* Grid::getPointMass(PointMass* _Array, const int& _Column, const int& _Row)
		{
			int iIndex = _Row * m_iLineNumber.x + _Column;

			return &_Array[iIndex];
		}


		float Grid::catmullRom(const float _Value1, const float _Value2, const float _Value3, const float _Value4, float _Amount)
		{
			// Using formula from http://www.mvps.org/directx/articles/catmull/

			float amountSquared = _Amount * _Amount;
			float amountCubed = amountSquared * _Amount;
			return (float)(0.5f * (2.0f * _Value2 +
								   (_Value3 - _Value1) * _Amount +
								   (2.0f * _Value1 - 5.0f * _Value2 + 4.0f * _Value3 - _Value4) * amountSquared +
								   (3.0f * _Value2 - _Value1 - 3.0f * _Value3 + _Value4) * amountCubed));
		}

		sf::Vector2f Grid::catmullRom(const sf::Vector2f& _Value1, const sf::Vector2f& _Value2, const sf::Vector2f& _Value3, const sf::Vector2f& _Value4, float _Amount)
		{
			return sf::Vector2f(catmullRom(_Value1.x, _Value2.x, _Value3.x, _Value4.x, _Amount), catmullRom(_Value1.y, _Value2.y, _Value3.y, _Value4.y, _Amount));
		}


		void Grid::update(const float& _TimeDelta)
		{
			m_RenderTexture->clear(sf::Color(0, 0, 0, 0));
			m_iDrawNumber = 0;

			for( size_t i(0); i < m_Springs.size(); i++ )
				m_Springs[i].update();

			for( int i(0); i < m_iPointNumber; i++ )
				m_Points[i].update();
		}

		void Grid::draw(sf::RenderTexture* _Render)
		{
			m_CameraView = GRAPHIC_MANAGER->getCurrentCamera()->getRect(); // get rekt lel

			sf::Vector2f VecLeft, VecUp, VecP, VecUpLeft, Mid;
			int iClamped, x, y;

			//Draw des lignes
			for( y = 1; y < m_iLineNumber.y; y++ )
			{
				for( x = 1; x < m_iLineNumber.x; x++ )
				{
					VecP = toVec2(getPointMass(m_Points, x, y)->m_Position);
					VecLeft = toVec2(getPointMass(m_Points, x - 1, y)->m_Position);
					VecUp = toVec2(getPointMass(m_Points, x, y - 1)->m_Position);

					//Interpolation des lignes (catmull-rom) si nécessaire, sinon draw "classique"
						//Horizontal
					if( y < m_iLineNumber.y - 1 ) //Evite de dessiner le "cadre", il est déssiné à part
					{
						iClamped = MIN(x + 1, m_iLineNumber.x - 1);
						Mid = catmullRom(toVec2(getPointMass(m_Points, x - 2, y)->m_Position), VecLeft, VecP, toVec2(getPointMass(m_Points, iClamped, y)->m_Position), 0.5f);

						if( math::Calc_DistanceSquared(Mid, (VecLeft + VecP) * 0.5f) > 1.0f && x > 1 ) //x > 1 ==> prévient une déformation peu esthétique
						{
							drawLineWithVisibilityTest(VecLeft, Mid, m_Color[0]);
							drawLineWithVisibilityTest(Mid, VecP, m_Color[0]);
						}
						else
							drawLineWithVisibilityTest(VecLeft, VecP, m_Color[0]);
					}
					//Vertical
					if( x < m_iLineNumber.x - 1 ) //Evite de dessiner le "cadre", il est déssiné à part 
					{
						iClamped = MIN(y + 1, m_iLineNumber.y - 1);
						Mid = catmullRom(toVec2(getPointMass(m_Points, x, y - 2)->m_Position), VecUp, VecP, toVec2(getPointMass(m_Points, x, iClamped)->m_Position), 0.5f);

						if( math::Calc_DistanceSquared(Mid, (VecUp + VecP) * 0.5f) > 1.0f && y > 1 ) //y > 1 ==> prévient une déformation peu esthétique
						{
							drawLineWithVisibilityTest(VecUp, Mid, m_Color[0]);
							drawLineWithVisibilityTest(Mid, VecP, m_Color[0]);
						}
						else
							drawLineWithVisibilityTest(VecUp, VecP, m_Color[0]);
					}

					//Dessine des lignes intermediaires
					if( m_bDrawMidLines )
					{
						VecUpLeft = toVec2(getPointMass(m_Points, x - 1, y - 1)->m_Position);
						drawLineWithVisibilityTest(0.5f * (VecUpLeft + VecUp), 0.5f * (VecLeft + VecP), m_Color[1]); //Vertical line
						drawLineWithVisibilityTest(0.5f * (VecUpLeft + VecLeft), 0.5f * (VecUp + VecP), m_Color[1]); //Horizontal line
					}
				}
			}

			//Draw du cadre
			sf::Vector2f Origin(getPosition() + sf::Vector2f(2.0f, 2.0f));
			sf::Vector2f RightTopPoint(Origin + sf::Vector2f((m_iLineNumber.x - 1) * m_fSpacing.x, 0.0f));
			sf::Vector2f LeftBotPoint(Origin + sf::Vector2f(0.0f, (m_iLineNumber.y - 1) * m_fSpacing.y));
			sf::Vector2f RightBotPoint(Origin + sf::Vector2f((m_iLineNumber.x - 1) * m_fSpacing.x, (m_iLineNumber.y - 1) * m_fSpacing.y));

			drawLine(Origin, RightTopPoint, m_Color[2]); //Haut
			drawLine(LeftBotPoint, RightBotPoint, m_Color[2]); //Bas
			drawLine(Origin, LeftBotPoint, m_Color[2]); //Gauche
			drawLine(RightTopPoint, RightBotPoint, m_Color[2]); //Droite

			//Application du bloom
			m_RenderTexture->display();
			if( m_BloomEffect )
				m_BloomEffect->applyEffect(*m_RenderTexture);

			//Rendu final
			m_Sprite.setTexture(m_RenderTexture->getTexture());
			_Render->draw(m_Sprite);
			//GAME_MANAGER->getParallaxe().addDrawToAccount(m_iDrawNumber - 1); //-1 car un draw est compté dans parallaxe
		}

		void Grid::drawLine(sf::Vector2f& _Point1, const sf::Vector2f& _Point2, const sf::Color& _Color)
		{
			m_iDrawNumber++;

			m_Line[0] = sf::Vertex(_Point1, _Color);
			m_Line[1] = sf::Vertex(_Point2, _Color);
			m_RenderTexture->draw(m_Line, 2, sf::Lines);
		}

		void Grid::drawLineWithVisibilityTest(sf::Vector2f& _Point1, sf::Vector2f& _Point2, const sf::Color& _Color)
		{
			if( m_CameraView.contains(_Point1) || m_CameraView.contains(_Point2) )
			{
				m_Line[0] = sf::Vertex(_Point1, _Color);
				m_Line[1] = sf::Vertex(_Point2, _Color);
				m_RenderTexture->draw(m_Line, 2, sf::Lines);

				m_iDrawNumber++;
			}
		}


		void Grid::applyDirectedForce(const sf::Vector3f& _Force, const sf::Vector3f& _Position, const float& _Radius)
		{
			float fSquaredRadius = _Radius * _Radius;

			for( int i(0); i < m_iPointNumber; i++ )
			{
				if( math::Calc_DistanceSquared(_Position, m_Points[i].m_Position) < fSquaredRadius )
					m_Points[i].applyForce(10.0f * _Force / (10.0f + math::Calc_Distance(_Position, m_Points[i].m_Position)));
			}
		}

		void Grid::applyImplosiveForce(const float& _Force, const sf::Vector3f& _Position, const float& _Radius)
		{
			float fSquaredRadius = _Radius * _Radius;

			for( int i(0); i < m_iPointNumber; i++ )
			{
				float dist2 = math::Calc_DistanceSquared(_Position, m_Points[i].m_Position);
				if( dist2 < fSquaredRadius )
				{
					m_Points[i].applyForce(10.0f * _Force * (_Position - m_Points[i].m_Position) / (100.0f + dist2));
					m_Points[i].increaseDamping(0.6f);
				}
			}
		}

		void Grid::applyExplosiveForce(const float& _Force, const sf::Vector3f& _Position, const float& _Radius)
		{
			float fSquaredRadius = _Radius * _Radius;

			for( int i(0); i < m_iPointNumber; i++ )
			{
				float dist2 = math::Calc_DistanceSquared(_Position, m_Points[i].m_Position);
				if( dist2 < fSquaredRadius )
				{
					m_Points[i].applyForce(100.0f * _Force * (m_Points[i].m_Position - _Position) / (10000.0f + dist2));
					m_Points[i].increaseDamping(0.6f);
				}
			}
		}


		sf::Vector2f Grid::toVec2(const sf::Vector3f& _Vector)
		{
			float factor = (_Vector.z + 2000.0f) * 0.0005f;

			return (sf::Vector2f(_Vector.x, _Vector.y) - m_ScreenSize * 0.5f) * factor + m_ScreenSize * 0.5f;
		}


		void Grid::setColor(sf::Color _Color)
		{
			m_Color[0] = _Color;
		}

		void Grid::setMidLinesColor(sf::Color _Color)
		{
			m_Color[1] = _Color;
		}

		void Grid::setBorderLinesColor(sf::Color _Color)
		{
			m_Color[2] = _Color;
		}


		void Grid::setDrawMidLines(bool _Boolean)
		{
			m_bDrawMidLines = _Boolean;
		}

		void Grid::setApplyBloom(bool _Boolean, float _EffectTextureDownScale, float _EffectLevel, float _SaturateLevel, sf::Vector2f _BlurOffset)
		{
			if( m_BloomEffect != NULL )
			{
				delete m_BloomEffect;
				m_BloomEffect = NULL;
			}

			if( _Boolean )
				m_BloomEffect = new Bloom(m_RenderTexture->getSize(), _EffectTextureDownScale, _EffectLevel, _SaturateLevel, _BlurOffset);
		}
}