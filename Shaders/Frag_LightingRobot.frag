#version 430

// per-fragment attributes coming in from the pipeline
in vec3 worldPosition;
in vec3 worldNormal;
in vec2 textureCoords;

// output value - the color of the fragment
out vec4 outputColor;

// texture sampling object
uniform sampler2D textureImage;

uniform sampler2D textureShinning;
uniform int state = 0;

// Must be the same as in MyApp.h!
const int SHADER_STATE_DEFAULT = 0;
const int SHADER_STATE_ROBOT = 1;
const int SHADER_STATE_TABLE = 2;
const int SHADER_DEBUG_NONE = 0;
const int SHADER_DEBUG_NORMALS = 1;
const int SHADER_DEBUG_SHADING = 2;
const int SHADER_DEBUG_DIFFUSE_TEXTURE = 3;
const int SHADER_DEBUG_SHINE_TEXTURE = 4;

uniform int debugState = SHADER_STATE_DEFAULT;

uniform vec3 cameraPosition;

// light source properties
uniform vec4 lightPosition = vec4( 0.0, 1.0, 0.0, 0.0);

uniform vec3 La = vec3(0.0, 0.0, 0.0 );
uniform vec3 Ld = vec3(1.0, 1.0, 1.0 );
uniform vec3 Ls = vec3(1.0, 1.0, 1.0 );


uniform vec4 lightPosition2 = vec4( 0.0, 1.0, 0.0, 0.0);
uniform vec3 La2 = vec3(0.0, 0.0, 0.0 );
uniform vec3 Ld2 = vec3(1.0, 1.0, 1.0 );
uniform vec3 Ls2 = vec3(1.0, 1.0, 1.0 );


uniform float lightConstantAttenuation    = 1.0;
uniform float lightLinearAttenuation      = 0.0;
uniform float lightQuadraticAttenuation   = 0.0;

// material properties

uniform vec3 Ka = vec3( 1.0 );
uniform vec3 Kd = vec3( 1.0 );
uniform vec3 Ks = vec3( 1.0 );

uniform vec3 lightColorMultiplier = vec3(1.0);
uniform vec3 darkColorMultiplier = vec3(0.8,0.8,0.9);


uniform float Shininess = 20.0;


struct LightProperties
{
	vec4 pos;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

struct MaterialProperties
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};

//vec3 lighting(LightProperties light){
//	vec3 ToLight; // Vector pointing TOWARDS the light source
//	float Attenuation = 1.0; // fading of light intensity
//	
//	if ( light.pos.w == 0.0 ) // directional light
//	{
//		//
//		// For directional lights, everything is illuminated from the same direction
//		ToLight	= light.pos.xyz;
//
//		//
//		// The attenuation is left as 1, so it won't change the lighting
//	}
//	else				  // positional light
//	{
//		//
//		// For positional lights, we calculate the vector pointing from the fragment point to the light source, ...
//		ToLight	= light.pos.xyz - position;
//		
//		// ... and the distance from the light source
//		float LightDistance = length(ToLight);
//		
//		// ... and finally the attenuation (fading)
//		Attenuation = 1.0 / ( light.constantAttenuation + light.linearAttenuation * LightDistance + light.quadraticAttenuation * LightDistance * LightDistance);
//	}
//	// Normalize the vector pointing towards the light source
//	ToLight = normalize(ToLight);
//	
//	// Ambient component
//	// Ambient light has the same intensity everywhere
//	vec3 Ambient = light.La * material.Ka;
//
//	// Diffuse component
//	//
//	//
//	// The ammount of light coming from the diffuse light source is proportional to the dot product of the vector pointing
//	// towards the light source and the normal vector, and the attenuation
//	float DiffuseFactor = max(dot(ToLight,light.normal), 0.0) * Attenuation;
//	vec3 Diffuse = DiffuseFactor * light.Ld * Kd;
//	
//	// Specular component
//	vec3 viewDir = normalize( cameraPosition - worldPosition ); // Vector pointing from the fragment to the camera
//	vec3 reflectDir = reflect( -ToLight, light.normal ); // Ideal reflection direction of the incoming light ray
//	
//	float FragShininess = Shininess;
//
//	if (state == SHADER_STATE_ROBOT)
//	{
//		FragShininess = texture(textureShinning, textureCoords).r * Shininess;
//	}
//
//	//
//	//
//	//
//	// The specular component depends on the reflection direction and the camera direction.
//	// It's intensity is calculated in the cos()^s form, where s is the shininess parameter.
//	// Also depends on the attenuation.
//	float SpecularFactor = pow(max( dot( viewDir, reflectDir) ,0.0), FragShininess) * Attenuation;
//	
//
//	if (state == SHADER_STATE_ROBOT && FragShininess < 1e-6f )
//	{
//		SpecularFactor = 0.0f;
//	}
//	
//	vec3 Specular = SpecularFactor * light.Ls * Ks;
//
//	return Ambient + Diffuse + Specular;
//}


