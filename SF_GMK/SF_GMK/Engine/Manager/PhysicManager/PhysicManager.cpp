namespace sfgmk
{
	namespace engine
	{
		PhysicManager::PhysicManager() : m_bDrawCollider(false)
		{

		}

		PhysicManager::~PhysicManager()
		{
			m_PhysicObjects.clear();
		}

		
		const bool& PhysicManager::getDraw()
		{
			return m_bDrawCollider;
		}

		void PhysicManager::setDraw()
		{
			m_bDrawCollider = !m_bDrawCollider;
		}

		void PhysicManager::update()
		{
			for (unsigned int i(0U); i < m_PhysicObjects.getElementNumber(); i++)
				if (m_PhysicObjects[i]->isActive())
					m_PhysicObjects[i]->getEntity()->onPhysicEnter();

			for( unsigned int i(0U); i < m_PhysicObjects.getElementNumber(); i++ )
			{
				if( m_PhysicObjects[i]->isActive() )
				{
					Entity *i_entity = m_PhysicObjects[i]->getEntity();

					for( unsigned int j(i + 1U); j < m_PhysicObjects.getElementNumber(); j++ )
					{
						if( m_PhysicObjects[j]->isActive() )
						{
							Entity *j_entity = m_PhysicObjects[j]->getEntity();

							if( testCollision(m_PhysicObjects[i], m_PhysicObjects[j]) )
							{
								m_PhysicObjects[i]->setCollide(true);
								m_PhysicObjects[j]->setCollide(true);

								//Callbacks
								i_entity->onPhysicCollision(j_entity);
								j_entity->onPhysicCollision(i_entity);
							}
						}
					}

					i_entity->onPhysicExit();
				}
			}
		}

		void PhysicManager::draw(sf::RenderTexture* _RenderTexture)
		{
			if( m_bDrawCollider )
			{
				for( unsigned int i(0U); i < m_PhysicObjects.getElementNumber(); i++ )
					drawCollider(m_PhysicObjects[i], _RenderTexture);
			}
		}

		void PhysicManager::drawCollider(Collider* _Collider, sf::RenderTexture* _RenderTexture)
		{
			sfgmk::Entity* Entity = _Collider->getEntity();
			bool bIsColliding = _Collider->Collide();
			sf::CircleShape CircleShape;
			sf::RectangleShape RectShape;
			const sf::Transform* Transform = &Entity->getVirtualTransform().getTransform();
			float fSphereRadius;

			if( _Collider->isActive() )
			{
				switch( _Collider->getType() )
				{
					case eCOLLIDER_TYPE::eSphere:
						fSphereRadius = ((SphereCollider*)_Collider)->getRadius();
						CircleShape.setRadius(fSphereRadius);
						CircleShape.setOutlineThickness(1);

						if( bIsColliding )
						{
							CircleShape.setFillColor(sf::Color(255, 0, 100, 75));
							CircleShape.setOutlineColor(sf::Color(255, 0, 150, 150));
						}
						else
						{
							CircleShape.setFillColor(sf::Color(0, 220, 150, 75));
							CircleShape.setOutlineColor(sf::Color(0, 220, 200, 150));
						}

						_RenderTexture->draw(CircleShape, *Transform);
						break;

					case eCOLLIDER_TYPE::eOBB:
						RectShape.setSize(((ObbCollider*)_Collider)->getSize());
						RectShape.setOutlineThickness(1.0f / Entity->getScale().x);

						if( bIsColliding )
						{
							RectShape.setFillColor(sf::Color(255, 0, 100, 75));
							RectShape.setOutlineColor(sf::Color(255, 0, 150, 150));
						}
						else
						{
							RectShape.setFillColor(sf::Color(0, 220, 150, 75));
							RectShape.setOutlineColor(sf::Color(0, 220, 200, 150));
						}

						_RenderTexture->draw(RectShape, *Transform);
						break;

					default:
						break;
				}
			}
		}

		void PhysicManager::addCollider(Collider* _NewCollider)
		{
			m_PhysicObjects.pushBack(_NewCollider);
		}

		void PhysicManager::removeCollider(Collider* _ColliderToRemove)
		{
			m_PhysicObjects.removeElement(_ColliderToRemove);
		}

		bool PhysicManager::testCollision(Collider* _Collider1, Collider* _Collider2)
		{
			bool bCollision(false);
			eCOLLIDER_TYPE Type1 = _Collider1->getType(), Type2 = _Collider2->getType();
			sfgmk::Entity *Entity1 = _Collider1->getEntity(), *Entity2 = _Collider2->getEntity();
			float fCollider1Z = Entity1->getZ(), fCollider2Z = Entity2->getZ();

			//Test profondeur
			if( fCollider1Z != fCollider2Z )
			{
				if( !(fCollider1Z > -2 && fCollider1Z < 2) )
					return false;
				if( !(fCollider2Z > -2 && fCollider2Z < 2) )
					return false;
			}

			//Si 2 shapes de même type
			if( Type1 == Type2 )
			{
				if( Type1 == eCOLLIDER_TYPE::eSphere )
					bCollision = collisionSphereSphere((SphereCollider*)_Collider1, (SphereCollider*)_Collider2, Entity1, Entity2);
				else if( Type1 == eCOLLIDER_TYPE::eOBB )
					bCollision = collisionObbObb((ObbCollider*)_Collider1, (ObbCollider*)_Collider2, Entity1, Entity2);
			}

			//Si shapes de types différents
			else
			{
				bool bInverse = Type1 > Type2;

				if( (Type1 + Type2) == eSphere + eOBB )
					bInverse ? bCollision = collisionSphereObb((SphereCollider*)_Collider2, (ObbCollider*)_Collider1, Entity1, Entity2)
							 : bCollision = collisionSphereObb((SphereCollider*)_Collider1, (ObbCollider*)_Collider2, Entity1, Entity2);
			}

			return bCollision;
		}

		bool PhysicManager::OverlapBoxes(sf::Vector2f _Box1Center, sf::Vector2f _Box1HalfSize, sf::Vector2f _Box1Ortho[3], sf::Vector2f _Box2Center, sf::Vector2f _Box2HalfSize, sf::Vector2f _Box2Ortho[3])
		{
			//Calcul de T (vecteur entre les centres)
			sf::Vector2f T = _Box2Center - _Box1Center;

			//Extends
			float fRa(0.0f);
			float fRb(0.0f);

			//Tests axes Box1
			for( int i(0); i < 2; i++ )
			{
				fRa = _Box1HalfSize.x * ABS(math::DotProduct(_Box1Ortho[0], _Box1Ortho[i]))
					+ _Box1HalfSize.y * ABS(math::DotProduct(_Box1Ortho[1], _Box1Ortho[i]));

				fRb = _Box2HalfSize.x * ABS(math::DotProduct(_Box2Ortho[0], _Box1Ortho[i]))
					+ _Box2HalfSize.y * ABS(math::DotProduct(_Box2Ortho[1], _Box1Ortho[i]));

				if( ABS(math::DotProduct(T, _Box1Ortho[i])) > fRa + fRb )
					return false;
			}

			//Tests axes Box2
			for( int i(0); i < 2; i++ )
			{
				fRa = _Box1HalfSize.x * ABS(math::DotProduct(_Box1Ortho[0], _Box2Ortho[i]))
					+ _Box1HalfSize.y * ABS(math::DotProduct(_Box1Ortho[1], _Box2Ortho[i]));

				fRb = _Box2HalfSize.x * ABS(math::DotProduct(_Box2Ortho[0], _Box2Ortho[i]))
					+ _Box2HalfSize.y * ABS(math::DotProduct(_Box2Ortho[1], _Box2Ortho[i]));

				if( ABS(math::DotProduct(T, _Box2Ortho[i])) > fRa + fRb )
					return false;
			}

			//Tests combinaisons d'axes
			sf::Vector2f L;

			for( int i(0); i < 2; i++ )
			{
				for( int j(0); j < 2; j++ )
				{
					//Construction de l'axe 
					L = math::CrossProduct(_Box1Ortho[i], _Box2Ortho[j]);

					//Test
					fRa = _Box1HalfSize.x * ABS(math::DotProduct(_Box1Ortho[0], L))
						+ _Box1HalfSize.y * ABS(math::DotProduct(_Box1Ortho[1], L));

					fRb = _Box2HalfSize.x * ABS(math::DotProduct(_Box2Ortho[0], L))
						+ _Box2HalfSize.y * ABS(math::DotProduct(_Box2Ortho[1], L));

					if( ABS(math::DotProduct(T, L)) > fRa + fRb )
						return false;
				}
			}

			return true;
		}

		bool PhysicManager::collisionSphereSphere(SphereCollider* _Collider1, SphereCollider* _Collider2, sfgmk::Entity* _Entity1, sfgmk::Entity* _Entity2)
		{
			float fDistanceSquared = math::Calc_DistanceSquared(_Collider1->getWorldCenter(), _Collider2->getWorldCenter());
			float fRadiusSquared = _Collider1->getWorldRadius() + _Collider2->getWorldRadius();
			fRadiusSquared *= fRadiusSquared;

			return fDistanceSquared < fRadiusSquared;
		}

		bool PhysicManager::collisionObbObb(ObbCollider* _Collider1, ObbCollider* _Collider2, sfgmk::Entity* _Entity1, sfgmk::Entity* _Entity2)
		{
			if (math::Calc_DistanceSquared(_Collider1->getWorldCenter(), _Collider2->getWorldCenter()) > (_Collider1->getSquaredWorldRadius() + _Collider2->getSquaredWorldRadius()))
				return false;

			//Données boite 1
			sf::Vector2f Box1Center = _Collider1->getWorldCenter();
			sf::Vector2f Box1HalfSize = _Collider1->getWorldSize() * 0.5f;
			sf::Vector2f Box1Ortho[2];
			sf::Transform Box1Transform = _Collider1->getEntity()->getTransform();
			Box1Ortho[0] = math::Calc_UnitVector(_Collider1->getRight());
			Box1Ortho[1] = math::Calc_UnitVector(_Collider1->getUp());

			//Données boite 2
			sf::Vector2f Box2Center = _Collider2->getWorldCenter();
			sf::Vector2f Box2HalfSize = _Collider2->getWorldSize() * 0.5f;
			sf::Vector2f Box2Ortho[2];
			sf::Transform Box2Transform = _Collider2->getEntity()->getTransform();
			Box2Ortho[0] = math::Calc_UnitVector(_Collider2->getRight());
			Box2Ortho[1] = math::Calc_UnitVector(_Collider2->getUp());

			//Test collision
			return OverlapBoxes(Box1Center, Box1HalfSize, Box1Ortho, Box2Center, Box2HalfSize, Box2Ortho);
		}

		bool PhysicManager::collisionSphereObb(SphereCollider* _SphereCollider, ObbCollider* _BoxCollider, sfgmk::Entity* _Entity1, sfgmk::Entity* _Entity2)
		{
			if (math::Calc_DistanceSquared(_SphereCollider->getWorldCenter(), _BoxCollider->getWorldCenter()) > (_SphereCollider->getSquaredWorldRadius() + _BoxCollider->getSquaredWorldRadius()))
				return false;

			float fSphereRadius(_SphereCollider->getWorldRadius()), fDifference(0.0f), fDistance(0.0);
			sf::Vector2f Scale = _BoxCollider->getEntity()->getScale();
			sf::Vector2f MinVector(_BoxCollider->getMin()), MaxVector(_BoxCollider->getMax());
			sf::Vector2f NewSphereCenter;
			const float* MatrixOBBInverse = _BoxCollider->getEntity()->getInverseTransform().getMatrix();

			//Transform sphere center from world coordinates to OBB coordinates
			NewSphereCenter = _BoxCollider->getEntity()->getInverseTransform().transformPoint(_SphereCollider->getWorldCenter());

			//X
			if( NewSphereCenter.x < MinVector.x )
			{
				fDifference = (NewSphereCenter.x - MinVector.x) * Scale.x;
				fDistance += fDifference * fDifference;
			}
			else if( NewSphereCenter.x > MaxVector.x )
			{
				fDifference = (NewSphereCenter.x - MaxVector.x) * Scale.x;
				fDistance += fDifference * fDifference;
			}

			//Y
			if( NewSphereCenter.y < MinVector.y )
			{
				fDifference = (NewSphereCenter.y - MinVector.y) * Scale.y;
				fDistance += fDifference * fDifference;
			}
			else if( NewSphereCenter.y > MaxVector.y )
			{
				fDifference = (NewSphereCenter.y - MaxVector.y) * Scale.y;
				fDistance += fDifference * fDifference;
			}

			return fDistance <= fSphereRadius * fSphereRadius;
		}
	}
}