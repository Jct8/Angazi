#shader VS
#version 450 core

layout(location = 0) in vec3 position;

out gl_PerVertex
{
    vec4 gl_Position;
};


void main()
{
	gl_Position = vec4(position,1.0f);
};

#shader PS
#version 450 core

out vec4 color;

void main()
{
	color = vec4(1.0 , 1.0 , 0.0 , 1.0);;
};