Shot::Shot(const int& _Id, const sf::Vector3f& _Direction, const float& _Speed, const int& _ShooterId, sfgmk::Grid* _GridPtr)
	: Entity(_Id), m_Direction(_Direction), m_fSpeed(_Speed), m_iShooter(_ShooterId), m_GridPtr(_GridPtr)
{
	m_Sprite = new sfgmk::Sprite;

	addTransformation(new sfgmk::TransformationTranslation(m_Direction, m_fSpeed, false));
}

Shot::~Shot()
{
}


void Shot::update(const float& _TimeDelta)
{
	//Update de la classe mère
	Entity::update(_TimeDelta);

	//Déformations de la grille
	m_GridPtr->applyExplosiveForce(300.0f, getPosition3D(), 15.0f);
}