#ifdef GL_ES
precision mediump float;
#endif


uniform sampler2D _Texture;
uniform vec2 _Offset;

void main()
{
	//Original point
	vec4 FinalColor = texture2D(_Texture, gl_TexCoord[0].xy) * 0.2270270270;

	//Positive offset
	FinalColor += texture2D(_Texture, gl_TexCoord[0].xy + 4 * _Offset) * 0.0162162162;
	FinalColor += texture2D(_Texture, gl_TexCoord[0].xy + 3 * _Offset) * 0.0540540541;
	FinalColor += texture2D(_Texture, gl_TexCoord[0].xy + 2 * _Offset) * 0.1216216216;
	FinalColor += texture2D(_Texture, gl_TexCoord[0].xy + _Offset) * 0.1945945946;

	//Negative offset
	FinalColor += texture2D(_Texture, gl_TexCoord[0].xy - _Offset) * 0.1945945946;
	FinalColor += texture2D(_Texture, gl_TexCoord[0].xy - 2 * _Offset) * 0.1216216216;
	FinalColor += texture2D(_Texture, gl_TexCoord[0].xy - 3 * _Offset) * 0.0540540541;
	FinalColor += texture2D(_Texture, gl_TexCoord[0].xy - 4 *_Offset) * 0.0162162162;

	gl_FragColor = FinalColor;
}