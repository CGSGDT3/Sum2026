/**/
#version 460

layout(location = 0) out vec4 OutColor;

in vec2 DrawTexCoord;

layout(binding = 0) uniform sampler2D Tex;

void main( void )
{                            
  OutColor = vec4(texture(Tex, DrawTexCoord).rgb, 1);
}