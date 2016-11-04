#version 330 core
// in vec3 color;

#define texture2D texture

in vec3 normals;
in vec2 textureCoord;

out vec4 aColor;

uniform sampler2D gSampler;

void main() {
    //aColor = vec4(color, 1.0);
    aColor = texture2D(gSampler, textureCoord);
//    aColor = texture2D(gSampler, vec2(10.0, 10.0));

    // aColor += vec4(normals, 0);
     //aColor = vec4(color, 1.0); //vec4(1.0, 0.0, 0.0, 1.0);
}
