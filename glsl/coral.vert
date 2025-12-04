#version 120
attribute vec3 aPos;
attribute vec3 aNormal;
varying vec3 vNormal;
varying vec3 vWorldPos;
void main(){
    // transform normal into world space (modelview already applied)
    vNormal = normalize(gl_NormalMatrix * aNormal);
    vec4 worldPos = gl_ModelViewMatrix * vec4(aPos, 1.0);
    vWorldPos = worldPos.xyz;
    gl_Position = gl_ProjectionMatrix * worldPos;
}
