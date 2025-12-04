#version 120
attribute vec3 aPos;
attribute vec3 aNormal;

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
    // --------- Orbit Path ------------
    // revolve around the center 
    float angle = uCurTime * uOrbitSpeed;
    vec3 orbitOffset = vec3(cos(angle), 0.0, sin(angle)) * uOrbitRadius;

    
    // find tangent line
    // compute tangent direction. ie: velocity direction along the orbit Path
    // derivation of circular path: -sin t, 0, cos t
    vec3 tangent = normalize(vec3(-sin(angle), 0.0, cos(angle)));

    // heading = angle need to rotate +Z to tangent
    // stick heading pos to tangent line
    float heading = atan(tangent.x, tangent.z);
    // calculate cos sin
    float c = cos(heading);
    float s = sin(heading);
    mat3 rotY = mat3(
        c,   0.0, -s,
        0.0, 1.0, 0.0,
        s,   0.0, c
    );
    
    // redefine +Y
    // tilt the model by -90 degrees around X
    const float tiltC = 0.0;   // cos(-90°)
    const float tiltS = -1.0;  // sin(-90°)
    mat3 rotXminus90 = mat3(
        1.0, 0.0,    0.0,
        0.0, tiltC, -tiltS,
        0.0, tiltS,  tiltC
    );

    // apply tilt
    vec3 curPos = rotY * (rotXminus90 * aPos);
    // apply yaw
    vec3 curNormal = rotY * (rotXminus90 * aNormal);

    // translate final rotated model to orbit position
    vec3 worldPos = uOrbitCenter + orbitOffset + curPos;

    // final output values
    vWorldPos = worldPos;
    vNormal = normalize(curNormal);

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(worldPos, 1.0);
}
