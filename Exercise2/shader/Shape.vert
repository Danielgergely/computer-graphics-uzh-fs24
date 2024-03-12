#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;

out vec3 objectColor;

uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;

uniform bool useNormalMatrix;
uniform bool normalsAsColor;

void main()
{
	gl_Position = mvpMatrix * vec4(vPosition, 1);
	
	// TODO: complete, use the bool variables to enable/switch the options useNormalMatrix and normalsAsColor
	if (useNormalMatrix) {
        // If useNormalMatrix is true, transform the normal to eye space
        vec3 eyeNormal = normalize(normalMatrix * vNormal);

        if (normalsAsColor) {
            objectColor = (eyeNormal + 1.0) * 0.5; // Map the normal vector to RGB colors
        } else {
            objectColor = eyeNormal;
        }
    } else {
        // If useNormalMatrix is false, use the vertex color
        objectColor = vColor;
    }
}

