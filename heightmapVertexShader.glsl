#version 330 core
layout(location=0) in vec3 posAttr;
layout(location=1) in vec2 textureCoordIn;
layout(location=2) in vec3 normalVec;

out vec2 textureCoord;
out vec3 normals;

uniform highp mat4 matrix;

void main() {

    gl_Position = matrix * vec4(posAttr.xyz, 1.0);
    textureCoord = textureCoordIn;
    normals = normalVec;
}
