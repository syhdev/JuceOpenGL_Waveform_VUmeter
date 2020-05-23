R"(
#version 150
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable

uniform vec2 resolution;
uniform float numSamples;
uniform sampler2D audioTexture;
out vec4 color;


float plot(vec2 st, float pct) {
	return smoothstep(pct-.01, pct, st.y) - smoothstep(pct, pct+.01, st.y);
}


#define WAVES 2.0

void main()
{
	float alpha = 0.8;

    vec2 st = gl_FragCoord.xy / resolution.xy;
	
    float data = texture(audioTexture, vec2(st.x /* numSamples / resolution.x*/, 0.0)).x;// * 5.0;

	float y = mix(0.05, .95, (data + 1.0) / 2.0) - 0.5;
		
	// basis color
	vec3 col1 = vec3(0.1, 0.1, 0.1);
	
	vec3 col2 = vec3(1.0, 0.2, 0.0);
	
	col1 = mix(col1, col2, plot(st, y));

//	float intensity = abs(0.01 / st.y) * clamp(data, 0.05, 0.75);
//    col1 += vec3(1.8 * intensity, 0.7 * intensity, 0.05 * intensity);

	color = vec4(col1, alpha);
}


)"