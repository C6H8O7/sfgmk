#ifdef GL_ES
precision mediump float;
#endif


uniform sampler2D _TextureSource;
uniform sampler2D _TextureEffect;
uniform float _EffectLevel;

/*vec4 AdjustSaturation(vec4 color, float saturation)
{
    // The constants 0.3, 0.59, and 0.11 are chosen because the
    // human eye is more sensitive to green light, and less to blue.
    vec4 grey = dot(color, vec3(0.3, 0.59, 0.11));

    return mix(grey, color, saturation);
}*/

void main()
{
	//Simple homemade version
	gl_FragColor = texture2D(_TextureSource, gl_TexCoord[0].xy) + texture2D(_TextureEffect, gl_TexCoord[0].xy) * _EffectLevel * 2;

	//Original version
	/*vec4 bloom = texture2D(_TextureEffect, gl_TexCoord[0].xy);
    vec4 base = texture2D(_TextureSource, gl_TexCoord[0].xy);
 
    // Adjust color saturation and intensity.
    bloom = AdjustSaturation(bloom, 1.5f) * 2.0f;
    base = AdjustSaturation(base, 1.0f) * 1.0f;
 
    // Darken down the base image in areas where there is a lot of bloom,
    // to prevent things looking excessively burned-out.
    base *= (1 - clamp(bloom, 0.0f, 1.0f));
 
    // Combine the two images.
    gl_FragColor = base + bloom;*/
}

