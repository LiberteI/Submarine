#version 120
attribute vec3 aPos;
attribute vec3 aNormal;
varying vec3 vNormal;
void main(){
    vNormal = aNormal;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(aPos, 1.0);
}
