#version 120

varying vec3 vWorldPos;

// Light (world space)
uniform vec3 lightPos;
uniform vec3 lightColor; // intensity/tint

// Material
uniform vec3 matDiffuse;
uniform vec3 matSpecular;
uniform float matShininess;

void main() {
   // flat upward normal for simple lighting
   vec3 N = vec3(0.0, 1.0, 0.0);

   // direction to light
   vec3 L = normalize(lightPos - vWorldPos);

   // simple diffuse
   float diff = max(dot(N, L), 0.0);

   // ambient to avoid black falloff
   vec3 ambient = matDiffuse * lightColor * 0.2;
   vec3 color = ambient + matDiffuse * lightColor * diff;

   // translucent water
   gl_FragColor = vec4(color, 0.45);
}
