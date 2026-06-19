#version 330

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;

uniform mat4 MatrWVP;
uniform mat3 MatrWInv;


out vec3 DrawPos;
out vec2 DrawTexCoord;

void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition/* + vec3(0, 1.30 * sin(5 * Time + InPosition.x), 0)*/, 1);
  DrawPos = InPosition;
  DrawTexCoord = InTexCoord; 
}
