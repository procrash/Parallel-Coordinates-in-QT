#version 330 core

#define texture2D texture

// in vec3 color;
in vec2 texCoord;
out vec4 aColor;
uniform sampler2D gSampler;

void main() {
    // aColor = vec4(color, 1.0);
   // vec4 textureColor = texture2D(gSampler, )

    vec4 frag = texture2D(gSampler, texCoord);


    aColor = frag; //vec4(1.0, 0.0, 0.0, 1.0);
}
