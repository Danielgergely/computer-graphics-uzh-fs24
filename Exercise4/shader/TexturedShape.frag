#version 410 core

/* TODO update your vertex shader so that...
 *      ...it has texture samplers declared
 *      ...it outputs a diffuse texture as diffuse color
 *      ...it uses a light map to modify the specular highlights
 */

in vec3 objectColor;

/* TODO Add the required input data */
in vec3 worldPos;
in vec2 TexCoords;
in mat3 TBN;

// END TODO

/* TODO declare texture samplers here */
uniform sampler2D diffuseTexture;
uniform sampler2D lightMapTexture;
uniform sampler2D normalMapTexture;

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

	// normal map
	vec3 normalMap = texture(normalMapTexture, TexCoords.xy).rgb;
	vec3 normal = normalMap * 2.0 - 1.0;
	normal = normalize(TBN * normal);

	vec3 diffuseColor = texture(diffuseTexture, TexCoords).xyz;
	vec3 lightMapColor = texture(lightMapTexture, TexCoords).xyz;

	// ambient
	float ambientFactor = 0.3f;
	vec3 ambient = (light.ambient.xyz * ambientFactor);

	// diffuse
	vec3 lightDirection = vec3(normalize(light.position - worldPos));
	float diffuseTerm = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diffuseTerm;

	// specular
	vec3 viewDirection = normalize(camPos - normal);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularTerm = pow(max(dot(viewDirection, reflectDirection), 0.0), 16);
	vec3 specular = light.specular * specularTerm * lightMapColor;

	color = (ambient + diffuse + specular) * diffuseColor.rgb, 1.0;

//	color = vec3(TexCoords, 1.f);

//	color = diffuseColor;


	// TODO END
}

