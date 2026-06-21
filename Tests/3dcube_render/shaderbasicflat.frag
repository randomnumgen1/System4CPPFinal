#version 330 core
out vec4 FragColor;

uniform vec4 color;

void main()
{
if (gl_FrontFacing)
    FragColor = vec4(1,0,0,1); // red
else
    FragColor = vec4(0,0,1,1); // blue
   // FragColor = color;
}
