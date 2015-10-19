/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		17/10/2015
	@brief		Coeur du moteur, accés aux managers

--------------------------------------------------------------------------------------------------*/

#ifndef CORE_HPP
#define CORE_HPP


namespace sfgmk
{
	namespace engine
	{
		class Core : public SingletonTemplate<Core>
		{
			friend class SingletonTemplate<Core>;

			private:

				Core();
				~Core();

				sf::Clock m_ClockTimeDelta;
				float m_fTimeDelta;
				
			public:

				void update();
				void preLoop();
				void loop();
				void postLoop();

				inline const float& getTimeDelta() { return m_fTimeDelta; }
		};
	}
}


#endif