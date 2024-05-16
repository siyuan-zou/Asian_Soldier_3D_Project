#version 330 core  // OpenGL 3.3

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 outcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Output position (in normalized device coordinates) is directly the position
    gl_Position =  vec4(position, 1.0);
    outcolor = color;
}