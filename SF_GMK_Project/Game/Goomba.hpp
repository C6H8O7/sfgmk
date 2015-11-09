#pragma once


using namespace sfgmk;

class Goomba : public Entity
{
	private:
		enum eGoombaAiState
		{
			eIdle = 0,
			eMove,
			eGoombaAiState_NUMBER
		};

		float m_fTimer;

	public:
		Goomba() : m_fTimer(0.0f)
		{
			setScale(2.0f, 2.0f);
			setPosition(sf::Vector3f(250.0f, 250.0f, 0.0f));
			getSprite()->setAnimation(DATA_MANAGER->getAnimation("goomba"));
			getSprite()->setColor(sf::Color::Red);

			//AI (attention si on utilise les foncterTemplate, le prototype correspondant doit figuré dans la classe "FoncterTemplate")
			FoncterTemplateInstance<Goomba, void, const int&>* PtrIdle = new sfgmk::FoncterTemplateInstance<Goomba, void, const int&>(this, &Goomba::GoombaIdle);
			FoncterTemplateInstance<Goomba, void, const int&>* PtrMove = new sfgmk::FoncterTemplateInstance<Goomba, void, const int&>(this, &Goomba::GoombaMove);

			addAiFsm(eIdle);
			//Seulement 5 premiers caractères pris en compte pour la string du state
			addAiState(eIdle, PtrIdle, "Idle");
			addAiState(eMove, PtrMove, "Move");
		}

		~Goomba()
		{
		}


		void GoombaIdle(const int& _Progress)
		{
			m_fTimer += TIME_DELTA;

			OnEnter
			{
				if( m_fTimer > 2.5f )
				{
					m_fTimer = 0.0f;
					m_AI->progress();
				}
			}

			OnUpdate
			{
				if( m_fTimer > 2.5f )
				{
					m_fTimer = 0.0f;
					m_AI->progress();
				}
			}

			OnExit
			{
				if( m_fTimer > 2.5f )
				{
					m_fTimer = 0.0f;
					m_AI->changeState(eMove);
				}
			}	
		}

		void GoombaMove(const int& _Progress)
		{
			m_fTimer += TIME_DELTA;
	
			OnEnter
			{
				if( m_fTimer > 2.5f )
				{
					m_fTimer = 0.0f;
					m_AI->progress();
				}
			}

			OnUpdate
			{
				move(sf::Vector2f(100.0f * TIME_DELTA, 0.0f));

				if( m_fTimer > 2.5f )
				{
					m_fTimer = 0.0f;
					m_AI->progress();
				}
			}

			OnExit
			{
				if( m_fTimer > 2.5f )
				{
					m_fTimer = 0.0f;
					m_AI->setNextState(eIdle);
					m_AI->progress();
				}
			}
		}
};