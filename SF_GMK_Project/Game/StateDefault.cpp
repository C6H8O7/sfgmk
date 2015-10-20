#include "stdafx.h"

#include "IncludesProjet.hpp"


StateDefault::StateDefault()
{
	sfgmk::DATA_MANAGER->loadLevel(m_sRessourcesPath);
	sfgmk::PARALLAXE.loadLevel(m_sRessourcesPath);
}

StateDefault::~StateDefault()
{
	sfgmk::DATA_MANAGER->unloadLevel(m_sRessourcesPath);
	sfgmk::PARALLAXE.unloadLevel(m_sRessourcesPath);
}

void StateDefault::init()
{
	goomba.setTexture(sfgmk::DATA_MANAGER->getTexture("goomba1"));

	//Test fog
	/*m_ImgFog.create(1280, 720);
	m_ImgSize = m_ImgFog.getSize();

	m_ImgFog2.create(1280, 720);
	m_ImgSize2 = m_ImgFog2.getSize();

	m_ImgFog3.loadFromFile("../data/states/stateDefault/layer/l4d.png");
	m_ImgSize3 = m_ImgFog3.getSize();*/
}

void StateDefault::update()
{
	//Goomba
	/*float fMove(sfgmk::TIME_DELTA * 25.0f);
	goomba.move(sf::Vector2f(fMove, fMove));

	//Fog
	sf::Vector2u MousePos = (sf::Vector2u)sfgmk::MOUSE_WINDOW_POS;
	unsigned int uiDistance(0U);
	static float fDistanceMax(10000.0f);
	sf::Color Color(0, 0, 0, 0);
	sf::Color Color2(0, 0, 0, 0);

	for( unsigned int i(0U); i < m_ImgSize.x; i++ )
	{
		for( unsigned int j(0U); j < m_ImgSize.y; j++ )
		{
			if( (uiDistance = sfgmk::math::Calc_DistanceSquared(i, j, MousePos.x, MousePos.y)) < fDistanceMax )
			{
				Color.a = MIN(m_ImgFog.getPixel(i, j).a, 255 * sf::Uint8((uiDistance / fDistanceMax)));
				m_ImgFog.setPixel(i, j, Color);
				Color.a = MIN(m_ImgFog2.getPixel(i, j).a, 255 * sf::Uint8((uiDistance / fDistanceMax)));
				m_ImgFog2.setPixel(i, j, Color);

				Color2 = m_ImgFog3.getPixel(i, j);
				Color2.a = 0;
				m_ImgFog3.setPixel(i, j, Color2);
			}
			else
			{
				Color.a = MAX(m_ImgFog2.getPixel(i, j).a, 225);
				m_ImgFog2.setPixel(i, j, Color);

				Color2 = m_ImgFog3.getPixel(i, j);
				Color2.a = 255;
				m_ImgFog3.setPixel(i, j, Color2);
			}
		}
	}

	m_TexFog.loadFromImage(m_ImgFog);
	m_TexFog2.loadFromImage(m_ImgFog2);
	m_TexFog3.loadFromImage(m_ImgFog3);*/
}

void StateDefault::deinit()
{
}

void StateDefault::draw()
{
	sfgmk::engine::GraphicManager::getSingleton()->getRenderTexture()->draw(goomba);

	/*m_SpFog.setTexture(m_TexFog3);
	sfgmk::GRAPHIC_MANAGER->getRenderTexture()->draw(m_SpFog);
	m_SpFog.setTexture(m_TexFog2);
	sfgmk::GRAPHIC_MANAGER->getRenderTexture()->draw(m_SpFog);
	m_SpFog.setTexture(m_TexFog);
	sfgmk::GRAPHIC_MANAGER->getRenderTexture()->draw(m_SpFog);*/
}