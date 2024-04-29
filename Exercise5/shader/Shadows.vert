#version 410 core

layout(location = 0) in vec3 vPosition;

uniform mat4 mvpMatrix;

out vec4 FragPosLightSpace;

void main()
{
	// TODO: complete
    gl_Position = mvpMatrix * vec4(vPosition, 1.0);
    FragPosLightSpace = mvpMatrix * vec4(vPosition, 1.0);
}

