#version 330
layout(location=0) in vec4 posAttr;
uniform highp mat4 matrix;
void main() {
   gl_Position = matrix * vec4(posAttr.xyz, 1.0);
}
