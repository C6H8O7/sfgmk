#ifndef STATE_DEFAULT_HPP
#define STATE_DEFAULT_HPP

class StateDefault : public sfgmk::engine::State
{
	private:
		sfgmk::Sprite goomba;

		/*sf::Texture m_TexFog;
		sf::Texture m_TexFog2;
		sf::Texture m_TexFog3;
		sf::Image m_ImgFog;
		sf::Image m_ImgFog2;
		sf::Image m_ImgFog3;
		sf::Sprite m_SpFog;
		sf::Vector2u m_ImgSize;
		sf::Vector2u m_ImgSize2;
		sf::Vector2u m_ImgSize3;*/

	public:
		StateDefault();
		~StateDefault();

		void init();
		void update();
		void deinit();

		void draw();
};

#endif