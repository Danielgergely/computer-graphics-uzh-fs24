#version 410 core

in vec4 FragPosLightSpace;

uniform sampler2D shadows;

out vec3 color;

void main()
{
    // TODO: complete
    color = vec3(gl_FragCoord.z);
}

