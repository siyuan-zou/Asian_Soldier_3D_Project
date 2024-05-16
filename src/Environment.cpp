#include "../third_party/src/glad/include/glad/glad.hpp"
#include <GLFW/glfw3.h>
// Update GLFW variables for some system
#ifndef GLFW_TRUE
#define GLFW_TRUE 1
#define GLFW_FALSE 0
#endif

#include "Environment.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/types.h>

#include <vector>

// Vertex data for a simple square ground
std::vector<GLfloat> position = {
    // positions          // colors
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f   // top left
};

std::vector<GLuint> indices = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

void Environment::init()
{
    setupScene();
}

void Environment::render(GLuint shader)
{

    // render the scene
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer

    // draw the shape
    glBindVertexArray(vao);
    glUseProgram(shader);
    // index = 0, count = 3, type of index = GL_UNSIGNED_INT, offset = 0
    // glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Environment::setupScene()
{

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo_position);
    glGenBuffers(1, &vbo_index);

    /* all the binds after will be stored in the VAO */
    // bind the VAO
    glBindVertexArray(vao);

    // bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(GLfloat), &position[0], GL_STATIC_DRAW);

    // set the vertex attributes pointers
    // positions
    // index = 0, size = 6, type = GL_FLOAT, normalized = GL_FALSE, stride = 6 * sizeof(float), offset = 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0); // index = 0

    // colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // index = 1

    // bind the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}
