/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== VUfragmentShader.glsl ==================
static const unsigned char temp_binary_data_0[] =
"R\"(\r\n"
"#version 150\r\n"
"#extension GL_ARB_explicit_uniform_location : enable\r\n"
"#extension GL_ARB_explicit_attrib_location : enable\r\n"
"in vec2 val;\r\n"
"uniform vec2 resolution;\r\n"
"uniform float numSamples;\r\n"
"uniform sampler2D myTexture;\r\n"
"out vec4 color;\r\n"
"\r\n"
"//void getAmplitudeForXPos (in float xPos, out float audioAmplitude)\r\n"
"//{\r\n"
"//   // Buffer size - 1\r\n"
"//   float perfectSamplePosition = 255.0 * xPos / resolution.x;\r\n"
"//   int leftSampleIndex = int (floor (perfectSamplePosition));\r\n"
"//   int rightSampleIndex = int (ceil (perfectSamplePosition));\r\n"
"//   audioAmplitude = mix (audioData[leftSampleIndex], audioData[rightSampleIndex], fract (perfectSamplePosition));\r\n"
"//}\r\n"
"//\r\n"
"//#define THICKNESS 0.02\r\n"
"//void main()\r\n"
"//{\r\n"
"//    float y = gl_FragCoord.y / resolution.y;\r\n"
"//    float amplitude = 0.0;\r\n"
"//    getAmplitudeForXPos (gl_FragCoord.x, amplitude);\r\n"
"//\r\n"
"//    // Centers & Reduces Wave Amplitude\r\n"
"//    amplitude = 0.5 - amplitude / 2.5;\r\n"
"//    float r = abs (THICKNESS / (amplitude-y));\r\n"
"//\r\n"
"//    color = vec4 (r - abs (r * 0.2), r - abs (r * 0.2), r - abs (r * 0.2), 1.0);\r\n"
"//}\r\n"
"\r\n"
"\r\n"
"#define WAVES 2.0\r\n"
"\r\n"
"void main()\r\n"
"{\r\n"
"    //vec2 uv = -1.0 + 2.0* gl_FragCoord.xy / vec2(3.0*resolution.x, 0.5*resolution.y);\r\n"
"    vec2 uv = gl_FragCoord.xy / vec2(resolution.x, resolution.y);\r\n"
"\t\r\n"
"    // basis color\r\n"
"\tvec3 col = vec3(0.1, 0.1, 0.1);\r\n"
"\r\n"
"    float freq = texture(myTexture, vec2(uv.x /* numSamples / resolution.x*/, 0.0)).x;// * 5.0;\r\n"
"\r\n"
"\tvec2 p = vec2(uv);\r\n"
"\r\n"
"    //p.x += 1.0 * 0.04 + freq * 0.03;\r\n"
"    p.y += sin(p.x * 16.0) * freq * 0.3 - 0.5;\r\n"
"    float intensity = abs(0.01 / p.y) * clamp(freq, 0.05, 0.75);\r\n"
"    col += vec3(1.8 * intensity, 0.7 * intensity, 0.05 * intensity);\r\n"
"\r\n"
"\tcolor = vec4(col, 1.0);\r\n"
"}\r\n"
"\r\n"
"\r\n"
")\"";

const char* VUfragmentShader_glsl = (const char*) temp_binary_data_0;

//================== VUvertexShader.glsl ==================
static const unsigned char temp_binary_data_1[] =
"R\"(\r\n"
"#version 150\r\n"
"#extension GL_ARB_explicit_uniform_location : enable\r\n"
"#extension GL_ARB_explicit_attrib_location : enable\r\n"
"layout(location = 0) in vec3 vertex;\r\n"
"layout(location = 1) in vec2 value;\r\n"
"\r\n"
"uniform mat4 viewMatrix;\r\n"
"uniform mat4 projectionMatrix;\r\n"
"\r\n"
"\r\n"
"out vec2 val;\r\n"
"\r\n"
"void main()\r\n"
"{\r\n"
"    val = value;\r\n"
"    gl_Position = vec4(vertex, 1.0);\r\n"
"}\r\n"
")\"";

