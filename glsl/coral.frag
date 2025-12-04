#version 120
varying vec3 vNormal;
void main(){
    // return dot product of normal 
    float scaler = dot(normalize(vNormal), vec3(0.0, 1.0, 0.0));
    float l = max(scaler, 0.2);
    // greenish coral tint
    vec3 base = vec3(0.2, 0.75, 0.35); 
    gl_FragColor = vec4(base * l, 1.0);
}
