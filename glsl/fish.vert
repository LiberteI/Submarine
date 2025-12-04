#version 120
attribute vec3 aPos;

// elapsed time (seconds)
uniform float uCurTime;     
// how far from the center to orbit      
uniform float uOrbitRadius;    
// angular speed (radians per second)
uniform float uOrbitSpeed;     
// point to orbit around
uniform vec3 uOrbitCenter;     

varying vec3 vWorldPos;
varying vec3 vNormal;
void main(){
    // base mesh position
    vec3 curPos = aPos;

    // revolve around the center on the XZ plane
    float angle = uCurTime * uOrbitSpeed;
    vec3 orbitOffset = vec3(cos(angle), 0.0, sin(angle)) * uOrbitRadius;
    vec3 worldPos = uOrbitCenter + orbitOffset + curPos;

    // track world position for lighting in the fragment stage
    vWorldPos = worldPos;

    // transform to clip space (fixed-function matrices)
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(worldPos, 1.0);
}
