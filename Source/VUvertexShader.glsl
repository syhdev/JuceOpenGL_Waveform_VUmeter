R"(
#version 150
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_explicit_attrib_location : enable
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 value;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


out vec2 val;

void main()
{
    val = value;
    gl_Position = vec4(vertex, 1.0);
}
)"