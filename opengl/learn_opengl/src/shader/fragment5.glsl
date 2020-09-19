#version 330 core
out vec4 FragColor;
uniform vec3 objectColor;
in vec3 ourColor;

void main() {
    FragColor = vec4(ourColor * objectColor, 1.0);
}
