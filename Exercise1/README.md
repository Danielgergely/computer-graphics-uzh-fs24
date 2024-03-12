## Geometry - Exercise 1 (3 + 1 points)
 
The aim of this exercise is to give you an impression how 3-dimensional objects are built and how to use different modes for rendering in OpenGL.


### Task (3 points)

In this exercise, you have to generate a parametrizable mesh with the shape of a torus knot. Further you should be able to offset the vertices later along their normals. For the visualization, you should be able to draw not only the faces but also the wire frame and use face culling.

Specifically, you need to write the code to compute all the vertices, vertex normals and faces of the mesh. Each vertex requires a 3D position value, a RGB color value and a 3D normal vector (for offsetting the positions later). Each face (or triangle) requires a triplet with the indices of its vertices.

After the torus has been created, you can apply the offset to overwrite the vertex positions with the offset ones (offset argument of the ```TorusKnot``` constructor).

You do not have to implement any interactivity for changing these parameters, having them as constants in the code is sufficient.

The source code of this project is the viewer from the first exercises extended with some functions to help you visualize the objects. An incomplete TorusKnot class has been created already. Specifically you need to complete code pieces in the following files:

- GLExample.cpp
- TorusKnot.h
- TorusKnot.cpp

Places where you should add certain functionality are marked with TODO in the code. 

Example screenshots of the result can be found in the doc folder. You are free to color you torus knot however you want. The four numbers in the name of the example images describe

- (1) the number of segments on the torus knot
- (2) the number of segments in the tube surface
- (3) the number of times the knot winds around the torus rotational symetry axis (p)
- (4) the number of times the knot winds around the circle in the interiot of the torus (q)
- (5) the radius of the torus knot tube

The base torus in the screenshots has major radius 10.0 and minor radius 3. 


### How to submit

Once your code compiles and works, create a **.zip** file with the source code (please include only the source files in the **src/** folder) and a screenshot of the rendering window working on your computer. Also add a screenshots each with an offset factor along the vertex normal above and below 0 as well as the wireframe mode and optionally a screenshot of the bonus task. Upload the zip-file to OLAT.

Important: Similar to the first exercise this project includes a setup script and a CMake config file to generate the building scripts for different build systems. You do this the same way as for exercise 0, so it should be straightforward to build the project using the setup script and CMake. 


### Learning targets

- You are able to procedurally generate an object with triangles

- You know how to enable/disable face culling in OpenGL 

- You know how to change the culling mode (frontface vs. backface culling in OpenGL)

- You know how to render a model in wireframe mode

- You know what vertex normals are and how the movement along them changes the shape of a 3D object


### Tips

- You can find more information about torus knots on [Wikipedia](https://en.wikipedia.org/wiki/Torus_knot).
- The torus knot itself is not 3 dimensional but a line on the surface of a torus, [here](https://www.gsn-lib.org/docs/nodes/MeshTorusNode.php) a good tutorial for creating the tube around the knot but consider that for your implementation the major radius should also be changeable.
- The vertex normals are already used to create the tube around said line
- A colouring scheme based on angles gives good results and was used for the example images


### Bonus task (1 additional point)

Setting the vertices off along their normal is quite boring, find another cool way how to manipulate the vertices of the torus knot and implement it. Do not forget to add a brief comment what the idea of your manipulation is. 


### Documentation & resources:

- [GLFW Documentation](http://www.glfw.org/docs/latest/index.html)
- [Glad Documentation on GitHub](https://github.com/Dav1dde/glad)
- [OpenGL - Getting started - FAQ](https://www.khronos.org/opengl/wiki/Getting_Started)
- [OpenGL-tutorial.org](http://www.opengl-tutorial.org)
- [Learn OpenGL tutorials [learnopengl.com]](https://learnopengl.com)
- [Torus knots on Wikipedia](https://en.wikipedia.org/wiki/Torus_knot)
- [3D torus knot function](https://www.gsn-lib.org/docs/nodes/MeshTorusNode.php)
