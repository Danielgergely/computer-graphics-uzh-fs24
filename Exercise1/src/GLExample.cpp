#include "GLExample.h"
#include "Cube.h"
#include <iostream>

namespace cgCourse
{
    GLExample::GLExample(glm::uvec2 _windowSize, std::string _title): GLApp(_windowSize, _title, false)
    {
        program = 0;
        mvpUniformLocation = 0;
        mvpMatrix = glm::mat4();
    }

    bool GLExample::init()
    {
        // Framebuffer size and window size may be different in high-DPI displays
        glm::uvec2 fbSize = getFramebufferSize();
        glViewport(0, 0, int(fbSize.x), int(fbSize.y));

        // load shader source code
        std::string vertShaderString = loadFile(std::string(SHADER_DIR) + "/Cube.vert");
        std::string fragShaderString = loadFile(std::string(SHADER_DIR) + "/Cube.frag");

        // Build and compile our shader program consisting of
        // a vertex and fragment shader
        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

        loadAndCompileShaderToGPU(vertShader, vertShaderString);
        loadAndCompileShaderToGPU(fragShader, fragShaderString);

        // Link shaders
        program = glCreateProgram();
        // check if it exists
        assert(program);
        // attach the shaders to the shader program
        glAttachShader(program, vertShader);
        glAttachShader(program, fragShader);
        // link the program to make it ready to use by the GPU
        glLinkProgram(program);
        // check the link status of the program and show linking errors
        // if there are any
        checkLinkStatusOfProgram(program);

        // shader programs can be marked as to delete. We don't have
        // to do this here if it is done later anywhen in the program. An explanation is here:
        // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteShader.xhtml
        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        // Get uniform locations for our mvp matrix
        mvpUniformLocation = glGetUniformLocation(program, "mvpMatrix");

        // Init models
        cube = std::make_shared<Cube>();
        bool cubeConstructed = cube->createVertexArray(0, 1, 2);
        if (!cubeConstructed) return cubeConstructed;
        cube->setShapePosition(glm::vec3(0.0, 0.0, 0.0));

        // Initialize the Torus Knot object with a specific offset along the normals
        torusKnot = std::make_shared<TorusKnot>(-1.0f);
        bool knotConstructed = torusKnot->createVertexArray(0, 1, 2);
        if (!knotConstructed) return knotConstructed;
        torusKnot->setShapePosition(glm::vec3(0.0, 0.0, 0.0));
        return knotConstructed;
    }

    bool GLExample::render()
    {
        glEnable(GL_DEPTH_TEST);

//         Clear the color and depth buffers
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable or disable face culling
        glEnable(GL_CULL_FACE); // Enable face culling
//        glDisable(GL_CULL_FACE); // Disable face culling (if required)

        // Set face culling to cull back faces or front faces
//        glCullFace(GL_BACK); // Cull back faces
        glCullFace(GL_FRONT); // Cull front faces (if required)

        // Set the orientation of front faces (counter-clockwise by default)
        glFrontFace(GL_CCW); // Front faces are defined in a counter-clockwise order
//         glFrontFace(GL_CW); // Front faces are defined in a clockwise order (if required)

//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Enable wireframe mode

        // Load program
        glUseProgram(program);

        // compute(currently) static matrix for the camera
        glm::mat4 viewProjectionMatrix = computeViewProjectionMatrix();

        // calculate model matrix for the cube object and set the uniform to store it in the shader
        mvpMatrix = viewProjectionMatrix * cube->getModelMatrix();
        glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &mvpMatrix[0][0]);
        cube->draw();

        // Draw Torus Knot
        mvpMatrix = viewProjectionMatrix * torusKnot->getModelMatrix();
        glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &mvpMatrix[0][0]);
        torusKnot->draw();

        return true;
    }

    bool GLExample::end()
    {
        if(program)
        {
            glDeleteProgram(program);
        }
        return true;
    }

    bool GLExample::loadAndCompileShaderToGPU(GLuint _program, const std::string & _source)
    {
        GLint status;
        GLchar errorMessage[512];

        char* shadersource =(char *)_source.c_str();
        int shaderstringlength = _source.length();
        glShaderSource(_program, 1,(const GLchar **)&shadersource, &shaderstringlength);
        glCompileShader(_program);
        glGetShaderiv(_program, GL_COMPILE_STATUS, &status);
        if(!status)
        {
            glGetShaderInfoLog(_program, 512, 0, errorMessage);
            std::cerr << "Shader compilation error:" << std::endl;
            std::cerr << errorMessage << std::endl;
            return false;
        }
        return true;
    }

    bool GLExample::checkLinkStatusOfProgram(GLuint _program)
    {
        GLint status;
        GLchar errorMessage[512];
        glGetProgramiv(_program, GL_LINK_STATUS, &status);
        if(!status)
        {
            glGetProgramInfoLog(_program, 512, 0, errorMessage);
            std::cerr << "Linking error:" << std::endl;
            std::cerr << errorMessage << std::endl;
            return false;
        }
        return true;
    }

    glm::mat4 GLExample::computeViewProjectionMatrix() const
    {
        // Camera matrix
        glm::mat4 view = glm::lookAt(
                glm::vec3(-1, 18, -18),	// Camera is at (-1, 18, -18), in World Space
                glm::vec3(0, 0, 0),		// and looks at the origin
                glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
        );

        // Projection matrix
        float fieldOfView = 45.0f;
        float nearPlane = 0.1;
        float farPlane = 50.0;
        glm::uvec2 fbSize = getFramebufferSize();
        float aspect = float(fbSize.x) / float(fbSize.y);

        glm::mat4 projection = glm::perspective(fieldOfView, aspect, nearPlane, farPlane);
        return projection * view;
    }
}