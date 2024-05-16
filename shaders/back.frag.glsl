#version 330 core  // OpenGL 3.3

// The output variable of the fragment is the color (R,G,B,A)
out vec4 Fragcolor; // Output color

in vec3 outcolor; // Input color

void main()
{
    Fragcolor = vec4(outcolor, 1.0);
}