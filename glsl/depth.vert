#version 410 core

layout(location = 0) in vec3 vert;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4x4 PROJECTION;
uniform mat4x4 VIEW;
uniform mat4x4 MODEL;

out vec4 frag_color;

void main()
{
    gl_Position = PROJECTION * VIEW * MODEL * vec4(vert,1.0f);
    frag_color = vec4(1.0,1.0,1.0,1.0);
}