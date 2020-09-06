#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float ratio;

void main() {
    FragColor = mix(texture(texture1, TexCoord.xy), texture(texture2, TexCoord.xy), ratio);
}
