#version 410 core

/* Update your vertex shader so that...
 *      ...it emits all necessary information to the fragment shader for phong illumination
 *      ...it emits the color in case you do gouraud shading
 */

// Input vertex data
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec3 vNormal;

// Output data will be interpolated for each fragment.
// Tip: Try to use the flat modifier to make color associations of a fragment visible for debugging.
out vec3 objectColor;
out vec3 normal;
out vec3 worldPos;

// matrices that stay constant for the whole mesh.
uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;

uniform bool gouraudShading;

// TODO Additional variables

// slides version
//layout(std140) uniform Material {
//    float ambient;
//    float diffuse;
//    float specular;
//    float shininess;
//} material;
//
//layout(std140) uniform Light {
//    vec4 position;
//    vec4 color;
//} light;

// V2

uniform struct Light {
    vec3 position;
    vec3 color;
} light;

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

uniform vec3 viewPosition;
uniform bool bonusTask;


// END TODO

void main()
{
    normal = mat3(transpose(inverse(modelMatrix))) * vNormal;

    // Output position of the vertex, in clip space : MVP * vPosition
    gl_Position = mvpMatrix * vec4(vPosition, 1);

    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    // ... uncomment this for color
    objectColor = vColor;

    // ... uncomment this for color according to normals
    // objectColor = vNormal;

    worldPos = vec3(modelMatrix * vec4(vPosition, 1));

    if (gouraudShading)
    {
        // TODO add there code for gouraud shading
        worldPos = vec3(modelMatrix * vec4(vPosition, 1.f));
        vec3 norm = normalize(normal);
        vec3 lightVector = normalize(light.position - worldPos);
        vec3 viewDirection = normalize(viewPosition - worldPos);
        vec3 eyeVector = reflect(-lightVector, norm);

        vec3 ambient = material.ambient * objectColor * light.color;
        float diff = max(dot(norm, lightVector), 0.0);
        vec3 diffuse = material.diffuse * diff * objectColor * light.color;

        float spec = pow(max(dot(viewDirection, eyeVector), 0.f), material.shininess);
        vec3 specular = material.specular * spec * objectColor * light.color;

        objectColor = (ambient + diffuse + specular) * light.color * objectColor;

        // comment out for showing each term
//        objectColor = specular * light.color * objectColor;

        // END TODO
    }

    // ********************** bonus task ******************************
    // comment out for spotlight only
//    if(bonusTask) {
//        objectColor = vColor;
//    }
}

