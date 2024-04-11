#version 410 core
/* Update your vertex shader so that...
 *      ...it emits the color for phong illumination
 *      ...it emits the color from the vertex shader in case you do gouraud shading there
 */

in vec3 objectColor;
in vec3 normal;
in vec3 worldPos;

/* TODO fill these structs with values from outside the shader similar
 *      to your matrix variables
 */

// slides version not working...
//layout(std140) uniform Light {
//    vec4 position;
//    vec4 color;
//} light;
//
//layout(std140) uniform Material {
//    float ambient;
//    float diffuse;
//    float specular;
//    float shininess;
//} material;

// V2

uniform vec3 viewPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform mat4 viewMatrix;

// END TODO

uniform bool gouraudShading;

// Ouput data
out vec3 color;

void main()
{
    // Output color = color specified in the vertex shader,
    // interpolated between all 3 surrounding vertices
    color = objectColor;

    if (!gouraudShading)
    {
        // TODO add there code for phong lighting
//        // slides version - not working...
//        vec4 v_position = viewMatrix * vec4(worldPos, 1.0);
//        // normal transform in camera coordinates
//        mat3 normMatrix = mat3(transpose(inverse(viewMatrix)));
//        vec3 n = normMatrix * normalize(normal);
//
//        // set base color
//        vec4 baseColor = vec4(objectColor, 1.0);
//
//        // light and eye vectors, in camera coordinates
//        vec3 l = normalize(light.position.xyz - vec3(v_position));
//        vec3 e = normalize(-vec3(v_position));
//
//        // ambient term
//        color = material.ambient * vec3(light.color) * vec3(baseColor);
//
//        // diffuse term
//        float diff = max(dot(n, l), 0.0);
//        if (diff > 0) {
//            color += material.diffuse * diff * vec3(light.color) * vec3(baseColor);
//
//            // specular term
//            vec3 r = reflect(-l, n);
//
//            float spec = max(dot(r, e), 0.0);
//            if (spec > 0) {
//                color += material.specular * pow(spec, material.shininess) * vec3(light.color) * vec3(baseColor);
//            }
//        }

        // V2
        vec3 norm = normalize(normal);
        vec3 lightVector = normalize(lightPosition - worldPos);
        vec3 viewDirection = normalize(viewPosition - worldPos);
        vec3 eyeVector = reflect(-lightVector, norm);

        // Ambient
        vec3 ambient = ambient * objectColor * lightColor;

        // Diffuse
        float diff = max(dot(norm, lightVector), 0.0);
        vec3 diffuse = diff * diffuse * objectColor * lightColor;

        // Specular
        float spec = pow(max(dot(viewDirection, eyeVector), 0.0), shininess);
        vec3 specular = spec * specular * objectColor * lightColor;

        color = (ambient + diffuse + specular) * objectColor * lightColor;

        // END TODO
    }
}