vec3 lighting(LightProperties light, vec3 position, vec3 normal, MaterialProperties material)
{
	vec3 ToLight; // Vector pointing TOWARDS the light source
	float Attenuation = 1.0; // fading of light intensity
	
	if ( light.pos.w == 0.0 ) // directional light
	{
		// For directional lights, everything is illuminated from the same direction
		ToLight	= light.pos.xyz;

		// The attenuation is left as 1, so it won't change the lighting
	}
	else				  // positional light
	{
		// For positional lights, we calculate the vector pointing from the fragment point to the light source, ...
		ToLight	= light.pos.xyz - position;
		
		// ... and the distance from the light source
		float LightDistance = length(ToLight);
		
		// ... and finally the attenuation (fading)
		Attenuation = 1.0 / ( light.constantAttenuation + light.linearAttenuation * LightDistance + light.quadraticAttenuation * LightDistance * LightDistance);
	}
	
	ToLight = normalize(ToLight);
	
	
	vec3 Ambient = light.La * material.Ka;

	
	float DiffuseFactor = max(dot(ToLight,normal), 0.0) * Attenuation;
	vec3 Diffuse = DiffuseFactor * light.Ld * material.Kd;
	
	
	vec3 viewDir = normalize( cameraPosition - position );  // Vector pointing from the fragment to the camera
	vec3 reflectDir = reflect( -ToLight, normal ); // Ideal reflection direction of the incoming light ray
	
	
	float SpecularFactor = pow(max( dot( viewDir, reflectDir) ,0.0), material.Shininess) * Attenuation;
	vec3 Specular = SpecularFactor * light.Ls * material.Ks;

	return Ambient + Diffuse + Specular;
}


void main()
{
	// The fragment's normal vector
	// We ALWAYS normalize it!
	vec3 normal = normalize( worldNormal );

	LightProperties light;
	light.pos = lightPosition;
	light.La = La;
	light.Ld = Ld;
	light.Ls = Ls;
	light.constantAttenuation = lightConstantAttenuation;
	light.linearAttenuation = lightLinearAttenuation;
	light.quadraticAttenuation = lightQuadraticAttenuation;

	MaterialProperties material;
	material.Ka = Ka;
	material.Kd = Kd;
	material.Ks = Ks;
	material.Shininess = Shininess;

	if (state == SHADER_STATE_ROBOT)
	{
		float FragShininess = texture(textureShinning, textureCoords).r;
		if ( FragShininess > 1e-6 )
		{
			material.Shininess = FragShininess * Shininess;
		}
		else
		{
			material.Ks = vec3(0.0);
		}
	}

	vec3 shadedColor = lighting(light, worldPosition, normal, material);

	if ( state == SHADER_STATE_TABLE || state == SHADER_STATE_ROBOT)
	{
		LightProperties light2 = light;
		light2.pos = lightPosition2;
		light2.La = La2;
		light2.Ld = Ld2;
		light2.Ls = Ls2;

		shadedColor += lighting(light2, worldPosition, normal, material);
	}

	vec4 texColor = texture(textureImage, textureCoords);
	if ( state == SHADER_STATE_TABLE )
	{
		uint val = uint(floor(8*textureCoords.x)+floor(8*textureCoords.y));
		
		vec3 mult = val % 2 == 0 ? lightColorMultiplier : darkColorMultiplier;
		texColor *= vec4(mult,1);
	}


	outputColor = vec4(shadedColor, 1) * texColor;

	// normal vector debug:
	if ( debugState == SHADER_DEBUG_NORMALS )
	{
		outputColor = vec4( normalize( worldNormal ) * 0.5 + 0.5, 1.0 );
	}
	if ( debugState == SHADER_DEBUG_SHADING )
	{
		outputColor = vec4( shadedColor, 1.0 );
	}
	if ( debugState == SHADER_DEBUG_DIFFUSE_TEXTURE )
	{
		outputColor = texture(textureImage, textureCoords);
	}
	if ( debugState == SHADER_DEBUG_SHINE_TEXTURE )
	{
		outputColor = texture(textureShinning, textureCoords);
	}
}