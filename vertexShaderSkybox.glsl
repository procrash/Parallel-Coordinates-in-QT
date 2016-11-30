#version 330 core
layout(location=0) in vec3 posAttr;
out vec2 texCoord;

uniform highp mat4 matrix;

void main() {
    // vnorm = vec2(gl_Position.x,gl_Position.y);
    texCoord = posAttr.xy;
    gl_Position = matrix * vec4(posAttr.xyz, 1.0);
}
