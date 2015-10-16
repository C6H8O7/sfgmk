#ifdef GL_ES
precision mediump float;
#endif


uniform sampler2D _Texture;
uniform float _SaturationFloor;

void main()
{
	gl_FragColor = clamp(( texture2D(_Texture, gl_TexCoord[0].xy) - _SaturationFloor) / (1.0f - _SaturationFloor), 0.0f, 1.0f);
}