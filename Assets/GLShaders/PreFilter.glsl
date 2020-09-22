#shader VS
#version 450 core
layout (location = 0) in vec3 aPos;
out vec3 worldPosition;

out gl_PerVertex
{
    vec4 gl_Position;
};

layout(std140, binding = 0) uniform TransformBuffer
{
    mat4x4 WVP;
};

layout(binding = 0) uniform samplerCube environmentMap;

void main()
{
    gl_Position = vec4(aPos, 1.0) * WVP;
    worldPosition = aPos;
}

#shader PS
#version 450 core
in vec3 worldPosition;
out vec4 FragColor;

layout(std140, binding = 0) uniform TransformBuffer
{
    mat4x4 WVP;
};

layout(std140, binding = 1) uniform Settings
{
    float roughness;
};

layout(binding = 0) uniform samplerCube environmentMap;

const float PI = 3.14159265359;
const float EPSILON = 1e-6f;

float DistributionGGX(vec3 normal, vec3 halfVector, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSquared = alpha * alpha;
    float nDotH = max(dot(normal, halfVector), 0.0f);

    float denominator = (nDotH * nDotH) * (alphaSquared - 1.0f) + 1.0f;
    return alphaSquared / max(PI* denominator * denominator, EPSILON);
}

float RadicalInverse_VdC(uint bits) 
{
    // VanDerCorpus calculation.
    // http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html

     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 normal, float roughness)
{
    float alpha = roughness * roughness;

    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (alpha * alpha - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    // Spherical to Cartesian 
    vec3 halfway;
    halfway.x = cos(phi) * sinTheta;
    halfway.y = sin(phi) * sinTheta;
    halfway.z = cosTheta;

    // Tangent-space Halfway vector to World-Space sample vector
    vec3 up = abs(normal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, normal));
    vec3 bitangent = cross(normal, tangent);

    return normalize((tangent * halfway.x) + (bitangent * halfway.y) + (normal * halfway.z));
}

void main()
{
    vec3 normal = normalize(worldPosition);
    vec3 view = normal;

    const uint numSamples = 1024;
    vec3 prefilteredColor = vec3(0.0);
    float totalWeight = 0.0f;

    for (uint i = 0; i < numSamples; ++i)
    {
        vec2 xi = Hammersley(i, numSamples);
        vec3 halfway = ImportanceSampleGGX(xi, normal, roughness);
        vec3 lightDir = normalize(2.0 * dot(view, halfway) * halfway - view);
        
        float nDotL = max(dot(normal, lightDir), 0.0);
        if (nDotL > 0.0)
        {
            // sample from the environment's mip level based on roughness/pdf
            float D = DistributionGGX(normal, halfway, roughness);
            float nDotH = max(dot(normal, halfway), 0.0);
            float hDotV = max(dot(halfway, view), 0.0);
            float pdf = D * nDotH / (4.0 * hDotV) + 0.0001; // Probability density function

            float resolution = 1024.0; // Resolution of source cubemap (per face)
            float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(numSamples) * pdf + 0.0001);

            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

            prefilteredColor += textureLod(environmentMap, lightDir, mipLevel).rgb * nDotL;
            totalWeight += nDotL;
        }
    }

    prefilteredColor = prefilteredColor / totalWeight;

    FragColor = vec4(prefilteredColor, 1.0);
}
