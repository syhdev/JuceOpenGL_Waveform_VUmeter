R"(
#version 150
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable
in vec2 val;
uniform vec2 resolution;
uniform float[64] audioData;
uniform sampler2D myTexture;
out vec4 color;

//void getAmplitudeForXPos (in float xPos, out float audioAmplitude)
//{
//   // Buffer size - 1
//   float perfectSamplePosition = 255.0 * xPos / resolution.x;
//   int leftSampleIndex = int (floor (perfectSamplePosition));
//   int rightSampleIndex = int (ceil (perfectSamplePosition));
//   audioAmplitude = mix (audioData[leftSampleIndex], audioData[rightSampleIndex], fract (perfectSamplePosition));
//}
//
//#define THICKNESS 0.02
//void main()
//{
//    float y = gl_FragCoord.y / resolution.y;
//    float amplitude = 0.0;
//    getAmplitudeForXPos (gl_FragCoord.x, amplitude);
//
//    // Centers & Reduces Wave Amplitude
//    amplitude = 0.5 - amplitude / 2.5;
//    float r = abs (THICKNESS / (amplitude-y));
//
//    color = vec4 (r - abs (r * 0.2), r - abs (r * 0.2), r - abs (r * 0.2), 1.0);
//}

//void main()
//{
//	vec2 uv = -1.0 + 2.0 * gl_FragCoord.xy / vec2(100,100).xy;
//	
//	vec3 color_temp = vec3(0.0);
//
//	for (float i=0.0; i<8 + 1.0; i++)
//	{
//		float freq = texture(myTexture, vec2(i / 8, 0.0)).x * 7.0;
//
//		vec2 p = vec2(uv);
//
//		p.x += i * 0.04 + freq * 0.03;
//		p.y += sin(p.x * 10.0) * cos(p.x * 2.0) * freq * 0.2 * ((i + 1.0) / 8);
//		float intensity = abs(0.01 / p.y) * clamp(freq, 0.35, 2.0);
//		color_temp += vec3(1.0 * intensity * (i / 5.0), 0.5 * intensity, 1.75 * intensity) * (3.0 / 8);
//	}
//
//	color = vec4(color_temp, 1.0);
//}

//void main()
//{
//	color = vec4(1.0, 0.0, 0.0, 1.0);
//}

#define WAVES 2.0

void main()
{
    vec2 uv = -1.5 + 2.0 * gl_FragCoord.xy / vec2(400,400).xy;
	
	vec3 col = vec3(0.0);

	float freq = texture(myTexture, vec2(1.0 / WAVES, 0.0)).x * 7.0;

	vec2 p = vec2(uv);

    p.x += 1.0 * 0.04 + freq * 0.03;
    p.y += sin(p.x * 10.0) * cos(p.x * 2.0) * freq * 0.2 * ((1.0 + 1.0) / WAVES);
    float intensity = abs(0.01 / p.y) * clamp(freq, 0.35, 0.5);
    col += vec3(2.0 * intensity * (1.0 / 5.0), 0.2 * intensity, 0.05 * intensity) * (3.0 / WAVES);

	color = vec4(col, 1.0);
}


)"