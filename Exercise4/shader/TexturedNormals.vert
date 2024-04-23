#version 410 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;
layout(location = 3) in vec2 vTexCoords;
layout(location = 4) in vec3 vTangent;

out vec3 fragColor;

uniform mat4 mvpMatrix;

/* TODO Add the normal sampler */
uniform sampler2D linestexNormal;

void main()
{
	if((gl_VertexID & 1) == 1)
	{
		gl_Position = mvpMatrix * vec4(vPosition, 1);
	}
	else
	{
		gl_Position = mvpMatrix * vec4(vPosition, 1);

		/* TODO Calcualte the gl_position of the enpoint fo the normal */
		vec3 normal = texture(linestexNormal, vTexCoords).rgb;
		normal = normal * 2.0 - 1.0;

		vec3 T = normalize(vTangent);
		vec3 N = normalize(vNormal);
		vec3 B = cross(N, T);
		mat3 TBN = mat3(T, B, N);

		normal = normalize(TBN * normal);
		vec3 normalEndpoint = vPosition + normal * 0.1;
		gl_Position = mvpMatrix * vec4(normalEndpoint, 1);
		/* TODO END */
	}

	fragColor = vColor;
}

