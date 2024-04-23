#version 410 core

/* TODO update your vertex shader so that...
 *      ...it emits all necessary information to the fragment shader texturing
 *
 */

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec2 vTexCoords;
layout(location = 4) in vec3 vTangent;

out vec3 objectColor;
// TODO Add the output data required in the fragment shader
out vec3 worldPos;
out vec2 TexCoords;
out mat3 TBN;

// TODO END

// matrices that stay constant for the whole mesh.
uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * vec4(vPosition, 1);
	objectColor = vColor;

	/* TODO: set texture coordinates for fragment shader here here and additionally required output*/
	worldPos = vec3(modelMatrix * vec4(vPosition, 1));
	TexCoords = vTexCoords;

	// normal mapping
	mat4 normalMatrix = transpose(inverse(modelMatrix));
	vec3 vertexNormal = normalize(vec3(normalMatrix * vec4(vNormal, 0.0)));
	vec3 tangent = normalize(vec3(normalMatrix * vec4(vTangent, 0.0)));
	vec3 bitangent = normalize(cross(vertexNormal, tangent));
	TBN = mat3(tangent, bitangent, vertexNormal);

	// End TODO
}

