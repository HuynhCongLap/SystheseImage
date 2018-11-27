
//! \file tuto9_texture1.glsl

#version 330

#ifdef VERTEX_SHADER

uniform mat4 mvpMatrix;
uniform vec3 camposition;

layout(location= 0) in vec3 position;
layout(location= 2) in vec3 normal;
layout(location= 1) in vec2 texcoord;

out vec3 pos;
out vec3 cam;
out vec3 nor;
out vec2 vertex_texcoord;

void main( )
{
    gl_Position= mvpMatrix * vec4(position, 1);
    nor = normal;
    pos = position;
    cam = camposition;
    vertex_texcoord= texcoord;
    
}

#endif


#ifdef FRAGMENT_SHADER

out vec4 fragment_color;

in vec3 pos;
in vec3 nor;
in vec3 cam;
in vec2 vertex_texcoord;

uniform sampler2D texture0;

void main( )
{
    vec4 color= texture(texture0, vertex_texcoord);

  

     vec3 SurfaceNormal  = normalize(nor);
     vec3 LightToSurface = normalize(vec3(50,500,0) - pos);
     vec3 EyeToSurface   = normalize(cam - pos);
    // vec3 ReflectedDirection = reflect(-LightToSurface, SurfaceNormal);
    // float DiffuseFactor  = max(0, dot(-LightToSurface, SurfaceNormal));
    // float SpecularFactor = pow(max(0, dot(EyeToSurface, ReflectedDirection)), 2);

    // vec3 MaterialAmbient  = vec3(0.7765, 0.8784, 0.3412);
    // vec3 MaterialDiffuse  = vec3(0.7765, 0.8784, 0.3412);
     //vec3 MaterialSpecular = vec3(0.3500, 0.3500, 0.3500);

	//vec3 Ambient  = MaterialAmbient;
	//vec3 Diffuse  = DiffuseFactor  * MaterialDiffuse;
	//vec3 Specular = SpecularFactor * MaterialSpecular;
 		
	
   	//vec3 Color = Ambient + Diffuse + Specular;
	float cos_theta = dot(SurfaceNormal, LightToSurface);
	vec3 h = normalize(vec3(100,500,0)+ vec3(0,500,200) + nor);
	float cos_theta_h = dot(SurfaceNormal,h);
	float fr = 0.65/3.1419 + (1-0.65)*(20+8)/(8*3.1419) * pow(cos_theta_h,20);
	float one = 0.6*(10+1)/(2*3.1419)*pow(cos_theta_h,10) ;
    	fragment_color= vec4(1,1,1,1)*color*one*max(cos_theta,0);
}

#endif

