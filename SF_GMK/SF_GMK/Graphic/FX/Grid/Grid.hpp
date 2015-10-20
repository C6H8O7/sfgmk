/*--------------------------------------------------------------------------------------------------

	@author		GMK (sources: Terence J. Grant / http://gamedevelopment.tutsplus.com/tutorials/make-a-neon-vector-shooter-for-ios-the-warping-grid--gamedev-14637)
	@date		13/04/2015
	@brief		Entité constituée de ressorts reliants des points affectés d'une masse

--------------------------------------------------------------------------------------------------*/

#ifndef GRID_HPP
#define GRID_HPP


namespace sfgmk
{
	class SFGMK_API Grid : public Entity
	{
		private:
			Bloom* m_BloomEffect;

			std::vector<Spring> m_Springs;
			PointMass* m_Points;
			PointMass* m_FixedPoints;
			sf::Vector2i m_iLineNumber;
			sf::Vector2f m_fSpacing;
			sf::Vector2f m_Size;
			int m_iPointNumber;
			sf::Vector2f m_ScreenSize;

			sf::Color m_Color[3];
			sf::RenderTexture* m_RenderTexture;
			sf::Vertex m_Line[2];
			sf::Sprite m_Sprite;

			bool m_bDrawMidLines;
			bool m_bApplyBloom;

			sf::Rect<float> m_CameraView;

		public:
			Grid();
			Grid(const sf::Vector2i& _Lines, const sf::Vector2f& _Spacing, const sf::Vector3f& _Position = sf::Vector3f(0.0f, 0.0f, 0.0f));
			~Grid();

			void init();
			void setLineNumber(const sf::Vector2i& _Lines);
			void setSpacing(const sf::Vector2f& _Spacing);
			const sf::Vector2f& getSize();

			void setPointMass(PointMass* _Array, const int& _Column, const int& _Row, const PointMass& _Point);
			PointMass* getPointMass(PointMass* _Array, const int& _Column, const int& _Row);

			static float Grid::catmullRom(const float _Value1, const float _Value2, const float _Value3, const float _Value4, float _Amount);
			static sf::Vector2f Grid::catmullRom(const sf::Vector2f& _Value1, const sf::Vector2f& _Value2, const sf::Vector2f& _Value3, const sf::Vector2f& _Value4, float _Amount);

			void update(const float& _TimeDelta);
			void draw(sf::RenderTexture* _Render);
			void drawLine(sf::Vector2f& _Point1, const sf::Vector2f& _Point2, const sf::Color& _Color);
			void drawLineWithVisibilityTest(sf::Vector2f& _Point1, sf::Vector2f& _Point2, const sf::Color& _Color);

			void applyDirectedForce(const sf::Vector3f& _Force, const sf::Vector3f& _Position, const float& _Radius);
			void applyImplosiveForce(const float& _Force, const sf::Vector3f& _Position, const float& _Radius);
			void applyExplosiveForce(const float& _Force, const sf::Vector3f& _Position, const float& _Radius);

			sf::Vector2f toVec2(const sf::Vector3f& _Vector);

			void setColor(sf::Color _Color);
			void setMidLinesColor(sf::Color _Color);
			void setBorderLinesColor(sf::Color _Color);

			void setDrawMidLines(bool _Boolean);
			void setApplyBloom(bool _Boolean, float _EffectTextureDownScale = 1.0f, float _EffectLevel = 2.0f, float _SaturateLevel = 0.3f, sf::Vector2f _BlurOffset = sf::Vector2f(0.0005f, 0.0005f));
	};
}


#endif