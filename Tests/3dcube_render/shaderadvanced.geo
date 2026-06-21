#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 FragPos[];
in vec3 Normal[];

uniform mat4 view;
uniform mat4 projection;

void main()
{
    for (int i = 0; i < 3; ++i) {
        // Emit the original vertex
        gl_Position = projection * view * vec4(FragPos[i], 1.0);
        EmitVertex();

        // Emit the endpoint of the normal line
        vec3 normalEnd = FragPos[i] + Normal[i] * 0.2; // scale length
        gl_Position = projection * view * vec4(normalEnd, 1.0);
        EmitVertex();

        EndPrimitive();
    }
}