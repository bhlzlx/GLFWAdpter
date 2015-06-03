#version 410 core

layout(location = 0) in vec3 vert;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 uv;
layout(location = 3) in vec3 tangent;

uniform mat4x4 PROJECTION;
uniform mat4x4 VIEW;
uniform mat4x4 MODEL;
uniform mat4x4 SHADOW_VIEW;

uniform vec3 LIGHT_POSITION0;
uniform vec3 LIGHT_POSITION1;
uniform vec3 LIGHT_POSITION2;
uniform vec3 LIGHT_POSITION3;

uniform vec3 CAMERA_POSITION;

out vec2  frag_uv;
out float frag_diff;
out vec4  shadow_coord;

out vec3 tbn_tangent;
out vec3 tbn_normal;
out vec3 tbn_bittangent;

out vec3 position_eye;
out vec3 position_light;
out vec3 vertex_world;

void main()
{
    gl_Position         = PROJECTION * VIEW * MODEL * vec4(vert,1.0f);

    vertex_world        = (MODEL * vec4(vert,1.0f)).xyz;

    tbn_normal          =  normalize( (MODEL * vec4(normal,0)).xyz);
    tbn_tangent         =  normalize( (MODEL * vec4(tangent,0)).xyz);
    tbn_bittangent      =  cross(tbn_normal,tbn_tangent);

    shadow_coord        = PROJECTION * SHADOW_VIEW * MODEL * vec4(vert,1.0f);
    shadow_coord        = shadow_coord / shadow_coord.w / 2 + 0.5;



    //

    frag_diff           = max(0.0f,dot( normalize( LIGHT_POSITION0 - vertex_world ) ,tbn_normal));
    frag_uv             = uv.xy;

   // float bias          = 0.0000008 * pow(tan(atan(frag_diff)),0.125); // cosTheta is dot( n,l ), clamped between 0 and 1

   // bias = (bias>>4)<<4;
    //bias = bias * 1000;

    //bias                = clamp(bias, 0.0f,0.001f);

    //shadow_coord.z      = shadow_coord.z - 0.0000004;

    position_eye        = CAMERA_POSITION;
    position_light      = LIGHT_POSITION0;
}
