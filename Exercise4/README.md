## Texturing - Exercise 4 (3 + 1 points)
 
### Subject / Goals:

Apply texture mapping (including normal mapping) onto the cube and the torus.

#### Task

In this exercise, you will learn how to use textures to give the Torus and Cube a more detailed and interesting appearance. You can find two times three textures in the res folder: color, specular, and normal texture.

We suggest that you start with the texture mapping of the color and the specular one and ignore the normal map for a start; further, the Cube (Cube.cpp) is simpler to create the texture coordinates for. We used one texture per side for the cube and for the torus (Torus.cpp) we wrapped the texture four times around it. To then use the texture, first, you have to load the texture onto the GPU (Texture.cpp). To use the texture in the shaders, you have to bind the texture to the shader (GLExample.cpp). Then you can sample the value in the shader (TexturedShape.frag, TexturedShape.vert). On this topic, advisable resources can be found in the chapter of learnopengl.com, namely "Textures" and "Lighting maps."

The process is the same for the normal mapping except that you have to additionally compute the tangents for the Cube and Torus (Cube.cpp and Torus.cpp). Those tangents are used to map the object's normals to point in the right direction. For this, the generation of a TBN (tangent, bitangent, normal) is required. The learnopengl.com chapter Normal Mapping has a nice introduction to this topic. To check your solution you should complete the MultiLine rendering for the normals using the normal map. THis can be done in the TexturedNormals shaders. You might check the BasicNormals shader for inspiration.

- Apply texture mapping to the cube and the torus.
- Apply a light map to the cube and the torus object.
- Combine the texture mapping with the (Blinn-)Phong illumination model implemented in the last exercise.
- Apply normal mapping and use the normal texture provided
- Make screenshots of all the results during the texturing process (texture coordinates, texture only, texture and light map applied).

Files with todos: TexturedShape.frag, TexturedShape.vert, Cube.cpp, Torus.cpp, GLExample.cpp, Texture.cpp, TexturedNormals.vert

### How to Submit:

Once your code compiles and works, create a ```.zip``` file with the source code (please include only the source files in the ```src/``` and ```shader/``` folder) and the requested screenshots. Upload the zip-file to OLAT.

### Learning Targets:

- You know how to load a texture with OpenGL 
- You know how to apply texture mapping in a shader program
- You know how to apply a light map to the (Blinn-)Phong model
- You know how to set uniforms for samplers in GLSL shaders
- You know how to load normal mapping with OpenGL
- You know how to generate tangents, bitangents, and the TBN matrix

### Setup

To load images, we will use cimg in this exercise. The setup script should handle everything correctly. However, on MacOS and Windows, you have to install imagemagick additionally. 

You can do so on MacOS with *brew* on by running: 

	brew install imagemagick

On Windows you can download imagemagick from the imagemagick website. The recommended version is fine. When installing, select also "Install development headers and libraries for C and C++".

If you are a Linux user, you may have to install imagemagick or graphicsmagick using your package manager. For Debian/Ubuntu users:

	sudo apt install graphicsmagick

### Bonus Task (1 additional point)

Generate at least a color texture to custom color the objects. The texture should be generated in the program code, e.g. not being loaded from an image file. 
Create screenshots of your custom texture applied and comment in the source code your method. 

### Links

- [Download Imagemagick on Windows](https://imagemagick.org/script/download.php)
- [Basic Lighting on learnopengl.com](https://learnopengl.com/Lighting/Basic-Lighting)
- [Shaders on learnopengl.com](https://learnopengl.com/Getting-started/Shaders)
- [Textures on learnopengl.com](https://learnopengl.com/Getting-started/Textures)
- [Lighting Maps on learnopengl.com](https://learnopengl.com/Lighting/Lighting-maps)
- [Normal Mapping on learnopengl.com](https://learnopengl.com/Advanced-Lighting/Normal-Mapping)
