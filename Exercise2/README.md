## Exercise 2: Transformations

### Subject / Goals:

Understand transformations and how to apply them to objects. In addition, we get to know normals and experiment with them.

### Tasks:

- Apply rotation, translation, and scalings for a shape object (see 'Additional Information' below).
- Animate the cube to spin around the axis (1, 1, 1).
- Animate the cube to scale, increasing and decreasing its size.
- Create and transform six torus so that each torus is parallel to the faces of the cube.
- Animate the toruses so that they keep their positions and size correlated to the cube.
- Show the normals for the torus objects as a MultiLine object.
- Show the toruses with colors according to normals.
- Calculate the normal matrix, apply it to the normals of the torus, and compare the color output.
- Apply the normal matrix of the toruses also to manipulate the line object.

### Bonus Task:

- Add a rotation over the faces of the cube to the animation of the toruses (see the video in doc/spin_torus.mp4 as a reference for one torus).
- Add some translation animation to the cube and, consequently, to the toruses.

### How to Submit:

Create a **.zip** file with the source code (please include only the source files, the **src/** and **shader/** folders), and upload it to OLAT with a screenshot of the rendering window working on your computer.

### Learning Targets:

- Apply a transformation to a model matrix.
- Draw lines-based objects.
- Apply a normal matrix to normals, and see the difference between right and wrong normals.

### Additional Information:

The starting point of this exercise is two objects (torus and cube) located at the same position. Implement the corresponding methods in the Shape class.

Look at the shaders (vertex and fragment shaders) for the objects' coloring.

Some screenshots and videos of the initial state and as references to the exercise tasks are in the **doc/** folder.
