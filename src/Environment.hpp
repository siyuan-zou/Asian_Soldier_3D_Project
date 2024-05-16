#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <GLFW/glfw3.h>

class Environment
{
public:
    void init();
    void render(GLuint shader);

private:
    GLuint vbo_position, vbo_index, vao;
    // Add private members and methods for your environment rendering here
    void setupScene();
};

#endif // ENVIRONMENT_HPP
