#version 410 core

/* TODO update your vertex shader so that...
 *      ...it has texture samplers declared
 *      ...it outputs a diffuse texture as diffuse color
 *      ...it uses a light map to modify the specular highlights
 */

in vec3 objectColor;

/* TODO Add the required input data */
in vec2 TexCoords;
in vec3 Normal;
in vec3 Tangent;

// END TODO

/* TODO declare texture samplers here */
uniform sampler2D diffuseTexture;
uniform sampler2D lightMapTexture;

// END TODO

uniform vec3 camPos;
uniform struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light;

out vec3 color;

void main()
{
	color = objectColor;

	/* TODO add the source code to do phong shading but with the values of the color, normal and specular texture.
	 *      Remember that you can also use the color output for debugging of the
	 *      texture maps and texture coordinates in case that you face troubles.
	 */

	vec3 diffuseColor = texture(diffuseTexture, TexCoords).rgb;
	vec3 lightMapColor = texture(lightMapTexture, TexCoords).rgb;

	vec3 N = normalize(Normal);
	vec3 lightDirection = normalize(light.position - camPos);

	float diffuseTerm = max(dot(N, lightDirection), 0.0);

	vec3 viewDirection = normalize(camPos - N);
	vec3 reflectDirection = reflect(-lightDirection, N);

	float specularTerm = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);

	vec3 ambient = light.ambient * diffuseColor;
	vec3 diffuse = light.diffuse * diffuseTerm * diffuseColor;
	vec3 specular = light.specular * specularTerm * lightMapColor;

	color = ambient + diffuse + specular;



	// TODO END
}

