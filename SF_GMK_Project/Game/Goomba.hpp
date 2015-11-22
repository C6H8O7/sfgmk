#pragma once

using namespace sfgmk;

class Goomba : public EntityWithPv
{
	private:
		enum eGoombaAiState
		{
			eIdle = 0,
			eMove,
			eGoombaAiState_NUMBER
		};

		float m_fTimer;

		float m_fSpeed;
		float m_fDistanceTolerance;
		sf::Vector2f* m_Destination;

	public:
		Goomba() : m_fTimer(0.0f), m_fSpeed(100.0f), m_Destination(NULL), m_fDistanceTolerance(10.0f)
		{
			setPosition(sf::Vector3f((float)RAND(100, 1180), (float)RAND(100, 620), 0.0f));
			getSprite()->setAnimation(DATA_MANAGER->getAnimation("goomba"));

			setInitialPv(RAND(10, 100));
			setPv(getInitialPv());
			addLifeBar(true);

			addObbCollider();

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
			SAFE_DELETE(m_Destination);
		}


		void GoombaIdle(const int& _Progress)
		{
			m_fTimer += TIME_DELTA;

			updateMsg();

			OnEnter
			{
				getSprite()->Stop();
				if( m_fTimer > 0.5f )
				{
					m_fTimer = 0.0f;
					m_AI->progress();
				}
			}

			OnUpdate
			{
				if( m_Destination )
				{
					m_fTimer = 0.0f;
					m_AI->progress();
				}
			}

			OnExit
			{
				if( m_Destination && m_fTimer > 0.5f )
				{
					m_fTimer = 0.0f;
					m_AI->changeState(eMove);
				}
			}	
		}

		void GoombaMove(const int& _Progress)
		{
			m_fTimer += TIME_DELTA;

			updateMsg();
	
			OnEnter
			{
				getSprite()->Play();
				if( m_fTimer > 0.5f )
				{
					m_fTimer = 0.0f;
					m_AI->progress();
				}
			}

			OnUpdate
			{
				sf::Vector2f Direction = *m_Destination - getCenter();

				if( math::Calc_Norm(Direction) < m_fDistanceTolerance || !m_Destination )
				{
					m_fTimer = 0.0f;
					SAFE_DELETE(m_Destination);
					m_AI->progress();
				}
				else
				{
					sf::Vector2f UnitDirection = math::Calc_UnitVector(Direction);

					move(sf::Vector2f(UnitDirection.x * m_fSpeed * TIME_DELTA, 0.0f));
					move(sf::Vector2f(0.0f, UnitDirection.y * m_fSpeed * TIME_DELTA));
				}
			}

			OnExit
			{
				if( m_fTimer > 0.5f )
				{
					m_fTimer = 0.0f;
					m_AI->setNextState(eIdle);
					m_AI->progress();
				}
			}
		}


		void setDestination(const sf::Vector2f& _Destination)
		{
			SAFE_DELETE(m_Destination);

			m_Destination = new sf::Vector2f(_Destination);
		}

		void updateMsg()
		{
			engine::Msg* NewMessagePtr = m_MsgActor.GetLastMessage();
			engine::Msg* NewMessage = new engine::Msg;
			while( NewMessagePtr != NULL )
			{
				memcpy(NewMessage, NewMessagePtr, sizeof(engine::Msg));
				void* data = NewMessage->GetData();

				//Déplacement
				if( NewMessage->GetTag() == engine::MSG_TAG::TAG_FLOAT )
				{
					float x = *(float*)((unsigned int)data);
					float y = *(float*)((unsigned int)data + 4);

					setDestination(sf::Vector2f(x, y));
				}

				//Kill
				if( NewMessage->GetTag() == engine::MSG_TAG::TAG_BOOL )
					setIsAlive(false);

				NewMessagePtr = m_MsgActor.GetLastMessage();
				
			}
			//delete NewMessage;
		}
};