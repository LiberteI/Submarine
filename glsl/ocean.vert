#version 120
attribute vec3 aPos;

// uniform attributes that are handed in from GL program
uniform float waveSpeed;
uniform float waveAmplitude;
uniform float curTime;
uniform float frequency;

varying vec3 vWorldPos;
void main() {
    // compute phase φ
    float phase = curTime * waveSpeed;

    // copy current vertex's pos
    vec3 curPos = aPos;

    float waveArg = (curPos.x + curPos.z) * frequency + phase;
    float height = sin(waveArg) * waveAmplitude;
    curPos.y += height;

    // world position tracking
    // --- using fixed pipeline matrix ---
    vec4 worldPos = gl_ModelViewMatrix * vec4(curPos, 1.0);
    vWorldPos = worldPos.xyz;

    gl_Position = gl_ProjectionMatrix * worldPos;
    
}
