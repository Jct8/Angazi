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

layout(binding = 0) uniform sampler2D equirectangularMap;

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

layout(binding = 0) uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 uv = SampleSphericalMap(normalize(worldPosition));
    FragColor = vec4(texture(equirectangularMap, uv).rgb,1.0f);
}