#ifdef GL_ES
precision mediump float;
#endif


uniform sampler2D _Texture;
uniform float _Ratio;

void main()
{
	vec4 CurrentColor = texture2D(_Texture, gl_TexCoord[0].xy);
	CurrentColor.a *= _Ratio;
	gl_FragColor = CurrentColor;
}