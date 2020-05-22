/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== fragmentShader.glsl ==================
static const unsigned char temp_binary_data_0[] =
"R\"(\r\n"
"#version 150\r\n"
"#extension GL_ARB_explicit_uniform_location : enable\r\n"
"#extension GL_ARB_explicit_attrib_location : enable\r\n"
"in vec2 val;\r\n"
"uniform vec2 resolution;\r\n"
"uniform float[64] audioData;\r\n"
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
"//void main()\r\n"
"//{\r\n"
"//\tvec2 uv = -1.0 + 2.0 * gl_FragCoord.xy / vec2(100,100).xy;\r\n"
"//\t\r\n"
"//\tvec3 color_temp = vec3(0.0);\r\n"
"//\r\n"
"//\tfor (float i=0.0; i<8 + 1.0; i++)\r\n"
"//\t{\r\n"
"//\t\tfloat freq = texture(myTexture, vec2(i / 8, 0.0)).x * 7.0;\r\n"
"//\r\n"
"//\t\tvec2 p = vec2(uv);\r\n"
"//\r\n"
"//\t\tp.x += i * 0.04 + freq * 0.03;\r\n"
"//\t\tp.y += sin(p.x * 10.0) * cos(p.x * 2.0) * freq * 0.2 * ((i + 1.0) / 8);\r\n"
"//\t\tfloat intensity = abs(0.01 / p.y) * clamp(freq, 0.35, 2.0);\r\n"
"//\t\tcolor_temp += vec3(1.0 * intensity * (i / 5.0), 0.5 * intensity, 1.75 * intensity) * (3.0 / 8);\r\n"
"//\t}\r\n"
"//\r\n"
"//\tcolor = vec4(color_temp, 1.0);\r\n"
"//}\r\n"
"\r\n"
"//void main()\r\n"
"//{\r\n"
"//\tcolor = vec4(1.0, 0.0, 0.0, 1.0);\r\n"
"//}\r\n"
"\r\n"
"#define WAVES 2.0\r\n"
"\r\n"
"void main()\r\n"
"{\r\n"
"    vec2 uv = -1.5 + 2.0 * gl_FragCoord.xy / vec2(400,400).xy;\r\n"
"\t\r\n"
"\tvec3 col = vec3(0.0);\r\n"
"\r\n"
"\tfloat freq = texture(myTexture, vec2(1.0 / WAVES, 0.0)).x * 7.0;\r\n"
"\r\n"
"\tvec2 p = vec2(uv);\r\n"
"\r\n"
"    p.x += 1.0 * 0.04 + freq * 0.03;\r\n"
"    p.y += sin(p.x * 10.0) * cos(p.x * 2.0) * freq * 0.2 * ((1.0 + 1.0) / WAVES);\r\n"
"    float intensity = abs(0.01 / p.y) * clamp(freq, 0.35, 0.5);\r\n"
"    col += vec3(2.0 * intensity * (1.0 / 5.0), 0.2 * intensity, 0.05 * intensity) * (3.0 / WAVES);\r\n"
"\r\n"
"\tcolor = vec4(col, 1.0);\r\n"
"}\r\n"
"\r\n"
"\r\n"
")\"";

const char* fragmentShader_glsl = (const char*) temp_binary_data_0;

//================== vertexShader.glsl ==================
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

const char* vertexShader_glsl = (const char*) temp_binary_data_1;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x114811a8:  numBytes = 2286; return fragmentShader_glsl;
        case 0x2aad25d4:  numBytes = 359; return vertexShader_glsl;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "fragmentShader_glsl",
    "vertexShader_glsl"
};

const char* originalFilenames[] =
{
    "fragmentShader.glsl",
    "vertexShader.glsl"
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
