/*--------------------------------------------------------------------------------------------------

@author		GMK
@date		13/11/2015
@brief		Prise en charge des maps exportees par Tiled

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_TILEDMAP_HPP
#define SFGML_TILEDMAP_HPP

namespace sfgmk
{
	struct TiledLayer
	{
		std::string name;
		int width, height, alpha;
		sf::RenderTexture texture;
		sfgmk::Sprite sprite;
	};

	class SFGMK_API TiledMap
	{
		public:

			bool loadFromFile(std::string& _filePath);

			void draw(sf::RenderTexture* _renderTexture);

		protected:

			int m_width, m_height;
			int m_tileWidth, m_tileHeight;

			int m_firstGid;
			std::string m_name;
			int m_spacing, m_margin, m_tileCount;

			std::string m_source;
			int m_sourceWidth, m_sourceHeight;

			sf::Texture *m_texture;

			sfgmk::Sprite *m_tiles;

			DynamicArray<TiledLayer*> m_layers;
	};
}

#endif