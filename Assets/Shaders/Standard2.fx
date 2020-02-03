//Description: Simple shader that does applies transformation and lighting

cbuffer TransformBuffer : register(b0)
{
	matrix World;
	matrix WVP; // world view projection - all three matricies combined . local - camera 
	float3 ViewPosition;
}

cbuffer LightBuffer : register(b1)
{
	float3 LightDirection;
	float4 LightAmbient;
	float4 LightDiffuse;
	float4 LightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
	float4 MaterialAmbient;
	float4 MaterialDiffuse;
	float4 MaterialSpecular;
	float  MaterialPower; //shininess
}

cbuffer SettingsBuffer : register(b3)
{
	float specularMapWeight : packoffset(c0.x);
	float bumpMapWeight : packoffset(c0.y);
	float normalMapWeight : packoffset(c0.z);
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 worldNormal : NORMAL;
	float3 worldTangent :TEXCOORD0;
	float3 dirToLight : TEXCOORD1;
	float3 dirToView : TEXCOORD2;
	float2 texCoord	: TEXCOORD3;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float3 localPosition = input.position + (input.normal *bumpMapWeight*0.5f );
	float3 worldPosition = mul(float4(localPosition, 1.0f), World).xyz;
	float3 worldNormal = mul(input.normal, (float3x3)World);
	float3 worldTangent = mul(input.tangent, (float3x3)World);

	output.position = mul(float4(localPosition, 1.0f), WVP);
	output.worldNormal = worldNormal;
	output.worldTangent = worldTangent;
	output.dirToLight = -LightDirection;
	output.dirToView = normalize(ViewPosition - worldPosition);
	output.texCoord = input.texCoord;

	return output;
}

//   |
//   v
// Rasterizer
//   |
//   v

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 worldNormal = normalize(input.worldNormal);
	float3 worldTangent = normalize(input.worldTangent);
	float3 worldBinormal = normalize(cross(worldNormal,worldTangent));
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	float3 normal = worldNormal;

	float4 ambient = LightAmbient * MaterialAmbient;

	float diffuseIntensity = saturate(dot(dirToView, normal));
	float4 diffuse = diffuseIntensity * LightDiffuse * MaterialDiffuse;

	float3 halfAngle = normalize(dirToLight + dirToView);
	float  specularBase = saturate(dot(halfAngle, normal));
	float  specularIntensity = pow(specularBase, MaterialPower);
	float4 specular = specularIntensity * LightSpecular * MaterialSpecular;

	float4 texColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float specularFactor = specularMap.Sample(textureSampler, input.texCoord).r;

	float4 color = (ambient + diffuse) * texColor + specular * (specularMapWeight != 0.0f ? specularFactor : 1.0f);
	//color = texColor * color.a + color * (1.0f - color.a);
	return color;
}

//technique11 Textured
//{
//	pass Pass1
//	{
//		BlendEnable = true;
//		SrcBlend = D3D11_BLEND_SRC_ALPHA;
//		DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
//		BlendOp = D3D11_BLEND_OP_ADD;
//		SrcBlendAlpha = D3D11_BLEND_ONE;
//		DestBlendAlpha = D3D11_BLEND_ZERO;
//		BlendOpAlpha = D3D11_BLEND_OP_ADD;
//		RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//	}
//}