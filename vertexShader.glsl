#version 330
layout(location=0) in vec4 posAttr;
layout(location=1) in vec3 colorAttr;
uniform highp mat4 matrix;

out vec3 color;
void main() {
    gl_Position = matrix * vec4(posAttr.xyz, 1.0);
    color = colorAttr;
}
