#version 410 core

out vec3 color;

void main()
{
	color = vec3(gl_FragCoord.z);
}
