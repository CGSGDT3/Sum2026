/**/
#version 460

layout(location = 0) out vec4 OutColor;

in vec2 DrawTexCoord;                         
uniform int Addon0;
uniform int Addon1;

layout(binding = 0) uniform sampler2D Tex;

void main( void )
{
  vec4 color = texture(Tex, DrawTexCoord);
  if (color.a < 0.030)
    discard;
  OutColor = vec4(color.rgb, color.a);
}