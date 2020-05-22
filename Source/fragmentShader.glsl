R"(
#version 150
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable
in vec2 val;
uniform vec2 resolution;
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


#define WAVES 2.0

void main()
{
    vec2 uv = -1.0 + 2.0* gl_FragCoord.xy / vec2(3.0*resolution.x, 0.5*resolution.y);
	
    // basis color
	vec3 col = vec3(0.1, 0.1, 0.1);

    float freq = texture(myTexture, vec2(uv.x, 0.0)).x * 5.0;

	vec2 p = vec2(uv);

    //p.x += 1.0 * 0.04 + freq * 0.03;
    p.y += sin(p.x * 9.0) * freq * 0.2 - 0.2;
    float intensity = abs(0.01 / p.y) * clamp(freq, 0.05, 0.75);
    col += vec3(1.8 * intensity , 0.05 * intensity, 0.05 * intensity);

	color = vec4(col, 1.0);
}


)"