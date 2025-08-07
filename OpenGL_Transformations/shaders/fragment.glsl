#version 460 core
in vec4 vertexColor;
in vec2 vertexTexCoord;
out vec4 FragColor;
uniform float u_time;
uniform float freq_x = 20.0;
uniform float freq_y = 20.0;

void main()
{
    float waveValue = sin(vertexTexCoord.x * freq_x + u_time * 2.0);
    float waveValue2 = cos(vertexTexCoord.y * freq_y + u_time * 1.5);
    float combinedWaves = (waveValue + waveValue2) * 1.0;
    combinedWaves = (combinedWaves + 1.0) * 0.5;
    vec3 colorA = vec3(0.1, 0.0, 0.4);
    vec3 colorB = vec3(0.9, 0.2, 0.5);
    vec3 finalColor = mix(colorA, colorB, combinedWaves);
    FragColor = vec4(finalColor, 1.0);
}