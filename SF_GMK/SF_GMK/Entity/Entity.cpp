namespace sfgmk
{
	Entity::Entity(const int& _Id) : m_iId(_Id), m_bIsAlive(true), m_uiSingleId(0U), m_bIsComputatedByParallax(true), m_bIsScaledByParallax(true), m_fPositionZ(0.0f), m_Sprite(NULL), m_Collider(NULL), m_AI(NULL)
	{
	}

	Entity::Entity(const sf::Vector3f& _Position, const int& _Id) : Entity(_Id)
	{
		setPosition(_Position);
	}

	Entity::~Entity()
	{
		ClearPtrCntr(m_Transformation);

		SAFE_DELETE(m_Sprite);

		SAFE_DELETE(m_Collider);

		SAFE_DELETE(m_AI);
	}


	void Entity::update(const float& _TimeDelta)
	{
		if (m_AI)
			m_AI->process(_TimeDelta);

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

		if( m_Sprite )
		{
			m_Sprite->finalize(_TimeDelta);

			m_Sprite->setOrigin(m_VirtualTransform.getOrigin());
			m_Sprite->setScale(m_VirtualTransform.getScale());
			m_Sprite->setRotation(m_VirtualTransform.getRotation());
			m_Sprite->setPosition(m_VirtualTransform.getPosition());
		}
	}

	void Entity::draw(sf::RenderTexture* _Render)
	{
		_Render->draw(*m_Sprite);
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

	const unsigned int& Entity::getSingleId()
	{
		return m_uiSingleId;
	}

	void Entity::setSingleId(const unsigned int& _Id)
	{
		m_uiSingleId = _Id;
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
		if( !m_Sprite )
			m_Sprite = new Sprite();

		return m_Sprite;
	}

	const sf::Vector2f& Entity::getRelativOrigin()
	{
		return m_RelativOrigin;
	}

	void Entity::setRelativOrigin(const float& _X, const float& _Y)
	{
		m_RelativOrigin.x = _X;
		m_RelativOrigin.y = _Y;

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
		sf::Vector2f Origin = getOrigin();
		Origin.x *= getScale().x;
		Origin.y *= getScale().y;

		sf::Vector2f Position = getPosition();
		sf::Vector2f Size = m_Sprite->getSize();

		return sf::Vector2f(Position.x - Origin.x + Size.x / 2.0f, Position.y - Origin.y + Size.y / 2.0f);
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

	void Entity::setVirtualTransformWithTransform()
	{
		m_VirtualTransform.setOrigin(getOrigin());
		m_VirtualTransform.setScale(getScale());
		m_VirtualTransform.setRotation(getRotation());
		m_VirtualTransform.setPosition(getPosition());
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

	void Entity::onPhysicEnter()
	{

	}

	void Entity::onPhysicCollision(Entity* _entity)
	{

	}

	void Entity::onPhysicExit()
	{

	}

	engine::AIStateMachine* Entity::getAiFsm()
	{
		return m_AI;
	}

	void Entity::addAiFsm(int _InitState)
	{
		m_AI = new engine::AIStateMachine(this, _InitState);
	}

	bool Entity::addAiState(int _StateId, FoncterTemplate* _NewFunction, const std::string& _StateName)
	{
		if( !m_AI )
			return false;

		return m_AI->addState(_StateId, _NewFunction, _StateName);
	}


	sfgmk::engine::MsgActor& Entity::getMsgActor()
	{
		return m_MsgActor;
	}
}