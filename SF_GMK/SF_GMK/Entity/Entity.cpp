namespace sfgmk
{
	Entity::Entity(const int& _Id) : m_iId(_Id), m_bIsAlive(true), m_bIsComputatedByParallax(true), m_bIsScaledByParallax(true), m_fPositionZ(0.0f), m_Sprite(NULL), m_Collider(NULL)
	{
	}

	Entity::Entity(const sf::Vector3f& _Position, const int& _Id) : Entity(_Id)
	{
		setPosition(_Position);
	}

	Entity::~Entity()
	{
		ClearPtrCntr(m_Transformation);

		if( m_Sprite )
			delete m_Sprite;

		if( m_Collider )
			delete m_Collider;
	}


	void Entity::update(const float& _TimeDelta)
	{
		if( m_Collider )
			m_Collider->setCollide(false);

		finalize(_TimeDelta);
	}

	void Entity::finalize(const float& _TimeDelta)
	{
		for( int i(m_Transformation.size() - 1); i >= 0; i-- )
		{
			m_Transformation[i]->update(_TimeDelta, this);

			if( m_Transformation[i]->checkTimer(_TimeDelta, this) == false )
			{
				EntityTransformation* Temp = m_Transformation[i];
				m_Transformation[i] = m_Transformation[m_Transformation.size() - 1];
				m_Transformation[m_Transformation.size() - 1] = Temp;
				delete Temp;
				m_Transformation.pop_back();
			}
		}
	}

	void Entity::draw(sf::RenderTexture* _Render)
	{
		m_Sprite->setRelativeOrigin(m_VirtualTransform.getOrigin().x, m_VirtualTransform.getOrigin().y);
		m_Sprite->setScale(m_VirtualTransform.getScale());
		m_Sprite->setRotation(m_VirtualTransform.getRotation());
		m_Sprite->setPosition(m_VirtualTransform.getPosition());
		_Render->draw(*m_Sprite);

		//m_bIsComputatedByParallax ? _Render->draw(*m_Sprite, m_VirtualTransform.getTransform()) : _Render->draw(*m_Sprite, getTransform());
	}


	const int& Entity::getId()
	{
		return m_iId;
	}

	void Entity::setId(int _Id)
	{
		m_iId = _Id;
	}

	const bool& Entity::getIsAlive()
	{
		return m_bIsAlive;
	}

	void Entity::setIsAlive(bool _State)
	{
		m_bIsAlive = _State;
	}


	const bool& Entity::getIsComputatedByParralax()
	{
		return m_bIsComputatedByParallax;
	}

	void Entity::setIsComputatedByParralax(bool _Boolean)
	{
		m_bIsComputatedByParallax = _Boolean;
	}

	const bool& Entity::getIsScaledByParralax()
	{
		return m_bIsScaledByParallax;
	}

	void Entity::setIsScaledByParralax(bool _Boolean)
	{
		m_bIsScaledByParallax = _Boolean;
	}


	Sprite* Entity::getSprite()
	{
		if( m_Sprite == NULL )
			m_Sprite = new Sprite;

		return m_Sprite;
	}

	void Entity::setRelativOrigin(const float& _X, const float& _Y)
	{
		if( m_Sprite )
		{
			sf::FloatRect Rec = m_Sprite->getLocalBounds();
			setOrigin(Rec.width * _X, Rec.height * _Y);
		}
	}

	void Entity::addTransformation(EntityTransformation* _Transformation)
	{
		m_Transformation.push_back(_Transformation);
	}

	void Entity::removeTransformation(int _Index)
	{
		int iSize = m_Transformation.size() - 1;

		if( _Index == iSize )
		{
			delete m_Transformation[iSize];
			m_Transformation.pop_back();
		}
		else
		{
			EntityTransformation* Temp = m_Transformation[_Index];
			m_Transformation[_Index] = m_Transformation[iSize];
			m_Transformation[iSize] = Temp;
			delete Temp;
			m_Transformation.pop_back();
		}
	}

	std::vector<EntityTransformation*>& Entity::getTransformation()
	{
		return m_Transformation;
	}


	const float& Entity::getZ()
	{
		return m_fPositionZ;
	}

	sf::Vector2f Entity::getCenter()
	{
		sf::Vector2f Size = m_Sprite->getSize();
		sf::Vector2f Origin = getOrigin();
		sf::Vector2f Position = getPosition();

		return sf::Vector2f(Position.x + ((0.5f - Origin.x) * Size.x), Position.y + ((0.5f - Origin.y) * Size.y));
	}

	sf::Vector3f Entity::getPosition3D()
	{
		return sf::Vector3f(getPosition().x, getPosition().y, m_fPositionZ);
	}

	void Entity::setPosition(const sf::Vector2f& position)
	{
		sf::Transformable::setPosition(position);
	}

	void Entity::setPosition(const sf::Vector3f& _Position)
	{
		sf::Transformable::setPosition(_Position.x, _Position.y);
		m_fPositionZ = _Position.z;
	}

	void Entity::move(const sf::Vector2f& _Vector)
	{
		sf::Transformable::move(_Vector);
	}

	void Entity::move(const sf::Vector3f& _Vector)
	{
		sf::Transformable::move(_Vector.x, _Vector.y);
		m_fPositionZ = _Vector.z;
	}


	sf::Transformable& Entity::getVirtualTransform()
	{
		return m_VirtualTransform;
	}


	Collider* Entity::getCollider()
	{
		return m_Collider;
	}

	void Entity::addSphereCollider()
	{
		float fRadius = MAX(m_Sprite->getTextureRect().width, m_Sprite->getTextureRect().height) * 0.5f;
		addSphereCollider(fRadius);
	}

	void Entity::addSphereCollider(const float& _Radius)
	{
		if( m_Collider )
			delete m_Collider;

		m_Collider = new SphereCollider(this, _Radius);
	}

	void Entity::addObbCollider()
	{
		sf::FloatRect RectSize = (sf::FloatRect)m_Sprite->getTextureRect();
		addObbCollider(sf::Vector2f(RectSize.width, RectSize.height));
	}

	void Entity::addObbCollider(const sf::Vector2f& _Size)
	{
		if( m_Collider )
			delete m_Collider;

		m_Collider = new ObbCollider(this, _Size);
	}


	void Entity::physicCallBack(const int& _ColliderId)
	{
	}
}