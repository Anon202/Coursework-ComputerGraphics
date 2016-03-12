#version 440

// The model matrix
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;

uniform float myTime;

// Texture
uniform sampler2D tex[2];

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 2) in vec3 normal;
// Incoming texture coordinates
layout (location = 10) in vec2 tex_coord_in;

// Outgoing position
layout (location = 0) out vec3 vertex_position;
// Outgoing normal
layout (location = 1) out vec3 transformed_normal;
// Outgoing texture coordinate
layout (location = 2) out vec2 tex_coord_out;


vec4 texture2D_bilinear(in sampler2D tex, in vec2 uv )
{
	uv *= 512.0f;
	vec2 f = fract( uv );
	vec4 t00 = texture2D( tex, uv );

	vec2 newUv =  uv + vec2( 1.0f/ 512.0f, 0.0f );

	vec4 t10 = texture2D(tex, newUv);
	vec4 tA = mix( t00, t10, f.x );

	newUv = uv + vec2( 0.0f, 1.0f/ 512.0f );
	vec4 t01 = texture2D( tex,newUv  );

	newUv = uv + vec2( 1.0f/512.0f, 1.0f/512.0f );
	vec4 t11 = texture2D( tex, newUv );
	vec4 tB = mix( t01, t11, f.x );
	return mix( tA, tB, f.y );
}

void main()
{
	// 
	vec4 newPosition; 

	vec4 sample_disp = texture2D(tex[1], tex_coord_in);
	
	vec3 displacementFactor = vec3(0.3*sample_disp.x, 0.59*sample_disp.y, 0.11*sample_disp.z);

	displacementFactor.x *= sin(myTime);
	displacementFactor.y *= cos(myTime);

	transformed_normal = N * normal;

	// move positon along the normal
	newPosition = vec4(position, 1.0) + vec4(transformed_normal * displacementFactor, 0.0);

	tex_coord_out = tex_coord_in;


	// set screenspace postion
	gl_Position = MVP * newPosition;

	// pass through vertex world positon for lighting calc
	vertex_position = vec3(M * newPosition);
	
}