namespace sfgmk
{
	TiledMap::TiledMap() : m_texture(0), m_tiles(0)
	{

	}

	TiledMap::~TiledMap()
	{
		delete [] m_tiles;

		m_layers.deleteAndClear();
	}

	bool TiledMap::loadFromFile(std::string& _filePath)
	{
		sfgmk::engine::DataManager *dataman = DATA_MANAGER;

		tinyxml2::XMLDocument mapFile;
		mapFile.LoadFile(_filePath.c_str());

		tinyxml2::XMLElement *map = mapFile.FirstChildElement("map");

		m_width = map->IntAttribute("width");
		m_height = map->IntAttribute("height");
		m_tileWidth = map->IntAttribute("tilewidth");
		m_tileHeight = map->IntAttribute("tileheight");

		tinyxml2::XMLElement *tileset = map->FirstChildElement("tileset");

		m_firstGid = tileset->IntAttribute("firstgid");
		m_name = tileset->Attribute("name");
		m_spacing = tileset->IntAttribute("spacing");
		m_margin = tileset->IntAttribute("margin");
		m_tileCount = tileset->IntAttribute("tilecount");

		tinyxml2::XMLElement *image = tileset->FirstChildElement("image");

		m_source = image->Attribute("source");
		m_sourceWidth = image->IntAttribute("width");
		m_sourceHeight = image->IntAttribute("height");

		std::string resName = m_source.substr(0, m_source.find_first_of("."));
		m_texture = &dataman->getTexture(resName);
		
		m_tiles = new sfgmk::Sprite[m_tileCount];

		int nW = (m_sourceWidth + m_spacing - 2 * m_margin) / m_tileWidth;
		int nH = (m_sourceHeight + m_spacing - 2 * m_margin) / m_tileHeight;

		sf::IntRect rect;
		rect.width = m_tileWidth;
		rect.height = m_tileHeight;

		int k = 0;
		for (int j = 0; j < nH; j++)
		{
			for (int i = 0; i < nW; i++)
			{
				rect.left = m_margin + i * (m_tileWidth + m_spacing);
				rect.top = m_margin + j * (m_tileHeight + m_spacing);

				m_tiles[k].setTexture(*m_texture);
				m_tiles[k].setTextureRect(rect);

				k++;
			}
		}

		tinyxml2::XMLElement *layerElement = map->FirstChildElement("layer");
		while(layerElement)
		{
			TiledLayer *tiledLayer = new TiledLayer;

			tiledLayer->name = layerElement->Attribute("name");
			tiledLayer->width = layerElement->IntAttribute("width");
			tiledLayer->height = layerElement->IntAttribute("height");
			
			float opacity = layerElement->FloatAttribute("opacity");
			
			if (opacity == 0.0f)
				opacity = 1.0f;

			tiledLayer->alpha = (int)(opacity * 255);

			tinyxml2::XMLElement *tileElement = layerElement->FirstChildElement("data")->FirstChildElement("tile");

			tiledLayer->texture.create(tiledLayer->width * m_tileWidth, tiledLayer->height * m_tileHeight);

			k = 0;

			while (tileElement)
			{
				int x = (k % tiledLayer->width) * m_tileWidth;
				int y = (k / tiledLayer->height) * m_tileHeight;

				int gid = tileElement->IntAttribute("gid") - m_firstGid;

				m_tiles[gid].setPosition((float)x, (float)y);
				tiledLayer->texture.draw(m_tiles[gid]);

				k++;

				tileElement = tileElement->NextSiblingElement("tile");
			}

			tiledLayer->texture.display();
			tiledLayer->sprite.setTexture(tiledLayer->texture.getTexture());

			m_layers.pushBack(tiledLayer);

			layerElement = layerElement->NextSiblingElement("layer");
		}

		return true;
	}

	void TiledMap::draw(sf::RenderTexture* _renderTexture)
	{
		for (unsigned int i = 0; i < m_layers.getElementNumber(); i++)
			_renderTexture->draw(m_layers[i]->sprite);
	}
}