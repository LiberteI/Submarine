#version 120
attribute vec3 aPos;

// uniform attributes that are handed in from GL program
uniform float waveSpeed;
uniform float waveAmplitude;
uniform float curTime;
uniform float frequency;

void main() {
    // compute phase φ
    float phase = curTime * waveSpeed;

    // copy current vertex's pos
    vec3 curPos = aPos;

    // compute height
    // Asin(kx + φ) + offset, k = 2pi / mu
    // heightAtVertex = sin(valueBasedOnPosition + phase + timeValue) * waveAmplitude
    float height = sin(( curPos.x + curPos.z ) * frequency + phase ) * waveAmplitude;
    curPos.y += height;

    // have to include:
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(curPos, 1.0);
}
