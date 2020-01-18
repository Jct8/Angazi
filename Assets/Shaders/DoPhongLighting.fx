//Description: Simple shader that does applies transformation

cbuffer TransformBuffer : register(b0)
{
	matrix WVP; // world view projection - all three matricies combined
}

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VS_OUTPUT VS(float3 position : POSITION, float4 color : COLOR)
{
    VS_OUTPUT output;
    output.position = mul(float4(position, 1.0f) , WVP);
    output.color = color;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
}