const char* VUvertexShader_glsl = (const char*) temp_binary_data_1;

//================== WVfragmentShader.glsl ==================
static const unsigned char temp_binary_data_2[] =
"R\"(\r\n"
"#version 150\r\n"
"#extension GL_ARB_explicit_uniform_location : enable\r\n"
"#extension GL_ARB_explicit_attrib_location : enable\r\n"
"\r\n"
"uniform vec2 resolution;\r\n"
"uniform float numSamples;\r\n"
"uniform sampler2D audioTexture;\r\n"
"out vec4 color;\r\n"
"\r\n"
"\r\n"
"float plot(vec2 st, float pct) {\r\n"
"\treturn smoothstep(pct-.01, pct, st.y) - smoothstep(pct, pct+.01, st.y);\r\n"
"}\r\n"
"\r\n"
"\r\n"
"#define WAVES 2.0\r\n"
"\r\n"
"void main()\r\n"
"{\r\n"
"\tfloat alpha = 0.8;\r\n"
"\r\n"
"    vec2 st = gl_FragCoord.xy / resolution.xy;\r\n"
"\t\r\n"
"    float data = texture(audioTexture, vec2(st.x /* numSamples / resolution.x*/, 0.0)).x;// * 5.0;\r\n"
"\r\n"
"\tfloat y = mix(0.05, .95, (data + 1.0) / 2.0) - 0.5;\r\n"
"\t\t\r\n"
"\t// basis color\r\n"
"\tvec3 col1 = vec3(0.1, 0.1, 0.1);\r\n"
"\t\r\n"
"\tvec3 col2 = vec3(1.0, 0.2, 0.0);\r\n"
"\t\r\n"
"\tcol1 = mix(col1, col2, plot(st, y));\r\n"
"\r\n"
"//\tfloat intensity = abs(0.01 / st.y) * clamp(data, 0.05, 0.75);\r\n"
"//    col1 += vec3(1.8 * intensity, 0.7 * intensity, 0.05 * intensity);\r\n"
"\r\n"
"\tcolor = vec4(col1, alpha);\r\n"
"}\r\n"
"\r\n"
"\r\n"
")\"";

const char* WVfragmentShader_glsl = (const char*) temp_binary_data_2;

//================== WVvertexShader.glsl ==================
static const unsigned char temp_binary_data_3[] =
"R\"(\r\n"
"\r\n"
"#version 150\r\n"
"#extension GL_ARB_explicit_uniform_location : enable\r\n"
"#extension GL_ARB_explicit_attrib_location : enable\r\n"
"layout(location = 0) in vec3 vertex;\r\n"
"\r\n"
"uniform mat4 viewMatrix;\r\n"
"uniform mat4 projectionMatrix;\r\n"
"\r\n"
"void main()\r\n"
"{\r\n"
"    gl_Position = vec4(vertex, 1.0);\r\n"
"}\r\n"
"\r\n"
")\"";

const char* WVvertexShader_glsl = (const char*) temp_binary_data_3;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x3566b889:  numBytes = 1714; return VUfragmentShader_glsl;
        case 0x865a10f5:  numBytes = 359; return VUvertexShader_glsl;
        case 0x29a18469:  numBytes = 934; return WVfragmentShader_glsl;
        case 0x189654d5:  numBytes = 289; return WVvertexShader_glsl;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "VUfragmentShader_glsl",
    "VUvertexShader_glsl",
    "WVfragmentShader_glsl",
    "WVvertexShader_glsl"
};

const char* originalFilenames[] =
{
    "VUfragmentShader.glsl",
    "VUvertexShader.glsl",
    "WVfragmentShader.glsl",
    "WVvertexShader.glsl"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
    {
        if (namedResourceList[i] == resourceNameUTF8)
            return originalFilenames[i];
    }

    return nullptr;
}

}
