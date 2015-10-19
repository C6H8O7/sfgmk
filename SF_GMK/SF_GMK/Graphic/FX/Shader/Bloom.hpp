/*--------------------------------------------------------------------------------------------------

	@author		GMK (sources: LaurentGomila / https://github.com/SFML/SFML-Game-Development-Book)
	@date		09/06/2015
	@brief		Différents shaders nécessaires à l'effet de bloom de la grille

--------------------------------------------------------------------------------------------------*/

#ifndef BLOOM_HPP
#define BLOOM_HPP


namespace sfgmk
{
		class Bloom
		{
			private:
			sf::Shader m_BrightnessShader;
			sf::Shader m_BlurShader;
			sf::Shader m_AddShader;
			sf::Sprite m_Sprite;

			sf::RenderTexture m_TempTexture1;
			sf::RenderTexture m_TempTexture2;

			sf::Vector2u m_Size;
			float m_fSizeDownScale;
			float m_fEffectLevel;
			float m_fSaturateLevel;
			sf::Vector2f m_BlurOffset;

			public:
			Bloom(const sf::Vector2u _Size, float _SizeDownScale, float _EffectLevel, float _SaturateLevel, sf::Vector2f _BlurOffset);
			~Bloom();

			void applyEffect(sf::RenderTexture& _Source);
		};
}


#endif