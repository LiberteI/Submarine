#version 120
varying vec3 vNormal;
void main(){
    float l = max(dot(normalize(vNormal), vec3(0.0, 1.0, 0.0)), 0.2);
    vec3 base = vec3(0.2, 0.75, 0.35); // greenish coral tint
    gl_FragColor = vec4(base * l, 1.0);
}
