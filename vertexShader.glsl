attribute highp vec4 posAttr;
attribute lowp vec4 colAttr;
varying lowp vec4 col;
void main() {
   //col = colAttr;
   gl_Position = posAttr;
   gl_FrontColor = gl_Color;
}
