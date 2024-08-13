#version 460

#extension GL_ARB_bindless_texture: require 

layout(location = 0) out vec4 OutColor;

const int MATERIAL_TYPE_IMAGE  = 1;
const int MATERIAL_TYPE_VECTOR = 2;

const float PI = 3.1415926535897932384626433832795;
const float EPSILON = 1.192092896e-07;

const float FLOAT_MAX = 3.402823466e+38;
const float FLOAT_MIN = 1.175494351e-38;

struct MaterialAttribute 
{
	vec4  Component;
	uvec4 RefToImage; //(ignore)
	uvec2 BindlessSampler;
	int   Type;
};

struct Material 
{
	MaterialAttribute Ambient;
	MaterialAttribute Albedo;
	MaterialAttribute Metallic;
	MaterialAttribute Emission;
	MaterialAttribute Roughness;

	float AlbedoFactor;  
	float MetallicFactor;
	float EmissionFactor;
};

struct MaterialData
{
	vec4  Ambient;
	vec4  Albedo;
	vec4  Metallic;
	vec4  Emission;
	float Roughness;
};

struct PointLight
{
	vec4 Position;
	vec4 Color;
	float Intensity;
	float Radius;
};

struct DirectionalLight
{
	vec4 Direction;
	vec4 Color;
	float Intensity;
};

readonly layout(std430, binding = 1) buffer u_MaterialBuffer 
{
	Material Data[];
} Materials;

readonly layout(std430, binding = 2) buffer u_PointLightBuffer 
{
	PointLight Data[];
} PointLights;

readonly layout(std430, binding = 3) buffer u_DirectionalLightBuffer 
{
	DirectionalLight Data[];
} DirectionalLights;

MaterialData FetchMaterialData(in Material material, in vec2 textureCoordinate)
{
	MaterialData data;

	if(material.Ambient.Type == MATERIAL_TYPE_IMAGE) 
		data.Ambient = texture(sampler2D(material.Ambient.BindlessSampler), textureCoordinate) * material.Ambient.Component;
	else 
		data.Ambient = material.Ambient.Component;

	if(material.Albedo.Type == MATERIAL_TYPE_IMAGE)
		data.Albedo = texture(sampler2D(material.Albedo.BindlessSampler), textureCoordinate) * material.Albedo.Component * material.AlbedoFactor;
	else 
		data.Albedo = material.Albedo.Component * material.AlbedoFactor;

	if(material.Metallic.Type == MATERIAL_TYPE_IMAGE)
		data.Metallic = texture(sampler2D(material.Metallic.BindlessSampler), textureCoordinate) * material.Metallic.Component * material.MetallicFactor;
	else   
		data.Metallic = material.Metallic.Component * material.MetallicFactor;

	if(material.Emission.Type == MATERIAL_TYPE_IMAGE)
		data.Emission = texture(sampler2D(material.Emission.BindlessSampler), textureCoordinate) * material.Emission.Component * material.EmissionFactor;
	else 
		data.Emission = material.Emission.Component * material.EmissionFactor;

	if(material.Roughness.Type == MATERIAL_TYPE_IMAGE)
		data.Roughness = texture(sampler2D(material.Albedo.BindlessSampler), textureCoordinate).r * material.Roughness.Component.r;
	else 
		data.Roughness = material.Roughness.Component.r;

	return data;
}

/*
	Trowbridge-Reitz GGX
	
	Used to statisticaly appromximate the relative surface area of microfacets
	aligned with the halfway vector.
*/

float DistributionGGX(float NdotH, float roughness)
{
	float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH2 = NdotH * NdotH;
	
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return a2 / denom; 
}

/*
	Geometry Function which is a combination of GGX and Schlick-Beckmann approximation.
	This decribes the self shadowing properties of a microfacet where one microfacet
	may occlude another (produce a shadow onto another).
*/

float GeometryGGX(float NdotX, float roughness)
{
	float k = (roughness * roughness)/2.0;
	
	float Denom = NdotX * (1.0 - k) + k;
	return NdotX / max(Denom, EPSILON);
}

float GeometryFun(float NdotV, float NdotL, float roughness)
{
	return GeometryGGX(NdotV, roughness) * GeometryGGX(NdotL, roughness);
}

/*
	the fresnel equation dictates how much reflection there is based on
	the view angle.
*/

vec4 FresnelApproximation(float NdotL, vec4 reflectivity)
{
	return reflectivity + (1.0 - reflectivity) * pow(1.0 - NdotL, 5.0);
}

float Attenuate(float radius, float dist)
{
	if(radius >= dist)
		return 1.0 / ((dist * dist) + EPSILON);

	return 0.0;
}

