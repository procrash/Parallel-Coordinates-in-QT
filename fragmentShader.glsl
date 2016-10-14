#version 330
in vec3 color;
out vec4 aColor;
void main() {
    aColor = vec4(color, 1.0); //vec4(1.0, 0.0, 0.0, 1.0);
}
