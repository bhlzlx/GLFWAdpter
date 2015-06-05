#version 410 core

uniform sampler2DShadow DEPTH_TEXTURE00;

uniform sampler2D       TEXTURE00;
uniform sampler2D       TEXTURE01;
uniform sampler2D       TEXTURE02;

in vec2     frag_uv;
in float    frag_diff;
in vec4     shadow_coord;

in vec3 tbn_tangent;
in vec3 tbn_normal;
in vec3 tbn_bittangent;

in vec3 position_eye;
in vec3 position_light;
in vec3 vertex_world;

out vec4 color;

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main()
{

   vec2 poissonDisk[16] = vec2[](
   vec2( -0.94201624, -0.39906216 ),
   vec2( 0.94558609, -0.76890725 ),
   vec2( -0.094184101, -0.92938870 ),
   vec2( 0.34495938, 0.29387760 ),
   vec2( -0.91588581, 0.45771432 ),
   vec2( -0.81544232, -0.87912464 ),
   vec2( -0.38277543, 0.27676845 ),
   vec2( 0.97484398, 0.75648379 ),
   vec2( 0.44323325, -0.97511554 ),
   vec2( 0.53742981, -0.47373420 ),
   vec2( -0.26496911, -0.41893023 ),
   vec2( 0.79197514, 0.19090188 ),
   vec2( -0.24188840, 0.99706507 ),
   vec2( -0.81409955, 0.91437590 ),
   vec2( 0.19984126, 0.78641367 ),
   vec2( 0.14383161, -0.14100790 )
);

    vec4 textureColor   = texture(TEXTURE00,frag_uv);

   // color = textureColor;



    vec4 tangentNormal  = texture(TEXTURE01,frag_uv);
    vec4 highLight      = texture(TEXTURE02,frag_uv);

    mat3x3 TBN =   mat3x3(tbn_tangent,tbn_bittangent,tbn_normal);

    vec4 normal = vec4(normalize(TBN * tangentNormal.xyz),0);
    vec3 reflect_light = reflect( normalize(vertex_world - position_light) ,normalize(normal.xyz) );
    vec3 eye_vector = normalize(position_eye - vertex_world);

    float spec = max(0.0, dot(reflect_light ,eye_vector));
    spec = pow(spec,256);


    //float visible = texture(DEPTH_TEXTURE00,vec3(shadow_coord.xy,shadow_coord.z - 0.00000002));

    float visible=1.0;
	float bias = 0.00000008;

	for (int i=0;i<4;i++)
    {
		 int index = int(16.0*random(floor(vertex_world*1000.0), i))%16;
          visible -= 0.2*(1.0-texture( DEPTH_TEXTURE00, vec3(shadow_coord.xy + poissonDisk[index]/5500.0,shadow_coord.z-bias) ));
	}
//    color = textureColor * frag_diff;

//    float visible = texture(DEPTH_TEXTURE00,shadow_coord.xyz);

    //visibility = texture( shadowMap, shadow_coord.xyz);

    color = (textureColor * frag_diff  + highLight * spec * 0.5 * frag_diff) * visible + textureColor * 0.3;
    //color = textureColor * frag_diff ;// + highLight * spec * 0.5 * frag_diff) * visible + textureColor * 0.6;
    //color = vec4(1.0,1.0,1.0,1.0) * visible;
}
