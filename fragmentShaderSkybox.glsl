#version 330 core

#define texture2D texture

// in vec3 color;
// in vec2 texCoord;

in vec3 texCoord;
out vec4 aColor;
//uniform samplerCube skyBoxSampler;
uniform sampler2D skyBoxSampler;

void main() {
    // aColor = vec4(color, 1.0);
   // vec4 textureColor = texture2D(gSampler, )

    // vec4 frag = texture(skyBoxSampler, -texCoord);
    vec4 frag = texture(skyBoxSampler, vec2(0.0,0.0));

    frag = vec4(frag.x, 1.0, 0.0, 1.0);

     // vec4 add = vec4(0.5,0.5,0.5,0.0);

     //frag += add;


//     aColor = frag; //vec4(1.0, 0.0, 0.0, 1.0);


     aColor = vec4(frag.xyz, 1.0);
     // aColor = vec4(texCoord, 1.0);
     // aColor = vec4(1.0, 0.0, 0.0, 1.0);
     // aColor = vec4(noise3(texCoord.y), 1.0);


}
