/*--------------------------------------------------------------------------------------------------

	@author		KCC
	@date		17/07/2015
	@brief		Texte dessiné avec des LightningBolt

--------------------------------------------------------------------------------------------------*/

#ifndef LIGHTNING_TEXT_HPP
#define LIGHTNING_TEXT_HPP

namespace sfgmk
{
	class LightningText : public FxText
	{
		private:
			sf::RenderTexture m_PreviousFrameRender;
			sfgmk::DynamicArray<LightningBolt*> m_Bolts;
			unsigned int m_uiArraySize;
			std::mt19937 m_MersenneTwister19937Generator;

			sf::Shader* m_Shader;

			unsigned int m_uiPrecision;
			unsigned int m_uiBoltChance;

		public:
			LightningText(std::string _String, sf::Font _Font, unsigned int _Size, unsigned int _Precision, unsigned int _BoltChance, sf::Color _Color = sf::Color::White);
			~LightningText();

			void update(const float& _TimeDelta);
			void draw(sf::RenderTexture* _Render);
	};
}


#endif