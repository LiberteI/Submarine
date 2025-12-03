#version 120

varying vec3 vWorldPos;
varying vec3 vNormal;

// Light (world space)
uniform vec3 lightPos;
uniform vec3 lightColor; // intensity/tint

// Material
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

void main() {
   
   gl_FragColor = vec4(1,1, 1, 1.0);
}
