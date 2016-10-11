#version 330
layout(location=0) in vec4 posAttr;
//attribute highp vec4 posAttr;
// attribute lowp vec4 colAttr;
// varying lowp vec4 col;
uniform highp mat4 matrix;
void main() {
   //col = colAttr;
   // gl_Position = posAttr;
   gl_Position = matrix * vec4(posAttr.xyz, 1.0);
   //gl_FrontColor = gl_Color;
}
