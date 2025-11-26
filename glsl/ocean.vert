#version 330 core

layout(location = 0) in vec3 aPos;

// model matrix will move | rotate | scale the mesh

uniform mat4 model;

void main(){
    gl_Position = model * vec4(aPos, 1.0);
}