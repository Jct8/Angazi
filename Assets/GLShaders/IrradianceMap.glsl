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

layout(binding = 0) uniform samplerCube environmentMap;
const float PI = 3.14159265359;

void main()
{
    vec3 normal = normalize(worldPosition);
    vec3 irradiance = vec3(0.0);

    vec3 right = cross(vec3(0.0f,1.0f,0.0f), normal);
    vec3 up    = cross(normal, right);

    float sampleDelta = 0.025f;
    float numSamples = 0.0f;

    for(float phi = 0.0; phi < 2.0f * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; 

            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            numSamples++;
        }
    }
    irradiance *= PI / numSamples;
    FragColor = vec4(irradiance, 1.0f);
}
