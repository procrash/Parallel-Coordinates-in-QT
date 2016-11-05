#version 330 core
// in vec3 color;

#define texture2D texture

in vec3 normals;
in vec2 textureCoord;

out vec4 aColor;

uniform sampler2D gSampler;

struct SimpleDirectionalLight
{
   vec3 vColor;
   vec3 vDirection;
   float fAmbientIntensity;
};

uniform SimpleDirectionalLight sunLight;

void main() {
    vec4 textureColor = texture2D(gSampler, textureCoord);
    vec4 vColor = vec4(1.0, 1.0, 1.0, 1.0);

    float fDiffuseIntensity = max(0.0, dot(normalize(normals), -sunLight.vDirection));
    aColor = textureColor*vColor*vec4(sunLight.vColor*(sunLight.fAmbientIntensity+fDiffuseIntensity), 1.0);
}
