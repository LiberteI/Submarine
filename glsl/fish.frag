#version 120
varying vec3 vNormal;

// light direction pointing from light toward the scene
uniform vec3 uLightDir;
// base diffuse color of the fish
uniform vec3 uDiffuseColor;

void main(){
    vec3 N = normalize(vNormal);
    vec3 L = normalize(-uLightDir); // flip so it points from fragment to light
    float diff = max(dot(N, L), 0.0);

    vec3 color = uDiffuseColor * diff;
    gl_FragColor = vec4(color, 1.0);
}
