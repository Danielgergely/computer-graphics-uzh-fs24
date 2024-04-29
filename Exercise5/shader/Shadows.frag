#version 410 core

in vec4 FragPosLightSpace;

uniform sampler2D shadows;

out vec3 color;

void main()
{
    // TODO: complete
    // perform perspective divide and transform to [0,1] range
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadows, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    color = vec3(shadow);
}