struct ShadeInfo
{
	MaterialData Data;

	vec3  WorldPosition;
	vec3  Normal;
	vec3  CameraPosition;
	vec3  LightDirection;
	vec3  LightColor;
	float Attenuation;
};

vec4 Shade(in ShadeInfo info)
{
	vec3 radiance = info.LightColor * info.Attenuation;

	vec3 viewDir    = normalize(info.CameraPosition - info.WorldPosition);
	vec3 halfwayDir = normalize(info.LightDirection + viewDir);

	float NdotL = max(dot(info.Normal, info.LightDirection), EPSILON);
	float NdotV = max(dot(info.Normal, viewDir),			 EPSILON);
	float NdotH = max(dot(info.Normal, halfwayDir),			 EPSILON);
	float VdotH = max(dot(viewDir,     halfwayDir),		     EPSILON);

	vec4 lambertianDiffuse = info.Data.Albedo / PI;

	vec4 F0 = vec4(0.04); 
	F0  = mix(F0, info.Data.Albedo, info.Data.Metallic);

	vec4 ks = FresnelApproximation(NdotL, F0);
	vec4 kd = (vec4(1.0) - ks) * (vec4(1.0) - info.Data.Metallic);

	vec3 cookTorranceNum =  DistributionGGX(NdotH, info.Data.Roughness)    *
							GeometryFun(NdotV, NdotL, info.Data.Roughness)  *
							ks.rgb;

    float cookTorranceDenom = (4.0 * NdotL * NdotV) + EPSILON;

	vec4 Specular = vec4(cookTorranceNum / cookTorranceDenom, 1.0);

	vec4 BRDF = kd * lambertianDiffuse + Specular; 

	return BRDF * vec4(radiance, 1.0) * NdotL;
}


/*
    Aces tone mapping designed to 
	map colors from a high dynamic range into a 
	low dynamic range.
*/

const mat3 AcesInputMatrix = mat3(
    vec3(0.59719, 0.35458, 0.04823),
    vec3(0.07600, 0.90834, 0.01566),
    vec3(0.02840, 0.13383, 0.83777)
);

const mat3 AcesOutputMatrix = mat3(
    vec3(1.60475, -0.53108, -0.07367),
    vec3(-0.10208, 1.10813, -0.00605),
    vec3(-0.00327, -0.07276, 1.07602)
);

vec3 Mul(in mat3 m, in vec3 v)
{
    float x = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2];
    float y = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2];
    float z = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2];
    return vec3(x, y, z);
}

vec3 RttAndOdtFit(vec3 v)
{
    vec3 a = v * (v + 0.0245786) - 0.000090537;
    vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
    return a / b;
}

vec3 AcesFitted(vec3 v)
{
    v = Mul(AcesInputMatrix, v);
    v = RttAndOdtFit(v);
    return Mul(AcesOutputMatrix, v);
}

in vec3 o_WorldPosition;
in vec3 o_Normal;
in vec2 o_TextureCoord;
in flat uint o_MaterialIndex;

 //TODO: should package everything into PerFrameData struct
uniform int u_PointLightSize;
uniform int u_DirectionalLightSize;
uniform vec3 u_CameraPosition;

void main()
{
	MaterialData materialData = FetchMaterialData(Materials.Data[o_MaterialIndex], o_TextureCoord);

	vec4 Color = materialData.Ambient + materialData.Emission;

	ShadeInfo info;
	info.Data = materialData;
	info.Normal = o_Normal;
	info.CameraPosition = u_CameraPosition;

	for(int i = 0; i < u_PointLightSize; i++)
	{
		float attenuation = Attenuate(PointLights.Data[i].Radius, distance(PointLights.Data[i].Position.xyz, o_WorldPosition));
		if(attenuation <= 0.0)
			continue;

		info.LightDirection = normalize(PointLights.Data[i].Position.xyz - o_WorldPosition); 
		info.LightColor = PointLights.Data[i].Color.rgb * PointLights.Data[i].Intensity;
		info.Attenuation = attenuation;
		
		Color += Shade(info);
	}

	for(int i = 0; i < u_DirectionalLightSize; i++)
	{
		info.LightDirection = DirectionalLights.Data[i].Direction.xyz;
		info.LightColor = DirectionalLights.Data[i].Color.rgb * DirectionalLights.Data[i].Intensity;
		info.Attenuation = 1.0;

		Color += Shade(info);
	}

    OutColor.rgb = AcesFitted(Color.rgb);
	OutColor.a = min(OutColor.a, 1.0);

    OutColor = pow(OutColor, vec4(1.0/2.2));  
}
