#version 120
attribute vec3 aPos;
void main() {
    gl_Position = gl_ModelViewProjectionMatrix * vec4(aPos, 1.0);
}